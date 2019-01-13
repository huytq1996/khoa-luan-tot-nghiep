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
uint8_t address, red, green, blue;
uint16_t VirtAddVarTab[NB_OF_VAR] = {0x5555, 0x6666, 0x7777};
uint16_t VarDataTab[NB_OF_VAR] = {0, 0, 0};
uint16_t VarValue,VarDataTmp = 0;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart_dmx;
DMA_HandleTypeDef hdma_adc1,hdma_uart;
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
	mode=20;
	KeyPad_init();
	lcd_init(16,2);
	lcd_clear();
/*  dmx_select_mode();
	switch(mode)
		{
			case MANUAL:
				MX_ADC1_Init();
				MX_DMA_Init();
			//	USART_Config();
				MX_USART1_UART_Init(MANUAL);
				
				MX_TIM3_Init();		
				HAL_TIM_Base_Start_IT(&htim3);
			
				htim3.Instance->CNT = 0;
				htim3.Instance->ARR=2000;
				break;
			case ANDROID: 
				MX_USART1_UART_Init(ANDROID);
				MX_USART3_UART_Init();
				USART_Config();
			
				break;
			case PC:
				 MX_USB_DEVICE_Init();
			//	MX_USART6_Init();
				MX_USART1_UART_Init(PC);
				break;
		}
	*/

 // MX_I2C1_Init();
  /* Initialize interrupts */
	DMX_GPIO_Init();
	MX_NVIC_Init();
  MX_TIM2_Init();
	DMX_DisableUsart(&huart_dmx);
	DMX_GPIO_PIN_WRITE(GPIO_PIN_SET);
	//HAL_GPIO_WritePin(GPIOD,GPIO_PIN_12,GPIO_PIN_SET);
//HAL_GPIO_WritePin(GPIOD,GPIO_PIN_13,GPIO_PIN_SET);
	dmxSendState=STATE_MBB;
	htim2.Instance->CNT = 0;
	//HAL_TIM_Base_Start_IT(&htim2);
 		MX_USART3_UART_Init();
		
	uint8_t buferbl[1];
//	HAL_UART_Receive_DMA(&huart2,buferbl,1);
//HAL_ADC_Start(&hadc1);
  //HAL_ADC_Start_DMA(&hadc1, adcbuf, DMX_NUMBER_ADC) ;
 // EE_Init();
    /* Start Conversation Error */
  //  Error_Handler(); 

	HAL_TIM_Base_Start_IT(&htim2);
	

//t1=EE_Init();
//Error();
//HAL_FLASH_Unlock();
//HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 0x08008000, 0);
//HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 0x08008000, 1);
//HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 0x08008004, 1);
//HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, 0x08008006, 1);

//t1=EE_WriteVariable(0,0x5555);Error();
//t1=EE_ReadVariable(0,&t);Error();

/*FLASH_EraseInitTypeDef EraseInitStruct;
EraseInitStruct.TypeErase   = FLASH_TYPEPROGRAM_BYTE;
EraseInitStruct.Sector   = FLASH_SECTOR_2;
EraseInitStruct.NbSectors = 1; 
EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError);


HAL_FLASH_Lock();
t=*((uint16_t*)(0x08008004));
t++;
*((uint16_t*)(0x08008004))=0xFFFF;*/
	uint8_t buff[3];
HAL_UART_Receive_IT(&huart2,buff,1);
	/*lcd_init(16,2);
	lcd_clear();
	lcd_setCursor(0,0);
	HAL_Delay(1000);*/
	HAL_UART_Transmit_IT(&huart2,"A",1);

		
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
		HAL_UART_Transmit(&huart2,"a",1,10);
		//key=KeyPad_getascii(KeyPad_press());
		HAL_Delay(700);


  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/


//**************************************************


/*void lcd_init(uint8_t cols, uint8_t lines)
{
	if (lines > 1) 
	{
		_displayFunction |= LCD_2LINE;
	}

	_numLines = lines;
	HAL_Delay(1);
	lcd_expanderWrite(0);
	HAL_Delay(1);
	lcd_write4bits(0x03 << 4);
	HAL_Delay(5);
	lcd_write4bits(0x03 << 4);
	HAL_Delay(5);
	lcd_write4bits(0x03 << 4);
	HAL_Delay(1);
	lcd_write4bits(0x02 << 4);
	HAL_Delay(5);

	lcd_command(LCD_FUNCTIONSET | _displayFunction);
	_displayControl = LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKON;
	lcd_display();
	_displayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	lcd_command(LCD_ENTRYMODESET | _displayMode);
	lcd_clear();
	lcd_home();


}

void lcd_expanderWrite(uint8_t _data)
{
	uint8_t cmd[1];
	cmd[0] = _data | LCD_BACKLIGHT;
	HAL_I2C_Mem_Write(&hi2c1, LCD_ADD, 0x00, 1, cmd, 1, 0x100);
}

void lcd_pulseEnable(uint8_t _data)
{
	lcd_expanderWrite(_data | En);
	HAL_Delay(1);

	lcd_expanderWrite(_data & ~En);
	HAL_Delay(1);

}

void lcd_write4bits(uint8_t value)
{
	lcd_expanderWrite(value);
	lcd_pulseEnable(value);
}

void lcd_send(uint8_t value, uint8_t mode)
{
	uint8_t highnib = value & 0xf0;
	uint8_t lownib = (value << 4) & 0xf0;
	lcd_write4bits((highnib) | mode);
	lcd_write4bits((lownib) | mode);
}

void lcd_write(uint8_t value)
{
	lcd_send(value, Rs);
	colCheck++;
	if (colCheck == 16)
	{
		if (rowCheck == 0)
		{
			lcd_setCursor(0, 1);
		}
		else
		{
			lcd_clear();
			lcd_home();
		}
		
	}
}




void lcd_command(uint8_t value)
{
	lcd_send(value, 0);
}

void lcd_display()
{
	_displayControl |= LCD_DISPLAYON;
	lcd_command(LCD_DISPLAYCONTROL | _displayControl);
}




void lcd_clear(void)
{
	lcd_command(LCD_CLEARDISPLAY);
	HAL_Delay(5);
}

void lcd_home()
{
	lcd_command(LCD_RETURNHOME);
	HAL_Delay(3);
	rowCheck = 0;
	colCheck = 0;
}
void lcd_setCursor(uint8_t col, uint8_t row)
{
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if (row > _numLines) {
		row = _numLines - 1;    // we count rows starting w/0
	}
	lcd_command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
	rowCheck = row;
	colCheck = col;
}
int colorNumber()
{
	int i,k,count;
	k = 0;
	i=100;
	count = 0;
	while (count<3)
	{
	i = Scankey();
	switch (i)
		{
		case 1:
			lcd_write('1');
			break;
		case 2:
			lcd_write('2');
			break;
		case 3:
			lcd_write('3');
			break;
		case 4:
			lcd_write('4');
			break;
		case 5:
			lcd_write('5');
			break;
		case 6:
			lcd_write('6');
			break;
		case 7:
			lcd_write('7');
			break;
		case 8:
			lcd_write('8');
			break;
		case 9:
			lcd_write('9');
			break;
		case 0:
			lcd_write('0');
			break;
		}
		if (i<=9 && i>=0)
		{
		k= (k*10) +i;
		count++;
		}
		HAL_Delay(200);
		i=100; 
	}
	
		return k;
}*/







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
