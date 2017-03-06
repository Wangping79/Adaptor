
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "ff_gen_drv.h"
#include "SPI_Mem.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Block Size in Bytes */
#define NOR_DEVICE_SIZE          8096  //BUGBUG
#define BLOCK_SIZE                512

/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

/* Private function prototypes -----------------------------------------------*/
DSTATUS SNORDISK_initialize (BYTE);
DSTATUS SNORDISK_status (BYTE);
DRESULT SNORDISK_read (BYTE, BYTE*, DWORD, UINT);
#if _USE_WRITE == 1
  DRESULT SNORDISK_write (BYTE, const BYTE*, DWORD, UINT);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT SNORDISK_ioctl (BYTE, BYTE, void*);
#endif /* _USE_IOCTL == 1 */
  
const Diskio_drvTypeDef  SNORDISK_Driver =
{
  SNORDISK_initialize,
  SNORDISK_status,
  SNORDISK_read, 
#if  _USE_WRITE
  SNORDISK_write,
#endif  /* _USE_WRITE == 1 */  
#if  _USE_IOCTL == 1
  SNORDISK_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS SNORDISK_initialize(BYTE lun)
{
  Stat = STA_NOINIT;
  
  /* Configure the SDRAM device */
  BSP_MEM_Init();
  
  Stat &= ~STA_NOINIT;
  return Stat;
}

/**
  * @brief  Gets Disk Status
  * @param  lun : not used
  * @retval DSTATUS: Operation status
  */
DSTATUS SNORDISK_status(BYTE lun)
{
  Stat = STA_NOINIT;
  
  Stat &= ~STA_NOINIT;

  return Stat;
}

/**
  * @brief  Reads Sector(s)
  * @param  lun : not used
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT SNORDISK_read(BYTE lun, BYTE *buff, DWORD sector, UINT count)
{
   BSP_MEM_ReadBlocks(sector * STORAGE_BLK_SIZ, buff, count, 0);
   return RES_OK;
}

/**
  * @brief  Writes Sector(s)
  * @param  lun : not used
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT SNORDISK_write(BYTE lun, const BYTE *buff, DWORD sector, UINT count)
{ 
   BSP_MEM_WriteBlocks(sector * STORAGE_BLK_SIZ, (uint8_t *)buff, count, 0);
   return RES_OK;
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  lun : not used
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT SNORDISK_ioctl(BYTE lun, BYTE cmd, void *buff)
{
  DRESULT res = RES_ERROR;
  NOR_CardInfo info;
  
  if (Stat & STA_NOINIT) return RES_NOTRDY;
  BSP_MEM_GetCardInfo(&info);
  switch (cmd)
  {
  /* Make sure that no pending write process */
  case CTRL_SYNC :
    res = RES_OK;
    break;
  
  /* Get number of sectors on the disk (DWORD) */
  case GET_SECTOR_COUNT :
     *(DWORD*)buff = info.CardCapacity / info.CardBlockSize;
     res = RES_OK;
     break;
  
  /* Get R/W sector size (WORD) */
  case GET_SECTOR_SIZE :
    *(WORD*)buff = info.CardBlockSize;
    res = RES_OK;
    break;
  
  /* Get erase block size in unit of sector (DWORD) */
  case GET_BLOCK_SIZE :
    *(DWORD*)buff = info.CardBlockSize;
    break;
  
  default:
    res = RES_PARERR;
  }
  
  return res;
}
#endif /* _USE_IOCTL == 1 */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

