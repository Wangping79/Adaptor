/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "Storage.h"
#include "Explorer.h"

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
#ifndef  AM_MASK
   #define AM_MASK		0x3F	/* Mask of defined bits */
#endif
/* Private defines -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
#define MAX_LEVEL       2
#define MAX_LENGTH      64
static uint8_t readNum  = 0;
static uint8_t readMax = 0;
static uint8_t byLevel = 0;
//------------------ Private function -----------------------------------------------

uint16_t readDir(char *path, char *pExt, FILINFO *fileList, uint8_t level)
{
   FRESULT res = FR_OK;
   FILINFO fno;
   DIR dir;
   char *fn;
//   uint8_t idx;

   uint16_t readCount = 0;
   char diskPath[MAX_LENGTH] = {0}, newDir[MAX_LENGTH] = {0};
   char ExtLower[8] = {0};

#if _USE_LFN
   static char lfn[_MAX_LFN + 1];   /* Buffer to store the LFN */
   fno.lfname = lfn;
   fno.lfsize = sizeof lfn;
#endif
   strcpy(diskPath, Storage_UnitName());

   strcat(diskPath, path);
   res = f_opendir(&dir, diskPath);
   if(res == FR_OK) 
   {
      while (readNum < readMax)
      {
         res = f_readdir(&dir, &fno);
         if(res != FR_OK || fno.fname[0] == 0 || byLevel >= MAX_LEVEL) 
         {
            f_closedir(&dir);
            return readCount;
         }
         else
         {
#if _USE_LFN
            fn = *fno.lfname ? fno.lfname : fno.fname;
#else
            fn = fno.fname;
#endif
            if (((fno.fattrib & AM_MASK) == AM_DIR) && MAX_LEVEL == 2)
            {
               strcpy(newDir, path);
               strcat(newDir, "/");
               strcat(newDir, fn);
               readDir(newDir, pExt, fileList, MAX_LEVEL);
               byLevel++;
            }
            else
            {
               FILE_GetExt(fn, ExtLower);
               if(strcasecmp(ExtLower, pExt) == 0)
               {
                  memcpy(&fileList[readNum], &fno, sizeof(FILINFO));
                  readNum++;
               }
            }
         }
      }
   }
   return readCount;
}
uint16_t EXP_listFile(uint8_t unit, char *path, char * pExt, uint8_t num, FILINFO *fileList)
{
   char readName[64];
   FRESULT  res;
   
   Storage_SetUnit(unit);
   strcpy(readName, Storage_UnitName());
   
   readNum = 0;
   readMax = num;
   byLevel = 1;

   readDir(path, pExt, fileList, MAX_LEVEL);
   return readNum;
}
      
BOOLEAN FILE_isDir(FILINFO *file)
{
   if((file->fattrib & AM_DIR) == AM_DIR)
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}

uint32_t FILE_readSize(FIL *fp)
{
   return f_size(fp);
}

uint32_t FILE_readFile(FIL *fp, BYTE *buf, uint16_t len)
{
   UINT readByte = 0;
   f_read(fp, buf, len, &readByte);
   return readByte;
}

uint32_t FILE_writeFile(FIL *fp, BYTE *buf, uint16_t len)
{
   UINT readByte = 0;
   f_write(fp, buf, len, &readByte);
   return readByte;
}

FRESULT FILE_openFile(FIL *fp, uint8_t unit, char *fileName, BYTE mode)
{
   char readName[64] = {0};
   FRESULT  res;
   
   Storage_SetUnit(unit);
   strcpy(readName, Storage_UnitName());
//   strcat(readName, "/");
   strcat(readName, fileName);
   
   res = f_open(fp, fileName, mode);
   
   return res;
}

FRESULT FILE_closeFile(FIL *fp)
{
   return f_close(fp);
}

void FILE_SplitFileName(char * pFile, char * pExt)
{
   int Len;
   int i;
   int j;
   
   /* Search beginning of extension */
   Len = strlen(pFile);
   for (i = Len; i > 0; i--) 
   {
      if (*(pFile + i) == '.') 
      {
         *(pFile + i) = '\0';     /* Cut extension from filename */
         break;
      }
   }
  
  /* Copy extension */
  j = 0;
  while (*(pFile + ++i) != '\0') 
  {
    *(pExt + j) = *(pFile + i);
    j++;
  }
  *(pExt + j) = '\0';          /* Set end of string */
}

void FILE_GetExt(char * pFile, char * pExt)
{
   int Len;
   int i;
   int j;
   
   /* Search beginning of extension */
   Len = strlen(pFile);
   for (i = Len; i > 0; i--) 
   {
      if (*(pFile + i) == '.') 
      {
//         *(pFile + i) = '\0';     /* Cut extension from filename */
         break;
      }
   }
  
  /* Copy extension */
  j = 0;
  while (*(pFile + ++i) != '\0') 
  {
    *(pExt + j) = *(pFile + i);
    j++;
  }
  *(pExt + j) = '\0';          /* Set end of string */
}


/*****************************END OF FILE****/
