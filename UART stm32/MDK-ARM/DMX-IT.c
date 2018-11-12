#include "stm32f4xx_hal.h"
#include "DMX-Init.h"
#include "DMX-handle.h"
extern volatile uint8_t dmxData[DMX_CHANNELS+1];
extern uint32_t adcbuf[DMX_NUMBER_ADC];
//extern uint8_t scanner_select[_M_ScannerNum];
//extern uint8_t element_arr_scanner;
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim2,htim3;
extern UART_HandleTypeDef huart1;
extern volatile uint8_t dmxSendState;
extern SCENE *scene_cur;
extern uint8_t *arr_cur;
extern uint8_t len_cur;
extern uint8_t v_state;
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(v_state==SELECT_ADD)
			dmx_add_scanner(scene_cur,arr_cur,len_cur);
	else if(v_state==SELECT_AUTO)
		{	
			uint16_t tem=5000/255*((adcbuf[3]>>2)<<2);
	
			htim3.Instance->ARR=tem;
				if(	htim3.Instance->CNT >(tem+1))
			{
				htim3.Instance->CNT=tem-20;
			}
		}
	/*for(int i=0;i<element_arr_scanner;i++)
	{
		
		dmxData[scanner_select[i]]=adcbuf[0];
		dmxData[scanner_select[i]+1]=adcbuf[1];
		dmxData[scanner_select[i]+2]=adcbuf[2];
	}*/
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{ 
	if(htim->Instance==TIM2)
	{
	switch(dmxSendState)
		{
		case STATE_MBB: // xong before => break
			HAL_GPIO_WritePin(DMX_TX_GPIO_Port,DMX_TX_Pin,GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_RESET);
			dmxSendState= STATE_BREAK;
			htim2.Instance->ARR = MARK_BREAK;
			htim2.Instance->CNT = 0;
			break;
		case STATE_BREAK: // xong break => after break
			dmxSendState = STATE_MAB;
			HAL_TIM_Base_Stop_IT(&htim2);
			HAL_GPIO_WritePin(DMX_TX_GPIO_Port, DMX_TX_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);
			DMX_EnableUsart(&huart1);
			DMX_GPIO_DeInit();
			htim2.Instance->ARR = MARK_AFTER_BREAK;
			htim2.Instance->CNT = 0;
			HAL_NVIC_EnableIRQ(TIM2_IRQn);
			HAL_TIM_Base_Start_IT(&htim2);
			break;
		case STATE_MAB:	
		dmxSendState = STATE_DMA;
		HAL_TIM_Base_Stop_IT(&htim2);
		HAL_NVIC_DisableIRQ(TIM2_IRQn);
	//	HAL_UART_Transmit_IT(&huart1, dmxData, DMX_CHANNELS + 1);
	//		HAL_GPIO_WritePin(DMX_TX_GPIO_Port, DMX_TX_Pin, GPIO_PIN_RESET);
		while (HAL_UART_Transmit_IT(&huart1, dmxData, DMX_CHANNELS + 1) != HAL_OK)
		{
		__NOP();
		};
		//HAL_UART_IRQHandler(&huart1);
			break;
		}
		}else if(htim->Instance==TIM3)
		{
			dmx_auto_play();
			HAL_GPIO_TogglePin(LD4_GPIO_Port,LD4_Pin);
		}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	// This function is called on USART interrupt
	//HAL_UART_IRQHandler(&huart1);
		// Finished sending data over USART. Next is Mark Before Break. Set TX high.
		// Account for the time it takes to setup GPIO output mode.
		HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);
		DMX_GPIO_Init();
		HAL_GPIO_WritePin(DMX_TX_GPIO_Port, DMX_TX_Pin, GPIO_PIN_SET);
		DMX_DisableUsart(&huart1);
		dmxSendState = STATE_MBB;
		htim2.Instance->ARR = MARK_BEFORE_BREAK;
		htim2.Instance->CNT = 0;
		HAL_NVIC_EnableIRQ(TIM2_IRQn);
		HAL_TIM_Base_Start_IT(&htim2);
	
}
