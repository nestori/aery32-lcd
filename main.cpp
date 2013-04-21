/*
	By Nestori, 2013
	don't blame me when stuff burns
	I mean if
*/
#include "lcd_defines.h"
#include "lcd_init.h"
#include "lcd_graphics.h"
#include "lcd_math.h"
#include "linear_algebra.h"
#include "graphics_3d.h"

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
	set_perspective(1, 100.0);
	
	init();
	//clear the screen
	draw_rect(0,0,320,240,0x0);
	flush_to_lcd();
	
	uint32_t count = 1;
	char str[10];
	//char str2[] = "buffered text, buffered text again. text text text";
	
	uint16_t colors[] = {0xF800, 0xF600, 0x7E0, 0x1F, 0xFFFF,0xF800, 0xF600, 0x7E0, 0x1F, 0xFFFF,0xF800, 0xF600, 0x7E0, 0x1F, 0xFFFF};

	//int16_t centerx = 320/2;
	//int16_t centery = 240/2;
	
	//int16_t polysize = 30;
	
	
	int16_t rot = 0;
	
	polygon poly;
	
	#ifdef FORAERY
	for (;;)
	#endif
	{
		count = 0;
		START_MEASURING_CYCLES();
		rot++;
		if (rot >= 360) rot = 0;
		
		/*
		for (int i = 0; i < 15; i++)
		{
			poly.x0 = (i-7)*20+centerx+lcd_cos(rot+120)*polysize;
			poly.y0 = centery+lcd_sin(rot+120)*polysize;
			poly.x1 = (i-7)*20+centerx+lcd_cos(rot+120*2)*polysize;
			poly.y1 = centery+lcd_sin(rot+120*2)*polysize;
			poly.x2 = (i-7)*20+centerx+lcd_cos(rot+120*3)*polysize;
			poly.y2 = centery+lcd_sin(rot+120*3)*polysize;
			draw_b_poly(poly, colors[i]);
		}
		for (int i = 0; i < 15; i++)
		{
			poly.x0 = (i-7)*20+centerx+lcd_cos(rot+120)*polysize;
			poly.y0 = 20+centery+lcd_sin(rot+120)*polysize;
			poly.x1 = (i-7)*20+centerx+lcd_cos(rot+120*2)*polysize;
			poly.y1 = 20+centery+lcd_sin(rot+120*2)*polysize;
			poly.x2 = (i-7)*20+centerx+lcd_cos(rot+120*3)*polysize;
			poly.y2 = 20+centery+lcd_sin(rot+120*3)*polysize;
			draw_b_poly(poly, colors[i]);
		}
		*/
 
				translate_camera(Vector3D(0,1,4));		
		rotate_camera(20,lcd_sin(rot)*180.0, 0);
		
		Polygon3D p;
		
		/*
		p.p0 = Vector3D(lcd_cos(2*rot)*3.5, -3.5, -10 + lcd_sin(2*rot)*3.5);
		p.p1 = Vector3D(lcd_cos(2*rot+120*2)*3.5, -3.5, -10 + lcd_sin(2*rot+120*2)*3.5);
		p.p2 = Vector3D(lcd_cos(2*rot+120)*3.5, -3.5, -10 + lcd_sin(2*rot+120)*3.5);
		*/
		
		//front
		p.p0 = Vector3D(-1, 1, 1);
		p.p1 = Vector3D(-1,-1, 1);
		p.p2 = Vector3D( 1,-1, 1);
		draw_poly3D(p, colors[0]);
		
		p.p0 = Vector3D(-1, 1, 1);
		p.p1 = Vector3D( 1,-1, 1);
		p.p2 = Vector3D( 1, 1, 1);
		draw_poly3D(p, colors[0]);
		
		//left side
		p.p0 = Vector3D(-1, 1, -1);
		p.p1 = Vector3D(-1,-1, -1);
		p.p2 = Vector3D(-1,-1, 1);
		draw_poly3D(p, colors[1]);
		
		p.p0 = Vector3D(-1, 1,-1);
		p.p1 = Vector3D(-1,-1, 1);
		p.p2 = Vector3D(-1, 1, 1);
		draw_poly3D(p, colors[1]);
		
		//right side
		
		p.p0 = Vector3D( 1, 1, 1);
		p.p1 = Vector3D( 1,-1, 1);
		p.p2 = Vector3D( 1,-1,-1);
		draw_poly3D(p, colors[2]);
		
		p.p0 = Vector3D( 1, 1, 1);
		p.p1 = Vector3D( 1,-1,-1);
		p.p2 = Vector3D( 1, 1,-1);
		draw_poly3D(p, colors[2]);
		
		//backside
		
		p.p0 = Vector3D( 1, 1,-1);
		p.p1 = Vector3D( 1,-1,-1);
		p.p2 = Vector3D(-1,-1,-1);
		draw_poly3D(p, colors[3]);
		
		p.p0 = Vector3D( 1, 1,-1);
		p.p1 = Vector3D(-1,-1,-1);
		p.p2 = Vector3D(-1, 1,-1);
		draw_poly3D(p,colors[3]);
		
		//top
		
		p.p0 = Vector3D(-1, 1,-1);
		p.p1 = Vector3D(-1, 1, 1);
		p.p2 = Vector3D( 1, 1, 1);
		draw_poly3D(p, colors[4]);
		
		p.p0 = Vector3D(-1, 1,-1);
		p.p1 = Vector3D( 1, 1, 1);
		p.p2 = Vector3D( 1, 1,-1);
		draw_poly3D(p, colors[4]);
		
		
		draw_poly_buffer();
		draw_text(str,0,220,0xF800);
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
		
		translate_camera(Vector3D(0,1,4));		
		rotate_camera(20,lcd_sin(rot)*180.0, 0);
		
		Polygon3D p;
		
		/*
		p.p0 = Vector3D(lcd_cos(2*rot)*3.5, -3.5, -10 + lcd_sin(2*rot)*3.5);
		p.p1 = Vector3D(lcd_cos(2*rot+120*2)*3.5, -3.5, -10 + lcd_sin(2*rot+120*2)*3.5);
		p.p2 = Vector3D(lcd_cos(2*rot+120)*3.5, -3.5, -10 + lcd_sin(2*rot+120)*3.5);
		*/
		
		//front
		p.p0 = Vector3D(-1, 1, 1);
		p.p1 = Vector3D(-1,-1, 1);
		p.p2 = Vector3D( 1,-1, 1);
		draw_poly3D(p, colors[0]);
		
		p.p0 = Vector3D(-1, 1, 1);
		p.p1 = Vector3D( 1,-1, 1);
		p.p2 = Vector3D( 1, 1, 1);
		draw_poly3D(p, colors[0]);
		
		//left side
		p.p0 = Vector3D(-1, 1, -1);
		p.p1 = Vector3D(-1,-1, -1);
		p.p2 = Vector3D(-1,-1, 1);
		draw_poly3D(p, colors[1]);
		
		p.p0 = Vector3D(-1, 1,-1);
		p.p1 = Vector3D(-1,-1, 1);
		p.p2 = Vector3D(-1, 1, 1);
		draw_poly3D(p, colors[1]);
		
		//right side
		
		p.p0 = Vector3D( 1, 1, 1);
		p.p1 = Vector3D( 1,-1, 1);
		p.p2 = Vector3D( 1,-1,-1);
		draw_poly3D(p, colors[2]);
		
		p.p0 = Vector3D( 1, 1, 1);
		p.p1 = Vector3D( 1,-1,-1);
		p.p2 = Vector3D( 1, 1,-1);
		draw_poly3D(p, colors[2]);
		
		//backside
		
		p.p0 = Vector3D( 1, 1,-1);
		p.p1 = Vector3D( 1,-1,-1);
		p.p2 = Vector3D(-1,-1,-1);
		draw_poly3D(p, colors[3]);
		
		p.p0 = Vector3D( 1, 1,-1);
		p.p1 = Vector3D(-1,-1,-1);
		p.p2 = Vector3D(-1, 1,-1);
		draw_poly3D(p,colors[3]);
		
		//top
		
		p.p0 = Vector3D(-1, 1,-1);
		p.p1 = Vector3D(-1, 1, 1);
		p.p2 = Vector3D( 1, 1, 1);
		draw_poly3D(p, colors[4]);
		
		p.p0 = Vector3D(-1, 1,-1);
		p.p1 = Vector3D( 1, 1, 1);
		p.p2 = Vector3D( 1, 1,-1);
		draw_poly3D(p, colors[4]);
		
		
		draw_poly_buffer();
		draw_text(str,0,220,0xF800);
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





