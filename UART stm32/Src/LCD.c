#include "LCD.h"
#include "stm32f4xx_hal_i2c.h"
uint8_t _displayFunction = 0;
uint8_t _numLines = 0;
uint8_t _displayControl = 0;
uint8_t _displayMode = 0;
int rowCheck = 0, colCheck = 0;
I2C_HandleTypeDef hi2c1;
void lcd_init(uint8_t cols, uint8_t lines)
{
	if (lines > 1) 
	{
		_displayFunction |= LCD_2LINE;
	}

	_numLines = lines;
	HAL_Delay(1);
	lcd_expanderWrite(0);
	HAL_Delay(1);
	lcd_write4bits(0x03 << 4);
	HAL_Delay(5);
	lcd_write4bits(0x03 << 4);
	HAL_Delay(5);
	lcd_write4bits(0x03 << 4);
	HAL_Delay(1);
	lcd_write4bits(0x02 << 4);
	HAL_Delay(5);

	lcd_command(LCD_FUNCTIONSET | _displayFunction);
	_displayControl = LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKON;
	lcd_display();
	_displayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	lcd_command(LCD_ENTRYMODESET | _displayMode);
	lcd_clear();
	lcd_home();


}

void lcd_expanderWrite(uint8_t _data)
{
	uint8_t cmd[1];
	cmd[0] = _data | LCD_BACKLIGHT;
//	HAL_I2C_Mem_Write(&hi2c1, LCD_ADD, 0x00, 1, cmd, 1, 0x100);
	HAL_I2C_Mem_Write(&hi2c1, LCD_ADD, 0x00, 1, cmd, 1, 0x100);
}

void lcd_pulseEnable(uint8_t _data)
{
	lcd_expanderWrite(_data | En);
	HAL_Delay(1);

	lcd_expanderWrite(_data & ~En);
	HAL_Delay(1);

}

void lcd_write4bits(uint8_t value)
{
	lcd_expanderWrite(value);
	lcd_pulseEnable(value);
}

void lcd_send(uint8_t value, uint8_t mode)
{
	uint8_t highnib = value & 0xf0;
	uint8_t lownib = (value << 4) & 0xf0;
	lcd_write4bits((highnib) | mode);
	lcd_write4bits((lownib) | mode);
}

void lcd_write(uint8_t value)
{
	lcd_send(value, Rs);
	colCheck++;
	if (colCheck == 16)
	{
		if (rowCheck == 0)
		{
			lcd_setCursor(0, 1);
		}
		else
		{
			lcd_clear();
			lcd_home();
		}
		
	}
}




void lcd_command(uint8_t value)
{
	lcd_send(value, 0);
}

void lcd_display()
{
	_displayControl |= LCD_DISPLAYON;
	lcd_command(LCD_DISPLAYCONTROL | _displayControl);
}




void lcd_clear(void)
{
	lcd_command(LCD_CLEARDISPLAY);
	HAL_Delay(5);
}

void lcd_home()
{
	lcd_command(LCD_RETURNHOME);
	HAL_Delay(3);
	rowCheck = 0;
	colCheck = 0;
}
void lcd_setCursor(uint8_t col, uint8_t row)
{
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	if (row > _numLines) {
		row = _numLines - 1;    // we count rows starting w/0
	}
	lcd_command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
	rowCheck = row;
	colCheck = col;
}
/*int colorNumber()
{
	int i,k,count;
	k = 0;
	i=100;
	count = 0;
	while (count<3)
	{
	i = Scankey();
	switch (i)
		{
		case 1:
			lcd_write('1');
			break;
		case 2:
			lcd_write('2');
			break;
		case 3:
			lcd_write('3');
			break;
		case 4:
			lcd_write('4');
			break;
		case 5:
			lcd_write('5');
			break;
		case 6:
			lcd_write('6');
			break;
		case 7:
			lcd_write('7');
			break;
		case 8:
			lcd_write('8');
			break;
		case 9:
			lcd_write('9');
			break;
		case 0:
			lcd_write('0');
			break;
		}
		if (i<=9 && i>=0)
		{
		k= (k*10) +i;
		count++;
		}
		HAL_Delay(200);
		i=100; 
	}
	
		return k;
}*/