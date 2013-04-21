#ifndef DEFINES_H
#define DEFINES_H
// FORPC = compiling for pc, with sfml2.0
// FORAERY = compiling for aery32
#define FORPC
//x size of the screen, better be divisible by 20, or change loop-unrolling in lcd_graphics.cpp, void draw_linebuffer() function
#define SCREENX 320
//y size of the screen
#define SCREENY 240

#include <stdint.h>

#endif