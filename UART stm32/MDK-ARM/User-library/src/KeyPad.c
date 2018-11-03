


#include "KeyPad.h"

//static char KeyPad_getascii(char button);
void KeyPad_init()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = Row_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Row_1_Port, &GPIO_InitStruct);
	

	GPIO_InitStruct.Pin = Row_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Row_2_Port, &GPIO_InitStruct);
	
	
	GPIO_InitStruct.Pin = Row_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Row_3_Port, &GPIO_InitStruct);
	

	GPIO_InitStruct.Pin = Row_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(Row_4_Port, &GPIO_InitStruct);
	

	GPIO_InitStruct.Pin = Column_1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Column_1_Port, &GPIO_InitStruct);
	
	
	GPIO_InitStruct.Pin = Column_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Column_2_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = Column_3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Column_3_Port, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = Column_4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(Column_4_Port, &GPIO_InitStruct);
	
}

unsigned char KeyPad_check_but()
{ //Ki?m tra nút nh?n
		HAL_GPIO_WritePin(Row_1_Port, Row_1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Row_2_Port, Row_2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Row_3_Port, Row_3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Row_4_Port, Row_4_Pin, GPIO_PIN_RESET);
      //  row0=row1=row2=row3=0; //Xu?t logic 0 ra các hàng d? ki?m tra
	
	  if(HAL_GPIO_ReadPin(Column_1_Port, Column_1_Pin) == GPIO_PIN_RESET)
			return 1;
		if(HAL_GPIO_ReadPin(Column_2_Port, Column_2_Pin) == GPIO_PIN_RESET)
			return 1;
		if(HAL_GPIO_ReadPin(Column_3_Port, Column_3_Pin) == GPIO_PIN_RESET)
			return 1;
		if(HAL_GPIO_ReadPin(Column_4_Port, Column_4_Pin) == GPIO_PIN_RESET)
			return 1;
      //  if(!col0 || !col1 || !col2 || !col3 )return 1; //OK có nút du?c nh?n
        return 0; //B? qua, không có nút nào du?c nh?n
}
void KeyPad_scan_row(unsigned char r){ //Quét hàng
        //row0=row1=row2=row3=1;
		HAL_GPIO_WritePin(Row_1_Port, Row_1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(Row_2_Port, Row_2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(Row_3_Port, Row_3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(Row_4_Port, Row_4_Pin, GPIO_PIN_SET);
    if(r==0)
			HAL_GPIO_WritePin(Row_1_Port, Row_1_Pin, GPIO_PIN_RESET);//row0=0;
     else if(r==1)
		  HAL_GPIO_WritePin(Row_2_Port, Row_2_Pin, GPIO_PIN_RESET);// row1=0;
     else if(r==2)
			HAL_GPIO_WritePin(Row_3_Port, Row_3_Pin, GPIO_PIN_RESET);// row2=0;
     else if(r==3)
			HAL_GPIO_WritePin(Row_4_Port, Row_4_Pin, GPIO_PIN_RESET);// row3=0;
}
unsigned char KeyPad_check_col(){ //Ki?m tra c?t
     unsigned char c=0;
     if(HAL_GPIO_ReadPin(Column_1_Port, Column_1_Pin) == GPIO_PIN_RESET)
			 c=1; 
     else if(HAL_GPIO_ReadPin(Column_2_Port, Column_2_Pin) == GPIO_PIN_RESET)
			 c=2;
     else if(HAL_GPIO_ReadPin(Column_3_Port, Column_3_Pin) == GPIO_PIN_RESET)
			 c=3;
     else if(HAL_GPIO_ReadPin(Column_4_Port, Column_4_Pin) == GPIO_PIN_RESET)
			 c=4;
     return c; //Tr? v? v? trí c?t
}
unsigned char KeyPad_get_key(){ //Hàm tr? v? v? trí nút nh?n 1-16, b?ng 0 là không nh?n
        unsigned char row,col;
        if(KeyPad_check_but()){ //Ki?m tra xem có nút nào du?c nh?n không
                HAL_Delay(KeyPad_delay); //Ch?ng rung phím do ti?p xúc kém
                if(KeyPad_check_but()){ //N?u dúng là nút dang nh?n không ph?i do rung phím
                        for(row=0;row<4;row++){ 
                                KeyPad_scan_row(row); // Quét hàng
                                col=KeyPad_check_col(); //L?y v? trí c?t
                                if(col>0)
																	return ((row*4)+col); //Tr? v? hàm v? trí nút nh?n
                        } 
                }
        }
        return 0; //Không có nút nào du?c nh?n ho?c dã nh? nút
}
bool checknum(char c)
{
	if(c==KEYPAD4x4__VALUE_0||c==KEYPAD4x4__VALUE_1||c==KEYPAD4x4__VALUE_2||c==KEYPAD4x4__VALUE_3||
		c==KEYPAD4x4__VALUE_4||c==KEYPAD4x4__VALUE_5||c==KEYPAD4x4__VALUE_6||c==KEYPAD4x4__VALUE_7||
	c==KEYPAD4x4__VALUE_8||c==KEYPAD4x4__VALUE_9)
		return true;
	else 
		return false;
}

unsigned char KeyPad_getKey(uint8_t option)
{
	
	unsigned char key=0;
	if(option == GET_CHAR)
	{
			do
		{
			key=KeyPad_get_key();
		} while(key==0||checknum(key));
		while(KeyPad_get_key()!=0);
	}else if (option == GET_NUM)
	{
			do
		{
			key=KeyPad_get_key();
		} while(key==0||!checknum(key));
		while(KeyPad_get_key()!=0);
	}else 
	{
				do
		{
			key=KeyPad_get_key();
		} while(key==0);
		while(KeyPad_get_key()!=0);
	}
	
	return key;
}
char KeyPad_getascii(char num_button)
{
	
	switch(num_button)
	{
		case KEYPAD4x4__VALUE_1    : return 		  '1'     ;
		case KEYPAD4x4__VALUE_2    : return	      '2'     ;
	  case KEYPAD4x4__VALUE_3    : return       '3'     ;
		case KEYPAD4x4__VALUE_4    : return       '4'     ;
		case KEYPAD4x4__VALUE_5    : return       '5'     ;
		case KEYPAD4x4__VALUE_6    : return       '6'     ;
		case KEYPAD4x4__VALUE_7    : return       '7'     ;
		case KEYPAD4x4__VALUE_8    : return       '8'     ;
		case KEYPAD4x4__VALUE_9    : return       '9'     ;
		case KEYPAD4x4__VALUE_A    : return       'A'     ;
		case KEYPAD4x4__VALUE_B    : return       'B'     ;
		case KEYPAD4x4__VALUE_C    : return       'C'     ;
		case KEYPAD4x4__VALUE_D    : return       'D'     ;
		case KEYPAD4x4__VALUE_0    : return       '0'     ;
		case KEYPAD4x4__VALUE_multi: return       '*'     ;
		case KEYPAD4x4__VALUE_sharp: return       '#'     ;
		default : return 0;
	}
}
