#ifndef STM32_KEYPAD4X4_H
#define	STM32_KEYPAD4X4_H
#include "stm32f4xx_hal.h"
#include <stdbool.h>
#ifdef	__cplusplus
extern "C" {
#endif

#define Column_1_Port 		GPIOE						
#define Column_1_Pin 	    GPIO_PIN_15  
                                       
#define Column_2_Port     GPIOB        
#define Column_2_Pin 	    GPIO_PIN_1  
                                       
#define Column_3_Port     GPIOB        
#define Column_3_Pin 	    GPIO_PIN_13   
                                       
#define Column_4_Port     GPIOB        
#define Column_4_Pin 	    GPIO_PIN_15  

#define Row_1_Port 				GPIOE					
#define Row_1_Pin 		     GPIO_PIN_7
                           
#define Row_2_Port 		     GPIOE
#define Row_2_Pin 		     GPIO_PIN_9
                           
#define Row_3_Port 		     GPIOE
#define Row_3_Pin 		     GPIO_PIN_11
                           
#define Row_4_Port	       GPIOE
#define Row_4_Pin 		     GPIO_PIN_13
#define KeyPad_delay 5    
    
#define KEYPAD4x4__VALUE_BEGIN                  0
#define KEYPAD4x4__VALUE_1                      1
#define KEYPAD4x4__VALUE_2                      2
#define KEYPAD4x4__VALUE_3                      3
#define KEYPAD4x4__VALUE_4                      5
#define KEYPAD4x4__VALUE_5                      6
#define KEYPAD4x4__VALUE_6                      7
#define KEYPAD4x4__VALUE_7                      9
#define KEYPAD4x4__VALUE_8                      10
#define KEYPAD4x4__VALUE_9                      11
#define KEYPAD4x4__VALUE_A                      4
#define KEYPAD4x4__VALUE_B                      8
#define KEYPAD4x4__VALUE_C                      12
#define KEYPAD4x4__VALUE_D                      16
#define KEYPAD4x4__VALUE_0                      14
#define KEYPAD4x4__VALUE_multi                  13
#define KEYPAD4x4__VALUE_sharp                  15
#define KEYPAD4x4__VALUE_END                    1111
#define GET_CHAR                                1
#define GET_NUM																	2
#define GET_CHAR_NUM														3
void KeyPad_init(void);
unsigned char KeyPad_check_but(void);
void KeyPad_scan_row(unsigned char r);
unsigned char KeyPad_check_col(void);
unsigned char KeyPad_get_key(void);
unsigned char KeyPad_getKey(uint8_t a);
char KeyPad_getascii(char num_button);
//unsigned char KeyPad_getNum(void);
//unsigned char KeyPad_getChar(void);
bool checknum(char c);
#ifdef	__cplusplus
}
#endif

#endif	/* STM32_KEYPAD4X4_H */
