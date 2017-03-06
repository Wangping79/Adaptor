/* Includes ------------------------------------------------------------------*/
#include "Type.h"
#include "BoardBsp.h"

/* Private variables ---------------------------------------------------------*/
#define IDLE      0
#define READ_BODY 1
#define CRC_ERROR 2
#define APP_POLL  3
#define APP_CHECK 4
#define READ_ONE  5
/* Private variables ---------------------------------------------------------*/
uint8_t LCPBUS_vCalcBcc(const UNSIGNED8 *pBuf, UNSIGNED8 telegram_length);

BOOLEAN LCPBUS_vCheckBcc(UNSIGNED8 *pBuf, UNSIGNED8 telegram_length);
//void USB_Respone(BYTE *pBuf, BYTE length);
void LCPBUS_vSend(uint16_t wTxLength);

void LCPBUS_vRecieve(void);

void LCPBUS_vSetPort(uint32_t baudRate, uint32_t parity, uint32_t stopBit);

UNSIGNED8 LCPPORT_AppCheckSum(const UNSIGNED8 *pBuf, UNSIGNED8 byLen);

void LCPPORT_vRecieve(UNSIGNED8 byLength);

void LCPBUS_Test(void);

void MS_TickCheck(void);
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
