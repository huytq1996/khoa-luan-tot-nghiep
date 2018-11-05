#include "dmx512.h"
volatile uint16_t v_UCLN;
 extern uint8_t input_data[LEN_DATA];
uint8_t dmx512_get_data(int i) {
    return input_data[i];
}
void dmx512_set_input(int i, uint8_t value) {
    input_data[i] = value;
}
void dmx512_clear_input(void) {
    memset((uint8_t*)input_data, 0, LEN_DATA);
}

//  f_counter = f_master / prescaler;
//  f_timer = f_counter / (period + 1);
void dmx512_rec_init(void) {
    dmx512_exti_enable();


    UART1_DeInit();
    UART1_Init(UART_BAUD_RATE, UART1_WORDLENGTH_8D, UART1_STOPBITS_2, UART1_PARITY_NO, UART1_SYNCMODE_CLOCK_DISABLE, UART1_MODE_RX_ENABLE);
    UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
    UART1_ClearITPendingBit(UART1_IT_RXNE);
    dmx512_clear_input();
    UART1_Cmd(ENABLE);
}

void dmx512_set_start_addr(int addr) {
    start_addr = CHANNEL;
}

uint16_t dmx512_get_start_addr(void) {
    return start_addr;
}

void dmx512_uart_disable(void) {
    UART1_Cmd(DISABLE);
    UART1_ITConfig(UART1_IT_RXNE_OR,DISABLE);
}
void dmx512_uart_enable(void) {
    UART1_Cmd(ENABLE);
    UART1_ITConfig(UART1_IT_RXNE_OR,ENABLE);
}
void dmx512_exti_enable(void) {
    GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_IN_FL_IT);
    GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_IN_FL_IT);

    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB, EXTI_SENSITIVITY_FALL_ONLY);
    EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOC, EXTI_SENSITIVITY_RISE_ONLY);
}

void dmx512_exti_disable(void) {
    GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_IN_FL_NO_IT);
    GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_IN_FL_NO_IT);
}


void dmx512_handler_falling()
{
   if( GPIO_ReadInputPin(GPIOB,GPIO_PIN_4)==RESET)
  {
   /* if (phase == MAB) 
        {
            TIM4_Cmd(DISABLE);
            mab_time=TIM4_GetCounter()+ TIM4_ovf_val*256;
             TIM4_ovf_val=0;
            if (mab_time > MABMIN && mab_time < MABMAX) 
              {
                start_flag = 1;
                phase = TRANSFER;
                dmx512_exti_disable();
                UART1->DR=0;
               // UART1_Cmd(ENABLE);
             //   UART1_ReceiveData8();
                UART1_ITConfig(UART1_IT_RXNE_OR, ENABLE);
              } 
            else 
              {
                //phase = WAITING;
                TIM4_SetCounter(0); 
                TIM4_ovf_val=0;
                phase = BREAKTIME;
                UART1_ITConfig(UART1_IT_RXNE_OR, DISABLE);
                UART1_Cmd(DISABLE);
                TIM4_Cmd(ENABLE);
              }
            
        }
      else
        {
          UART1_Cmd(DISABLE);
          UART1_ITConfig(UART1_IT_RXNE_OR, DISABLE);
          TIM4_SetCounter(0);  
          TIM4_ovf_val=0;
          TIM4_Cmd(ENABLE);
          phase = BREAKTIME;
          return;
        }*/
    
          dmx512_uart_disable();
          TIM4_SetCounter(0);  
          TIM4_ovf_val=0;
          TIM4_Cmd(ENABLE);
          phase = BREAKTIME;
          return;
  }
 
}


void dmx512_handler_rising()
{
  if( GPIO_ReadInputPin(GPIOC,GPIO_PIN_3)!=RESET)
 {
           if (phase == BREAKTIME)
          {
            TIM4_Cmd(DISABLE);
            break_time=TIM4_GetCounter()+ TIM4_ovf_val*256;
            if (break_time >= BREAKMIN && break_time <= BREAKMAX)
            { 
                start_flag = 1;
                phase = MAB;
                dmx512_exti_disable();
                dmx512_uart_enable();
            }
            else 
            {
              phase = WAITING;
              TIM4_SetCounter(0);
              TIM4_ovf_val=0;
              TIM4_Cmd(ENABLE);
            }
          }
           else
          {
            phase=WAITING;
            TIM4_SetCounter(0);
            TIM4_ovf_val=0;
            TIM4_Cmd(ENABLE);
          }
  }
}
void dmx512_handler_rx_uart()
{
   uint8_t in;
   uint16_t data_count=0,channel=CHANNEL;
   if(start_flag==0)
   {
      UART1_ITConfig(UART1_IT_RXNE_OR, DISABLE);
      UART1_Cmd(DISABLE);
      TIM4_Cmd(ENABLE);
      TIM4_SetCounter(0);
      TIM4_ovf_val=0;
      dmx512_exti_enable(); 
      byte_count=0;
      return;
   }
      in = UART1_ReceiveData8();       
   while(byte_count < channel)  
   {
     //in = UART1_ReceiveData8();
      while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET)
          {
           dmx_timeout_val++;
              if(dmx_timeout_val>DEF_TIMEOUT)
                goto end;
          }
      in = UART1_ReceiveData8();
      byte_count++;
   }
   while(byte_count < (channel+LEN_DATA))   
  {
      dmx512_set_input(data_count,in);
      while (UART1_GetFlagStatus(UART1_FLAG_RXNE) == RESET)
      {
       dmx_timeout_val++;
       if(dmx_timeout_val>DEF_TIMEOUT)
            goto end;
      }
      in = UART1_ReceiveData8();
      byte_count++;
      data_count++;
  }
 end: byte_count = 0;
      dmx_timeout_val=0;
      start_flag=0;
      phase=WAITING;      
      UART1_Cmd(DISABLE);
      UART1_ITConfig(UART1_IT_RXNE_OR, DISABLE);
     // mab_time=0;
     // break_time=0;
      TIM4_Cmd(ENABLE);
      TIM4_SetCounter(0); 
      TIM4_ovf_val=0;
      dmx512_exti_enable(); 
}
void dmx512_tim2_config(uint8_t ch1_pw, uint8_t ch2_pw, uint8_t ch3_pw) {
  // TIM2 Frequency = TIM2 counter clock/(ARR + 1)
  TIM2_DeInit();
  TIM2_TimeBaseInit(TIM2_PRESCALER_1024, 255);
  TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, ch1_pw, TIM2_OCPOLARITY_HIGH);
  TIM2_OC1PreloadConfig(ENABLE);

  TIM2_OC2Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, ch2_pw, TIM2_OCPOLARITY_HIGH);
  TIM2_OC2PreloadConfig(ENABLE);

  TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, ch3_pw, TIM2_OCPOLARITY_HIGH);
  TIM2_OC3PreloadConfig(ENABLE);

  TIM2_ARRPreloadConfig(ENABLE);

  TIM2_Cmd(ENABLE);
  
}
void dmx512_tim1_init()
{
    TIM1_DeInit();
    CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER1,ENABLE);
    TIM1_TimeBaseInit(15999, TIM1_COUNTERMODE_UP, 3500, 0);
    TIM1_ARRPreloadConfig(ENABLE);
    TIM1_ITConfig(TIM1_IT_UPDATE,ENABLE);
    TIM1_ClearFlag(TIM1_FLAG_UPDATE);
    TIM1_Cmd(ENABLE);
    
}
void dmx512_pwm_refesh(uint8_t *data) {
  TIM2->CCR1H = 0;
  TIM2->CCR1L = data[0];
  TIM2->CCR2H = 0;
  TIM2->CCR2L = data[1];
  TIM2->CCR3H = 0;
  TIM2->CCR3L = data[2];
}

void dmx512_clk_config(void)
{
  CLK_HSECmd(DISABLE);
  CLK_LSICmd(DISABLE);
  
  CLK_DeInit();
  CLK_HSICmd(ENABLE);
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);
  CLK_PeripheralClockConfig(( CLK_PERIPHERAL_TIMER2 ), ENABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART1, ENABLE);
  CLK_PeripheralClockConfig(CLK_PERIPHERAL_TIMER4, ENABLE);
}
uint8_t UART1_Read_Char()
{
	while (!UART1_GetFlagStatus(UART1_FLAG_RXNE)); // Doi gui xong ky tu truoc
	return UART1_ReceiveData8(); // Moi cho phep nhan ky tu tiep theo
}
void dmx512_init_gpio_sw()
{
   GPIO_Init(GPIOA, GPIO_PIN_1|GPIO_PIN_2, GPIO_MODE_IN_PU_NO_IT);
   GPIO_Init(GPIOB, GPIO_PIN_5, GPIO_MODE_IN_PU_NO_IT);
   GPIO_Init(GPIOC, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_MODE_IN_PU_NO_IT);
   GPIO_Init(GPIOD, GPIO_PIN_2, GPIO_MODE_IN_PU_NO_IT);
//   GPIO_Init(GPIOD, GPIO_PIN_1, GPIO_MODE_IN_PU_NO_IT);//debug
 // GPIO_ReadInputPin(GPIOC,GPIO_PIN_5);
}
uint16_t check(BitStatus a)
{
  if(a==RESET)
     return 0;
  else 
    return 1;
}
uint16_t dmx512_set_and_read_channel()
{
  uint16_t ch=0;
  ch|=check((GPIO_ReadInputPin(GPIOA,GPIO_PIN_2)))<<6;
  ch|=check((GPIO_ReadInputPin(GPIOA,GPIO_PIN_1)))<<5;
  ch|=check((GPIO_ReadInputPin(GPIOB,GPIO_PIN_5)))<<4;
  ch|=check((GPIO_ReadInputPin(GPIOC,GPIO_PIN_4)))<<3;
  ch|=check((GPIO_ReadInputPin(GPIOC,GPIO_PIN_5)))<<2;
  ch|=check((GPIO_ReadInputPin(GPIOC,GPIO_PIN_6)))<<1;
  ch|=check((GPIO_ReadInputPin(GPIOC,GPIO_PIN_7)))<<0;
 // ch|=check((GPIO_ReadInputPin(GPIOD,GPIO_PIN_1)))<<0;
 // ch|=check((GPIO_ReadInputPin(GPIOD,GPIO_PIN_2)))<<7;
  CHANNEL=((~ch)&0x7F);
  return CHANNEL;
                
}
uint16_t UCLN(uint16_t a,uint16_t b)
{
  if(a==0&&b==0)
  {
    v_UCLN=0;
    return 0;
  }
    if(a==0&&b!=0)
    {
      v_UCLN=b;
        return b;
    }
      if(a!=0&&b==0)
      {
        v_UCLN=a;
        return a;
      }
    while ( a != b)
    {
        if (a > b)
            a = a - b;
        else
            b = b - a;
    }
    v_UCLN=a;
    return a;
}
void dmx512_blink()
{
  static uint8_t state=0;
  if(state==1)
  {
    color_red=0;
    color_green=0;
    color_blue=0;
    state=0;
  }
  else
  {
    color_red=input_data[0];
    color_green=input_data[1];
    color_blue=input_data[2];
    state=1;
  }
}
void dmx512_fade()
{
  static uint8_t state_r=1,state_g=1,state_b=1;
  if(state_r==1)
  {
    if(color_red>=input_data[0]-resolution)
      {
        state_r=0;
        color_red=input_data[0];
      }
    else  
    {
        color_red+=resolution;
    }
  }
  else
  {
    if(color_red<=resolution)
      {
        state_r=1;
        color_red=0;
      }
    else  
    {
        color_red-=resolution;
    }
  } 
  if(state_g==1)
  {
    if(color_green>=input_data[1]-resolution)
      {
        state_g=0;
        color_green=input_data[1];
      }
    else  
    {
        color_green+=resolution;
    }
  }
  else
  {
    if(color_green<=resolution)
      {
        state_g=1;
        color_green=0;
      }
    else  
    {
        color_green-=resolution;
    }
  } 
  if(state_b==1)
  {
    if(color_blue>=input_data[2]-resolution)
      {
        state_b=0;
        color_blue=input_data[2];
      }
    else  
    {
        color_blue+=resolution;
    }
  }
  else
  {
    if(color_blue<=resolution)
      {
        state_b=1;
        color_blue=0;
      }
    else  
    {
        color_blue-=resolution;
    }
  } 
 
}
 void dmx512_IT_timer1()
 {
   static uint8_t blink=0;
   uint8_t x;
   static uint8_t fade=0;
   blink++;
   fade++;
   x=(((uint16_t)input_data[3]*12)/v_UCLN);
   if(input_data[3]!=0&&blink>=x)
   {
     blink=0;
     dmx512_blink();
   }
   x=(((uint16_t)input_data[4]*4)/v_UCLN);
   if(input_data[4]!=0&&fade>=x)
   {
     fade=0;
     dmx512_fade();
   }
   TIM1_ClearITPendingBit(TIM1_IT_UPDATE);
 }