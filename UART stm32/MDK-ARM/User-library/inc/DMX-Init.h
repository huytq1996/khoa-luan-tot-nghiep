
#ifndef _DMX_
#define _DMX_
#include "stm32f4xx_hal.h"

#define STATE_MBB 1
#define STATE_BREAK 2
#define STATE_MAB 4
#define STATE_DMA 8

#define BAUDRATE 250000
#define MARK_BETWEEN 10
#define MARK_BEFORE_BREAK 70//64
#define MARK_BREAK 176//176
#define MARK_AFTER_BREAK  64// 64

#define DMX_CHANNELS 20
#define DMX_NUMBER_ADC 5
#define DMX_NUMBER_SCENE 16
#define DMX_NUMBER_SCANNER 16
#define DMX_NUMBER_SET 5
#define DMX_START_CODE 0
#define DMX_TX_GPIO_Port GPIOD
#define DMX_TX_Pin GPIO_PIN_10

void MX_GPIO_Init(void);

void SystemClock_Config(void);
void MX_GPIO_Init(void);
void MX_DMA_Init(void);
void MX_USART1_UART_Init(void);
void MX_TIM2_Init(void);
void MX_TIM3_Init(void);
void MX_ADC1_Init(void);
void MX_NVIC_Init(void);
void DMX_EnableUsart(UART_HandleTypeDef *huart1) ;
void DMX_DisableUsart(UART_HandleTypeDef *huart1) ;
void DMX_GPIO_DeInit(void);
void DMX_GPIO_Init(void);
#endif