/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "cmsis_os.h"
#include "LogProc.h"
#include "Storage.h"
#include "Explorer.h"

//--------------------Public Defination -------------------------------------
osSemaphoreId LOGPROC_sem;
//--------------------Private Defination--------------------------------------
#define  MAX_BUF  100
typedef struct
{
   int32_t LogBuf[MAX_BUF];
   uint16_t len;
   uint8_t logFormat;
} LOG_INFO;
static LOG_INFO logInfo = {0};
static FIL LogFile;                   /* File object */
//---------------------------------------------------------------------------
//  Name     :        LogRoutine
//                  ======================================
//  Function : Called by OS to schedule
//             Run when upgrade-firmare condition is ready
//---------------------------------------------------------------------------
static void LogRoutine(void const * argument)
{
   FRESULT res;
   uint32_t byteswritten;                     /* File write/read counts */
   uint8_t wtext[] = "[WP]:This is Log-USB test/r/n"; /* File write buffer */
   char fileName[16];

static uint8_t logIdx = 0, errorLog = 0;
//StorageStatus[LogUnit].bEnableLog = 1;
uint32_t fLenth;
FILINFO fileList[32] = {0};
Storage_EnableLog(USB_DISK_UNIT, TRUE);
   for (;;)
   {
//      if (osSemaphoreWait(LOGPROC_sem , osWaitForever) == osOK)
      {
         if (logInfo.logFormat == LOG_OVERWRITE)
         {
            //TBD
            
         }  
         else if (logInfo.logFormat == LOG_APPEND)
         {
            //TBD
         }
         else
         {
            // Nothing to do
         }
         
#if 0
     //    if ((Storage_GetStatus(USB_DISK_UNIT) == STORGAGE_OK))
         {
     //       EXP_listFile(USB_DISK_UNIT, "firmware", "oss", 20, fileList);
            if (logIdx < 10)
            {
               /* Open the text file object with read access */
              if (FILE_openFile(&LogFile, USB_DISK_UNIT, "Mylog.TXT", FA_OPEN_ALWAYS | FA_WRITE) == FR_OK)
              {
                  fLenth = FILE_readSize(&LogFile);
                  res = f_lseek(&LogFile, fLenth);
                  byteswritten = FILE_writeFile(&LogFile, wtext, sizeof(wtext));
                  
                  if((byteswritten == 0) || (res != FR_OK))
                  {
                    /* 'STM32.TXT' file Write or EOF Error */
                     errorLog++;
                   
                  }
                  else
                  {
                    /* Close the open text file */
                    logIdx++;
                    f_close(&LogFile);
                  }
               }
            }
         }
#endif      
      }
      osDelay(500);
   }
}

void LOG_vInit(void)
{
   osThreadDef(LOG_Thread, LogRoutine, osPriorityLow, 0, 2048);
   osThreadCreate (osThread(LOG_Thread), NULL);
   
   /* Create Semaphore lock */
//   osSemaphoreDef(SEM_LOG);  
   /* Create the Semaphore used by the two threads */
//   LOGPROC_sem= osSemaphoreCreate(osSemaphore(SEM_LOG), 1); 
}

void LOG_SetInfo(BYTE byLog, int32_t *logBuf, uint16_t len)
{
   logInfo.logFormat = byLog;
   logInfo.len = len;
   memcpy(logInfo.LogBuf, logBuf, len);
}
//---------------------------------------------------------------------------
//  End of file
//---------------------------------------------------------------------------
