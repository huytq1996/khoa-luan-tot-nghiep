#include "DMX-handle.h"
#include "string.h"
#include "stdlib.h"
#include "LCD.h"
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1,hdma_uart;
extern TIM_HandleTypeDef htim2,htim3;
extern UART_HandleTypeDef huart1,huart2;
extern uint8_t rowCheck , colCheck ;
extern MODE_STATE mode;
char v_display_buffer[2][17]={"",""};
static uint8_t flagEndSet=0;
uint8_t v_state=SELECT_ADD;
uint8_t button=0;
stSET set[DMX_NUMBER_SET];
uint8_t element_arr_scanner=0;
uint8_t element_arr_scene=0;
uint8_t set_len=0;
//SCENE scene_select[DMX_NUMBER_SCENE];
SCENE *scene_cur=NULL;
uint16_t *arr_cur=NULL;
uint16_t len_cur=0;
//volatile uint8_t flag_timer2=1;
//static uint8_t out_set_cur=0;
static uint8_t out_scene_cur=0;
extern uint8_t dmxData[DMX_CHANNELS+1];
extern uint32_t adcbuf[DMX_NUMBER_ADC];
void (*scene_current)(void);
//void set_timer(uint32_t time);
void dmx_getstring(char *pString,uint8_t option);

void dmx_main()
{

	while(1)
	{
		switch (v_state)
		{
			case SELECT_ADD :
				
				HAL_ADC_Stop_DMA(&hadc1);

				memset(dmxData,0,DMX_CHANNELS);
					dmx_select_set();
			v_state=SELECT_AUTO;
			break;
			case SELECT_AUTO :
			//	if(v_state==SELECT_AUTO)
				{
					HAL_TIM_Base_Start_IT(&htim3);		
					HAL_ADC_Start_DMA(&hadc1, adcbuf, DMX_NUMBER_ADC) ;
					lcd_clear();
					lcd_home();
					lcd_write_string("   SELECT SPEED    Press A ");
					while(	KeyPad_getKey(GET_CHAR)!=YES);
					HAL_ADC_Stop_DMA(&hadc1);
					v_state=PLAYING;
					lcd_clear();
					lcd_home();
					lcd_write_string("   PLAYING   ");
				}
			case PLAYING: break;
			//			dmx_auto_play();
			default : break;
		}
				
			
	}
}


void dmx_getstring(char *pString,uint8_t option)
{
	char buf=0;
	memset(pString,0,4);
	uint8_t tmplen=0,count=0;
	buf=KeyPad_getKey(option);
	if(option==GET_NUM)
		{
			option =GET_CHAR_NUM;
		}
	while(/*buf!=SELECT_ADD&&buf!=YES&&buf!=NO&&buf!=SELECT_AUTO&&*/count<4)
	{
		tmplen=strlen(pString);
		if(buf==KEYPAD4x4__VALUE_multi)
		{
			if(tmplen>0)
			{
				pString[tmplen-1]=0;
			  lcd_setCursor(rowCheck,colCheck-1);
				lcd_write(' ');
				lcd_setCursor(rowCheck,colCheck-1);
				if(count>0)
				{
					count--;
				}else{}
			}
			else{}
		}
		else
		{
			if(checknum(buf)&&count!=3)
			{
					pString[tmplen]=KeyPad_getascii(buf);
					lcd_write(pString[tmplen]);
					count++;
					pString[tmplen+1]=0;
			}
		}
		buf=  KeyPad_getKey(GET_CHAR_NUM);
			if(buf==NO)
			{
			//	free(pString);
			//	pString=NULL;
			//	v_state=NO;
				button=NO;
		//		return ;
			}
	else if(buf==YES)
			{
			//	free(pString);
				//pString=NULL;
				//v_state=YES;
				button=YES;
				return;
			}
	else if(buf==SELECT_AUTO)
			{
			//	free(pString);
				//pString=NULL;
				//v_state=SELECT_AUTO;
				button=SELECT_AUTO;
			//	return ;
			}
	}


}
void dmx_select_set()
{
	//nhap so luong set
	char tmp1[5]="";
	//nhap so luong set
	lcd_clear();
	lcd_home();
	lcd_write_string("number of profile: ");
	
	dmx_getstring(tmp1,GET_NUM);
	set_len=atoi(tmp1);
	//set tung canh
	for(int i=0;i<set_len;i++)
	{
		char temp[32]="";
		lcd_clear();
		lcd_home();
		sprintf(temp,"number scene of profile %d: ",i+1);
		lcd_write_string(temp);
		dmx_add_set(&set[i]);
		
	}
}
void dmx_add_set(stSET *SET)
{
		
	
	char tmp1[5]="";
	dmx_getstring(tmp1,GET_NUM);
	SET->len=atoi(tmp1);
		//set tung canh
	for(int i=0;i<SET->len;i++)
	{
		//nhap canh thu i+1
		char temp[32]="";
		lcd_clear();
		lcd_home();
		sprintf(temp,"number group of scene %d: ",i+1);
		lcd_write_string(temp);
		dmx_add_scene(&(SET->scene[i]));		
	}

}
void dmx_add_scene(SCENE *scene)
{
	uint8_t group;
	char tmp1[5]="";
	dmx_getstring(tmp1,GET_NUM);
	group=atoi(tmp1);
	scene->len=0;
	for(int i=0;i<group;i++)
	{
		char temp[32]="";
		static uint16_t arr[16]={0};
		memset(dmxData,0,DMX_CHANNELS);
		lcd_clear();
		lcd_home();
		sprintf(temp,"number scaner of group %d: ",i+1);
		lcd_write_string(temp);
		dmx_add_group(scene,arr);
	}
	
}
void dmx_add_group(SCENE *scene,uint16_t *arr)
{
	char temp[32]="";
	uint16_t num=0,len=0;
	char tmp1[5]="";
	dmx_getstring(tmp1,GET_NUM);
	len=atoi(tmp1);
	scene->len+=len;
	for(int i=0;i<len;i++)
	{//nhap dia chi den 1
		lcd_clear();
		lcd_home();
		sprintf(temp,"Addr of scanner %d: ",i+1);
		lcd_write_string(temp);
		dmx_getstring(tmp1,GET_NUM);
		num=atoi(tmp1);
		arr[i]=num;
	}
	
	scene_cur=scene;
	arr_cur=arr;
	len_cur=len;
	
	HAL_ADC_Start_DMA(&hadc1,adcbuf,DMX_NUMBER_ADC);

	lcd_clear();
	lcd_home();
	lcd_write_string("select effect    press A");
	while(	KeyPad_getKey(GET_CHAR)!=YES);
	HAL_ADC_Stop_DMA(&hadc1);
}

void dmx_scan_color(SCENE *scene,uint16_t *arr,uint16_t len)
{
	
	for(int i=0;i<len;i++)
	{
		if(scene->scanner[arr[i]].Addr!=0)
		{
			dmxData[arr[i]]=scene->scanner[i].red;
			dmxData[arr[i]+1]=scene->scanner[i].green;
			dmxData[arr[i]+2]=scene->scanner[i].blue;
			dmxData[arr[i]+3]=scene->scanner[i].blink;
			dmxData[arr[i]+4]=scene->scanner[i].fade;
		}
	}
}
void dmx_add_scanner(SCENE *scene,uint16_t *arr,uint16_t len)
{
	uint16_t temp =0;
	for(int i=0;i<len;i++)
	{
		temp=scene->len-len+i;
		scene->scanner[temp].Addr=arr[i];
		scene->scanner[temp].red=adcbuf[0];
		dmxData[arr[i]]=adcbuf[0];
		scene->scanner[temp].green=adcbuf[1];
		dmxData[arr[i]+1]=adcbuf[1];
		scene->scanner[temp].blue=adcbuf[2];
		dmxData[arr[i]+2]=adcbuf[2];
		scene->scanner[temp].blink=adcbuf[3];
		dmxData[arr[i]+3]=adcbuf[3];
		scene->scanner[temp].fade=adcbuf[4];
		dmxData[arr[i]+4]=adcbuf[4];
		//scene->scanner[arr[i]].mode=??????;
	//	scene->scanner[arr[i]].timer=adcbuf[3];
	}
	
//	dmx_scan_color(scene,arr,len);
	
}
void dmx_out_scanner(SCANNER *scanner)
{
	dmxData[scanner->Addr]=scanner->red;
	dmxData[scanner->Addr+1]=scanner->green;
	dmxData[scanner->Addr+2]=scanner->blue;
	dmxData[scanner->Addr+3]=scanner->blink;
	dmxData[scanner->Addr+4]=scanner->fade;
}

void dmx_out_scene(SCENE *scene)
{
		for(int i=0;i<scene->len;i++)
	{
			dmx_out_scanner(&(scene->scanner[i]));
	}
	
	/*if(out_scene_cur<scene->len)
	{
		memset(dmxData,0,DMX_CHANNELS);
		dmx_out_scanner(&(scene->scanner[out_scene_cur]));
		out_scene_cur++;
	}
	else
	{
		out_set_cur++;
		out_scene_cur=0;
		memset(dmxData,0,DMX_CHANNELS);
		dmx_out_scanner(&(scene->scanner[out_scene_cur]));
	}*/
}

void dmx_out_set(stSET *set1)
{
/*	for(int i=0;i<set1->len;i++)
	{
			dmx_out_scene(&(set1->scene[i]));
	}*/
	if(out_scene_cur<set1->len)
	{
		memset(dmxData,0,DMX_CHANNELS);
		dmx_out_scene(&(set1->scene[out_scene_cur]));
		out_scene_cur++;
	}
	else
	{
		flagEndSet=1;
		out_scene_cur=0;
		memset(dmxData,0,DMX_CHANNELS);
		dmx_out_scene(&(set1->scene[out_scene_cur]));
			out_scene_cur=1;
	}
	
}
void dmx_auto_play()
{
	static uint8_t count=0;
	
	if(v_state==PLAYING||v_state==SELECT_AUTO)
	{
		if(count<set_len)
			dmx_out_set(&set[count]);
		else
		{
			count=0;
			dmx_out_set(&set[count]);
		}
		//set_timer(1000);
	}
	if(flagEndSet==1)
		{
			count++;
			flagEndSet=0;
		}
			
}






void dmx_select_mode()
{
	uint8_t key;
	lcd_write_string("Press B or C to select mode: ");

	do
		{
			
			key=KeyPad_getKey(GET_CHAR);
			if(key==KEYPAD4x4__VALUE_B)
				{
					mode++;
					if(mode>=3)
						mode=MANUAL;
				}
				if(key==KEYPAD4x4__VALUE_C)
					{
					
					if(mode==0)
						mode=PC;
					else
						mode--;

		    	}
					lcd_clear();
					HAL_Delay(50);
					lcd_setCursor(0,0);
					HAL_Delay(100);
					lcd_write_string("Mode present: ");
					HAL_Delay(10);
				if(mode==MANUAL)
						lcd_write_string("MANUAL");
					else 	if(mode==ANDROID)
								lcd_write_string("ANDROID");
					else 	if(mode==PC)
								lcd_write_string("PC ");
		}
			while(key!=YES);
			lcd_write_string(" OK");
			
}
void DMX_GPIO_PIN_WRITE(GPIO_PinState st)
{
		HAL_GPIO_WritePin(DMX_TX_Port,DMX_TX_Pin,st);
}
