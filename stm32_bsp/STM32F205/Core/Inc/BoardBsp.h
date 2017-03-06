/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef BOARDBAP_H
#define BOARDBAP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"
#include "stm32f2xx_hal_conf.h"
#include "usbd_def.h"
//#include "usbh_def.h"
#include "mxconstants.h" 
/* Exported types ------------------------------------------------------------*/
    
extern PCD_HandleTypeDef hpcd;
    
extern HCD_HandleTypeDef hhcd;
    
extern TIM_HandleTypeDef htim6;

extern HCD_HandleTypeDef hhcd;

extern PCD_HandleTypeDef hpcd;
    
extern USBD_HandleTypeDef USBD_HID;

extern USBD_HandleTypeDef USBD_SMC;

extern SPI_HandleTypeDef hspi2_nor;

extern SPI_HandleTypeDef hspi1_BlueBRG;

extern TIM_HandleTypeDef htim2;

extern UART_HandleTypeDef huart3;

extern ADC_HandleTypeDef hadc1;

//extern USBH_HandleTypeDef hUSB_Host;

extern DMA_HandleTypeDef hdma_usart3_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;

/* Exported constants --------------------------------------------------------*/
#if BLE_CONFIG_DBG_ENABLE
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#define SYSCLK_FREQ     96000000
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */ 
void BspInit(void);
void BSP_SetUSBMode(uint8_t mode);
uint8_t BSP_GetUSBMode(void);

void Error_Handler(uint8_t error_s);

void Enable_SPI_IRQ(void);
void Disable_SPI_IRQ(void);
void Clear_SPI_IRQ(void);
void Clear_SPI_EXTI_Flag(void);

#ifdef __cplusplus
}
#endif

#endif /*BOARDBAP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
