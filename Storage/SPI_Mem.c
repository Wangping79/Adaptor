/* Includes ------------------------------------------------------------------*/
#include "BoardBsp.h"
#include "SPI_Mem.h"

/* Private variables ---------------------------------------------------------*/
/* LINK SD Card */
#define NOR_DUMMY_BYTE              0xFF    
#define NOR_NO_RESPONSE_EXPECTED    0x80

#define  DATA_AREA_SIZE_8MB         (1024*1024)//when memory size is 8M-Bits, 64Kbytes for Protected Area, the rest for Data area, which is used for FAT FS
#define  MEM_SIZE_8MB               0x14      //8-Mbyte code is 0x14
#define  MEM_SIZE_16MB              0x18      //16-Mbyte code is 0x18
#define  DATA_AREA_SIZE_DEFAULT      1        //The default number of page for data area
#define  MBYTE_TO_BYTE              1048576

#define  DATAAREA_BASE              0x100000  //The base address of data area (64KByte for protected area)
#define  PAGE_NUMBER                4096      //The number of pages
#define  MICRON_MANUFA_ID           0x20      //The manufactor is Micron

#define PAGE_SIZE                   256       // 256 bytes in one page 
#define FLASHROM_END_ADDRESS        0xFFE00
#define SR_WIP                      0x01

/* Private function prototypes -----------------------------------------------*/
static void SPI2_WriteData(uint8_t *DataIn, uint16_t DataLength);
static void SPI2_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLegnth);
static void SPI2_Error (void);

/* Private function prototypes -----------------------------------------------*/
__IO uint8_t SdStatus = NOR_PRESENT;
/******************************************************************************
                            BUS OPERATIONS
*******************************************************************************/
static uint32_t SPI2_Read(void)
{
  HAL_StatusTypeDef status = HAL_OK;
  uint8_t          readvalue = 0;
  //uint32_t          writevalue = 0xFFFFFFFF;
  
  //status = HAL_SPI_TransmitReceive(&hspi2_nor, (uint8_t*) &writevalue, (uint8_t*) &readvalue, 1, SPI2_TIMEOUT_MAX);
  status = HAL_SPI_Receive(&hspi2_nor, &readvalue, 1, SPI2_TIMEOUT_MAX);
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPI2_Error();
  }

  return readvalue;
}
/**
  * @brief  SPI Write a byte to device
  * @param  Value: value to be written
  * @retval None
*/
static void SPI2_WriteReadData(const uint8_t *DataIn, uint8_t *DataOut, uint16_t DataLength)
{
  HAL_StatusTypeDef status = HAL_OK;
  
  status = HAL_SPI_TransmitReceive(&hspi2_nor, (uint8_t*) DataIn, DataOut, DataLength, SPI2_TIMEOUT_MAX);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPI2_Error();
  }
}

/**
  * @brief  SPI Write an amount of data to device
  * @param  Value: value to be written
  * @param  DataLength: number of bytes to write
  * @retval None
  */
static void SPI2_WriteData(uint8_t *DataIn, uint16_t DataLength)
{
  HAL_StatusTypeDef status = HAL_OK;

  status = HAL_SPI_Transmit(&hspi2_nor, DataIn, DataLength, SPI2_TIMEOUT_MAX);
  
  /* Check the communication status */
  if(status != HAL_OK)
  {
    /* Execute user timeout callback */
    SPI2_Error();
  }
}
static void SPI_ReadData(uint8_t *DataOut, uint16_t DataLength)
{
   HAL_StatusTypeDef status = HAL_OK;
//   uint32_t          writevalue = 0xFFFFFFFF;
  
  //status = HAL_SPI_TransmitReceive(&hspi2_nor, (uint8_t*) &writevalue, DataOut, DataLength, SPI2_TIMEOUT_MAX);
   status = HAL_SPI_Receive(&hspi2_nor, DataOut, DataLength, SPI2_TIMEOUT_MAX);
   /* Check the communication status */
   if(status != HAL_OK)
   {
      /* Execute user timeout callback */
      SPI2_Error();
   }
}

/**
  * @brief  SPI error treatment function
  * @retval None
  */
static void SPI2_Error (void)
{
  /* De-initialize the SPI communication BUS */
  HAL_SPI_DeInit(&hspi2_nor);
}
/******************************************************************************
                            LINK OPERATIONS
*******************************************************************************/
/**
  * @brief  Set the NOR_CS pin.
  * @param  pin value.
  * @retval None
  */
static void NOR_SPI_CSState(uint8_t val)
{
  if(val == 1) 
  {
    NOR_CS_SELECTED();
}
  else
  {
    NOR_CS_NOTSEL();
  }
}

static uint8_t NOR_SPI_WriteByte(uint8_t Data)
{
  uint8_t tmp;
  /* Send the byte */
  SPI2_WriteReadData(&Data, &tmp, 1);
  return tmp;
}

uint8_t NOR_SPI_ReadByte(void)
{
  uint8_t data = 0;
  
  /* Get the received data */
//  data = SPI2_Read();
  SPI_ReadData(&data, 1);
  /* Return the shifted data */
  return data;
}

static uint8_t NOR_SPI_ReadStatus(void)
{
   uint8_t busy;
   
   NOR_CS_SELECTED();
   NOR_SPI_WriteByte(SPI_RDSR);
   busy = NOR_SPI_ReadByte();
   NOR_CS_NOTSEL();
   
   return(busy);
}

static void NOR_SPI_CheckStatua(void)
{
   uint8_t a=1;
   do 
   {
      a = NOR_SPI_ReadStatus();
   } while (a & SR_WIP);
}

static void NOR_SPI_WriteStatus(void)
{
   NOR_CS_SELECTED();
   NOR_SPI_WriteByte(SPI_WRSR);
   NOR_SPI_WriteByte(0x00);           //Clear the register
   NOR_CS_NOTSEL();
   NOR_SPI_CheckStatua();
}

static void NOR_SPI_EnableWrite(void)
{
   NOR_CS_SELECTED();
   NOR_SPI_WriteByte(SPI_WREN);
   NOR_CS_NOTSEL();
}

static void NOR_SPI_DisableWrite(void)
{
   NOR_CS_SELECTED();
   NOR_SPI_WriteByte(SPI_WRDI);
   NOR_CS_NOTSEL();
   NOR_SPI_CheckStatua();
}

//----------------Public function----------------------
uint8_t BSP_MEM_Init(void)
{ 
   NOR_SPI_CSState(1);

  /* Check SD card detect pin */
   if(BSP_MEM_IsDetected()==NOR_NOT_PRESENT) 
   {
      SdStatus = NOR_NOT_PRESENT;
      return MEM_NOR_ERROR;
   }
   else
   {
      SdStatus = NOR_PRESENT;
      return MEM_NOR_OK;
   }
}

/**
 * @brief  Detects if SD card is correctly plugged in the memory slot or not.
 * @retval Returns if SD is detected or not
 */
uint8_t BSP_MEM_IsDetected(void)
{
  __IO uint8_t status = NOR_PRESENT;

  return status;
}

/**
  * @brief  Returns information about specific card.
  * @param  pCardInfo: pointer to a NOR_CardInfo structure that contains all SD 
  *         card information.
  * @retval The SD Response:
  *         - MEM_NOR_ERROR : Sequence failed
  *         - MEM_NOR_OK    : Sequence succeed
  */
uint8_t BSP_MEM_GetCardInfo(NOR_CardInfo *pCardInfo)
{
  uint8_t status = MEM_NOR_ERROR;
  
   NOR_CS_SELECTED();
   NOR_SPI_WriteByte(SPI_RDID);
   pCardInfo->factInfo.u8Fac_Id= NOR_SPI_ReadByte();        //Manufacture ID
   pCardInfo->factInfo.u8Dev_Id= NOR_SPI_ReadByte();        //Drive Type 
   pCardInfo->factInfo.u8Dev_Size= NOR_SPI_ReadByte();      //Memory Size Code
   NOR_CS_NOTSEL();
   
   if(pCardInfo->factInfo.u8Fac_Id==MICRON_MANUFA_ID)      //Manufacturor ID
   {
      pCardInfo->CardBlockSize = STORAGE_BLK_SIZ;          //Size of one page
      
      switch (pCardInfo->factInfo.u8Dev_Size)
      {
         case MEM_SIZE_8MB:
            pCardInfo->CardCapacity = DATA_AREA_SIZE_8MB;
         break;
         
         case MEM_SIZE_16MB:
            pCardInfo->CardCapacity = DATA_AREA_SIZE_8MB;
         break;

         default:    
            pCardInfo->CardCapacity = DATA_AREA_SIZE_DEFAULT;
         break;
       } 
     }

  /* Returns the reponse */
  return status;
}

uint8_t BSP_MEM_ReadBlocks(uint32_t addr, uint8_t *readbuff, uint32_t BlockSize, uint16_t blk_len)
{
   uint32_t addr_DataArea = addr;// + DATAAREA_BASE;    //Reserve page(s) for the protected area
      
   NOR_CS_SELECTED();
   NOR_SPI_WriteByte(SPI_READ);
   NOR_SPI_WriteByte((addr_DataArea&0xffffff)>>16);
   NOR_SPI_WriteByte((addr_DataArea&0xffff)>>8);
   NOR_SPI_WriteByte(addr_DataArea&0xff);
   
   SPI_ReadData(readbuff, BlockSize * STORAGE_BLK_SIZ);

   NOR_CS_NOTSEL();       
 
  /* Returns the reponse */
  return MEM_NOR_OK;
}

uint8_t BSP_MEM_WriteBlocks(uint32_t addr, uint8_t *writebuff, uint32_t BlockSize, uint16_t blk_len)
{
   uint32_t /*dwI, */dwN, dwTempLen, dwSum=0;
   uint8_t abyComBuf[STORAGE_BLK_SIZ] = {0};         
   unsigned long addr_DataArea = addr/* + DATAAREA_BASE*/;    //Reserve page(s) for the protected area

   if(writebuff == NULL || addr_DataArea > FLASHROM_END_ADDRESS)
   {
      return MEM_NOR_ERROR;
   }

   dwTempLen = BlockSize * STORAGE_BLK_SIZ;
   while(dwTempLen)
   {
      dwN = PAGE_SIZE - (addr_DataArea&(PAGE_SIZE-1));
      
      if (dwTempLen < dwN)
      {
         dwN = dwTempLen;
      }

      NOR_SPI_EnableWrite();
      NOR_CS_SELECTED();
      NOR_SPI_WriteByte(SPI_PP);
      NOR_SPI_WriteByte((uint8_t)(addr_DataArea>>16));
      NOR_SPI_WriteByte((uint8_t)(addr_DataArea>>8));
      NOR_SPI_WriteByte((uint8_t)(addr_DataArea>>0));

      SPI2_WriteData(writebuff+dwSum, dwN);
      
      NOR_SPI_CheckStatua();
      NOR_CS_NOTSEL();
      NOR_SPI_DisableWrite();
      
      addr_DataArea += dwN;
      dwTempLen -= dwN;
      dwSum += dwN;
   }
   
//   BSP_MEM_ReadBlocks(addr, abyComBuf, 1, 0);
   return MEM_NOR_OK;
}


/**
  * @brief  Returns the SD status.
  * @retval The SD status.
  */
uint8_t BSP_MEM_GetStatus(void)
{
  return MEM_NOR_OK;
}

uint8_t BSP_MEM_Erase(uint32_t StartAddr)
{
   unsigned long addr_DataArea = StartAddr;// + DATAAREA_BASE;
   NOR_SPI_WriteStatus();
   NOR_SPI_EnableWrite();

   NOR_CS_SELECTED();  
   NOR_SPI_WriteByte(SPI_SECTE);
   NOR_SPI_WriteByte((uint8_t)(addr_DataArea&0xffffff)>>16);
   NOR_SPI_WriteByte((uint8_t)(addr_DataArea&0xffff)>>8); 
   NOR_SPI_WriteByte((uint8_t)addr_DataArea&0xff);
   NOR_CS_NOTSEL();
   NOR_SPI_CheckStatua();
   
  /* Return the reponse */
  return MEM_NOR_OK;
}

void BSP_MEM_EraseAll(void)
{
   NOR_SPI_WriteStatus();
   NOR_SPI_EnableWrite();

   NOR_CS_SELECTED();  
   NOR_SPI_WriteByte(SPI_BULKE);
   NOR_CS_NOTSEL();
   NOR_SPI_CheckStatua();
}
/*****************************END OF FILE**************************************/
