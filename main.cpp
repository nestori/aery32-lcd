/*
	By Nestori, 2013
	don't blame me when stuff burns
	I mean if
*/
#include "lcd_defines.h"
#include "lcd_init.h"
#include "lcd_graphics.h"

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
	
	uint32_t cyclecount = 0;
	char str[10] = "0";
	
	clear_screen(0x1f);

	polygon p = {10,10,20,10,10,20};
	
	START_MEASURING_CYCLES();
	draw_polygon(p,0xFFFF);
	draw_rect(40,10,40,10,0xFFFF);
	END_MEASURING_CYCLES(cyclecount);
	
	#ifdef FORAERY
		itoa(cyclecount,str);
	#endif
	draw_text(str,0,0,0xFFFF);
	cyclecount = 0;
	
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
		
		//render
		simscreen_render();
		sfmlwindow.display();
		//sleep
		sf::sleep(sf::milliseconds(33));
	}
	#endif
	return 0;
}





