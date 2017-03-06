/* Includes ------------------------------------------------------------------*/
#include "usbd_storage.h"
#include "SPI_Mem.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define STORAGE_LUN_NBR                  1  
#define STORAGE_BLK_NBR                  0x10000  


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* USB Mass storage Standard Inquiry Data */
int8_t STORAGE_Inquirydata[] = { /* 36 */
  /* LUN 0 */
  0x00,		
  0x80,		
  0x02,		
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 5),
  0x00,
  0x00,	
  0x00,
  'D', 'a', 'n', 'f', 'o', 's', 's', ' ', /* Manufacturer: 8 bytes  */
  'D', 'r', 'i', 'v', 'e', ' ', ' ', ' ', /* Product     : 16 Bytes */
  ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  '0', '.', '0','1',                      /* Version     : 4 Bytes  */
}; 

/* Private function prototypes -----------------------------------------------*/
int8_t STORAGE_Init(uint8_t lun);
int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t *block_num, uint16_t *block_size);
int8_t STORAGE_IsReady(uint8_t lun);
int8_t STORAGE_IsWriteProtected(uint8_t lun);
int8_t STORAGE_Read(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
int8_t STORAGE_Write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
int8_t STORAGE_GetMaxLun(void);

USBD_StorageTypeDef USBD_DISK_fops = {
  STORAGE_Init,
  STORAGE_GetCapacity,
  STORAGE_IsReady,
  STORAGE_IsWriteProtected,
  STORAGE_Read,
  STORAGE_Write,
  STORAGE_GetMaxLun,
  STORAGE_Inquirydata, 
};
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initailizes the storage unit (medium)       
  * @param  lun: Logical unit number
  * @retval Status (0 : Ok / -1 : Error)
  */
int8_t STORAGE_Init(uint8_t lun)
{
  BSP_MEM_Init();
  return 0;
}

/**
  * @brief  Returns the medium capacity.      
  * @param  lun: Logical unit number
  * @param  block_num: Number of total block number
  * @param  block_size: Block size
  * @retval Status (0: Ok / -1: Error)
  */
int8_t STORAGE_GetCapacity(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
  NOR_CardInfo info;
  
  int8_t ret = -1;
  
  if(BSP_MEM_IsDetected() != NOR_NOT_PRESENT)
  {
    BSP_MEM_GetCardInfo(&info);
    
    *block_num = (info.CardCapacity)/info.CardBlockSize  - 1;
    *block_size = info.CardBlockSize - 1;
    ret = 0;
  }
  return ret;
}

/**
  * @brief  Checks whether the medium is ready.  
  * @param  lun: Logical unit number
  * @retval Status (0: Ok / -1: Error)
  */
int8_t STORAGE_IsReady(uint8_t lun)
{
  static int8_t prev_status = 0;
  int8_t ret = -1;
  
  if(BSP_MEM_IsDetected() != NOR_NOT_PRESENT)
  {
    if(prev_status < 0)
    {
      BSP_MEM_Init();
      prev_status = 0;
      
    }
    if(BSP_MEM_GetStatus() == MEM_NOR_OK)
    {
      ret = 0;
    }
  }
  else if(prev_status == 0)
  {
    prev_status = -1;
  }
  return ret;
}

/**
  * @brief  Checks whether the medium is write protected.
  * @param  lun: Logical unit number
  * @retval Status (0: write enabled / -1: otherwise)
  */
int8_t STORAGE_IsWriteProtected(uint8_t lun)
{
  return 0;
}

/**
  * @brief  Reads data from the medium.
  * @param  lun: Logical unit number
  * @param  blk_addr: Logical block address
  * @param  blk_len: Blocks number
  * @retval Status (0: Ok / -1: Error)
  */
int8_t STORAGE_Read(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  int8_t ret = -1;
  
  if(BSP_MEM_IsDetected() != NOR_NOT_PRESENT)
  {
    BSP_MEM_ReadBlocks(blk_addr * STORAGE_BLK_SIZ, buf, STORAGE_BLK_SIZ, blk_len);
    ret = 0;
  }
  return ret;
}

/**
  * @brief  Writes data into the medium.
  * @param  lun: Logical unit number
  * @param  blk_addr: Logical block address
  * @param  blk_len: Blocks number
  * @retval Status (0 : Ok / -1 : Error)
  */
int8_t STORAGE_Write(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  int8_t ret = -1;
  
  if(BSP_MEM_IsDetected() != NOR_NOT_PRESENT)
  {
    BSP_MEM_WriteBlocks(blk_addr * STORAGE_BLK_SIZ, buf, STORAGE_BLK_SIZ, blk_len);
    ret = 0;
  }
  return ret;
}

/**
  * @brief  Returns the Max Supported LUNs.   
  * @param  None
  * @retval Lun(s) number
  */
int8_t STORAGE_GetMaxLun(void)
{
  return(STORAGE_LUN_NBR - 1);
}
 

/*****************************END OF FILE****/

