/*
	By Nestori, 2013
	don't blame me when stuff burns
	I mean if
*/
#include "lowlvl.h"
#ifdef FORAERY
//data pins, db00 - db15 are portb 0 - 15 pins
	
//controlling pins 
const uint8_t PIN_RS = AVR32_PIN_PA23;
const uint8_t PIN_WR = AVR32_PIN_PA24;
const uint8_t PIN_RD = AVR32_PIN_PA25;
const uint8_t PIN_CS = AVR32_PIN_PA26;
const uint8_t PIN_RESET = AVR32_PIN_PA27;


// low level
void lcd_com(uint8_t command)
{
	//set cs, rs, and data pins low, hard-coding a value here does not incr performance.
	AVR32_GPIO_LOCAL.port[1].ovrc = 0xFFFF;
	AVR32_GPIO_LOCAL.port[0].ovrc = (1 << GPIO_NUM2PIN(PIN_CS)) | (1 << GPIO_NUM2PIN(PIN_RS));
	//set data pins
	AVR32_GPIO_LOCAL.port[1].ovrs = command;
	//pulse wr pins, and set cs, rs back on
	AVR32_GPIO_LOCAL.port[0].ovrc = (1 << GPIO_NUM2PIN(PIN_WR));
	AVR32_GPIO_LOCAL.port[0].ovrs = (1 << GPIO_NUM2PIN(PIN_WR)) | (1 << GPIO_NUM2PIN(PIN_CS)) | (1 << GPIO_NUM2PIN(PIN_RS));
}

void lcd_data(uint16_t data)
{
	AVR32_GPIO_LOCAL.port[1].ovrc = 0xFFFF;
	AVR32_GPIO_LOCAL.port[0].ovrc = (1 << GPIO_NUM2PIN(PIN_CS));
	AVR32_GPIO_LOCAL.port[1].ovrs = data;
	AVR32_GPIO_LOCAL.port[0].ovrc = (1 << GPIO_NUM2PIN(PIN_WR));
	AVR32_GPIO_LOCAL.port[0].ovrs = (1 << GPIO_NUM2PIN(PIN_WR)) | (1 << GPIO_NUM2PIN(PIN_CS));
}

void command_lcd(uint8_t command, uint16_t data)
{
	lcd_com(command);
	lcd_data(data);
}

#endif
//aery32 ends here

#ifdef FORPC
void lcd_com(uint8_t command)
{
	//we dont need this to do anything on pc
}
void lcd_data(uint16_t data)
{
//i hope this does not need commenting, other than data == color
	simscreen_setpixel(data);
}
void command_lcd(uint8_t command, uint16_t data)
{
	//if we get command 0x4e, by ssd1289 documentation it means moving y pointer position in lcd's graphical ram
	if (command == 0x4E)
	{
		simscreen_movey(data);
	}
	//moves x pointer position in lcd's graphical ram
	if (command == 0x4F)
	{
		simscreen_movex(data);
	}
}
#endif