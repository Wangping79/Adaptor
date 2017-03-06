
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef EXPLORER_H
#define EXPLORER_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "Type.h"
#include "ff.h"

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint16_t EXP_listFile(uint8_t uint, char *path, char * pExt, uint8_t num, FILINFO *fileList);
BOOLEAN FILE_isDir(FILINFO *file);
uint32_t FILE_readSize(FIL *fp);
uint32_t FILE_readFile(FIL *fp, BYTE *buf, uint16_t len);
FRESULT FILE_openFile(FIL *fp, uint8_t unit, char *fileName, BYTE mode);
FRESULT FILE_closeFile(FIL *fp);
uint32_t FILE_writeFile(FIL *fp, BYTE *buf, uint16_t len);
    
void FILE_GetExt(char * pFile, char * pExt);

#ifdef __cplusplus
}
#endif

#endif /*EXPLORER_H */

/*****************************END OF FILE****/
