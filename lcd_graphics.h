/*
	By Nestori, 2013
	don't blame me when stuff burns
	I mean if
*/
#ifndef GRAPHICSSTUFF_H
#define GRAPHICSSTUFF_H

#include "lcd_defines.h"
#include "lut.h"
#include "lowlvl.h"
#include "lcd_math.h"
#include <string.h>
#include <algorithm>

struct point{
	int16_t x,y;
};

//basic triangle polygon
struct polygon{
	int16_t x0, y0;
	int16_t x1, y1;
	int16_t x2, y2;
};

class Drawable{
public:
	void init_as_rect(int16_t x, int16_t y, int16_t xsize, int16_t ysize, uint16_t color_in);
	void init_as_poly(point a, point b, point c, uint16_t color_in);
	void init_as_line(int16_t x0_, int16_t y0_, int16_t x1_, int16_t y1_, uint16_t color_in);
	void init_as_text(char *text, int16_t x, int16_t y, uint16_t color, uint16_t max_width = SCREENX);
	void draw(int16_t yline);
private:
	int16_t x0,x1,x2,y0,y1,y2;
	uint16_t color;
	uint8_t type;
};

void draw_b_pixel(int16_t x0, int16_t y0, uint16_t color);
void draw_b_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void draw_b_rect(int16_t x0, int16_t y0, int16_t xsize, int16_t ysize, uint16_t color);
void draw_b_poly(polygon p, uint16_t color);
void draw_b_text(char *text, int16_t x, int16_t y, uint16_t color, uint16_t max_width = SCREENX);

void flush_to_lcd();

#endif