#ifndef _DMX_HANDLE 
#define _DMX_HANDLE 
#include "stm32f4xx_hal.h"
#include "string.h"
#include "KeyPad.h"
#include <stdbool.h>
#include "DMX-Init.h"
#define MaxString 17


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
	SELECT_SCENE
	
//	SELECT_AUTO = 16;
	
}STATE;

typedef struct SCANNER
{
	uint8_t Addr;
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	uint8_t blink;
	uint8_t fade;
	
}SCANNER;
typedef struct SCENE
	{

		SCANNER scanner[DMX_NUMBER_SCENE];
		uint8_t len;
	}SCENE;
typedef struct 
{
	SCENE scene[16];
	uint8_t len;
}stSET;
void dmx_select_set(void);
void dmx_add_scene(SCENE *SCENE);
void dmx_add_group(SCENE *scene,uint8_t *arr);
void dmx_add_set(stSET *SET);
void dmx_main(void);
void dmx_sang_dan(void);
void dmx_tat_dan(void);
void dmx_next_scene(void);
void dmx_auto_play(void);
void dmx_add_scanner(SCENE *scene,uint8_t *arr,uint8_t n);
void dmx_add_color(void);
void dmx_scene_normal(uint8_t scene_number);
#endif
