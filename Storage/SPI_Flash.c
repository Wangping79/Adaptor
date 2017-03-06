#include  <ctype.h>
#include "SPI_Flash.h"
#include "stm32f4xx_gpio.h"

#define PAGE_SZ                 256         // 256 bytes in one page 
#define FLASHROM_END_ADDRESS    0x800000//0x7FFFFF    
#define SR_WriteBusy                   0x01
#define CRCPR_DEFAULT_VALUE       0x7

//SPI Instructions
#define SPI_WREN                0x06
#define SPI_WRDI                0x04
#define SPI_ReadStatus          0x05
#define SPI_WRSR                0x01
#define SPI_READ                0x03
#define SPI_PP                  0x02        //Page Program
#define SPI_SECTE               0xd8        //Sector Erase (64KB)
#define SPI_BULKE               0xc7        //Whole Chip Erase 
#define SPI_DP                  0xb9
#define SPI_RES                 0xab
#define SPI_RDID                0x9E
#define SPI_SUBSECTE            0x20        //Subsector Erase (4KB)

void SPI_Flash_Init(void);
u8 SPI_Flash_ReadByte(void);
u8 SPI_Flash_SendByte(u8 byte);

void WriteBusy(void);
void EnableWrite(void);
void DisableWrite(void);
void WriteStatus(void);
unsigned char ReadStatus(void);
void FlashReadID(FactoryInfor *pFac_Infor);
void ErasePage(unsigned long a1);  
void SPI_Flash_Init(void);

//-------------------- private functions ------------------------------------                                                                                
//---------------------------------------------------------------------------
//  Name     :        SPI_Flash_SendByte
//                  ======================================
//
//  Function : Sends a byte through the SPI interface and return the byte
//             received from the SPI 
//
//  Input    : byte : byte to send.
//
//  Output   : The value of the received byte.
//
//--------------------------------------------------------------------------- 
static u8 SPI_Flash_SendByte(u8 byte)
{
  /* Loop while DR register in not emplty */
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
  //NotSelect_Flash();  while(1);
  /* Send byte through the SPI2 peripheral */
  SPI_I2S_SendData(SPI1, byte);

  /* Wait to receive a byte */
  while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(SPI1);
}     
    
//---------------------------------------------------------------------------
//  Name     :        SPI_FLASH_ReadByte
//                  ======================================
//
//  Function : Reads a byte from the SPI Flash.
//             This function must be used only if the Start_Read_Sequence
//             function has been previously called.
//  Input    : None
//
//  Output   : Byte Read from the SPI Flash.
//
//--------------------------------------------------------------------------- 
static u8 SPI_Flash_ReadByte(void)
{
  return (SPI_Flash_SendByte(Dummy_Byte));
}
            
void EnableWrite(void)
{
   Select_Flash();
   SPI_Flash_SendByte(SPI_WREN);
   NotSelect_Flash();
}

void DisableWrite(void)
{
   Select_Flash();
   SPI_Flash_SendByte(SPI_WRDI); 
   NotSelect_Flash();
   WriteBusy();
}   
                                
void WriteStatus(void)
{   
   Select_Flash();
   SPI_Flash_SendByte(SPI_WRSR);
   SPI_Flash_SendByte(0x00);           //Clear the register
   NotSelect_Flash();
   WriteBusy();
}
                                                     
void WriteBusy(void)
{
   unsigned char a=1;
   while((a&0x01)==1) a=ReadStatus();  
}
                                                           
unsigned char ReadStatus(void)
{
   unsigned char busy;
   Select_Flash();
   SPI_Flash_SendByte(SPI_ReadStatus);
   busy = SPI_Flash_ReadByte();
   NotSelect_Flash();
   return(busy);
   
}
                                                         
//---------------------------------------------------------------------------
//  Name     :        NOR_R_BLOCK
//                  ======================================
//
//  Function : Read page
//
//  Input    : addr: the address for reading
//             writebuff: the pointer of read buffer
//             BlockSize: the length for read
//---------------------------------------------------------------------------
void NOR_R_BLOCK(unsigned long addr, unsigned char *readbuff, unsigned int BlockSize)
{
   unsigned int i=0; 
   unsigned long addr_DataArea = addr;// + DATAAREA_BASE;    //Reserve page(s) for the protected area
      
   Select_Flash();
   SPI_Flash_SendByte(SPI_READ);
   SPI_Flash_SendByte((addr_DataArea&0xffffff)>>16);
   SPI_Flash_SendByte((addr_DataArea&0xffff)>>8);
   SPI_Flash_SendByte(addr_DataArea&0xff);
   while(i<BlockSize)
   {   
      readbuff[i]=SPI_Flash_ReadByte();
      
      i++;
   }
   NotSelect_Flash();       
}
                                                          
//---------------------------------------------------------------------------
//  Name     :        NOR_W_BLOCK
//                  ======================================
//
//  Function : Program page
//
//  Input    : addr: the address for writing
//             writebuff: the pointer of write buffer
//             BlockSize: the length for writing
//
//  Output   :
//
//---------------------------------------------------------------------------
void NOR_W_BLOCK(uint32_t addr, u8 *writebuff, uint16_t BlockSize)
{
   u8 bySr, byOffsetData;
   uint32_t dwI, dwN, dwTempLen, dwSum=0;
   u8 abyComBuf[PAGE_SZ];         
   unsigned long addr_DataArea = addr;// + DATAAREA_BASE;    //Reserve page(s) for the protected area

   if( writebuff == NULL )
   {
      return;
   }
   if( addr_DataArea > FLASHROM_END_ADDRESS )
   {
      return;
   }

   ErasePage(addr_DataArea);                         //Erase subsector

   dwTempLen = BlockSize;
   while(dwTempLen)
   {
      dwN=PAGE_SZ - (addr_DataArea&(PAGE_SZ-1));
      if (dwTempLen<dwN) dwN=dwTempLen;
      byOffsetData=0;

      NOR_R_BLOCK(addr_DataArea, abyComBuf, dwN);

      for(dwI=0;dwI<dwN;dwI++)                          //find the different data
      {
         if(abyComBuf[dwI] == writebuff[dwSum+dwI])
         {
            byOffsetData++;
         }else
         {
            break;
         }
      }
      if(byOffsetData != dwN)    //The new data are totally the same as the old
      {
         EnableWrite();

         Select_Flash();
         SPI_Flash_SendByte(SPI_PP);
         SPI_Flash_SendByte((u8)((addr_DataArea+byOffsetData)>>16));
         SPI_Flash_SendByte((u8)((addr_DataArea+byOffsetData)>>8));
         SPI_Flash_SendByte((u8)((addr_DataArea+byOffsetData)>>0));
         for (dwI=byOffsetData;dwI<dwN;dwI++)
         {
            SPI_Flash_SendByte(*(writebuff+dwSum+dwI));
         }
         NotSelect_Flash();

         do{
            bySr = ReadStatus();
         }while (bySr & SR_WriteBusy);
      }                                                          

      addr_DataArea += dwN;
      dwTempLen  -= dwN;
      dwSum += dwN;
  }

}
                                                             
//---------------------------------------------------------------------------
//  Name     :        ErasePage
//                  ======================================
//--------------------------------------------------------------------------- 
void NOR_ErasePage(unsigned long a1)
{
   WriteStatus();
   EnableWrite();     
   Select_Flash();     
   SPI_Flash_SendByte(SPI_SUBSECTE);
   SPI_Flash_SendByte((a1&0xffffff)>>16);
   SPI_Flash_SendByte((a1&0xffff)>>8); 
   SPI_Flash_SendByte(a1&0xff);
   NotSelect_Flash();
   WriteBusy();
   
}
                                                             
//---------------------------------------------------------------------------
//  Name     :        FlashReadID
//                  ======================================
//
//  Function : Read the manufacture information, including manufacture ID, Drive Type, Memory Size Code
//
//  Input    : 
//
//  Output   :
//
//--------------------------------------------------------------------------- 
void FlashReadID(FactoryInfor *pFac_Infor)
{
   Select_Flash();   
   SPI_Flash_SendByte(SPI_RDID);
   pFac_Infor->u8Fac_Id= SPI_Flash_ReadByte();                //Manufacture ID
   pFac_Infor->u8Dev_Id= SPI_Flash_ReadByte();                 //Drive Type 
   pFac_Infor->u8Dev_Size= SPI_Flash_ReadByte();             //Memory Size Code
   NotSelect_Flash();   
}


//---------------------------------------------------------------------------
//  Name     :        SPI_Flash_Init
//                  ======================================
//
//  Function : Initializes the peripherals used by the SPI FLASH driver.
//
//  Input    : None
//
//  Output   : None
//
//--------------------------------------------------------------------------- 
void SPI_Flash_Init(void)
{
   /* Peripheral clock enable */
   __HAL_RCC_SPI2_CLK_ENABLE();
   
   /**SPI2 GPIO Configuration    
   PB12     ------> SPI2_NSS
   PB13     ------> SPI2_SCK
   PB14     ------> SPI2_MISO
   PB15     ------> SPI2_MOSI 
   */
   GPIO_InitStruct.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_15;
   GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
   
   GPIO_InitStruct.Pin = GPIO_PIN_14;
   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);   
  NotSelect_Flash();  
}
         
/** @} */  /* End of group */
//---------------------------------------------------------------------------
//  End of file
//---------------------------------------------------------------------------

