
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STORAGE_H
#define STORAGE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "Type.h"
/* FatFs includes component */
#include "ff_gen_drv.h"
#include "usbh_diskio.h"
#include "snor_diskio.h"

/* Exported constants --------------------------------------------------------*/

#define NUM_DISK_UNITS       2
#define USB_DISK_UNIT        1
#define NOR_DISK_UNIT        0

#define  STORGAGE_OK   1
#define  STORGAGE_NAK  0

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void     Storage_vInit(void);
uint8_t  Storage_GetStatus (uint8_t unit);
void     Storage_SetStatus(uint8_t unit, uint8_t status);
uint32_t Storage_GetCapacity (uint8_t unit);
uint32_t Storage_GetFree (uint8_t unit);
void     Storage_EnableLog(uint8_t unit, BOOLEAN bLog);
void     Storage_SetUnit(uint8_t unit);
uint8_t  Storage_GetUnit(void);
char*    Storage_UnitName(uint8_t unit);

#ifdef __cplusplus
}
#endif

#endif /*STORAGE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
