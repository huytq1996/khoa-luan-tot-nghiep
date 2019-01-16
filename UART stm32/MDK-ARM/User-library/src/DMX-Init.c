//#include "DMX-Init.h"
#include "DMX-handle.h"

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1,hdma_uart,hdma_bluetooth;
extern TIM_HandleTypeDef htim2,htim3;
extern UART_HandleTypeDef huart1,huart2;
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart_dmx;
extern volatile uint8_t dmxSendState;
extern MODE_STATE mode;
uint8_t dmxData[DMX_CHANNELS+1]={0};
extern uint32_t adcbuf[DMX_NUMBER_ADC];
void MX_I2C1_Init(void)
{
		
	GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.Pin = GPIO_PIN_7|GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
	
	
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  HAL_I2C_Init(&hi2c1);


}
void DMX_GPIO_DeInit()
{
  /*Configure GPIO pin : PD10 */
	GPIO_InitTypeDef GPIO_InitStruct;
	
		GPIO_InitStruct.Pin = DMX_TX_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(DMX_TX_Port, &GPIO_InitStruct);
	

  

}
void DMX_GPIO_Init()
{
  /*Configure GPIO pin : PD10 */
	GPIO_InitTypeDef GPIO_InitStruct;
		
	
		GPIO_InitStruct.Pin = DMX_TX_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(DMX_TX_Port, &GPIO_InitStruct);
	

}
void DMX_DisableUsart(UART_HandleTypeDef *huart1) 
{
	//	HAL_UART_MspDeInit(huart1);
	  __HAL_RCC_USART6_CLK_DISABLE();
    HAL_GPIO_DeInit(GPIOC,GPIO_PIN_6);//C6 TX
    /* USART1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(USART6_IRQn);
}

void DMX_EnableUsart(UART_HandleTypeDef *huart1) {
	 GPIO_InitTypeDef GPIO_InitStruct;
	    /**USART6 GPIO Configuration    
    PC6     ------> USART6_TX
    PC7     ------> USART6_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	 __HAL_RCC_USART6_CLK_ENABLE();
		HAL_NVIC_EnableIRQ(USART6_IRQn);
}


/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
	
	 if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}

/** NVIC Configuration
*/
void MX_NVIC_Init(void)
{
  /* TIM2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM2_IRQn);

	HAL_NVIC_SetPriority(USART6_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART6_IRQn);
	if(mode==MANUAL)
  {
	HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADC_IRQn);
	HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
		
	}
	else if(mode==ANDROID)
  {

		HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART3_IRQn);
		HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
	}

}

/* ADC1 init function */
void MX_ADC1_Init(void)
{
 __HAL_RCC_ADC1_CLK_ENABLE();
  ADC_ChannelConfTypeDef sConfig;
	GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = DMX_ADC_C1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DMX_ADC_C1_Port, &GPIO_InitStruct);
	
	

	
  GPIO_InitStruct.Pin = DMX_ADC_C2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DMX_ADC_C2_Port, &GPIO_InitStruct);
	


  GPIO_InitStruct.Pin = DMX_ADC_C3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DMX_ADC_C3_Port, &GPIO_InitStruct);
	
	
  GPIO_InitStruct.Pin = DMX_ADC_C4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DMX_ADC_C4_Port, &GPIO_InitStruct);
	
	

  GPIO_InitStruct.Pin = DMX_ADC_C5_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DMX_ADC_C5_Port, &GPIO_InitStruct);
    /**Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
    */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
  hadc1.Init.Resolution = ADC_RESOLUTION_8B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
	//hadc1.Init.NbrOfDiscConversion = 0;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = DMX_NUMBER_ADC;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  HAL_ADC_Init(&hadc1);

    /**Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time. 
    */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
	
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	sConfig.Channel = ADC_CHANNEL_1;
  sConfig.Rank = 2;
	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	
  sConfig.Channel = ADC_CHANNEL_2;
  sConfig.Rank = 3;
	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	
	sConfig.Channel = ADC_CHANNEL_3;
	sConfig.Rank = 4;
	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	
	sConfig.Channel = ADC_CHANNEL_8;
	sConfig.Rank = 5;
	sConfig.SamplingTime = ADC_SAMPLETIME_480CYCLES;
  HAL_ADC_ConfigChannel(&hadc1, &sConfig);

 
}

/* TIM2 init function */
void MX_TIM2_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 84;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = MARK_BEFORE_BREAK;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htim2);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);

}
void MX_TIM3_Init(void)
{
  __HAL_RCC_TIM3_CLK_ENABLE();
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim3.Instance = TIM3;
  htim3.Init.Prescaler =42000-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
  HAL_TIM_Base_Init(&htim3);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig);

}
/* USART1 init function */
//void MX_USART1_UART_Init(MODE_STATE a)
//{
//if(a==MANUAL||a==ANDROID)
//  {
//		huart_dmx.Instance = USART1;
//	}
//else if (a== PC)
//	{
//		huart_dmx.Instance = USART6;
//	}
//  huart_dmx.Init.BaudRate = BAUDRATE;
//  huart_dmx.Init.WordLength = UART_WORDLENGTH_8B;
//  huart_dmx.Init.StopBits = UART_STOPBITS_2;
//  huart_dmx.Init.Parity = UART_PARITY_NONE;
//  huart_dmx.Init.Mode = UART_MODE_TX_RX;
//  huart_dmx.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//  huart_dmx.Init.OverSampling = UART_OVERSAMPLING_16;
//  HAL_UART_Init(&huart_dmx);

//}
void MX_USART3_UART_Init(void)
{
	USART_Config();
	huart2.Instance = USART3;
  huart2.Init.BaudRate = BAUDRATE_BL;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart2);

	  __HAL_RCC_DMA1_CLK_ENABLE();
 
	hdma_bluetooth.Instance = DMA1_Stream1;
	hdma_bluetooth.Init.Channel = DMA_CHANNEL_4;
		
	hdma_bluetooth.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_bluetooth.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_bluetooth.Init.MemInc = DMA_MINC_ENABLE;
	hdma_bluetooth.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_bluetooth.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_bluetooth.Init.Mode = DMA_CIRCULAR;
	hdma_bluetooth.Init.Priority = DMA_PRIORITY_LOW;
	hdma_bluetooth.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_bluetooth) != HAL_OK)
    {
     // _Error_Handler(__FILE__, __LINE__);
    }

    __HAL_LINKDMA(&huart2,hdmarx,hdma_bluetooth);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART3_IRQn);


}
void MX_USART6_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
		/**USART6 GPIO Configuration    
	PC6     ------> USART6_TX
	PC7     ------> USART6_RX 
	*/
	__HAL_RCC_USART6_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF8_USART6;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	
  huart_dmx.Instance = USART6;
  huart_dmx.Init.BaudRate = BAUDRATE;
  huart_dmx.Init.WordLength = UART_WORDLENGTH_8B;
  huart_dmx.Init.StopBits = UART_STOPBITS_2;
  huart_dmx.Init.Parity = UART_PARITY_NONE;
  huart_dmx.Init.Mode = UART_MODE_TX_RX;
  huart_dmx.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart_dmx.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart_dmx);
}
static void USART_Config(void)
{

		GPIO_InitTypeDef GPIO_InitStruct;
		__HAL_RCC_USART3_CLK_ENABLE();
  
    /**USART1 GPIO Configuration    
    PB11     ------> USART3_RX
    PD8     ------> USART3_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}
/** 
  * Enable DMA controller clock
  */
void MX_DMA_Init(void) 
{
  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();
 
	hdma_adc1.Instance=DMA2_Stream0;
  hdma_adc1.Init.Channel = DMA_CHANNEL_0;
  hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
  hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
  hdma_adc1.Init.Mode = DMA_CIRCULAR;
  hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
  hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
  //hdma_adc1.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
 // hdma_adc1.Init.MemBurst = DMA_MBURST_SINGLE;
  //hdma_adc1.Init.PeriphBurst = DMA_PBURST_SINGLE;
//	hdma_adc1.XferCpltCallback=INT_DMA;
  HAL_DMA_Init(&hdma_adc1);
	__HAL_LINKDMA(&hadc1,DMA_Handle,hdma_adc1);
  /* DMA interrupt init */
	__HAL_DMA_ENABLE_IT(&hdma_adc1,DMA_IT_TC);
	
  /* DMA2_Stream0_IRQn interrupt configuration */


}


void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();


 

  /*Configure GPIO pin : LD4_Pin */
  GPIO_InitStruct.Pin = LD4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(LD4_GPIO_Port, &GPIO_InitStruct);

}
/* USER CODE BEGIN 4 */

/* USER CODE END 4 */
