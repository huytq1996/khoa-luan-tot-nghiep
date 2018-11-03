#include "DMX-handle.h"
#include "string.h"
#include "stdlib.h"

extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_adc1;
extern TIM_HandleTypeDef htim2,htim3;
extern UART_HandleTypeDef huart1;
char v_display_buffer[2][17]={"",""};
uint8_t v_state=SELECT_ADD;
stSET set[DMX_NUMBER_SET];
uint8_t element_arr_scanner=0;
uint8_t element_arr_scene=0;
uint8_t set_len=0;
SCENE scene_select[DMX_NUMBER_SCENE];
SCENE *scene_cur=NULL;
uint8_t *arr_cur=NULL;
uint8_t len_cur=0;
static uint8_t out_set_cur=0;
static uint8_t out_scene_cur=0;
extern uint8_t dmxData[DMX_CHANNELS+1];
extern uint32_t adcbuf[DMX_NUMBER_ADC];
void (*scene_current)(void);
//void set_timer(uint32_t time);
static void copyarr(uint8_t *des, const uint8_t *src,uint8_t n );
static void copyarr(uint8_t *des, const uint8_t *src,uint8_t n )
{
	for(int i=0;i<n;i++)
	{
		des[i]=src[i];
	}
}
void dmx_main()
{
	static uint8_t st=0;
	while(1)
	{
		switch (v_state)
		{
			case SELECT_ADD :HAL_ADC_Stop_IT(&hadc1);
			st=SELECT_ADD;
					dmx_select_set();break;
			case SELECT_AUTO :
				if(st==SELECT_ADD)
				{
					HAL_TIM_Base_Start_IT(&htim3);		
					st=SELECT_AUTO;
				}
					//	dmx_auto_play();
			default : break;
		}
				
			
	}
}
static void append(char subject[], const char insert[], int pos) {
 
		uint8_t len = strlen(subject) + strlen(insert) + 2;
		
			
		char *buf = (char*)malloc(len);
		memset(buf, 0, 100);
    strncpy(buf, subject, pos); // copy at most first pos characters
    len = strlen(buf);
    strcpy(buf+len, insert); // copy all of insert[] at the end
    len += strlen(insert);  // increase the length by length of insert[]
    strcpy(buf+len, subject+pos); // copy the rest
		if(strlen(buf)>=MaxString)
    buf[16]=0;  // copy it back to subject
		strcpy(subject, buf); 
    // Note that subject[] must be big enough, or else segfault.
    // deallocate buf[] here, if used malloc()
    // e.g. free(buf);
		free(buf);
}
//char string[17]="";

char *dmx_getstring()
{
	char buf=0;
	char *string=calloc(1,5);
	uint8_t tmplen=0,count=0;
	buf=KeyPad_getKey(GET_CHAR_NUM);
	
	while(buf!=SELECT_ADD&&buf!=YES&&buf!=NO&&buf!=SELECT_AUTO&&count<4)
	{

		tmplen=strlen(string);
		
		if(buf==KEYPAD4x4__VALUE_multi)
		{
			if(tmplen>0)
			{
				string[tmplen-1]=0;
				if(count>0)
				{
					count--;
				}else{}
			}
			else{}
		}
		else
		{
			if(checknum(buf))
			{
				string[tmplen]=KeyPad_getascii(buf);
				count++;
			}
		}
		string[tmplen+1]=0;
		
		buf=  KeyPad_getKey(GET_CHAR_NUM);
	
	//append(string,buf,0);
	//append(v_display_buffer,buf,0);
	}
		if(buf==NO)
			{
				free(string);
				string=NULL;
				v_state=NO;
				
			}
		else if(buf==YES)
			{
				free(string);
				string=NULL;
				v_state=YES;
				
			}
		else if(buf==SELECT_AUTO)
			{
				free(string);
				string=NULL;
				v_state=SELECT_AUTO;
				
			}
	return string;
}
void dmx_add_group(SCENE *scene,uint8_t *arr)
{
	uint8_t num=0,len=0;
	char *tmp1;

//nhap so luong den cua group 
	tmp1=dmx_getstring();
	len=atoi(tmp1);
	scene->len+=len;
	for(int i=0;i<len;i++)
	{//nhap dia chi den 1
		tmp1=dmx_getstring();
		num=atoi(tmp1);
		arr[i]=num;
	}
	
	scene_cur=scene;
	arr_cur=arr;
	len_cur=len;
	HAL_ADC_Start_IT(&hadc1);
	//dmx_add_scanner(scene,arr,len);
//	tmp1=dmx_getstring();
	free(tmp1);
	while(v_state!=YES)
	{
		tmp1=dmx_getstring();
		free(tmp1);
	}
	v_state=SELECT_ADD;
	HAL_ADC_Stop_IT(&hadc1);
}
void dmx_add_scene(SCENE *scene)
{
	uint8_t group;
	char *tmp1;
  
//nhap so luong group 
	tmp1=dmx_getstring();
	group=atoi(tmp1);
	scene->len=0;
	for(int i=0;i<group;i++)
	{
		uint8_t arr[16]="";
		memset(dmxData,0,DMX_CHANNELS);
		dmx_add_group(scene,arr);
	}
		free(tmp1);
}
void dmx_scan_color(SCENE *scene)
{
	
	for(int i=0;i<scene->len;i++)
	{
		if(scene->scanner[i].Addr!=0)
		{
			dmxData[scene->scanner[i].Addr]=scene->scanner[i].red;
			dmxData[scene->scanner[i].Addr+1]=scene->scanner[i].green;
			dmxData[scene->scanner[i].Addr+2]=scene->scanner[i].blue;
			dmxData[scene->scanner[i].Addr+3]=scene->scanner[i].blink;
			dmxData[scene->scanner[i].Addr+4]=scene->scanner[i].fade;
		}
	}
}
void dmx_add_scanner(SCENE *scene,uint8_t *arr,uint8_t len)
{
	for(int i=0;i<len;i++)
	{
		scene->scanner[i].Addr=arr[i];
		scene->scanner[i].red=adcbuf[0];
		scene->scanner[i].green=adcbuf[1];
		scene->scanner[i].blue=adcbuf[2];
		scene->scanner[i].blink=adcbuf[3];
		scene->scanner[i].fade=adcbuf[4];
		//scene->scanner[arr[i]].mode=??????;
	//	scene->scanner[arr[i]].timer=adcbuf[3];
	}
	
	dmx_scan_color(scene);
	
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

	if(out_scene_cur<scene->len)
	{
		memset(dmxData,0,DMX_CHANNELS);
		dmx_out_scanner(&(scene->scanner[out_scene_cur]));
		out_scene_cur++;
	}
	else
	{
		out_set_cur++;
		out_scene_cur=0;
	}
}
void dmx_out_set(stSET *set1)
{

	if(out_set_cur<set1->len)
	{
		dmx_out_scene(&(set1->scene[out_set_cur]));
		//out_set_cur++;
	}
	else
	{
		out_set_cur=0;
	}
	
}
void dmx_auto_play()
{
	static uint8_t count=0;
	
	if(v_state==SELECT_AUTO)
	{
		if(count<set_len)
			dmx_out_set(&set[count]);
		else
			count=0;
		//set_timer(1000);
	}
}


void dmx_select_set()
{
	//nhap so luong set
	char *tmp1;
	//nhap so luong set
	tmp1=dmx_getstring();
	set_len=atoi(tmp1);
	//set tung canh
	for(int i=0;i<set_len;i++)
	{
	
		dmx_add_set(&set[i]);
		
	}
	tmp1=dmx_getstring();
	
	free(tmp1);
	while(v_state!=SELECT_AUTO)
	{
	tmp1=dmx_getstring();
	
	free(tmp1);	
	}
	HAL_ADC_Stop_IT(&hadc1);
}
void dmx_add_set(stSET *SET)
{
		
	
	char *tmp1;
	//nhap so luong canh
	tmp1=dmx_getstring();
	SET->len=atoi(tmp1);
		//set tung canh
	for(int i=0;i<SET->len;i++)
	{
		//nhap canh thu i+1
		dmx_add_scene(&(SET->scene[i]));		
	}
	free(tmp1);
}
HAL_StatusTypeDef save_set(stSET set)
{
	
}