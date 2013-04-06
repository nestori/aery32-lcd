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

//experimental

class Drawable{
public:
	void init_as_rect(int16_t x, int16_t y, int16_t xsize, int16_t ysize, uint16_t color_in);
	void init_as_poly(point a, point b, point c, uint16_t color_in);
	void init_as_line(int16_t x0_, int16_t y0_, int16_t x1_, int16_t y1_, uint16_t color_in);
	
	void draw(int16_t yline);
private:
	
	int16_t x0,x1,x2,y0,y1,y2;
	uint16_t color;
	uint8_t type;
};


void draw_b_rect(int16_t x0, int16_t y0, int16_t xsize, int16_t ysize, uint16_t color);
void draw_b_poly(polygon p, uint16_t color);

void draw_linebuffer();
void draw_line_to_buf(int16_t x, int16_t length, uint16_t color);

void flush_to_lcd();

void sort_points_by_y(point &a, point &b, point &c);
int16_t find_x_along_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t target_x, int16_t target_y);
//experimental



//monochromatic noise uses rednoise level as the amount of noise.
//to use it do something like .setnoise(false,false,true,6,0,0);
//and colornoise levels are 3bit, so use vals from 0 to 7
//this whole thing is highly experimental and unoptimized
//so it is slow, but works atm
struct texture{
	texture(): color(), texture_num(), alpha(), noise(){}	
	void setnoise(bool texturenoise, bool colornoise = false, bool mononoise = false, uint8_t redlvl = 0, uint8_t greenlvl = 0, uint8_t bluelvl = 0)
	{
		noise |= ((texturenoise & 0x1) << 15) | ((colornoise & 0x1) << 14) | ((mononoise & 0x1) << 13) |
		( (redlvl&0x7) << 6 | (greenlvl&0x7) << 3 | (bluelvl&0x7) );
	}
	bool gettexnoise() { return (noise & 0x8000);}
	bool getcolornoise() { return (noise & 0x4000);}
	bool getmononoise() { return (noise & 0x2000);}
	uint8_t getnoisered() {return (noise >> 6) & 0x7;}
	uint8_t getnoisegreen() {return (noise >> 3) & 0x7;}
	uint8_t getnoiseblue() {return noise & 0x7;}
	
	uint16_t color;
	uint8_t texture_num;
	uint8_t alpha;
/*	16 = msb = texture noise on/of
	15 = msb-1 = color noise on/off
	14 = msb-2 = monochromatic noise
	9 - 7	= peak red noise	3
	6 - 4	= peak green noise	3
	3 - 1	= peak blue noise	3
*/
	uint16_t noise;
};

//clear whole screen with given color
void clear_screen(uint16_t color);

//draw pixel to given location with given color, safe to give range outside of the screen limits, like -1,-1 coords
void draw_pixel(int16_t x, int16_t y, uint16_t color);
//draws line from x0,y0 to x1,y1 with given color, safe to give range outside of the screen limits
void draw_line(int16_t x0, int16_t x1, int16_t y0, int16_t y1, uint16_t color);
//draws horizontal line of given coords with given length+color, is much faster than draw_line
void draw_line_hor(int16_t x0, int16_t length, int16_t y0, uint16_t color);
//draws rect, safe to give range outside the screen limits
void draw_rect(int16_t x0, int16_t xsize, int16_t y0, int16_t ysize, uint16_t color);
//draws text, safe to give range outside the screen limits
void draw_text(char *array, int16_t x, int16_t y, uint16_t color);

//gets x min limits for polygon on every y-line, and x lengths on said lines
void get_polygon_limits(int16_t x0, int16_t x1, int16_t y0, int16_t y1, int16_t *xvals, int16_t *xlens, uint16_t size, int16_t minY);
//draws given polygon with given color, safe to give polygon coords outside the screen limits
void draw_polygon(polygon p, uint16_t color);

//experimental
//draws rect with given texture
void draw_rect(int16_t x0, uint16_t xsize, int16_t y0, uint16_t ysize, uint16_t color, texture tex);

#endif