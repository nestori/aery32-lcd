/*
	By Nestori, 2013
	don't blame me when stuff burns
	I mean if
*/
#include "simscreen.h"
#ifdef FORPC
//simulated screen, 8 msb's are unused, after that comes RGB, each 8 bits. lsb = lsb of blue
uint16_t current_color = 0;
uint32_t simscreen[SCREENX*SCREENY];
//cursor position
uint16_t simscreen_x_pointer = 0;
uint16_t simscreen_y_pointer = 0;

//moves cursor to given position
void simscreen_movex(int16_t x)
{
	if (x < 0) x = 0;
	if (x >= SCREENX) x = SCREENX-1;
	simscreen_x_pointer = x;
}
//moves cursor to given position
void simscreen_movey(int16_t y)
{
	if (y < 0) y = 0;
	if (y >= SCREENY) y = SCREENY-1;
	simscreen_y_pointer = y;
}

//sets pixel to given color, at current cursor position
void simscreen_setpixel(uint16_t color)
{
	//uint16_t's to avoid warning about overflow
	uint16_t red	= ((color & 0xF800) << 3) >> 11;
	uint16_t green	= ((color & 0x7E0) << 2) >> 5;
	uint16_t blue	= (color & 0x1F) << 3;
	simscreen[simscreen_x_pointer+simscreen_y_pointer*SCREENX] = (red << 2*8) | (green << 8) | blue;
	simscreen_x_pointer++;
	if (simscreen_x_pointer >= SCREENX)
	{
		simscreen_x_pointer = 0;
		simscreen_y_pointer++;
	}
	if (simscreen_y_pointer >= SCREENY)
	{
		simscreen_y_pointer = 0;
	}
}

//flushes simulated screen to actual computer screen
void simscreen_render()
{	
	glBegin(GL_POINTS);
	for(int y = 0; y < SCREENY; y++)
	{
		for(int x = 0; x < SCREENX; x++)
		{
			glColor3ub(	((simscreen[x+y*SCREENX] >> 16) & 0xFF),
						((simscreen[x+y*SCREENX] >> 8) & 0xFF),
						(simscreen[x+y*SCREENX] & 0xFF));
			glVertex2i(x,y);
		}
	}
	glEnd();
}

#endif