/*
	By Nestori, 2013
	don't blame me when stuff burns
	I mean if
*/
#include "lcd_graphics.h"

//clears the whole screen with given color
void clear_screen(uint16_t color)
{
	for (uint32_t y = 0; y < SCREENY; y++)
	{
		//0x4e is for moving y pointer position
		//0x4f is for moving x pointer position
		command_lcd(0x4e, (uint16_t)y);
		command_lcd(0x4f, 0);
		//command 0x22 is needed so the lcd will accept color data 
		lcd_com(0x22);
		set_pins(color);
		for (uint32_t x = 0; x < SCREENX; x++)
		{
			pulse_pins();
		}
		unset_pins();
	}
}

void draw_pixel(int16_t x, int16_t y, uint16_t color)
{
	//clamp x and y to proper values
	if (y >= SCREENY) return;
	if (x >= SCREENX) return;
	if (x < 0) return;
	if (y < 0) return;
	
	//set cursor to correct position in graphical ram
	command_lcd(0x4e,y);
    command_lcd(0x4f,x);
    //0x22 is needed so the screen will accept color data
    lcd_com(0x22);
    lcd_data(color);
}

//whole function is from wikipedia, contains lots of unexplained magic
void draw_line(int16_t x0, int16_t x1, int16_t y0, int16_t y1, uint16_t color)
{
	int16_t sx, sy, err, e2;
	int16_t dx = lcd_abs(x1-x0);
	int16_t dy = lcd_abs(y1-y0);
	
	(x0 < x1) ? sx = 1 : sx = -1;
	(y0 < y1) ? sy = 1 : sy = -1;
	err = dx-dy;

	for(;;)
	{
		//since we're only using draw_pixel, we do not have to check for limits
		//as draw_pixel does it by itself
		draw_pixel(x0,y0,color);
		if ((x0 == x1) && (y0 == y1))
		{
			break;
		}
		e2 = err*2;
		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

/*	fast horizontal line draw function, uses the fact that graphical ram position
	moves +1 to the right after inserting color value
	
	notice that 2nd argument is length, not end point on x-axis
*/
void draw_line_hor(int16_t x0, int16_t length, int16_t y0, uint16_t color)
{
	//clamp x,y vals between correct values
	if (y0 < 0) return;
	if (x0 >= SCREENX) return;
	if (y0 >= SCREENY) return;
	
	//if x<0, lets cut negative part out of length, and set x=0
	//so we still render some line, even if we "start" from neg x coordinate
	if (x0 < 0)
	{
		length += x0;
		x0 = 0;
	}
	//lets not get over the screen, to avoid wrapping to next y-line, as per screen spec
	if (length+x0 >= SCREENX)
	{
		length = SCREENX-x0;
	}
	
	//set cursor to correct position in graphical ram
	command_lcd(0x4e,y0);
    command_lcd(0x4f,x0);
    //0x22 is needed so the screen will accept color data
    lcd_com(0x22);
    set_pins(color);
	for (int32_t i = 0; i < length; i++)
	{
		pulse_pins();
	}
	unset_pins();
}

//draws rectangle of solid color
void draw_rect(int16_t x0, int16_t xsize, int16_t y0, int16_t ysize, uint16_t color)
{
	//clamp stuff
	if ( x0 < 0 )
	{
		xsize += x0;
		x0 = 0;
	}
	if ( y0 < 0 )
	{
		ysize += y0;
		y0 = 0;
	}
	if (xsize+x0 >= SCREENX)
	{
		xsize = SCREENX-x0;
	}
	if (ysize+y0 >= SCREENY)
	{
		ysize = SCREENY-y0;
	}

	//draw rect
    for (int32_t i = 0; i < ysize; i++)
    {
    	command_lcd(0x4e,y0+(uint16_t)i);
   		command_lcd(0x4f,x0);
   		lcd_com(0x22);
   		set_pins(color); //exp
   		for (int32_t j = 0; j < xsize; j++)
   		{
   			pulse_pins();
   		}
   		unset_pins();	//exp
    }
}

//draws text to given position, starting at upper left corner, with given color
void draw_text(char *array, int16_t x, int16_t y, uint16_t color)
{
	//probably should replace strlen to something else to save bin space?
	int32_t len = strlen(array);
	//pointer to x position, where we're going
	int32_t pointer = 0;
	int32_t i, j, k;
	
	for (i = 0; i < len; i++)
	{
		//ascii chars from 40 to 122 only
		if ((array[i] > 39) && (array[i] < 123))
		{
			//char width
			for (j = 0; j < 5; j++)
			{
				//char height
				for (k = 0; k < 8; k++)
				{
					//get bit from character look-up table
					if (charLUT[j+5*(array[i]-40)] & (0x01 << k))
					{
						//since we're using draw_pixel only, we do not have to check for
						//screen limits. draw_pixel does that for us
						draw_pixel(pointer+x+j, y+k, color);
					}
				}
			}
		}	
		//char width 5, so we advance 6 pixels to the right
		pointer += 6;
	}
}

//gets x min limits for polygon on every y-line, and x lengths on said lines
void get_polygon_limits(int16_t x0, int16_t x1, int16_t y0, int16_t y1, int16_t *xvals, int16_t *xlens, uint16_t size, int16_t minY)
{
	//this is default line-drawing method, without actually drawing anything. look up draw_line()	
	int16_t sx, sy, err;
	int16_t dx = lcd_abs(x1-x0);
	int16_t dy = lcd_abs(y1-y0);
	
	(x0 < x1) ? sx = 1 : sx = -1;
	(y0 < y1) ? sy = 1 : sy = -1;
	err = dx-dy;
	
	for(;;)
	{
		//set xvals[y] as min x encountered so far
		int16_t xval = *(xvals+(y0-minY));
		int16_t xlen = *(xlens+(y0-minY));
		
		if (x0 < xval) 
		{
			xvals[y0-minY] = x0;
		}
		
		if (x0 > xlen)
		{
			xlens[y0-minY] = x0;
		}
		
		if ((x0 == x1) && (y0 == y1))
		{
			break;
		}
		int16_t e2 = err*2;
		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}
void draw_polygon(polygon p, uint16_t color)
{
	//check the y-height of the polygon
	int16_t minY = find_min(p.y0, p.y1, p.y2);
	int16_t maxY = find_max(p.y0, p.y1, p.y2);
	int16_t height = maxY - minY + 1;
	
	/*	init 2 arrays holding x-values at y-line
		xvals holds min values of x of polygon p, at line y
		xlens holds max values of x of polygon p, at line y
	*/
	int16_t xvals[height];
	int16_t xlens[height];
	
	//init min x values as max screen width, so we can start lowering them
	//init max x values as 0 so we can start incresing them
	for (int32_t i = 0; i < height; i++)
	{
		xvals[i] = SCREENX-1;
		xlens[i] = 0;
	}
	
	//pointers to arrays to pass around
	int16_t *p_xvals;
	int16_t *p_xlens;
	p_xvals = xvals;
	p_xlens = xlens;
	
	//fill xvals,xlens arrays with values, defining polygon edges on left and right side
	get_polygon_limits( p.x0, p.x1, p.y0, p.y1, p_xvals, p_xlens, height, minY);
	get_polygon_limits( p.x1, p.x2, p.y1, p.y2, p_xvals, p_xlens, height, minY);
	get_polygon_limits( p.x2, p.x0, p.y2, p.y0, p_xvals, p_xlens, height, minY);
	
	//draw "height" amount of lines, to position defined by xvals, and lenght of xlen  
	for (int32_t i = 0; i < height; i++)
	{
		draw_line_hor(xvals[i], xlens[i]-xvals[i], i+minY, color);
	}
}


uint16_t mix_colors(uint16_t base_color, uint16_t overlay_color, uint8_t alpha)
{

	uint16_t basered, basegreen, baseblue;
	uint16_t overred, overgreen, overblue;
	basered =   (base_color >> 11) & 0x1f;
	basegreen = (base_color >> 5) & 0x3f;
	baseblue =   base_color & 0x1f;
	overred =   (overlay_color >> 11) & 0x1f;
	overgreen = (overlay_color >> 5) & 0x3f;
	overblue =   overlay_color & 0x1f;
	return	((basered   * alpha + overred   * (255-alpha))/255 << 11) |
			((basegreen * alpha + overgreen * (255-alpha))/255 << 5)|
			((baseblue  * alpha + overblue  * (255-alpha))/255);
	
	//uint16_t bred = ((base_color >> 11) & alpha & 0x1f) + ((overlay_color >> 11) & ~alpha & 0x1f) >> 1;
	//uint16_t bgreen = ((base_color >> 5) & alpha & 0x3f) + ((overlay_color >> 5) & ~alpha & 0x3f) >> 1;
	//uint16_t bblue = (base_color & alpha & 0x1f) + (overlay_color & ~alpha & 0x1f) >> 1;
	//return  bred << 11 | bgreen << 5 | bblue;
	
	//return ((base_color * alpha) + overlay_color * (255 - alpha)) / 255;
}

uint16_t mix_colors(uint8_t a_red, uint8_t a_green, uint8_t a_blue, uint8_t b_red, uint8_t b_green, uint8_t b_blue, uint8_t alpha)
{
	return	((a_red		* (255 - alpha) + b_red		* alpha) / 255 << 11) |
			((a_green	* (255 - alpha) + b_green	* alpha) / 255 << 5) |
			((a_blue	* (255 - alpha) + b_blue	* alpha) / 255);
}

/*
	rgb components gets added to base color, msb=1 signifies addition, msb=0 is reduction of said color
	ie. basecolor += component*(-1+msb*2)
*/
uint16_t mix_a_color(uint16_t base_color, uint8_t red, uint8_t green, uint8_t blue)
{
	int16_t basered, basegreen, baseblue;
	//separate components from base color
	basered =   (base_color >> 11) & 0x1f;
	basegreen = (base_color >> 5) & 0x3f;
	baseblue =   base_color & 0x1f;
	
	
		
	basered += (red & 0x7F)    * (-1+(red&0x80>>6));
	basegreen += (green & 0x7F)* (-1+(green&0x80>>6));
	baseblue += (blue & 0x7F)  * (-1+(blue&0x80>>6));
		
	if (basered < 0) basered = 0;
	if (basegreen < 0) basegreen = 0;
	if (baseblue < 0) baseblue = 0;
	
	if (basered > 0x1f) basered = 0x1f;
	if (basegreen > 0x3f) basegreen = 0x3f;
	if (baseblue > 0x1f) baseblue = 0x1f;
	
	return (basered&0x1F) << 11 | (basegreen&0x3F) << 5 | (baseblue&0x1F);
}

//experimental
void draw_rect(int16_t x0, uint16_t xsize, int16_t y0, uint16_t ysize, uint16_t color, texture tex)
{
	if ( x0 < 0 )
	{
		xsize += x0;
		x0 = 0;
	}
	if ( y0 < 0 )
	{
		ysize += y0;
		y0 = 0;
	}
	if (y0+ysize >= SCREENY)
	{
		ysize = SCREENY - y0;
	}
	if (x0+xsize >= SCREENX)
	{
		xsize = SCREENX - x0;
	}
	
	//uint16_t colorbuffer[2];
   	//colorbuffer[0] = color;
   	//colorbuffer[1] = mix_colors(color, tex.color, tex.alpha);
	//uint16_t linebuffer[xsize];
	//uint32_t randbuffer = prng();
	
	uint8_t red = color >> 11 & 0x1F;
	uint8_t green = color >> 5 & 0x3F;
	uint8_t blue = color & 0x1F;
	
	uint32_t randbuffer;
	uint16_t colorbuffer[16];
	
	/*initialize iterators, and use ifs to check which mode we're going to use later
	 *we do this to avoid if's inside for-loops
	 texnoise iterator, chromatic / monochromatic noise iterator, normal iterator
	 */
	uint16_t tn_it = ysize, cn_it = ysize, n_it = ysize;
	
	//texture noise
	if (tex.gettexnoise())
	{
		tn_it = 0;
		colorbuffer[0] = color;
		colorbuffer[1] = mix_colors(color,tex.color, tex.alpha);
	}
	
	//chromatic noise, color noise
	if (tex.getcolornoise())
	{
		cn_it = 0;
		for (uint16_t i = 0; i < 8; i++)
		{		
			randbuffer = prng();
			colorbuffer[i] = mix_a_color(color, (randbuffer >> 16) % (1 << tex.getnoisered()), (randbuffer >> 8) % (1 <<tex.getnoisegreen()), (randbuffer % (1 <<tex.getnoiseblue())));
			colorbuffer[i+8] = mix_a_color(color, (randbuffer >> 16) % (1 << tex.getnoisered()), (randbuffer >> 8) % (1 <<tex.getnoisegreen()), (randbuffer % (1 <<tex.getnoiseblue())));
		}
	}
	
	//monochromatic noise
	if (tex.getmononoise())
	{
		cn_it = 0;
		for (uint16_t i = 0; i < 8; i++)
		{
			colorbuffer[i] = mix_colors(red,green,blue,0x0, 0x0, 0x0, i*(tex.getnoisered()*5+1));
			colorbuffer[i+8] = mix_colors(red,green,blue,0x1F, 0x3F, 0x1F,i*(tex.getnoisered()*5+1));
		}	
	}
	
	//normal texture operation
	//if sum of previous iterators is ysize*3, then we havent used them, so we do normal texture stuff here
	if (tn_it+cn_it == ysize*2)
	{
		n_it = 0;
		colorbuffer[0] = color;
		colorbuffer[1] = mix_colors(color,tex.color, tex.alpha);
	}


	//forloop for texture noise
	for (; tn_it < ysize; tn_it++)
	{
		command_lcd(0x4e,y0+tn_it);
   		command_lcd(0x4f,x0);
   		lcd_com(0x22);
   		for (uint16_t i = 0; i < xsize; i++)
		{
			lcd_data(colorbuffer[prng()%2]);
		}
	}
	
	//forloop for chromatic/monochromatic noise
	for (; cn_it < ysize; cn_it++)
	{
		command_lcd(0x4e,y0+cn_it);
   		command_lcd(0x4f,x0);
   		lcd_com(0x22);
		for (uint16_t i = 0; i < xsize; i++)
		{
			lcd_data(colorbuffer[(prng())%16]);
		}
	}
	
	//normal texture operation
	for (; n_it < ysize; n_it++)
	{
		command_lcd(0x4e,y0+n_it);
   		command_lcd(0x4f,x0);
   		lcd_com(0x22);
   		for (uint16_t i = 0; i < xsize; i++)
		{
			lcd_data(colorbuffer[def_textures[tex.texture_num][i%16] >> (n_it%16) & 0x1]);
		}
	}
}