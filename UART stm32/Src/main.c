#include "stm32f4xx_hal.h"
#include "string.h"
#include "KeyPad.h"
//#include "DMX-Init.h"
#include "DMX-handle.h"
#include "eeprom.h"
#include "LCD.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

char dataReceive[15];
uint8_t databluetooth[15];
uint8_t address, red, green, blue;
uint16_t VirtAddVarTab[NB_OF_VAR] = {0x5555, 0x6666, 0x7777};
uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0};
uint16_t VarValue,VarDataTmp = 0;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart_dmx;
DMA_HandleTypeDef hdma_adc1,hdma_uart, hdma_bluetooth;
volatile uint8_t dmxSendState;
ADC_HandleTypeDef hadc1;
uint32_t key=0;
uint32_t adcbuf[DMX_NUMBER_ADC];
uint16_t t,t1;
volatile MODE_STATE mode;



int main(void)
{
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
	MX_I2C1_Init();// Init LCD
	KeyPad_init();
	lcd_init(16,2);
	lcd_clear();
	dmx_select_mode();
	//mode=ANDROID;
	switch(mode)
		{

		case MANUAL:
				MX_ADC1_Init();
				MX_DMA_Init();
				MX_TIM3_Init();		
				htim3.Instance->CNT = 0;
				htim3.Instance->ARR=1000;
				break;
			case ANDROID: 
				MX_USART3_UART_Init();
				HAL_UART_Receive_DMA(&huart2,databluetooth,15);
				break;
			case PC:
				
				 MX_USB_DEVICE_Init();
				break;
		}

	MX_USART6_Init();
	DMX_GPIO_Init();
	MX_NVIC_Init();
  MX_TIM2_Init();
	DMX_DisableUsart(&huart_dmx);
	DMX_GPIO_PIN_WRITE(GPIO_PIN_SET);
	dmxSendState=STATE_MBB;
	htim2.Instance->CNT = 0;
	htim2.Instance->ARR=MARK_BEFORE_BREAK;
	HAL_TIM_Base_Start_IT(&htim2);

  while (1)
 {
	 	switch(mode)
		{
			case MANUAL:
			
				dmx_main();
				break;
			case ANDROID: 
				break;
			case PC: break;
			default : break;
		}
		//HAL_Delay(700);

  }
  /* USER CODE END 3 */

}





#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
