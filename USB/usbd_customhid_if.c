/* Includes ------------------------------------------------------------------*/
#include "usbd_customhid_if.h"
#include "LCP_Port.h"
#include "usb_core.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
extern BOOLEAN bConnectWithVFD;
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static int8_t CustomHID_Init(void);
static int8_t CustomHID_DeInit(void);
static int8_t CustomHID_OutEvent(uint8_t event_idx, uint8_t state);
static int8_t CustomHID_OutData(uint8_t *pBuf, uint8_t endpoint);
static int8_t CustomHID_ReportFeature(USBD_HandleTypeDef  *pdev, uint8_t length, uint8_t value);
/* Private variables ---------------------------------------------------------*/

uint8_t byTxBuffer[FCBUFFER_SIZE] = {0};
uint8_t byRxBuffer[FCBUFFER_SIZE] = {0};
extern USBD_HandleTypeDef USBD_Device;

__ALIGN_BEGIN static uint8_t CustomHID_ReportDesc[USBD_CUSTOM_HID_REPORT_DESC_SIZE] __ALIGN_END =
{
   // Global Usage Page
   HIDReport_GLOBAL_USAGEPAGE, 0xFF, 0xFF,                              // Usage Page (Vendor-defined), define Device function
   /* 3 */
   
   // Collection: Application
   HIDReport_LOCAL_USAGE, 0xFF,                                         // Usage (Vendor-defined), define Usage
   HIDReport_COLLECTION,  HIDReport_COLLECTION_APPLICATION,             // Collection(application), create a set
   /* 7 */
   
   // Input report: Vendor-defined
   HIDReport_GLOBAL_REPORTID,       0x03,                               // REPORT_ID(3)
   HIDReport_LOCAL_USAGE,           0xFF,                               // Vendor-defined usage
   HIDReport_GLOBAL_REPORTCOUNT,    HIDDTransferDriver_REPORTSIZE - 1,  // Report Count (Report Size - 1)
   HIDReport_GLOBAL_REPORTSIZE,     8,
   HIDReport_GLOBAL_LOGICALMINIMUM, 0x80,                               // Usage Minimum size:(uint8_t)-128,
   HIDReport_GLOBAL_LOGICALMAXIMUM, 0x7F,                               // Usage Maximum size:-127
   HIDReport_INPUT,                 0,                                  // Input(constant)  No Modifiers
   /* 21 */

   // Output report: vendor-defined
   HIDReport_GLOBAL_REPORTID,       0x04,                               // REPORT_ID(4)
   HIDReport_LOCAL_USAGE,           0xFF,                               // Vendor-defined usage
   HIDReport_GLOBAL_REPORTCOUNT,    HIDDTransferDriver_REPORTSIZE - 1,
   HIDReport_GLOBAL_REPORTSIZE,     8,
   HIDReport_GLOBAL_LOGICALMINIMUM, 0x80,                               // Usage Minimum size:(uint8_t)-128,
   HIDReport_GLOBAL_LOGICALMAXIMUM, 0x7F,                               // Usage Maximum size:-127
   HIDReport_OUTPUT,                0,                                  // No Modifiers
   /* 35 */

   //Feature
   HIDReport_GLOBAL_REPORTID,       0x01,                               // REPORT_ID(1)
   HIDReport_LOCAL_USAGE,           0xFF,                               // Vendor-defined usage
   HIDReport_GLOBAL_REPORTCOUNT,    1,
   HIDReport_GLOBAL_REPORTSIZE,     8,
   HIDReport_FEATURE,               0x82,                               // No Modifiers
   /* 45 */

   //Feature
   HIDReport_GLOBAL_REPORTID,      0x04,                                // REPORT_ID(4)
   HIDReport_LOCAL_USAGE,          0xFF,                                // Vendor-defined usage
   HIDReport_GLOBAL_REPORTCOUNT,   1,
   HIDReport_GLOBAL_REPORTSIZE,    16,
   HIDReport_FEATURE,              0x82,                                // No Modifiers
   /* 55 */

   HIDReport_ENDCOLLECTION
}; 

USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops = 
{
  CustomHID_ReportDesc,
  CustomHID_Init,
  CustomHID_DeInit,
  CustomHID_OutEvent,
  CustomHID_OutData,
  CustomHID_ReportFeature,
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  CustomHID_Init
  *         Initializes the CUSTOM HID media low layer
  * @param  None
  * @retval Result of the opeartion: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CustomHID_Init(void)
{
  return (0);
}

/**
  * @brief  CustomHID_DeInit
  *         DeInitializes the CUSTOM HID media low layer
  * @param  None
  * @retval Result of the opeartion: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t CustomHID_DeInit(void)
{
  /*
  Add your deinitialization code here 
  */  
  return (0);
}


/**
  * @brief  CustomHID_OutEvent
  *         Manage the CUSTOM HID class Out Event    
  * @param  event_idx: LED Report Number
  * @param  state: LED states (ON/OFF)
  */
static int8_t CustomHID_OutEvent  (uint8_t event_idx, uint8_t state)
{
  return (0);
}

static uint8_t rxCount = 0;
static int8_t CustomHID_OutData(uint8_t *pBuf, uint8_t endpoint)
{
   uint8_t len = 0;
   
   memset(byTxBuffer, 0xFF, FCBUFFER_SIZE);
   
   rxCount++;
   if (pBuf[2] == 0x02)
   {
      len = pBuf[3];
      memcpy(byTxBuffer, &pBuf[2], len+2);
      len += 2;
//      byTxBuffer[2] = 0x01;
   }
   else
   {
      strcpy((char *)byTxBuffer, (char *)&pBuf[2]);
      len = strlen((char *)byTxBuffer);
   }
   if (bConnectWithVFD)
   {
      LCPBUS_vSend(len);
   }
//   USB_Respone(byTxBuffer, 64);
   
   return 0;
}

static int8_t CustomHID_ReportFeature(USBD_HandleTypeDef  *pdev, uint8_t length, uint8_t value)
{
   /*static */uint8_t s_byReport1Data[3] = {HID_REPORTID_PACKET_COUNT, 0, 0x0};
   /*static*/ uint8_t s_byReport4Data[16] = {HID_REPORTID_INFORMATION};
   uint8_t err = 0;
   
   if ((value & 0xFF) == HID_REPORTID_PACKET_COUNT)
   {
      s_byReport1Data[1] = rxCount;
      /* Set EP0 State */
      pdev->ep0_state = USBD_EP0_DATA_IN;
      pdev->ep_in[0].total_length = 2;
      pdev->ep_in[0].rem_length   = 2;
      /* Start the transfer */
      USBD_LL_Transmit (pdev, 0x00, &s_byReport1Data[0], 2);  
   }
   else if ((value & 0xFF) == HID_REPORTID_INFORMATION)
   {
      /* Set EP0 State */
      pdev->ep0_state = USBD_EP0_DATA_IN;
      pdev->ep_in[0].total_length = 16;
      pdev->ep_in[0].rem_length   = 16;
      /* Start the transfer */
      USBD_LL_Transmit (pdev, 0x00, &s_byReport4Data[0], 16);
   }
   else
   {
      USBD_CtlError (pdev, NULL);
      err = (uint8_t)USBD_FAIL;
   }

   return err;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
