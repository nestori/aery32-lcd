/*
	By Nestori, 2013
	don't blame me when stuff burns
	I mean if
*/
#include "lcd_graphics.h"

void draw_linebuffer();
void draw_line_to_buf(int16_t x, int16_t length, uint16_t color);
void draw_pixel_to_buf(int16_t x, uint16_t color);
void sort_points_by_y(point &a, point &b, point &c);

//linebuffer for lcd, color depth is 16-bits so uint16 == 1 pixel
//this will be drawn for each y-line. and each y-line gets it's own pixels from drawable_buffer primitives
uint16_t line_buffer[SCREENX] = {};

//buffer sizes

const uint16_t buf_amount_max = 150;
const uint16_t text_amount_max = 512;

//drawable primitives buffered here, for each drawn frame
uint16_t buffered_amount = 0;
Drawable drawable_buffer[buf_amount_max];

uint16_t start_drawing[buf_amount_max];
uint16_t end_drawing[buf_amount_max];

//text buffer pointer points to place where text can be written, on text_buffer
uint16_t text_buffer_pointer = 0;
char text_buffer[text_amount_max];


void Drawable::init_as_rect(int16_t x, int16_t y, int16_t xsize, int16_t ysize, uint16_t color_in)
{
	buffered_amount++;
	type = 0;
	color = color_in;
	x0 = x;
	y0 = y;
	x1 = xsize;
	y1 = ysize;
	
	//y2 = max y val
	y2 = y0+y1;
}

void Drawable::init_as_poly(point a, point b, point c, uint16_t color_in)
{
	buffered_amount++;
	color = color_in;
	
	sort_points_by_y(a,b,c);
	/*
		we need some order here
		so lets order the coords in following order:
		let x0,y0 be min  y, min x point. in that order
		let x1,y1 be next y, min x point
		let x2,y2 be max  y, max x point
		
		if the triangle is pointy side up, like a pyramid, init it as type 1
		if it upside down, like ice cream cone, init it as type 2
	*/
	
	//assign drawable's vals, and lets correct them below v
	x0 = a.x; y0 = a.y;
	x1 = b.x; y1 = b.y;
	x2 = c.x; y2 = c.y;
	
	//after sorting, if 2 last points share y-val, then we have pyramid
	if (b.y == c.y)
	{
		type = 1;
		
		//if b's x is greater, it should be last x2 etc
		if (b.x > c.x)
		{
			x1 = c.x; y1 = c.y;
			x2 = b.x; y2 = b.y;
		}
		
	// if 2 first values share y-val, then we have top-down triangle
	} else {
		type = 2;
		
		//if a's x is bigger than b's, we need to flip them
		if (a.x > b.x)
		{
			x0 = b.x; y0 = b.y;
			x1 = a.x; y1 = a.y;
		}
	}
	

}
void Drawable::init_as_line(int16_t x0_, int16_t y0_, int16_t x1_, int16_t y1_, uint16_t color_in)
{
	buffered_amount++;
	type = 3;
	color = color_in;
	x0 = x0_; x1 = x1_;
	y0 = y0_; y1 = y1_;
	
	//lets calculate y-limits for the line, so we can check against them when drawing
	
	//if y1 is smaller than y0, we switch em around, y0 needs to be smaller
	if (y1 < y0)
	{
		y0 = y1_;
		y1 = y0_;
		x0 = x1_;
		x1 = x0_;
	}

	//let y2 be our max-y value
	y2 = y1;
}

void Drawable::init_as_text(char *text, int16_t x_in, int16_t y_in, uint16_t color_in, uint16_t max_width)
{
	if (text_buffer_pointer+strlen(text) >= text_amount_max) return;
	buffered_amount++;
	
	//x2 is where the actual text starts in text_buffer
	x2 = text_buffer_pointer;
	
	//y1 is the length of the text
	y1 = strlen(text);
	
	type = 4;
	x0 = x_in;
	y0 = y_in;
	color = color_in;
	
	//x1 is the max width of the text, if the text is longer than this, it will be split into n lines
	if (max_width < 6) max_width = 6;
	x1 = max_width;
	
	//init y1 as max y val for easy checking on draw
	//lets count how many lines we need for the text
	int16_t lines = y2*6 / max_width;
	//we need atleast 1 line, and text is always, and will be 7 pixels high + 1 pixel between lines
	y2 = y0 + (1+lines)*8;
	
	strcpy(text_buffer+text_buffer_pointer,text);
	
	//text_buffer_pointer += text length
	text_buffer_pointer += y1;
}

void Drawable::draw(int16_t yline)
{
	//y0 is always min y, y2 is always max y
	//if (yline < y0) return;
	//if (yline > y2) return;
	//helper ints for scanline start/stop points on x-axis
	int16_t start_x, stop_x;

	switch (type)
	{
	//draw rect
	case 0:
		draw_line_to_buf(x0, x1, color);
	break;
	
	//draw pyramid triangle
	case 1:
		start_x = x0 + (x1 - x0) * (yline - y0) / (y1 - y0);
		stop_x = x0 + (x2 - x0) * (yline - y0) / (y2 - y0);
		
		draw_line_to_buf(start_x, stop_x-start_x, color);
	break;
	
	//draw ice cream cone triangle
	case 2:
		start_x = x2 + (x0 - x2) * (y2 - yline) / (y2 - y0);
		stop_x = x2 + (x1 - x2) * (y2 - yline) / (y2 - y1);
		
		draw_line_to_buf(start_x, stop_x-start_x, color);
	break;
	
	//draw line
	case 3:
		start_x = x0;
		if (x0 != x1)
		{
			if (y1 != y0)
			{
				start_x = x0 + (x1 - x0) * (yline - y0) / (y1 - y0);
			}
		}
		
		draw_line_to_buf(start_x, 1, color);
	break;
	
	//draw text
	case 4:
		/*
		1. get the line on which we're working on
		2. get the y-line we're working on, on that line of text
		
		3. loop thru characters in that line, or to the end
		*/
		int16_t current_line = (yline - y0) / 8;
		int16_t text_y_line = yline - y0 - current_line*8;
		int16_t text_per_line = x1 / 6;
		
		for (int32_t i = 0; (i < text_per_line) && (i < (y1 - current_line*text_per_line)); i++)
		{
			//current character we're working with, on text_buffer
			int16_t cur_char = current_line*text_per_line + i + x2;
			if ((text_buffer[cur_char] > 39) && (text_buffer[cur_char] < 123))
			{
				//characters are 5 pixels in width.
				if (charLUT[5*(text_buffer[cur_char]-40)+0] & (1 << text_y_line)) draw_pixel_to_buf(x0+i*6+0, color);
				if (charLUT[5*(text_buffer[cur_char]-40)+1] & (1 << text_y_line)) draw_pixel_to_buf(x0+i*6+1, color);
				if (charLUT[5*(text_buffer[cur_char]-40)+2] & (1 << text_y_line)) draw_pixel_to_buf(x0+i*6+2, color);
				if (charLUT[5*(text_buffer[cur_char]-40)+3] & (1 << text_y_line)) draw_pixel_to_buf(x0+i*6+3, color);
				if (charLUT[5*(text_buffer[cur_char]-40)+4] & (1 << text_y_line)) draw_pixel_to_buf(x0+i*6+4, color);
			}
		}
	break;
	}
}

void draw_pixel(int16_t x0, int16_t y0, uint16_t color)
{
	//pixel is 1x1 so we use 2 times x0, y0
	drawable_buffer[buffered_amount].init_as_line(x0, y0, x0, y0, color);
}

void draw_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
	drawable_buffer[buffered_amount].init_as_line(x0, y0, x1, y1, color);
}

void draw_rect(int16_t x0, int16_t y0, int16_t xsize, int16_t ysize, uint16_t color)
{
	//draw bufferect rectangle
	drawable_buffer[buffered_amount].init_as_rect(x0, y0, xsize, ysize, color);
}

void draw_text(char *text, int16_t x, int16_t y, uint16_t color, uint16_t max_width)
{
	drawable_buffer[buffered_amount].init_as_text(text, x, y, color, max_width);
}

void draw_poly(polygon p, uint16_t color)
{
	//check if poly is 0-height
	if ((p.y0 == p.y1) && (p.y0 == p.y2) && ( p.y1 == p.y2)) return;
	
	point min_y, med_y, max_y, split;
	min_y.x = p.x0; min_y.y = p.y0;
	med_y.x = p.x1; med_y.y = p.y1;
	max_y.x = p.x2; max_y.y = p.y2;
	sort_points_by_y(min_y, med_y, max_y);
	
	//check if we need to split the triangle in 2
	if (!((p.y0 == p.y1) || (p.y1 == p.y2) || (p.y0 == p.y2)))
	{
		//we want to split the triangle along the med point, so new point has same y as med point
		split.y = med_y.y;
		//split.x = find_x_along_line(min_y.x, min_y.y, max_y.x, max_y.y, med_y.x, med_y.y);
		split.x = min_y.x + (max_y.x - min_y.x) * (med_y.y - min_y.y) / (max_y.y - min_y.y);
		
		drawable_buffer[buffered_amount].init_as_poly(min_y, med_y, split, color);
		drawable_buffer[buffered_amount].init_as_poly(max_y, med_y, split, color);
	} else {
		drawable_buffer[buffered_amount].init_as_poly(min_y, med_y, max_y, color);
	}
}

//draws a line of pixels to the linebuffer.
void draw_line_to_buf(int16_t x, int16_t length, uint16_t color)
{
	if (x >= SCREENX) return;
	
	if (x < 0)
	{
		length += x;
		x = 0;
	}
	if (length+x >= SCREENX)
	{
		length = SCREENX-x;
	}
	for (int32_t i = x; i < length+x; i++)
	{
		line_buffer[i] = color;
	}
}

//draws a single pixel to the linebuffer
void draw_pixel_to_buf(int16_t x, uint16_t color)
{
	if (x >= SCREENX) return;
	if (x < 0) return;
	line_buffer[x] = color;
}

//draws the linebuffer to the lcd, some loop unrolling
void draw_linebuffer()
{
	set_pins();
	for (int32_t i = 0; i < SCREENX; i+=20)
	{
		set_data(line_buffer[i]);
		pulse_pins();
		set_data(line_buffer[i+1]);
		pulse_pins();
		set_data(line_buffer[i+2]);
		pulse_pins();
		set_data(line_buffer[i+3]);
		pulse_pins();
		set_data(line_buffer[i+4]);
		pulse_pins();
		set_data(line_buffer[i+5]);
		pulse_pins();
		set_data(line_buffer[i+6]);
		pulse_pins();
		set_data(line_buffer[i+7]);
		pulse_pins();
		set_data(line_buffer[i+8]);
		pulse_pins();
		set_data(line_buffer[i+9]);
		pulse_pins();
		set_data(line_buffer[i+10]);
		pulse_pins();
		set_data(line_buffer[i+11]);
		pulse_pins();
		set_data(line_buffer[i+12]);
		pulse_pins();
		set_data(line_buffer[i+13]);
		pulse_pins();
		set_data(line_buffer[i+14]);
		pulse_pins();
		set_data(line_buffer[i+15]);
		pulse_pins();
		set_data(line_buffer[i+16]);
		pulse_pins();
		set_data(line_buffer[i+17]);
		pulse_pins();
		set_data(line_buffer[i+18]);
		pulse_pins();
		set_data(line_buffer[i+19]);
		pulse_pins();
	}
	unset_pins();
}


//flushes the buffers to the lcd and resets everything
//so call this only once per loop, since this draws the frame
void flush_to_lcd()
{
	for (int32_t i = 0; i < buffered_amount; i++)
	{
		start_drawing[i] = drawable_buffer[i].get_start();
		end_drawing[i] = drawable_buffer[i].get_end();
	}
	
	command_lcd(0x4e, 0);
	command_lcd(0x4f, 0);
	lcd_com(0x22);
	//lets iterate thru all y-lines
	for (int32_t y = 0; y < SCREENY; y++)
	{
		//empty the line buffer with black
		draw_line_to_buf(0,SCREENX,0x0);
		
		//iterate thru all drawable objects
		for (int32_t i = 0; i < buffered_amount; i++)
		{
			if (start_drawing[i] > y) continue;
			if (end_drawing[i] < y) continue;
			//draw 1 pixel height y-slice of the drawable object to the linebuffer
			drawable_buffer[i].draw(y);
		}
		
		//pulse the current linebuffer to the lcd
		draw_linebuffer();
	}

	//reset drawable objects
	buffered_amount = 0;
	text_buffer_pointer = 0;
}

//sort points by y, returns them ordered, lowest y 1st
void sort_points_by_y(point &min, point &med, point &max)
{
	point temp;
	//if min's y is larger than med's, switch em
	if (min.y > med.y)
	{
		temp = min;
		min = med;
		med = temp;
	}
	//if min's y is larger than max's, switch em, so we def. have min right
	if (min.y > max.y)
	{
		temp = min;
		min = max;
		max = temp;
	}
	
	//now we just have to sort out, which is larger, med or max
	if (med.y > max.y)
	{
		temp = max;
		max = med;
		med = temp;
	}
}