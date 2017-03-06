/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_hal.h"

extern void Error_Handler(void);

extern DMA_HandleTypeDef hdma_usart3_tx;

extern DMA_HandleTypeDef hdma_usart3_rx;

/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{
  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

  /* System interrupt init*/
  /* MemoryManagement_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
  /* BusFault_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
  /* UsageFault_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
  /* SVCall_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
  /* DebugMonitor_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
  /* PendSV_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(PendSV_IRQn, 15, 0);
  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hadc->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();
  
    /**ADC1 GPIO Configuration    
    PC4     ------> ADC1_IN14 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* USER CODE BEGIN ADC1_MspInit 1 */

  /* USER CODE END ADC1_MspInit 1 */
  }
}
void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{

  if(hadc->Instance==ADC1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();
  
    /**ADC1 GPIO Configuration    
    PC4     ------> ADC1_IN14 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_4);
  }
}
void HAL_SPI_MspInit(SPI_HandleTypeDef* hspi)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(hspi->Instance==SPI1)
  {
    /* Peripheral clock enable */
    __HAL_RCC_SPI1_CLK_ENABLE();
  
    /**SPI1 GPIO Configuration    
    PA4     ------> SPI1_NSS
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI 
    */
    GPIO_InitStruct.Pin = BlueNRG_CSI_Pin|BlueNRG_CLK_Pin|BlueNRG_MISO_Pin|BlueNRG_MOSI_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
  else if(hspi->Instance==SPI2)
  {
    /* Peripheral clock enable */
    __HAL_RCC_SPI2_CLK_ENABLE();
  
    /**SPI2 GPIO Configuration    
    PB13     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI 
    */
    GPIO_InitStruct.Pin = NOR_CLK_Pin|NOR_DATAOUT_Pin|NOR_DATAIN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }

}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* hspi)
{
  if(hspi->Instance==SPI1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_SPI1_CLK_DISABLE();
  
    /**SPI1 GPIO Configuration    
    PA4     ------> SPI1_NSS
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI 
    */
    HAL_GPIO_DeInit(GPIOA, BlueNRG_CSI_Pin|BlueNRG_CLK_Pin|BlueNRG_MISO_Pin|BlueNRG_MOSI_Pin);
  }
  else if(hspi->Instance==SPI2)
  {
    /* Peripheral clock disable */
    __HAL_RCC_SPI2_CLK_DISABLE();
  
    /**SPI2 GPIO Configuration    
    PB13     ------> SPI2_SCK
    PB14     ------> SPI2_MISO
    PB15     ------> SPI2_MOSI 
    */
    HAL_GPIO_DeInit(GPIOB, NOR_CLK_Pin|NOR_DATAOUT_Pin|NOR_DATAIN_Pin);
  }
}


void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

   GPIO_InitTypeDef GPIO_InitStruct;
   if(huart->Instance==USART3)
   {
      /* DMA controller clock enable */
      __HAL_RCC_DMA1_CLK_ENABLE();
      /* Peripheral clock enable */
      __HAL_RCC_USART3_CLK_ENABLE();

      /**USART3 GPIO Configuration    
      PC10     ------> USART3_TX
      PC11     ------> USART3_RX 
      */
      GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
      GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
      GPIO_InitStruct.Pull = GPIO_NOPULL;
      GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
      GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
      HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
      
      /* Peripheral interrupt init */
      HAL_NVIC_SetPriority(USART3_IRQn, 5, 0);
      HAL_NVIC_EnableIRQ(USART3_IRQn);

      /* Peripheral DMA init*/

//      hdma_usart3_tx.Instance = DMA1_Stream3;
//      hdma_usart3_tx.Init.Channel = DMA_CHANNEL_4;
//      hdma_usart3_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
//      hdma_usart3_tx.Init.PeriphInc = DMA_PINC_DISABLE;
//      hdma_usart3_tx.Init.MemInc = DMA_MINC_ENABLE;
//      hdma_usart3_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//      hdma_usart3_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//      hdma_usart3_tx.Init.Mode = DMA_CIRCULAR;
//      hdma_usart3_tx.Init.Priority = DMA_PRIORITY_LOW;
//      hdma_usart3_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//      if (HAL_DMA_Init(&hdma_usart3_tx) != HAL_OK)
//      {
//         Error_Handler();
//      }

//      __HAL_LINKDMA(huart,hdmatx,hdma_usart3_tx);

//      hdma_usart3_rx.Instance = DMA1_Stream1;
//      hdma_usart3_rx.Init.Channel = DMA_CHANNEL_4;
//      hdma_usart3_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
//      hdma_usart3_rx.Init.PeriphInc = DMA_PINC_DISABLE;
//      hdma_usart3_rx.Init.MemInc = DMA_MINC_ENABLE;
//      hdma_usart3_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
//      hdma_usart3_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
//      hdma_usart3_rx.Init.Mode = DMA_CIRCULAR;
//      hdma_usart3_rx.Init.Priority = DMA_PRIORITY_LOW;
//      hdma_usart3_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
//      if (HAL_DMA_Init(&hdma_usart3_rx) != HAL_OK)
//      {
//         Error_Handler();
//      }

//      __HAL_LINKDMA(huart,hdmarx,hdma_usart3_rx);

//      /* DMA interrupt init */
//      /* DMA1_Stream1_IRQn interrupt configuration */
//      HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 5, 0);
//      HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
//      /* DMA1_Stream3_IRQn interrupt configuration */
//      HAL_NVIC_SetPriority(DMA1_Stream3_IRQn, 5, 0);
//      HAL_NVIC_EnableIRQ(DMA1_Stream3_IRQn);
   }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{

  if(huart->Instance==USART3)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART3_CLK_DISABLE();
  
    /**USART3 GPIO Configuration    
    PC10     ------> USART3_TX
    PC11     ------> USART3_RX 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_10|GPIO_PIN_11);

    /* Peripheral DMA DeInit*/
    HAL_DMA_DeInit(huart->hdmatx);
    HAL_DMA_DeInit(huart->hdmarx);
  }
  /* USER CODE BEGIN USART3_MspDeInit 1 */

  /* USER CODE END USART3_MspDeInit 1 */

}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
