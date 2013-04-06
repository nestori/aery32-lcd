/*
	By Nestori, 2013
	don't blame me when stuff burns
	I mean if
*/
#ifndef INITSTUFF_H
#define INITSTUFF_H
#include "lcd_defines.h"
#include "lowlvl.h"

#ifdef FORAERY
	#include "board.h"
	using namespace aery;
	#define LED AVR32_PIN_PC04
#endif

#ifdef FORPC
	#include <SFML/OpenGL.hpp>
	#include <SFML/Graphics.hpp>
	#include "simscreen.h"
	void initGL(int xkoko, int ykoko);
	extern sf::RenderWindow sfmlwindow;
#endif
	void init();
#endif