#include "main.h"

volatile uint8_t input_data[LEN_DATA];
 uint8_t j=0;

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
uint32_t xclk=0;

int main(void) {
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
  xclk=CLK_GetClockFreq();
  dmx512_init_gpio_sw();
  dmx512_tim1_init();
  do
  {
    xclk=dmx512_set_and_read_channel();
  }
  while(!(xclk<512&&xclk>=0));
  

    input_data[0]=255;
  input_data[1]=255;
    input_data[2]=255;
    input_data[3]=200;
    input_data[4]=0;

  while(1)
  {
 
    TIM1_SetAutoreload(UCLN(input_data[3],input_data[4]));
  //  TIM1_SetCounter(0);
     TIM1_ClearFlag(TIM1_FLAG_UPDATE);
 /*   TIM2->CCR1L=dmx512_get_data(1);
    TIM2->CCR2L=dmx512_get_data(0);
    TIM2->CCR3L=dmx512_get_data(2);*/
   /* TIM2_SetCompare1(255-j);
    TIM2_SetCompare2(j);
    TIM2_SetCompare3(20);
    
    
    j=j+1;
    if(j==255)
      j=0;*/
    //xclk=dmx512_set_and_read_channel();
  // GPIO_WriteReverse(GPIOB,GPIO_PIN_5);
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
