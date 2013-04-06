/*
	By Nestori, 2013
	don't blame me when stuff burns
	I mean if
*/
#ifndef LOWLEVELSTUFF_H
#define LOWLEVELSTUFF_H
#include "lcd_defines.h"

#ifdef FORAERY
	#include <aery32/all.h>
	#include "board.h"
	using namespace aery;
	
	//controlling pins 
	extern const uint8_t PIN_RS;
	extern const uint8_t PIN_WR;
	extern const uint8_t PIN_RD;
	extern const uint8_t PIN_CS;
	extern const uint8_t PIN_RESET;

	//set data-pins on
	inline void set_pins()
	{
		AVR32_GPIO_LOCAL.port[0].ovrc = (1 << GPIO_NUM2PIN(PIN_CS));
	}
	inline void set_data(uint16_t data)
	{
		AVR32_GPIO_LOCAL.port[1].ovrc = 0xFFFF;

		AVR32_GPIO_LOCAL.port[1].ovrs = data&0x00FF;
		AVR32_GPIO_LOCAL.port[1].ovrs = data&0xFF00;
		
	}
	//pulse write so datapins will be written on the lcd
	inline void pulse_pins()
	{
		AVR32_GPIO_LOCAL.port[0].ovrc = (1 << GPIO_NUM2PIN(PIN_WR));
		AVR32_GPIO_LOCAL.port[0].ovrs = (1 << GPIO_NUM2PIN(PIN_WR));	
	}
	//if you call set_pins, call unset_pins after you're done with that color/data
	//you probably dont really have to :D
	inline void unset_pins()
	{
		AVR32_GPIO_LOCAL.port[0].ovrs = (1 << GPIO_NUM2PIN(PIN_CS));
	}
#endif

#ifdef FORPC
	#include "simscreen.h"
	inline void set_pins(){;}
	
	inline void set_data(uint16_t data)
	{
		current_color = data;
	}
	inline void pulse_pins()
	{
		simscreen_setpixel(current_color);
	}
	inline void unset_pins(){;}
#endif

//commands
void lcd_com(uint8_t command);
void lcd_data(uint16_t data);
void command_lcd(uint8_t command, uint16_t data);

#endif