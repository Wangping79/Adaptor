/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"
#include "BoardBsp.h"
#include "usb_core.h"


//--------------------Public Defination--------------------------------------
#define USB_STATE_HID      0
#define USB_STATE_MSC      1
#define USB_STATE_NULL     0xFF
//USBH_HandleTypeDef  USBH_Host;
USBD_HandleTypeDef  USBD_Device;

extern USBD_StorageTypeDef USBD_DISK_fops;

extern USBD_DescriptorsTypeDef USBD_Desc;
extern USBD_DescriptorsTypeDef MSC_Desc;
//--------------------Private Defination--------------------------------------

static uint8_t usb_state = USB_STATE_NULL;//, usb_oldstate = USB_STATE_HID;
static BOOLEAN usb_msc = FALSE;
//---------------Private Function---------------------------------------------
static void USBHID_vInit()
{
   /* Init Application */
   USBD_Init(&USBD_Device, &USBD_Desc, 0);  
   /* Add Supported Class */
   USBD_RegisterClass(&USBD_Device, &USBD_CUSTOM_HID);
   /* Add Storage callbacks for CustomHid Class */
   USBD_CUSTOM_HID_RegisterInterface(&USBD_Device, &USBD_CustomHID_fops);
   /* Start Process */
   USBD_Start(&USBD_Device);
}

static void USBMSC_vInit()
{
   /* Init Application */
   USBD_Init(&USBD_Device, &MSC_Desc, 0);  
   /* Add Supported Class */
   USBD_RegisterClass(&USBD_Device, &USBD_MSC);
   /* Add Storage callbacks for MSC Class */
   //USBD_RegisterClass(&USBD_Device, &USBD_MSC_CustomHID);
   //USBD_CUSTOM_HID_RegisterInterface(&USBD_Device, &USBD_CustomHID_fops);
   USBD_MSC_RegisterStorage(&USBD_Device, &USBD_DISK_fops); 
   /* Start Process */
   USBD_Start(&USBD_Device);
}

static void USBHandler(void const * argument)
{
//   USBHID_vInit();
   while(1)
   {
      // Add codes to detect USB MSC or USB HID
      if (usb_msc)
      {
         if (usb_state != USB_STATE_MSC)
         {
//            USBD_DeInit(&USBD_Device);
            USBMSC_vInit();
            usb_state = USB_STATE_MSC;
         }
      }
      else
      {
        if (usb_state != USB_STATE_HID)
         {
//            USBD_DeInit(&USBD_Device);
            USBHID_vInit();
            usb_state = USB_STATE_HID;
         }
      }
      osDelay(500);
   }
}


//-------------Public function--------------------------------------------
void USB_vInit(void)
{
   osThreadDef(USB_Thread, USBHandler, osPriorityBelowNormal, 0, 256);
   osThreadCreate (osThread(USB_Thread), NULL);
}

static BYTE usbBuf[100];
void USB_Respone(BYTE *pBuf, BYTE length)
{
   memset(usbBuf, 0xFF, HIDDTransferDriver_REPORTSIZE);
   usbBuf[0] = 0x03;
   usbBuf[1] = HIDDTransferDriver_REPORTSIZE-2;////0x10;
   
   memcpy(&usbBuf[2], pBuf, length);
   
   USBD_CUSTOM_HID_SendReport(&USBD_Device, usbBuf, HIDDTransferDriver_REPORTSIZE);
   HAL_GPIO_TogglePin(LED_WARNIG_GPIO_Port, LED_WARNIG_Pin);
}


//---------------------------------------------------------------------------
//  End of file
//---------------------------------------------------------------------------
