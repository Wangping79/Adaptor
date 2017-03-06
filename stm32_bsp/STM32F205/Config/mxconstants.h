/**
  ******************************************************************************
  * File Name          : mxconstants.h
  * Description        : This file contains the common defines of the application
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MXCONSTANT_H
#define __MXCONSTANT_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
#define LED_ALARM_Pin               GPIO_PIN_13
#define LED_ALARM_GPIO_Port         GPIOC
#define LCD_LED_Pin                 GPIO_PIN_14
#define LCD_LED_GPIO_Port           GPIOC
#define LED_OFF_Pin                 GPIO_PIN_15
#define LED_OFF_GPIO_Port           GPIOC
#define KEY_IN1_Pin                 GPIO_PIN_0
#define KEY_IN1_GPIO_Port           GPIOC
#define KEY_IN2_Pin                 GPIO_PIN_1
#define KEY_IN2_GPIO_Port           GPIOC
#define KEY_IN3_Pin                 GPIO_PIN_2
#define KEY_IN3_GPIO_Port           GPIOC
#define KEY_IN4_Pin                 GPIO_PIN_3
#define KEY_IN4_GPIO_Port           GPIOC
#define LCD_RST_Pin                 GPIO_PIN_0
#define LCD_RST_GPIO_Port           GPIOA
#define LCD_RW_Pin                  GPIO_PIN_1
#define LCD_RW_GPIO_Port            GPIOA
#define LCD_RD_Pin                  GPIO_PIN_2
#define LCD_RD_GPIO_Port            GPIOA
#define LCD_A0_Pin                  GPIO_PIN_3
#define LCD_A0_GPIO_Port            GPIOA

#define LCD_DATA0_Pin               GPIO_PIN_0
#define LCD_DATA0_GPIO_Port         GPIOB
#define LCD_DATA1_Pin               GPIO_PIN_1
#define LCD_DATA1_GPIO_Port         GPIOB
#define LCD_DATA2_Pin               GPIO_PIN_2
#define LCD_DATA2_GPIO_Port         GPIOB
#define KEY_OUT3_Pin                GPIO_PIN_10
#define KEY_OUT3_GPIO_Port          GPIOB
#define KEY_OUT4_Pin                GPIO_PIN_11
#define KEY_OUT4_GPIO_Port          GPIOB
#define LED_WARNIG_Pin              GPIO_PIN_7
#define LED_WARNIG_GPIO_Port        GPIOC
#define LED_ON_Pin                  GPIO_PIN_8
#define LED_ON_GPIO_Port            GPIOC
#define LED_HAND_Pin                GPIO_PIN_9
#define LED_HAND_GPIO_Port          GPIOC
#define LED_AUTO_Pin                GPIO_PIN_15
#define LED_AUTO_GPIO_Port          GPIOA
#define KEY_OUT2_Pin                GPIO_PIN_12
#define KEY_OUT2_GPIO_Port          GPIOC
#define KEY_OUT1_Pin                GPIO_PIN_2
#define KEY_OUT1_GPIO_Port          GPIOD
#define LCD_DATA3_Pin               GPIO_PIN_3
#define LCD_DATA3_GPIO_Port         GPIOB
#define LCD_DATA4_Pin               GPIO_PIN_4
#define LCD_DATA4_GPIO_Port         GPIOB
#define LCD_DATA5_Pin               GPIO_PIN_5
#define LCD_DATA5_GPIO_Port         GPIOB
#define LCD_DATA6_Pin               GPIO_PIN_6
#define LCD_DATA6_GPIO_Port         GPIOB
#define LCD_DATA7_Pin               GPIO_PIN_7
#define LCD_DATA7_GPIO_Port         GPIOB
#define RS485_DIR_Pin               GPIO_PIN_8
#define RS485_DIR_GPIO_Port         GPIOB
#define LCD_CS_Pin                  GPIO_PIN_9
#define LCD_CS_GPIO_Port            GPIOB

#define LCD_DATA_PORT               GPIOB
#define LCD_CTRL_PORT               GPIOA

#define BlueNRG_CSI_Pin             GPIO_PIN_4
#define BlueNRG_CSI_GPIO_Port       GPIOA
#define BlueNRG_CLK_Pin             GPIO_PIN_5
#define BlueNRG_CLK_GPIO_Port       GPIOA
#define BlueNRG_MISO_Pin            GPIO_PIN_6
#define BlueNRG_MISO_GPIO_Port      GPIOA
#define BlueNRG_MOSI_Pin            GPIO_PIN_7
#define BlueNRG_MOSI_GPIO_Port      GPIOA
#define NOR_CS_Pin                  GPIO_PIN_12
#define NOR_CS_GPIO_Port            GPIOB
#define NOR_CLK_Pin                 GPIO_PIN_13
#define NOR_CLK_GPIO_Port           GPIOB
#define NOR_DATAOUT_Pin             GPIO_PIN_14
#define NOR_DATAOUT_GPIO_Port       GPIOB
#define NOR_DATAIN_Pin              GPIO_PIN_15
#define NOR_DATAIN_GPIO_Port        GPIOB
#define NOR_WPROCT_Pin              GPIO_PIN_6
#define NOR_WPROCT_GPIO_Port        GPIOC

#define VBUS_AI_Pin                 GPIO_PIN_4
#define VBUS_AI_GPIO_Port           GPIOC
#define VBUS_EN_Pin                 GPIO_PIN_5
#define VBUS_EN_GPIO_Port           GPIOC

#define USB_OTG_HS_ID               GPIO_PIN_10
#define USB_OTG_HS_VBUS             GPIO_PIN_9
#define USB_OTG_HS_DM               GPIO_PIN_11
#define USB_OTG_HS_DP               GPIO_PIN_12
#define USB_OTG_GPIO_Port           GPIOA

#define GPIO_AF_OTG_FS              GPIO_AF10_OTG_FS

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MXCONSTANT_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
