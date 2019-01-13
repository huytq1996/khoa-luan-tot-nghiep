#ifndef _DMX_HANDLE 
#define _DMX_HANDLE 
#include "stm32f4xx_hal.h"
#include "string.h"
#include "KeyPad.h"
#include "DMX-Init.h"
#include <stdbool.h>
#define MaxString 17


//#define SELECT_SCENE 7

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
void DMX_GPIO_PIN_WRITE(GPIO_PinState st);
void dmx_select_mode(void);
#endif
