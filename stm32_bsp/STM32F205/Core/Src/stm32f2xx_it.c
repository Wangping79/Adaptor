/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"
#include "stm32f2xx.h"
#include "stm32f2xx_it.h"
#include "cmsis_os.h"
#include "BoardBsp.h"
#include "LCP_Port.h"

/* External variables --------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
   Error_Handler(1);
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
 Error_Handler(2);
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
   Error_Handler(3);
}

/**
* @brief This function handles Prefetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
  Error_Handler(4);
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
  Error_Handler(5);
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
   
}

/**
* @brief This function handles System tick timer.
*/
extern uint32_t ms_tick;
void SysTick_Handler(void)
{
  osSystickHandler();
  MS_TickCheck();
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/
/**
  * @brief  This function handles USB-On-The-Go FS global interrupt request.
  * @param  None
  * @retval None
  */
void OTG_FS_IRQHandler(void)
{
//  HAL_HCD_IRQHandler(&hhcd);
  HAL_PCD_IRQHandler(&hpcd);
}

/**
  * @brief  This function handles USB OTG FS or HS Wakeup IRQ Handler.
  * @param  None
  * @retval None
  */  
void OTG_FS_WKUP_IRQHandler(void)
{
  if((&hpcd)->Init.low_power_enable)
  {
    /* Reset SLEEPDEEP bit of Cortex System Control Register */
    SCB->SCR &= (uint32_t)~((uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk));  
    
    /* Configures system clock after wake-up from STOP: enable HSE, PLL and select 
    PLL as system clock source (HSE and PLL are disabled in STOP mode) */
    
    __HAL_RCC_HSE_CONFIG(RCC_HSE_ON);
    
    /* Wait till HSE is ready */  
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_HSERDY) == RESET)
    {}
    
    /* Enable the main PLL. */
    __HAL_RCC_PLL_ENABLE();
    
    /* Wait till PLL is ready */  
    while(__HAL_RCC_GET_FLAG(RCC_FLAG_PLLRDY) == RESET)
    {}
    
    /* Select PLL as SYSCLK */
    MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_SYSCLKSOURCE_PLLCLK);
    
    while (__HAL_RCC_GET_SYSCLK_SOURCE() != RCC_CFGR_SWS_PLL)
    {}
    
    /* ungate PHY clock */
    __HAL_PCD_UNGATE_PHYCLOCK((&hpcd)); 
  }
  /* Clear EXTI pending Bit*/
  __HAL_USB_OTG_FS_WAKEUP_EXTI_CLEAR_FLAG();

  /* Clear EXTI pending Bit*/
  __HAL_USB_OTG_HS_WAKEUP_EXTI_CLEAR_FLAG();  
}
/**
* @brief This function handles USART3 global interrupt.
*/
void USART3_IRQHandler(void)
{
  HAL_UART_IRQHandler(&huart3);
}

/**
* @brief This function handles TIM6 global interrupt.
*/
void TIM6_DAC_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim6);
}

void DMA1_Stream1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart3_rx);
}

/**
* @brief This function handles DMA1 stream3 global interrupt.
*/
void DMA1_Stream3_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_usart3_tx);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
