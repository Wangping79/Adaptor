
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USBD_CUSTOMHID_IF_H
#define __USBD_CUSTOMHID_IF_H

/* Includes ------------------------------------------------------------------*/
#include "usbd_customhid.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define FCBUFFER_SIZE   128
//------------------Here is defination for Danfoss-----------------------------
// Physical collection.
#define HIDReport_COLLECTION_PHYSICAL           0x00
// Application collection.
#define HIDReport_COLLECTION_APPLICATION        0x01
// Logical collection.
#define HIDReport_COLLECTION_LOGICAL            0x02
// Report collection.
#define HIDReport_COLLECTION_REPORT             0x03
// Named array collection.
#define HIDReport_COLLECTION_NAMEDARRAY         0x04
// Usage switch collection.
#define HIDReport_COLLECTION_USAGESWITCH        0x05
// Usage modifier collection
#define HIDReport_COLLECTION_USAGEMODIFIER      0x06

/******* Size *******/
// Size of the report descriptor in bytes.
#define HIDDTransferDriverDescriptors_REPORTSIZE         56
// Size of the input and output report, in bytes
#define HIDDTransferDriver_REPORTSIZE                    64

/***** Item Type ***/
// Input item.
#define HIDReport_INPUT                         0x81
// Output item.
#define HIDReport_OUTPUT                        0x91
// Feature item.
#define HIDReport_FEATURE                       0xB1
// Collection item.
#define HIDReport_COLLECTION                    0xA1
// End of collection item.
#define HIDReport_ENDCOLLECTION                 0xC0

// Current usage page.
#define HIDReport_GLOBAL_USAGEPAGE              0x06
// Minimum value that a variable or array item will report.
#define HIDReport_GLOBAL_LOGICALMINIMUM         0x15
// Maximum value that a variable or array item will report.
#define HIDReport_GLOBAL_LOGICALMAXIMUM         0x25
// Minimum value for the physical extent of a variable item.
#define HIDReport_GLOBAL_PHYSICALMINIMUM        0x34
// Maximum value for the physical extent of a variable item.
#define HIDReport_GLOBAL_PHYSICALMAXIMUM        0x44
// Value of the unit exponent in base 10.
#define HIDReport_GLOBAL_UNITEXPONENT           0x54
// Unit values.
#define HIDReport_GLOBAL_UNIT                   0x64
// Size of the report fields in bits.
#define HIDReport_GLOBAL_REPORTSIZE             0x75
// Specifies the report ID.
#define HIDReport_GLOBAL_REPORTID               0x85
// Number of data fields for an item.
#define HIDReport_GLOBAL_REPORTCOUNT            0x95
// Places a copy of the global item state table on the stack.
#define HIDReport_GLOBAL_PUSH                   0xA4
// Replaces the item state table with the top structure from the stack.
#define HIDReport_GLOBAL_POP                    0xB4

// Suggested usage for an item or collection.
#define HIDReport_LOCAL_USAGE                   0x09
// Defines the starting usage associated with an array or bitmap.
#define HIDReport_LOCAL_USAGEMINIMUM            0x18
// Defines the ending usage associated with an array or bitmap.
#define HIDReport_LOCAL_USAGEMAXIMUM            0x28
// Determines the body part used for a control.
#define HIDReport_LOCAL_DESIGNATORINDEX         0x38
// Defines the index of the starting designator associated with an array or
// bitmap.
#define HIDReport_LOCAL_DESIGNATORMINIMUM       0x48
// Defines the index of the ending designator associated with an array or
// bitmap.
#define HIDReport_LOCAL_DESIGNATORMAXIMUM       0x58
// String index for a string descriptor.
#define HIDReport_LOCAL_STRINGINDEX             0x78
// Specifies the first string index when assigning a group of sequential
// strings to controls in an array or bitmap.
#define HIDReport_LOCAL_STRINGMINIMUM           0x88
// Specifies the last string index when assigning a group of sequential
// strings to controls in an array or bitmap.
#define HIDReport_LOCAL_STRINGMAXIMUM           0x98
// Defines the beginning or end of a set of local items.
#define HIDReport_LOCAL_DELIMITER               0xA8

/***** HID Report ID for Danfoss ****/
//GetFeature: Return packetCount
#define HID_REPORTID_PACKET_COUNT      0x01
//GetFeature: Information on internal FIFOs
#define HID_REPORTID_INFORMATION       0x04

/***** Report Type ******/
// Input report.
#define HIDReportRequest_INPUT                  1
// Output report.
#define HIDReportRequest_OUTPUT                 2
// Feature report.
#define HIDReportRequest_FEATURE                3

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern USBD_CUSTOM_HID_ItfTypeDef USBD_CustomHID_fops;

#endif /* __USBD_CUSTOMHID_IF_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
