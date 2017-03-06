/* Includes ------------------------------------------------------------------*/
#include "BoardBsp.h"
#include "usbd_customhid_if.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_msc.h"
#include "usbd_customhid.h"
#include "usbd_customhid_if.h"
#include "LCP_Port.h"
#include "usb_core.h"


/* Private variables ---------------------------------------------------------*/
#define STX       2
#define ADAPTOR_TYPE 0x55
uint8_t eFcState = IDLE;
uint16_t wRxLength = 0;
uint32_t ms_tick = 0;

extern USBD_HandleTypeDef  USBD_Device;
extern uint8_t byTxBuffer[FCBUFFER_SIZE];
extern uint8_t byRxBuffer[FCBUFFER_SIZE];
extern UART_HandleTypeDef huart3;
extern BOOLEAN bConnectWithVFD;
uint8_t byUsbSendReportBuf[FCBUFFER_SIZE];

static UNSIGNED8 byLCPPortAPP[7] = {0xE9, 0x7, 0x00, 0x55, 0x00, 0x00, 0x00};
static uint8_t byPolling[3] = {0xE9, 0x05, 0x16};
#define RS485_DIR_TR          HAL_GPIO_WritePin(RS485_DIR_GPIO_Port, RS485_DIR_Pin, GPIO_PIN_SET);
#define RS485_DIR_RE          HAL_GPIO_WritePin(RS485_DIR_GPIO_Port, RS485_DIR_Pin, GPIO_PIN_RESET);

static uint32_t tickDelay = 0;
   static uint16_t readIdx = 0;
/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
UNSIGNED8 LCPBUS_vCalcBcc(const UNSIGNED8 *pBuf, UNSIGNED8 telegram_length)
{
    UNSIGNED8 bcc = 0;
    UNSIGNED8 i;

   for (i=0; i<telegram_length; i++)
   {
       bcc ^= pBuf[i];
   }

   return bcc;
}

BOOLEAN LCPBUS_vCheckBcc(UNSIGNED8 *pBuf, UNSIGNED8 telegram_length)
{
   if (pBuf[telegram_length] == LCPBUS_vCalcBcc(pBuf, telegram_length))
   {
      return TRUE;
   }
   else
   {
      return FALSE;
   }
}

/* Private function prototypes -----------------------------------------------*/
void LCPBUS_Test(void)
{
   memcpy(byTxBuffer, "LCP ok", 6);
   LCPBUS_vSend(6);
}

void LCPBUS_vSend(uint16_t wTxLength)
{
   RS485_DIR_TR;
   HAL_HalfDuplex_EnableTransmitter(&huart3);
   HAL_UART_Transmit_IT(&huart3, byTxBuffer, wTxLength);
}

void LCPBUS_vRecieve(void)
{
   memset(byRxBuffer, 0xFF, 100);
   
   RS485_DIR_RE;
   eFcState = IDLE;
   HAL_HalfDuplex_EnableReceiver(&huart3);
   HAL_UART_Receive_IT(&huart3, (uint8_t *)byRxBuffer, 5/*2*/); //STX & LGE
}

void LCPPORT_vRecieve(UNSIGNED8 byLength)
{
   memset(byRxBuffer, 0, 100);
   
   RS485_DIR_RE;
   
   HAL_HalfDuplex_EnableReceiver(&huart3);
   HAL_UART_Receive_IT(&huart3, (uint8_t *)byRxBuffer, byLength); //STX & LGE
}

void LCPBUS_vSetPort(uint32_t baudRate, uint32_t parity, uint32_t stopBit)
{
//  HAL_UART_DeInit(&huart3);
  huart3.Instance = USART3;
  huart3.Init.BaudRate = baudRate;//19200;
  if (parity == UART_PARITY_NONE)
  {
     huart3.Init.WordLength = UART_WORDLENGTH_8B;
  }
  else
  {
      huart3.Init.WordLength = UART_WORDLENGTH_9B;  //TBD, 8B or 9B
  }
  huart3.Init.StopBits = stopBit;
  huart3.Init.Parity = parity;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart3);
}

UNSIGNED8 LCPPORT_AppCheckSum(const UNSIGNED8 *pBuf, UNSIGNED8 byLen)
{
   UNSIGNED8 byChecksum = 0, byIdx;

   for (byIdx = 0; byIdx < byLen; byIdx++)
   {
      byChecksum += pBuf[byIdx];
   }

   return byChecksum;
}
//---------------------------------------------------------------------------
//  Name     :        LCPBUS_vRxISR / LCPBUS_vTxISR
//                  ======================================
//  Function : Receive/Transmit Interrupt handlers
//---------------------------------------------------------------------------
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
   if (!bConnectWithVFD)
   {
      LCPPORT_vRecieve(5);
   }
   else
   {
      LCPBUS_vRecieve();
   }
}

extern uint32_t dwCounter;
static uint8_t CheckPolling(void)
{
   uint8_t len = 0;
   uint8_t ret = 1;

   if (strstr((const char *)byRxBuffer, "LCPTM") != 0)
   {
      memcpy(byTxBuffer, "LCPok", 5);
      byTxBuffer[5] = 0x0A;
      len = 6;
      LCPBUS_vSend(len);
      bConnectWithVFD = TRUE;
      eFcState = IDLE;
   }
   else if (strstr((const char *)byRxBuffer, "mal") != 0)
   {
      LCPBUS_vSetPort(19200, UART_PARITY_EVEN, UART_STOPBITS_1);
      dwCounter = 0;
   }
   else if (memcmp(byPolling, byRxBuffer, 3) == 0)
   {
      memcpy(byTxBuffer, byLCPPortAPP, 6);
      byTxBuffer[3] = ADAPTOR_TYPE;               // This is Adaptor
      byTxBuffer[6] = LCPPORT_AppCheckSum(byTxBuffer, 6);
      len = 7;
      LCPBUS_vSend(len);
      eFcState = APP_CHECK;
   }
   else
   {
      ret = 0;
   }
   
   return ret;
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
   uint8_t len = 0;
   uint16_t idx = 0;
   
   switch((UNSIGNED8)eFcState)
   {
      case IDLE :
         if (byRxBuffer[0] != STX)
         {
            if (!CheckPolling())
            {
               while (!(byRxBuffer[idx] == 'R' || byRxBuffer[idx] == 'r' || byRxBuffer[idx] == 0x0A || byRxBuffer[idx] == 0x0D)
                     && idx < 5)
               {
                  idx++;
               }
               if (idx < 5)
               {
                  eFcState = READ_ONE;
                  readIdx = 5-idx;
                  HAL_UART_Receive_IT(&huart3, (uint8_t *)(byRxBuffer + readIdx), 1);
               }
               else
               {
                  LCPBUS_vRecieve();
               }
            }
            tickDelay = ms_tick;
         }
         else
         {
            wRxLength = byRxBuffer[1];
            eFcState = READ_BODY;
            HAL_UART_Receive_IT(&huart3, (uint8_t *)(byRxBuffer + 5), (wRxLength-3));
            tickDelay = ms_tick;
         }
         break;
   
      case READ_BODY: // receive rest of long telegram
         if (LCPBUS_vCheckBcc(byRxBuffer, wRxLength+1))// Include STX+LEN
         {
            eFcState = IDLE;
            USB_Respone(byRxBuffer, wRxLength+2);
            tickDelay = ms_tick;
         }
         else
         {
            eFcState = CRC_ERROR;
         }
         break;
      case READ_ONE:
         {
            readIdx++;
            HAL_UART_Receive_IT(&huart3, (uint8_t *)(byRxBuffer + readIdx), HIDDTransferDriver_REPORTSIZE-2);
            tickDelay = ms_tick;
         }
         break;
      case APP_POLL:
         if (!CheckPolling())
         {
            LCPBUS_vRecieve();
         }
//         if (strstr((const char *)byRxBuffer, "LCPTM") != 0)
//         {
//            memcpy(byTxBuffer, "LCPok", 5);
//            byTxBuffer[5] = 0x0A;
//            len = 6;
//            LCPBUS_vSend(len);
//            bConnectWithVFD = TRUE;
//            eFcState = IDLE;
//         }
//         else if (strstr((const char *)byRxBuffer, "mal") != 0)
//         {
//            LCPBUS_vSetPort(19200, UART_PARITY_EVEN, UART_STOPBITS_1);
//            dwCounter = 0;
//         }
//         else
//         {
//            memcpy(byTxBuffer, byLCPPortAPP, 6);
//            byTxBuffer[3] = ADAPTOR_TYPE;               // This is Adaptor
//            byTxBuffer[6] = LCPPORT_AppCheckSum(byTxBuffer, 6);
//            len = 7;
//            LCPBUS_vSend(len);
//            eFcState = APP_CHECK;
//         }
         break;
      case APP_CHECK:
         if (byRxBuffer[0] == 0xE9)
         {
            if (byRxBuffer[3] == ADAPTOR_TYPE)
            {
               HAL_UART_DeInit(&huart3);
               LCPBUS_vSetPort(115200, UART_PARITY_NONE, UART_STOPBITS_1);
               bConnectWithVFD = TRUE;
//               HAL_GPIO_WritePin(LED_ALARM_GPIO_Port, LED_ALARM_Pin, GPIO_PIN_SET);
               eFcState = IDLE;
            }
            else
            {
               memcpy(byTxBuffer, byLCPPortAPP, 6);
               byTxBuffer[3] = ADAPTOR_TYPE;               // This is Adaptor
               byTxBuffer[6] = LCPPORT_AppCheckSum(byTxBuffer, 6);
               len = 7;
               LCPBUS_vSend(len);
            }
          }
         else
         {
            eFcState = APP_POLL;
         }
         break;
      default:
         break;
   }
}

void MS_TickCheck(void)
{
   uint8_t tmLength = 0;
   ms_tick++;
   if (eFcState == READ_ONE)
   {
      if((ms_tick-tickDelay) > 10)
      {
         if (huart3.RxXferCount+5 > HIDDTransferDriver_REPORTSIZE-2)
         {
            tmLength = HIDDTransferDriver_REPORTSIZE-2;
         }
         else
         {
            tmLength = huart3.RxXferCount+5;
         }
         USB_Respone(byRxBuffer, tmLength);//readIdx);
         readIdx = 0;
         eFcState = IDLE;
         tickDelay = ms_tick;
      }
   }
   else if (eFcState == READ_BODY || eFcState == IDLE)
   {
      if ((ms_tick - tickDelay) > 10000)
      {
         readIdx = 0;
         eFcState = IDLE;
         tickDelay = ms_tick;
      }
   }
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
