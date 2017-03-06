#ifndef _USB_CORE_H_
#define _USB_CORE_H_

//-------------------- include files ----------------------------------------
#include "Type.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc.h"
#include "usbd_msc_customhid.h"
#include "usbd_customhid.h"
#include "usbd_customhid_if.h"
//-------------------- public definitions -----------------------------------
void USB_vInit(void);

void USB_Respone(BYTE *pBuf, BYTE length);

#endif

//---------------------------------------------------------------------------
//  End of file
//---------------------------------------------------------------------------

