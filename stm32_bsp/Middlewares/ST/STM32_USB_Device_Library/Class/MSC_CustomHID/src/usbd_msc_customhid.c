
/* Includes ------------------------------------------------------------------*/
#include "USBD_MSC_CustomHID.h"
#include "usbd_msc.h"
#include "usbd_customhid.h"
#include "usbd_desc.h"
#include "usbd_ctlreq.h"

/* Private functions----------------------------------------------------------*/

static uint8_t  USBD_MSC_CustomHID_Init (USBD_HandleTypeDef  *pdev, uint8_t cfgidx);

static uint8_t  USBD_MSC_CustomHID_DeInit (USBD_HandleTypeDef  *pdev, uint8_t cfgidx);

static uint8_t  USBD_MSC_CustomHID_Setup (USBD_HandleTypeDef  *pdev, USBD_SetupReqTypedef *req);

static uint8_t  *USBD_MSC_CustomHID_GetCfgDesc (uint16_t *length);

static uint8_t  USBD_MSC_CustomHID_DataIn (USBD_HandleTypeDef  *pdev, uint8_t epnum);
static uint8_t  USBD_MSC_CustomHID_DataOut(USBD_HandleTypeDef *pdev , uint8_t epnum);

static uint8_t  *USBD_MSC_CustomHID_GetOtherSpeedCfgDesc(uint16_t *length);
static uint8_t  *USBD_MSC_CustomHID_GetDeviceQualifierDescriptor(uint16_t *length);

static uint8_t tans_ch = CUSTOMHID_INTERFACE;
/* Public defination ----------------------------------------------------------*/
#ifdef USE_USB_OTG_HS 
   #define MSC_MAX_PACKET  MSC_MAX_HS_PACKET
#else
   #define MSC_MAX_PACKET  MSC_MAX_FS_PACKET
#endif

USBD_ClassTypeDef  USBD_MSC_CustomHID = 
{
  USBD_MSC_CustomHID_Init,
  USBD_MSC_CustomHID_DeInit,
  USBD_MSC_CustomHID_Setup,
  NULL, /*EP0_TxSent*/  
  NULL, /*EP0_RxReady*/
  USBD_MSC_CustomHID_DataIn, /*DataIn*/
  USBD_MSC_CustomHID_DataOut, /*DataOut*/
  NULL, /*SOF */
  NULL,
  NULL,      
  USBD_MSC_CustomHID_GetCfgDesc,
  USBD_MSC_CustomHID_GetCfgDesc, /* use same config as per FS */
  USBD_MSC_CustomHID_GetOtherSpeedCfgDesc,
  USBD_MSC_CustomHID_GetDeviceQualifierDescriptor,
};

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */ 
/* USB HID device Configuration Descriptor */
__ALIGN_BEGIN uint8_t USBD_MSC_CustomHID_CfgDesc[USB_MSC_HID_CONFIG_DESC_SIZ] __ALIGN_END =
{
  0x09, /* bLength: Configuration Descriptor size */
  USB_DESC_TYPE_CONFIGURATION, /* bDescriptorType: Configuration */
  USB_MSC_HID_CONFIG_DESC_SIZ,
  /* wTotalLength: Bytes returned */
  0x00,
  0x02,         /*bNumInterfaces: 2 interface*/
  0x01,         /*bConfigurationValue: Configuration value*/
  0x00,         /*iConfiguration: Index of string descriptor describing
  the configuration*/
  0xC0,         /*bmAttributes: bus powered and Support Remote Wake-up */
  0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/
  
  /************** Descriptor of CUSTOM HID interface ****************/
  /* 09 */
  0x09,         /*bLength: Interface Descriptor size*/
  USB_DESC_TYPE_INTERFACE,/*bDescriptorType: Interface descriptor type*/
  CUSTOMHID_INTERFACE,    /*bInterfaceNumber: Number of Interface*/
  0x00,         /*bAlternateSetting: Alternate setting*/
  0x02,         /*bNumEndpoints*/
  0x03,         /*bInterfaceClass: CUSTOM_HID*/
  0x00,         /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
  0x00,         /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
  0,            /*iInterface: Index of string descriptor*/
  
  /******************** Descriptor of CUSTOM_HID *************************/
  /* 18 */
  0x09,         /*bLength: CUSTOM_HID Descriptor size*/
  CUSTOM_HID_DESCRIPTOR_TYPE, /*bDescriptorType: CUSTOM_HID*/
  0x11,         /*bCUSTOM_HIDUSTOM_HID: CUSTOM_HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of CUSTOM_HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  USBD_CUSTOM_HID_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
  /******************** Descriptor of Custom HID endpoints ********************/
  /* 27 */
  0x07,          /*bLength: Endpoint Descriptor size*/
  USB_DESC_TYPE_ENDPOINT, /*bDescriptorType:*/
  
  CUSTOM_HID_EPIN_ADDR,     /*bEndpointAddress: Endpoint Address (IN)*/
  0x03,          /*bmAttributes: Interrupt endpoint*/
  CUSTOM_HID_EPIN_SIZE, /*wMaxPacketSize: 2 Byte max */
  0x00,
  0x20,          /*bInterval: Polling Interval (20 ms)*/
  /* 34 */
  
  0x07,	         /* bLength: Endpoint Descriptor size */
  USB_DESC_TYPE_ENDPOINT,	/* bDescriptorType: */
  CUSTOM_HID_EPOUT_ADDR,  /*bEndpointAddress: Endpoint Address (OUT)*/
  0x03,	/* bmAttributes: Interrupt endpoint */
  CUSTOM_HID_EPOUT_SIZE,	/* wMaxPacketSize: 2 Bytes max  */
  0x00,
  0x10,	/* bInterval: Polling Interval (20 ms) */
  /* 41 */
  
  /********************  Mass Storage interface ********************/
  0x09,   /* bLength: Interface Descriptor size */
  0x04,   /* bDescriptorType: */
  MSC_INTERFACE,   /* bInterfaceNumber: Number of Interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x02,   /* bNumEndpoints*/
  0x08,   /* bInterfaceClass: MSC Class */
  0x06,   /* bInterfaceSubClass : SCSI transparent*/
  0x50,   /* nInterfaceProtocol */
  0x01,          /* iInterface: */
  /********************  Mass Storage Endpoints ********************/
  0x07,   /*Endpoint descriptor length = 7*/
  0x05,   /*Endpoint descriptor type */
  MSC_EPIN_ADDR,   /*Endpoint address (IN, address 1) */
  0x02,   /*Bulk endpoint type */
  LOBYTE(MSC_MAX_PACKET),
  HIBYTE(MSC_MAX_PACKET),
  0x00,   /*Polling interval in milliseconds */
  
  0x07,   /*Endpoint descriptor length = 7 */
  0x05,   /*Endpoint descriptor type */
  MSC_EPOUT_ADDR,   /*Endpoint address (OUT, address 1) */
  0x02,   /*Bulk endpoint type */
  LOBYTE(MSC_MAX_PACKET),
  HIBYTE(MSC_MAX_PACKET),
  0x20     /*Polling interval in milliseconds*/
} ;

/* USB Standard Device Descriptor */
__ALIGN_BEGIN uint8_t USBD_MSC_CustomHID_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
{
  USB_LEN_DEV_QUALIFIER_DESC,
  USB_DESC_TYPE_DEVICE_QUALIFIER,
  0x00,
  0x02,
  0x00,
  0x00,
  0x00,
  0x40,
  0x01,
  0x00,
};

/* Private function prototypes -----------------------------------------------*/

/*********************************************
   MSC Device library callbacks
*********************************************/
extern uint8_t  USBD_MSC_Init (USBD_HandleTypeDef  *pdev, uint8_t cfgidx);
extern uint8_t  USBD_MSC_DeInit (USBD_HandleTypeDef  *pdev, uint8_t cfgidx);
extern uint8_t  USBD_MSC_Setup (USBD_HandleTypeDef  *pdev, USBD_SetupReqTypedef *req);
extern uint8_t  USBD_MSC_DataIn (USBD_HandleTypeDef  *pdev, uint8_t epnum);
extern uint8_t  USBD_MSC_DataOut (USBD_HandleTypeDef  *pdev,  uint8_t epnum);
extern uint8_t  *USBD_MSC_GetCfgDesc (uint8_t speed, uint16_t *length);
extern uint8_t  USBD_MSC_CfgDesc[USB_MSC_CONFIG_DESC_SIZ];

/*********************************************
   HID Device library callbacks
*********************************************/
extern uint8_t  USBD_CUSTOM_HID_Init (USBD_HandleTypeDef  *pdev, uint8_t cfgidx);
extern uint8_t  USBD_CUSTOM_HID_DeInit (USBD_HandleTypeDef  *pdev, uint8_t cfgidx);
extern uint8_t  USBD_CUSTOM_HID_Setup (USBD_HandleTypeDef  *pdev, USBD_SetupReqTypedef *req);
extern uint8_t  *USBD_CUSTOM_HID_GetCfgDesc (uint8_t speed, uint16_t *length);
extern uint8_t  USBD_CUSTOM_HID_DataIn (USBD_HandleTypeDef  *pdev, uint8_t epnum);
extern uint8_t  USBD_CUSTOM_HID_DataOut (USBD_HandleTypeDef  *pdev, uint8_t epnum);

/* Functions-------------------------------------------------------------------*/
/**
  * @brief  USBD_MSC_CustomHID_Init
  *         Initialize the MSC-HID interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_MSC_CustomHID_Init (USBD_HandleTypeDef  *pdev, uint8_t cfgidx)
{
  /* HID initialization */
  USBD_CUSTOM_HID_Init(pdev,cfgidx);
  
  /* MSC initialization */
  USBD_MSC_Init (pdev,cfgidx);
  
  return USBD_OK;
}

/**
  * @brief  USBD_MSC_CustomHID_DeInit
  *         DeInitialize the MSC_HID layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_MSC_CustomHID_DeInit (USBD_HandleTypeDef  *pdev, uint8_t cfgidx)
{
  /* HID De-initialization */
  USBD_CUSTOM_HID_DeInit (pdev,cfgidx);
  
  /* MSC De-initialization */
  USBD_MSC_DeInit (pdev,cfgidx);
  
  return USBD_OK;
}

/**
  * @brief  USBD_MSC_CustomHID_Setup
  *         Handle the MSC_HID specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  USBD_MSC_CustomHID_Setup (USBD_HandleTypeDef  *pdev, USBD_SetupReqTypedef *req)
{
  switch (req->bmRequest & USB_REQ_RECIPIENT_MASK)
  {
  case USB_REQ_RECIPIENT_INTERFACE:
    if (req->wIndex == CUSTOMHID_INTERFACE)
    {
      return (USBD_CUSTOM_HID_Setup (pdev, req));
    }
    else
    {
      return (USBD_MSC_Setup(pdev, req));
    }
    
  case USB_REQ_RECIPIENT_ENDPOINT:
    if (req->wIndex == CUSTOMHID_INTERFACE)
    {
      return (USBD_CUSTOM_HID_Setup (pdev, req));   
    }
    else
    {
      return (USBD_MSC_Setup(pdev, req));
    }
  }   
  return USBD_OK;
}

/**
  * @brief  USBD_MSC_CustomHID_GetCfgDesc 
  *         return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_MSC_CustomHID_GetCfgDesc (uint16_t *length)
{
  *length = sizeof (USBD_MSC_CustomHID_CfgDesc);
  return USBD_MSC_CustomHID_CfgDesc;
}

/**
  * @brief  USBD_MSC_CustomHID_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_MSC_CustomHID_DataIn (USBD_HandleTypeDef  *pdev, uint8_t epnum)
{
   /*DataIN can be for MSC or CustomHID */
   
   //if (epnum == (MSC_EPIN_ADDR & (~0x80)))
   if (tans_ch != CUSTOMHID_INTERFACE)
   {
      return (USBD_MSC_DataIn(pdev, epnum));
   }
   else
   {
      return (USBD_CUSTOM_HID_DataIn(pdev, epnum));
   }
}

/**
  * @brief  USBD_MSC_CustomHID_DataOut
  *         handle data OUT Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_MSC_CustomHID_DataOut(USBD_HandleTypeDef  *pdev, uint8_t epnum)
{
   /*DataIN can be for MSC or CustomHID */
   //if (epnum == MSC_EPOUT_ADDR)
   if (tans_ch != CUSTOMHID_INTERFACE)
   {
      return (USBD_MSC_DataOut(pdev, epnum));
   }
   else // CustomHid (epnum == CUSTOM_HID_EPOUT_ADDR)
   {
      return (USBD_CUSTOM_HID_DataOut(pdev, epnum));
   }
}

static uint8_t  *USBD_MSC_CustomHID_GetOtherSpeedCfgDesc(uint16_t *length)
{
   *length = sizeof (USBD_MSC_CustomHID_CfgDesc);
   return USBD_MSC_CustomHID_CfgDesc;
}
static uint8_t  *USBD_MSC_CustomHID_GetDeviceQualifierDescriptor(uint16_t *length)
{
   *length = sizeof (USBD_MSC_CustomHID_DeviceQualifierDesc);
   return USBD_MSC_CustomHID_DeviceQualifierDesc;
}


void MSC_HID_CtrlAdr(uint8_t channel)
{
   tans_ch = channel;
}
/*****************************END OF FILE****/
