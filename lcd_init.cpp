/*
	By Nestori, 2013
	don't blame me when stuff burns
	I mean if
*/
#include "lcd_init.h"

#ifdef FORAERY
	extern const uint8_t PIN_RS;
	extern const uint8_t PIN_WR;
	extern const uint8_t PIN_RD;
	extern const uint8_t PIN_CS;
	extern const uint8_t PIN_RESET;
#endif

#ifdef FORPC
	//opengl init
	void initGL(int xsize, int ysize)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		//-1 is "left" and "top" value in coords, has to be -1 on osx for some reason
		//ie. if it is 0 and you try to render to 0,0, it will not render.
		glOrtho(-1, xsize, ysize, -1, 0, 1);
		glMatrixMode(GL_MODELVIEW);
		glDisable(GL_DEPTH_TEST);
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_ALPHA);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glLoadIdentity();
		glPointSize(2.0);
	};
	int xsize = SCREENX, ysize = SCREENY;
	//depth buffer, stencilbuffer, antialiasing, major, minor
	sf::ContextSettings context(8,2,0,2,1);
	//actual window created, +1 to sizes because damn glOrtho shenanigans
	sf::RenderWindow sfmlwindow(sf::VideoMode(xsize+1, ysize+1, 32), "aery", 7U, context);
#endif

void init()
{
	#ifdef FORAERY
	
	board::init();
	gpio_init_pins(portb, 0xffffffff, GPIO_OUTPUT);
	gpio_enable_localbus();
	//read-pin has to be high during operation
	lportb->ovrs = (1 << GPIO_NUM2PIN(PIN_RD)) | (1 << GPIO_NUM2PIN(PIN_RESET));
	delay_ms(15);
	lportb->ovrc = (1 << GPIO_NUM2PIN(PIN_RESET));
	delay_ms(15);
	lportb->ovrs = (1 << GPIO_NUM2PIN(PIN_RESET));
	delay_ms(15);
	//boot up
	
	//inits screen to 320x240, switch to SCREENX,SCREENY at some point
	command_lcd(0x00,0x0001); delay_ms(2);	//oscillator on
	command_lcd(0x03,0xA8A4); delay_ms(2);	//power control 1
	command_lcd(0x0C,0x0000); delay_ms(2);	//power control 2
	command_lcd(0x0D,0x080C); delay_ms(2);	//power control 3
	command_lcd(0x0E,0x2B00); delay_ms(2);	//power control 4
	command_lcd(0x1E,0x00B7); delay_ms(2);	//power control 5
	command_lcd(0x01,0x293F); delay_ms(2);	//driver output control
	command_lcd(0x02,0x0600); delay_ms(2);	//lcd driving waveform control					//def 0x0600
	command_lcd(0x10,0x0000); delay_ms(30);	//sleepmode deactivated, 30ms as per specs
	command_lcd(0x11,0x6078); delay_ms(2);	//entry mode, set up which way screen is, etc
	command_lcd(0x05,0x0000); delay_ms(2);	//compare register 1
	command_lcd(0x06,0x0000); delay_ms(2);	//compare register 2
	command_lcd(0x16,0xEF1C); delay_ms(2);	//horizontal porch
	command_lcd(0x17,0x0003); delay_ms(2);	//vertical porch
	command_lcd(0x07,0x0233); delay_ms(2);	//display control
	command_lcd(0x0B,0x5388); delay_ms(2);	//frame cycle control
	command_lcd(0x0F,0x0000); delay_ms(2);	//gate scan start position
	command_lcd(0x41,0x0000); delay_ms(2);	//vertical scroll control 1
	command_lcd(0x42,0x0000); delay_ms(2);	//vertical scroll control 2
	command_lcd(0x48,0x0000); delay_ms(2);	//1st window start
	command_lcd(0x49,0x013F); delay_ms(2);	//1st window end
	command_lcd(0x4A,0x0000); delay_ms(2);	//2nd window start
	command_lcd(0x4B,0x0000); delay_ms(2);	//2nd window end
	command_lcd(0x44,0xEF00); delay_ms(2);	//horizontal ram address position
	command_lcd(0x45,0x0000); delay_ms(2);	//vertical ram address start position
	command_lcd(0x46,0x013F); delay_ms(2);	//vertical ram address end position
	command_lcd(0x30,0x0707); delay_ms(2);	// gamma control 1
	command_lcd(0x31,0x0204); delay_ms(2);	// gamma control 2..
	command_lcd(0x32,0x0204); delay_ms(2);
	command_lcd(0x33,0x0502); delay_ms(2);
	command_lcd(0x34,0x0507); delay_ms(2);
	command_lcd(0x35,0x0204); delay_ms(2);
	command_lcd(0x36,0x0204); delay_ms(2);
	command_lcd(0x37,0x0502); delay_ms(2);
	command_lcd(0x3A,0x0302); delay_ms(2);
	command_lcd(0x3B,0x0302); delay_ms(2);	// gamma control 10
	command_lcd(0x23,0x0000); delay_ms(2);	//ram write data mask1
	command_lcd(0x24,0x0000); delay_ms(2);	//ram write data mask2
	command_lcd(0x25,0x8000); delay_ms(2);	//frame frequency
	command_lcd(0x4f,0x0000); delay_ms(2);	//set ram y counter
	command_lcd(0x4e,0x0000); delay_ms(2);	//set ram x counter
	
	gpio_set_pin_high(LED);
	#endif
	
	#ifdef FORPC
		sfmlwindow.setVerticalSyncEnabled(true);
		initGL(xsize,ysize);
	#endif
}