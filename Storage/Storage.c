/* Includes ------------------------------------------------------------------*/
#include "BoardBsp.h"
#include "Storage.h"
#include "SPI_Mem.h"
//#include "Explorer.h"
/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
#define LOGIC_DISK_LEN     5

/* Private defines -----------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

typedef struct
{
   uint8_t Status;
   BOOLEAN bEnableLog;
} STORE_STATUS;

typedef struct
{
   char         pathName[LOGIC_DISK_LEN];
   FATFS*       diskFatFs;
   STORE_STATUS diskStatus;
} STORAGE_INFO;

static STORAGE_INFO storeInfo[NUM_DISK_UNITS] = 
{
   {"0:", NULL, {0}},
   {"1:", NULL, {0}}
};
//static FATFS USBDISK_FatFs;         /* File system object for USB disk logical drive */
static FATFS NORDISK_FatFs;
static uint8_t activeUnit = 0;

//------------------ Private function -----------------------------------------------
/* Public function-----------------------------------------------------*/
void Storage_vInit(void)
{
   FRESULT rt;

  // Link to Nor-Flash
   if(FATFS_LinkDriver(&SNORDISK_Driver, storeInfo[NOR_DISK_UNIT].pathName) == 0)
   {
      rt = f_mount(&NORDISK_FatFs, (TCHAR const*)storeInfo[NOR_DISK_UNIT].pathName, 1);
      if (rt != FR_OK)
      {
         //Try again????
         rt = f_mount(&NORDISK_FatFs, (TCHAR const*)storeInfo[NOR_DISK_UNIT].pathName, 1);
         if (rt == FR_NO_FILESYSTEM /*!= FR_OK*/)
         {
            BSP_MEM_EraseAll();
            rt = f_mkfs(storeInfo[NOR_DISK_UNIT].pathName, 1, 0/*1024*/);
         }
      }
      storeInfo[NOR_DISK_UNIT].diskFatFs = &NORDISK_FatFs;
      if (rt == FR_OK)
      {
         storeInfo[NOR_DISK_UNIT].diskStatus.Status = STORGAGE_OK;
      }
   }
}


/**
  * @brief  Storage get status
  * @param  unit: logical storage unit index.
  * @retval int
  */
uint8_t Storage_GetStatus (uint8_t unit)
{  
  return storeInfo[unit].diskStatus.Status;
}

void Storage_SetStatus(uint8_t unit, uint8_t status)
{
   if (status == STORGAGE_OK)
   {
      storeInfo[unit].diskStatus.Status = STORGAGE_OK;
      if (unit == USB_DISK_UNIT)
      {
         /* Link the USB Host disk I/O driver */
//        if (FATFS_LinkDriver(&USBH_Driver, storeInfo[unit].pathName) == 0)
//        {
//           f_mount(&USBDISK_FatFs,  storeInfo[unit].pathName,  1);
//           storeInfo[USB_DISK_UNIT].diskFatFs = &USBDISK_FatFs;
//        }
     }
   }
   else
   {
      storeInfo[unit].diskStatus.Status = STORGAGE_NAK;
      if (unit == USB_DISK_UNIT)
      {
         f_mount(0,  storeInfo[unit].pathName,  1);
      }
   } 
}

/**
  * @brief  Storage get capacity
  * @param  unit: logical storage unit index.
  * @retval int
  */
uint32_t Storage_GetCapacity (uint8_t unit)
{  
   uint32_t   tot_sect = 0;
   FATFS *fs;
  
   if(unit <  NUM_DISK_UNITS)
   {
      tot_sect = (storeInfo[unit].diskFatFs->n_fatent - 2) * fs->csize;
   }
   return (tot_sect);
}

/**
  * @brief  Storage get free space
  * @param  unit: logical storage unit index. 
  * @retval int
  */
uint32_t Storage_GetFree (uint8_t unit)
{ 
   uint32_t   fre_clust = 0;
   FATFS *fs;
   FRESULT res = FR_INT_ERR;
   
   if(unit <  NUM_DISK_UNITS)
   {
      res = f_getfree(storeInfo[unit].pathName, (DWORD *)&fre_clust, &fs);
      if(res == FR_OK)
      {
         return (fre_clust * fs->csize);
      }
      else
      {
         return 0;
      }
   }
   else
   {
      return 0;
   }
}

void Storage_EnableLog(uint8_t unit, BOOLEAN bLog)
{
   storeInfo[unit].diskStatus.bEnableLog = bLog;
}

void Storage_SetUnit(uint8_t unit)
{
   activeUnit = unit;
}

uint8_t Storage_GetUnit(void)
{
   return activeUnit;
}

char* Storage_UnitName(uint8_t unit)
{
   return storeInfo[unit].pathName;
}
   
/*****************************END OF FILE****/
