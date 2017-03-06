
/* Includes ------------------------------------------------------------------*/

#ifndef __USB_MSC_CUSTOMHID_H_
#define __USB_MSC_CUSTOMHID_H_

#include  "usbd_ioreq.h"

#define CUSTOMHID_INTERFACE 0x0
#define MSC_INTERFACE       0x1

#define USB_MSC_HID_CONFIG_DESC_SIZ  (USB_CUSTOM_HID_CONFIG_DESC_SIZ - 9 + USB_MSC_CONFIG_DESC_SIZ)

extern USBD_ClassTypeDef  USBD_MSC_CustomHID;

void MSC_HID_CtrlAdr(uint8_t channel);


#endif  /* __USB_MSC_CUSTOMHID_H_ */

  
/*****************************END OF FILE****/
