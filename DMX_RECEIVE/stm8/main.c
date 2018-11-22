#include "main.h"
extern uint16_t v_UCLN;
volatile uint8_t input_data[LEN_DATA];
extern uint8_t flag_receive;
extern uint8_t state;
uint8_t j=0;
uint32_t test;
 extern uint8_t fade;
 extern uint8_t blink;
void delay_us(uint32_t x)
{
     while(x--)
     {
           //nop();
           //nop();
           nop();
           nop();
           nop();
           nop();
           nop();
           nop();
           nop();
           nop();
           nop();
     }
}


int main(void) {
  uint16_t xclk=0;
  dmx512_clk_config();
  dmx512_tim2_config(255,255 , 255);
  dmx512_rec_init();
  dmx512_set_start_addr(CHANNEL);
  dmx512_uart_disable();
  TIM4_DeInit();
  TIM4_TimeBaseInit(TIM4_PRESCALER_16,255);
  TIM4_ClearITPendingBit(TIM4_IT_UPDATE);
  TIM4_ITConfig(TIM4_IT_UPDATE,ENABLE); 
  TIM4_Cmd(DISABLE);
  enableInterrupts();
  dmx512_init_gpio_sw();
  dmx512_tim1_init();
  do
  {
    xclk=dmx512_set_and_read_channel();
  }
  while(!((xclk<512)&&(xclk>=0)));
  
  
  input_data[0]=255;
  input_data[1]=255;
  input_data[2]=255;
  input_data[3]=0;
  input_data[4]=10;
  static uint8_t  input_before[2]={0,0};
  TIM1_SetAutoreload(1);
  while(1)
  {
    if(input_data[3]!=0 || input_data[4]!=0)
    {
    
          
        TIM1_Cmd(!DISABLE);
        test=TIM1_GetCounter();
        input_before[0]=input_data[3];
        input_before[1]=input_data[4];
        TIM1_ClearFlag(TIM1_FLAG_UPDATE);
        TIM1_ClearFlag(TIM1_FLAG_BREAK);
   
  } else
    {
      TIM1_Cmd(DISABLE);
      color_red=input_data[0];
      color_green=input_data[1];
      color_blue=input_data[2];
    }
}
}
 

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
    j=0;
  }
}
#endif
