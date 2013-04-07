/*
	By Nestori, 2013
	don't blame me when stuff burns
	I mean if
*/
#include "lcd_defines.h"
#include "lcd_init.h"
#include "lcd_graphics.h"
#include "lcd_math.h"

#ifdef FORPC
	#include <iostream>
	extern sf::RenderWindow sfmlwindow;
	#define START_MEASURING_CYCLES()
	#define END_MEASURING_CYCLES(rv) ((rv) = 0)
#endif

#ifdef FORAERY
	#define START_MEASURING_CYCLES() (__builtin_mtsr(AVR32_COUNT, 0))
	#define END_MEASURING_CYCLES(rv) ((rv) = __builtin_mfsr(AVR32_COUNT))
#endif

int main()
{
	init();
	
	//clear the screen
	draw_b_rect(0,0,320,240,0x0);
	flush_to_lcd();
	
	uint32_t count = 1;
	char str[10];
	char str2[] = "buffered text, buffered text again. text text text";

	int16_t centerx = 320/2;
	int16_t centery = 240/2;
	int16_t rot = 0;
	
	polygon p0, p1, p2;
	
	#ifdef FORAERY
	for (;;)
	#endif
	{
		count = 0;
		START_MEASURING_CYCLES();
		rot++;
		if (rot >= 360) rot = 0;

		p0.x0 = centerx+lcd_cos(rot+120)*50;
		p0.y0 = centery+lcd_sin(rot+120)*50;
		
		p0.x1 = centerx+lcd_cos(rot+120*2)*50;
		p0.y1 = centery+lcd_sin(rot+120*2)*50;
		
		p0.x2 = centerx+lcd_cos(rot+120*3)*50;
		p0.y2 = centery+lcd_sin(rot+120*3)*50;
		
		p1.x0 = centerx+lcd_cos(rot+120)*50-30;
		p1.y0 = centery+lcd_sin(rot+120)*50;
		
		p1.x1 = centerx+lcd_cos(rot+120*2)*50-30;
		p1.y1 = centery+lcd_sin(rot+120*2)*50;
		
		p1.x2 = centerx+lcd_cos(rot+120*3)*50-30;
		p1.y2 = centery+lcd_sin(rot+120*3)*50;
		
		p2.x0 = centerx+lcd_cos(rot+120)*50+30;
		p2.y0 = centery+lcd_sin(rot+120)*50;
		
		p2.x1 = centerx+lcd_cos(rot+120*2)*50+30;
		p2.y1 = centery+lcd_sin(rot+120*2)*50;
		
		p2.x2 = centerx+lcd_cos(rot+120*3)*50+30;
		p2.y2 = centery+lcd_sin(rot+120*3)*50;
		
		draw_b_poly(p1, 0xF800);
		draw_b_poly(p0, 0x7E00);
		draw_b_poly(p2, 0x001F);
		draw_b_text(str,0,220,0xF800);
		draw_b_text(str2,10,10,0xF600, 60);
		
		flush_to_lcd();
		
		END_MEASURING_CYCLES(count);
		#ifdef FORAERY
		itoa(66000000/count,str);
		#endif
	}

	#ifdef FORPC
	while (sfmlwindow.isOpen())
	{
		//input
		sf::Vector2i localPosition = sf::Mouse::getPosition(sfmlwindow);
		
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			//std::cout<<"x: " <<localPosition.x << " y: " << localPosition.y <<"\n";
		}
		sf::Event eventti;
		while (sfmlwindow.pollEvent(eventti))
		{
			switch(eventti.type)
			{
				case sf::Event::Closed:
					sfmlwindow.close();
					break;
				default:
					break;
			}
			
		}
		//logic
		
		rot++;
		if (rot >= 360) rot = 0;
		
		p0.x0 = centerx+lcd_cos(rot+120)*50;
		p0.y0 = centery+lcd_sin(rot+120)*50;
		
		p0.x1 = centerx+lcd_cos(rot+120*2)*50;
		p0.y1 = centery+lcd_sin(rot+120*2)*50;
		
		p0.x2 = centerx+lcd_cos(rot+120*3)*50;
		p0.y2 = centery+lcd_sin(rot+120*3)*50;
		
		draw_b_poly(p0, 0xF600);
		draw_b_text(str,0,220,0xF800);
		draw_b_text(str2,10,10,0xF600, 60);
		flush_to_lcd();

		
		//render
		simscreen_render();
		sfmlwindow.display();
		//sleep
		sf::sleep(sf::milliseconds(33));
	}
	#endif
	return 0;
}





