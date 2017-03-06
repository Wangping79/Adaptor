
#ifndef SPI_FLASH_H_
#define SPI_FLASH_H_

//-------------------- include files ----------------------------------------
//-------------------- public definitions -----------------------------------
#define Dummy_Byte 0xA5

/* Select SPI FLASH: ChipSelect pin low  */
#define Select_Flash()     GPIO_ResetBits(GPIOB, GPIO_Pin_12) 
/* Deselect SPI FLASH: ChipSelect pin high */
#define NotSelect_Flash()    GPIO_SetBits(GPIOB, GPIO_Pin_12)

//-------------------- public data ------------------------------------------
//-------------------- public functions -------------------------------------
void SPI_Flash_Init(void);	         
void NOR_R_BLOCK(unsigned long addr, unsigned char *readbuff, unsigned int BlockSize);
void NOR_W_BLOCK(uint32_t addr, u8 *writebuff, uint16_t BlockSize);
void FlashReadID(FactoryInfor *pFac_Infor);
void NOR_ErasePage(unsigned long a1);

#endif /* SPI_FLASH_H_ */
/** @} */  /* End of group */
//---------------------------------------------------------------------------
//  End of file
//---------------------------------------------------------------------------
	

