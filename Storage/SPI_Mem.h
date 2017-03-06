#ifndef __SPI_MEM_H
#define __SPI_MEM_H


//-------------------include file------------------------------
#include "BoardBsp.h"
//----------------- Public Defination---------------------------

/* Maximum Timeout values for flags waiting loops. These timeouts are not based
   on accurate values, they just guarantee that the application will not remain
   stuck if the SPI communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define SPI2_TIMEOUT_MAX                   100

/**
  * @brief  SD Control Lines management
  */  
#define NOR_CS_SELECTED()    HAL_GPIO_WritePin(NOR_CS_GPIO_Port, NOR_CS_Pin, GPIO_PIN_RESET)
#define NOR_CS_NOTSEL()      HAL_GPIO_WritePin(NOR_CS_GPIO_Port, NOR_CS_Pin, GPIO_PIN_SET)
    
/** 
  * @brief  SD status structure definition  
  */     
#define MEM_NOR_OK         0x00
#define MEM_NOR_ERROR      0x01
#define NOR_PRESENT        ((uint8_t)0x01)
#define NOR_NOT_PRESENT    ((uint8_t)0x00)

typedef enum
{
/**
  * @brief  SD reponses and error flags
  */
  NOR_RESPONSE_NO_ERROR      = (0x00),
  NOR_IN_IDLE_STATE          = (0x01),
  NOR_ERASE_RESET            = (0x02),
  NOR_ILLEGAL_COMMAND        = (0x04),
  NOR_COM_CRC_ERROR          = (0x08),
  NOR_ERASE_SEQUENCE_ERROR   = (0x10),
  NOR_ADDRESS_ERROR          = (0x20),
  NOR_PARAMETER_ERROR        = (0x40),
  NOR_RESPONSE_FAILURE       = (0xFF),

/**
  * @brief  Data response error
  */
  NOR_DATA_OK                = (0x05),
  NOR_DATA_CRC_ERROR         = (0x0B),
  NOR_DATA_WRITE_ERROR       = (0x0D),
  NOR_DATA_OTHER_ERROR       = (0xFF)
}NOR_Info;

typedef struct
{
   unsigned char u8Fac_Id;
   unsigned short u8Dev_Id;
   unsigned char u8Dev_Size;
} FACT_INFO;

/** 
  * @brief SD Card information 
  */
typedef struct
{
  FACT_INFO factInfo;
  uint32_t CardCapacity;  /* Card Capacity */
  uint32_t CardBlockSize; /* Card Block Size */
} NOR_CardInfo;
  
//SPI Instructions
#define SPI_WREN                0x06
#define SPI_WRDI                0x04
#define SPI_RDSR                0x05
#define SPI_WRSR                0x01
#define SPI_READ                0x03
#define SPI_PP                  0x02        //Page Program
#define SPI_SECTE               0xd8        //Sector Erase (64KB)
#define SPI_BULKE               0xc7        //Whole Chip Erase 
#define SPI_DP                  0xb9
#define SPI_RES                 0xab
#define SPI_RDID                0x9F
//#define SPI_SUBSECTE            0x20        //Subsector Erase (4KB)

#define STORAGE_BLK_SIZ         0x200

/******************Public function*****************************/
uint8_t BSP_MEM_Init(void);
uint8_t BSP_MEM_IsDetected(void);
uint8_t BSP_MEM_ReadBlocks(uint32_t addr, uint8_t *readbuff, uint32_t BlockSize, uint16_t blk_len);
uint8_t BSP_MEM_WriteBlocks(uint32_t addr, uint8_t *writebuff, uint32_t BlockSize, uint16_t blk_len);
uint8_t BSP_MEM_Erase(uint32_t StartAddr);
uint8_t BSP_MEM_GetStatus(void);
uint8_t BSP_MEM_GetCardInfo(NOR_CardInfo *pCardInfo);

void BSP_MEM_EraseAll(void);

#endif

/****************End of file**********************************/

