#include "stm32f4xx_hal.h"
//#include "DMX-Init.h"
#include "DMX-handle.h"
extern uint8_t databluetooth[6];
extern volatile uint8_t dmxData[DMX_CHANNELS+1];
extern uint32_t adcbuf[DMX_NUMBER_ADC];
//extern uint8_t scanner_select[_M_ScannerNum];
//extern uint8_t element_arr_scanner;
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim2,htim3;
extern UART_HandleTypeDef huart_dmx,huart2;
extern volatile uint8_t dmxSendState;
extern SCENE *scene_cur;
extern uint8_t *arr_cur;
extern uint8_t len_cur;
extern uint8_t v_state;
//extern uint8_t flag_timer2;
void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
	static int c=0;

	if(c<50)
	{
		c++;
		return;
	}
	else 
		c=0;
	
	if(v_state==SELECT_ADD||v_state==SELECT_AUTO)
	{
		if(v_state==SELECT_ADD)
		dmx_add_scanner(scene_cur,arr_cur,len_cur);
		else
		{	
				if(adcbuf[3]<5)
				{
				//	flag_timer2=1;
					HAL_TIM_Base_Stop_IT(&htim3);
				}
				else 
				{
			
					uint16_t tem;
					tem=50*adcbuf[3];
					htim3.Instance->ARR=tem;
						if(	htim3.Instance->CNT >(tem+1))
					{
						htim3.Instance->CNT=0;
					}
					HAL_TIM_Base_Start_IT(&htim3);
				}
		}
	}	
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{ 
	if(htim->Instance==TIM2)
	{
	switch(dmxSendState)
		{
		case STATE_MBB: // xong before => break
			DMX_GPIO_PIN_WRITE(GPIO_PIN_RESET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_RESET);
			dmxSendState= STATE_BREAK;
			htim2.Instance->ARR = MARK_BREAK;
			htim2.Instance->CNT = 0;
			break;
		case STATE_BREAK: // xong break => after break
			dmxSendState = STATE_MAB;
			HAL_TIM_Base_Stop_IT(&htim2);
			DMX_GPIO_PIN_WRITE(GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);
			DMX_EnableUsart(&huart_dmx);
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
		while (HAL_UART_Transmit_IT(&huart_dmx, dmxData, DMX_CHANNELS +1) != HAL_OK)
		{
		__NOP();
		};
		/*if(HAL_UART_Transmit_IT(&huart_dmx, dmxData, DMX_CHANNELS + 1)!= HAL_OK)
			HAL_UART_TxCpltCallback(&huart_dmx);*/
		/*flag_timer2++;
		if(flag_timer2==3)
		flag_timer2=0;*/
			break;
		}
		}
	else if(htim->Instance==TIM3)
		{
			
			if(v_state==SELECT_AUTO||v_state==PLAYING)
			{
				memset(dmxData,0,DMX_CHANNELS);
				dmx_auto_play();
			}
		//	flag_timer2=1;
			htim2.Instance->ARR = MARK_BEFORE_BREAK;
			htim2.Instance->CNT = 0;
			HAL_GPIO_TogglePin(LD4_GPIO_Port,LD4_Pin);
		}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	// This function is called on USART interrupt
	//HAL_UART_IRQHandler(&huart1);
		// Finished sending data over USART. Next is Mark Before Break. Set TX high.
		// Account for the time it takes to setup GPIO output mode.
	if(huart->Instance==USART6)
		{
			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);
			DMX_GPIO_Init();
			DMX_GPIO_PIN_WRITE(GPIO_PIN_SET);
			DMX_DisableUsart(&huart_dmx);
			dmxSendState = STATE_MBB;
			htim2.Instance->ARR = MARK_BEFORE_BREAK;
			htim2.Instance->CNT = 0;
			HAL_NVIC_EnableIRQ(TIM2_IRQn);
			HAL_TIM_Base_Start_IT(&htim2);
		}
	
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	{
		if(huart->Instance==USART3)
		{
			memset(dmxData,0,DMX_CHANNELS);
			for(int i=0;i<5;i++)
				{
					dmxData[i*5+1]=databluetooth[i*3];
					dmxData[i*5+2]=databluetooth[i*3+1];
					dmxData[i*5+3]=databluetooth[i*3+2];
				}
					
		
	
			
		}
	}
