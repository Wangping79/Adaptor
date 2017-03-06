/* Includes ------------------------------------------------------------------*/
//#include "stm32f1xx_hal.h"
#include "cmsis_os.h"
#include "ff.h"
#include "BoardBsp.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2_nor ;
SPI_HandleTypeDef hspi1_BlueBRG;
TIM_HandleTypeDef htim2;
UART_HandleTypeDef huart3;
ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_usart3_tx;
DMA_HandleTypeDef hdma_usart3_rx;
/* Private variables ---------------------------------------------------------*/
static uint8_t error_debug = 0;
static uint8_t USB_CurMode = USB_OTG_MODE_DEVICE;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void GPIO_Init(void);
static void SPI1_Init(void);
static void SPI2_Init(void);
static void TIM2_Init(void);
static void ADC1_Init(void);

void BspInit(void)
{
  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
  GPIO_Init();
  SPI1_Init();
  SPI2_Init();
  TIM2_Init();

  ADC1_Init();
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 192;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 8;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler(6);
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler(7);
  }
  
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

/* SPI2 init function */
static void SPI2_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  /* Configurare GPIO of SPI, including SCK, MISO, MOSI */
  GPIO_InitStruct.Pin = NOR_CLK_Pin|NOR_DATAOUT_Pin|NOR_DATAIN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  //Configurate selection pin(CS) 
  GPIO_InitStruct.Pin = NOR_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
   
  hspi2_nor.Instance = SPI2;
  hspi2_nor.Init.Mode = SPI_MODE_MASTER;
  hspi2_nor.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2_nor.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2_nor.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2_nor.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2_nor.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi2_nor.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2_nor.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2_nor.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2_nor.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2_nor.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi2_nor ) != HAL_OK)
  {
    Error_Handler(8);
  }
}

/* SPI1 init function */
static void SPI1_Init(void)
{
   GPIO_InitTypeDef GPIO_InitStruct;
  /* Configurare GPIO of SPI, including SCK, MISO, MOSI */
  GPIO_InitStruct.Pin = BlueNRG_CLK_Pin|BlueNRG_MISO_Pin|BlueNRG_MOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  //Configurate selection pin(CS) 
  GPIO_InitStruct.Pin = BlueNRG_CSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
   
  hspi1_BlueBRG.Instance = SPI1;
  hspi1_BlueBRG.Init.Mode = SPI_MODE_MASTER;
  hspi1_BlueBRG.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1_BlueBRG.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1_BlueBRG.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1_BlueBRG.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1_BlueBRG.Init.NSS = SPI_NSS_HARD_OUTPUT;
  hspi1_BlueBRG.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1_BlueBRG.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1_BlueBRG.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1_BlueBRG.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1_BlueBRG.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1_BlueBRG) != HAL_OK)
  {
    Error_Handler(9);
  }

}

/* TIM2 init function */
static void TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_SlaveConfigTypeDef sSlaveConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler(10);
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler(11);
  }

  if (HAL_TIM_OC_Init(&htim2) != HAL_OK)
  {
    Error_Handler(12);
  }

  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  if (HAL_TIM_SlaveConfigSynchronization(&htim2, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler(13);
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler(14);
  }

  sConfigOC.OCMode = TIM_OCMODE_TIMING;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler(15);
  }

}

/* ADC1 init function */
static void ADC1_Init(void)
{

  ADC_ChannelConfTypeDef sConfig;

    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler(17);
  }

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_14;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler(18);
  }
   
  HAL_ADC_Start(&hadc1);
}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
     PA9   ------> USB_OTG_FS_VBUS
     PA10   ------> USB_OTG_FS_ID
*/
static void GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LED_ALARM_Pin|LCD_LED_Pin|LED_OFF_Pin|VBUS_EN_Pin 
                     |LED_WARNIG_Pin|LED_ON_Pin|LED_HAND_Pin|KEY_OUT2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_RST_Pin|LCD_RW_Pin|LCD_RD_Pin|LCD_A0_Pin 
                   |LED_AUTO_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_DATA0_Pin|LCD_DATA1_Pin|LCD_DATA2_Pin|KEY_OUT3_Pin 
                          |KEY_OUT4_Pin|LCD_DATA3_Pin|LCD_DATA4_Pin|LCD_DATA5_Pin 
                          |LCD_DATA6_Pin|LCD_DATA7_Pin|RS485_DIR_Pin|LCD_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED_ALARM_Pin LCD_LED_Pin LED_OFF_Pin VBUS_EN_Pin 
                           LED_WARNIG_Pin LED_ON_Pin LED_HAND_Pin KEY_OUT2_Pin */
  GPIO_InitStruct.Pin = LED_ALARM_Pin|LED_OFF_Pin|VBUS_EN_Pin 
                          |LED_WARNIG_Pin|LED_ON_Pin|LED_HAND_Pin|KEY_OUT2_Pin|LCD_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : KEY_IN1_Pin KEY_IN2_Pin KEY_IN3_Pin KEY_IN4_Pin */
  GPIO_InitStruct.Pin = KEY_IN1_Pin|KEY_IN2_Pin|KEY_IN3_Pin|KEY_IN4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;//GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RST_Pin LCD_RW_Pin LCD_RD_Pin LCD_A0_Pin 
                           LED_AUTO_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin|LCD_RW_Pin|LCD_RD_Pin|LCD_A0_Pin 
                          |LED_AUTO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : VBUS_AI_Pin */
  GPIO_InitStruct.Pin = VBUS_AI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(VBUS_AI_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_DATA0_Pin LCD_DATA1_Pin LCD_DATA2_Pin KEY_OUT3_Pin 
                           KEY_OUT4_Pin LCD_DATA3_Pin LCD_DATA4_Pin LCD_DATA5_Pin 
                           LCD_DATA6_Pin LCD_DATA7_Pin RS485_DIR_Pin LCD_CS_Pin */
  GPIO_InitStruct.Pin = LCD_DATA0_Pin|LCD_DATA1_Pin|LCD_DATA2_Pin|KEY_OUT3_Pin 
                          |KEY_OUT4_Pin|LCD_DATA3_Pin|LCD_DATA4_Pin|LCD_DATA5_Pin 
                          |LCD_DATA6_Pin|LCD_DATA7_Pin|RS485_DIR_Pin|LCD_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PA9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA10 */
  GPIO_InitStruct.Pin = GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY_OUT1_Pin */
  GPIO_InitStruct.Pin = KEY_OUT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(KEY_OUT1_GPIO_Port, &GPIO_InitStruct);
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM6) 
  {
    HAL_IncTick();
  }
}

void BSP_SetUSBMode(uint8_t mode)
{
   USB_CurMode = mode;
}
uint8_t BSP_GetUSBMode(void)
{
   return USB_CurMode;
}
/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(uint8_t error_s)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  error_debug = error_s;
  while(1) 
  {
     osDelay(2);
     error_debug = error_debug;
  }
  /* USER CODE END Error_Handler */ 
}


/*****************************END OF FILE****/
