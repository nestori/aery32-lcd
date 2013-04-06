/*
	By Nestori, 2013
	don't blame me when stuff burns
	I mean if
*/
#include "lcd_defines.h"
#ifdef FORPC
#ifndef SIMULATED_SCREEN_H
#define SIMULATED_SCREEN_H
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>

extern uint16_t current_color;

void simscreen_movex(int16_t x);
void simscreen_movey(int16_t y);
void simscreen_setpixel(uint16_t color);

void simscreen_render();

inline void delay_ms(int32_t time)
{
	sf::sleep(sf::milliseconds(time));
}

#endif
#endif