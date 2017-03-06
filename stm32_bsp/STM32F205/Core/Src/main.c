/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"
#include "BoardBsp.h"
#include "LCP_Port.h"
#include "USB_Core.h"

//---------------Private Defination---------------------------------
BOOLEAN bConnectWithVFD = FALSE;
extern uint8_t eFcState;
static osTimerId CTRLCore_timer;
/*static */uint32_t dwCounter = 0;

#define POLLING_TICK  20
#define DELAY_1S      (1000/POLLING_TICK)
#define DELAY_2S      (2000/POLLING_TICK)
static void CoreTimeRoutine(void const *n)
{
//  for(;;)
  {
     if (!bConnectWithVFD)
     {
         if (eFcState != APP_CHECK)
            eFcState = APP_POLL;
         
         dwCounter++;
         if (dwCounter == DELAY_1S)
         {
           LCPBUS_vSetPort(19200, UART_PARITY_EVEN, UART_STOPBITS_1);
            eFcState = APP_POLL;
         }
         else if (dwCounter == DELAY_2S)
         {
            LCPBUS_vSetPort(115200, UART_PARITY_NONE, UART_STOPBITS_1);
            dwCounter = 0;
            eFcState = APP_POLL;
         }
         LCPPORT_vRecieve(5);
     }
     else
     {
        HAL_GPIO_WritePin(LED_ALARM_GPIO_Port, LED_ALARM_Pin, GPIO_PIN_SET);
     }
  }
}

//---------------Main Entry----------------------------------------
int main(void)
{
   BspInit();
   
   dwCounter = 0;
   HAL_GPIO_WritePin(LED_ON_GPIO_Port, LED_ON_Pin, GPIO_PIN_SET);
   LCPBUS_vSetPort(115200, UART_PARITY_NONE, UART_STOPBITS_1);
   
//   LCPBUS_vSetPort(19200, UART_PARITY_EVEN, UART_STOPBITS_1);
   

   USB_vInit();

   osTimerDef(CTRL_Timer, CoreTimeRoutine);
   CTRLCore_timer =  osTimerCreate(osTimer(CTRL_Timer), osTimerPeriodic, (void *)0);

   /* Start the TS Timer */
   osTimerStart(CTRLCore_timer, POLLING_TICK);

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  for( ;; );
}


void HAL_Delay (__IO uint32_t Delay)
{
  while(Delay) 
  {
    if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) 
    {
      Delay--;
    }
  }
}

/*****************************END OF FILE****/
