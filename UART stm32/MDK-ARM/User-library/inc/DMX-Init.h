#ifndef _DMX_
#define _DMX_
#include "stm32f4xx_hal.h"
#include "DMX-handle.h"
#define STATE_MBB 1
#define STATE_BREAK 2
#define STATE_MAB 4
#define STATE_DMA 8

#define BAUDRATE 250000
#define BAUDRATE_BL 115200

#define MARK_BETWEEN 10
#define MARK_BEFORE_BREAK 70//64
#define MARK_BREAK 176//176
#define MARK_AFTER_BREAK  64// 64

#define DMX_CHANNELS 512
#define DMX_NUMBER_ADC 5
#define DMX_NUMBER_SCENE 16
#define DMX_NUMBER_SCANNER 16
#define DMX_NUMBER_SET 4
#define DMX_START_CODE 0
#define DMX_TX_Port GPIOC
#define DMX_TX_Pin GPIO_PIN_8

#define DMX_ADC_C1_Port GPIOA
#define DMX_ADC_C1_Pin GPIO_PIN_0
#define DMX_ADC_C2_Port GPIOA
#define DMX_ADC_C2_Pin GPIO_PIN_1
#define DMX_ADC_C3_Port GPIOA
#define DMX_ADC_C3_Pin GPIO_PIN_2
#define DMX_ADC_C4_Port GPIOA
#define DMX_ADC_C4_Pin GPIO_PIN_3
#define DMX_ADC_C5_Port GPIOB
#define DMX_ADC_C5_Pin GPIO_PIN_0
#define DMX_GPIO_Port_MODE_STATE GPIOA
#define DMX_GPIO_Pin_MODE_STATE GPIO_PIN_1


typedef enum 
{
		MANUAL ,
		ANDROID ,
		PC 
}MODE_STATE;
//#define SELECT_SCENE 7
typedef enum 
{
	YES =  KEYPAD4x4__VALUE_A,
	NO =  KEYPAD4x4__VALUE_B,
	SELECT_ADD = KEYPAD4x4__VALUE_C  ,
	
	//SELECT_SCENE = KEYPAD4x4__VALUE_,
	//SELECT_SCENE_ = KEYPAD4x4__VALUE_C,
	SELECT_AUTO = KEYPAD4x4__VALUE_sharp ,
	SELECT_SCANNER,
	SELECT_SET,
	SELECT_SCENE,
	PLAYING =30
//	SELECT_AUTO = 16;
	
}STATE;

typedef struct SCANNER
{
	uint16_t Addr;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t blink;
	uint8_t fade;
	
}SCANNER;
typedef struct SCENE
	{

		SCANNER scanner[DMX_NUMBER_SCANNER];
		uint8_t len;
	}SCENE;
typedef struct 
{
	SCENE scene[DMX_NUMBER_SCENE];
	uint8_t len;
}stSET;
void MX_USART3_UART_Init(void);
void MX_GPIO_Init(void);
static void USART_Config(void);
void SystemClock_Config(void);
void MX_I2C1_Init(void);
void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_ADC1_Init(void);
void MX_NVIC_Init(void);
void DMX_EnableUsart(UART_HandleTypeDef *huart1) ;
void DMX_DisableUsart(UART_HandleTypeDef *huart1) ;
void DMX_GPIO_DeInit(void);
void DMX_GPIO_Init(void);
void MX_USART6_Init(void);
#endif
