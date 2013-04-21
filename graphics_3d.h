/*
	By Nestori, 2013
	don't blame me when stuff burns
	I mean if
*/
#ifndef GRAPHICS3D_H
#define GRAPHICS3D_H

#include "lcd_defines.h"
#include "lcd_graphics.h"
#include "lcd_math.h"
#include "linear_algebra.h"


struct Polygon3D
{	
	Vector3D p0, p1, p2;
	uint16_t color;
};

//draws 3d polygon to the buffer, with perspective matrix
void draw_poly3D(Polygon3D a, uint16_t color);

//draws buffered polygons to lcd_graphics' buffer
void draw_poly_buffer();

//sets the perspective matrix, try near 1 far 100
void set_perspective(float near, float far);


//rotate and translate camera
void rotate_camera(float x, float y, float z);
void translate_camera(Vector3D vec);

#endif