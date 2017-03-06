#ifndef _LOGPROC_H_
#define _LOGPROC_H_

#include "Type.h"
#include "cmsis_os.h"
//-------------------- include files ----------------------------------------

#define NO_LOG          0
#define LOG_OVERWRITE   1
#define LOG_APPEND      2

//-------------------- public definitions -----------------------------------
extern osSemaphoreId LOGPROC_sem;

void LOG_vInit(void);
void LOG_Enable(BYTE byLog);

#endif

//---------------------------------------------------------------------------
//  End of file
//---------------------------------------------------------------------------

