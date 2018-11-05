#ifndef __DMX512_H
#define __DMX512_H

#include "stm8s.h"
#include "string.h"
/* Private macro -------------------------------------------------------------*/
#define UART_BAUD_RATE 250000
#define DMX_ADD_VAL start_addr
#define resolution      3
#define color_green     TIM2->CCR1L  
#define color_red       TIM2->CCR2L  
#define color_blue      TIM2->CCR3L  
#define LEN_DATA 5
#define WAITING 0
#define BREAKTIME 1
#define MAB 2
#define TRANSFER 3
#define BREAKMIN 160
#define BREAKMAX 180
#define MABMIN 10
#define MABMAX 200
#define DEF_TIMEOUT 1000
/* Private variables ---------------------------------------------------------*/
static volatile uint16_t CHANNEL=1;
static volatile int dmx_error = 1;
static volatile int byte_count = 0;
static volatile int start_flag = 0;
static volatile int TIM4_ovf_val = 0;
static volatile int dmx_timeout_val = 0;
static volatile uint8_t phase = WAITING;
static volatile uint32_t break_time, mab_time = 0;
static uint8_t start_addr;


void dmx512_set_input(int i, uint8_t value);
void dmx512_clear_input(void);
void dmx512_rec_init(void);
void dmx512_uart_disable(void);
uint8_t dmx512_get_data(int);
void dmx512_set_start_addr(int addr);
uint16_t dmx512_get_start_addr(void);

void dmx512_exti_enable(void);
void dmx512_exti_disable(void);


void dmx512_handler_rising();
void dmx512_handler_falling();
void dmx512_handler_rx_uart();

void dmx512_tim2_config(uint8_t ch1_pw, uint8_t ch2_pw, uint8_t ch3_pw);

void dmx512_pwm_refesh(uint8_t *data);
void dmx512_clk_config(void);
uint8_t UART1_Read_Char();
void dmx512_init_gpio_sw();
uint16_t dmx512_set_and_read_channel();
void dmx512_IT_timer1();
uint16_t UCLN(uint16_t a,uint16_t b);
void dmx512_tim1_init();
#endif