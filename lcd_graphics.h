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

struct Polygon3D;

struct point{
	int16_t x,y;
};

//basic triangle polygon
struct polygon{
	polygon() {;}
	polygon(int16_t a0, int16_t b0, int16_t a1, int16_t b1, int16_t a2, int16_t b2)
	{
		x0 = a0; y0 = b0;
		x1 = a1; y1 = b1;
		x2 = a2; y2 = b2;
	}

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
	int16_t get_start() { return y0; }
	int16_t get_end() { return y2; }
private:
	int16_t x0,x1,x2,y0,y1,y2;
	uint16_t color;
	uint8_t type;
};

void draw_pixel(int16_t x0, int16_t y0, uint16_t color);
void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void draw_rect(int16_t x0, int16_t y0, int16_t xsize, int16_t ysize, uint16_t color);
void draw_poly(polygon p, uint16_t color);
void draw_text(char *text, int16_t x, int16_t y, uint16_t color, uint16_t max_width = SCREENX);

void flush_to_lcd();

#endif