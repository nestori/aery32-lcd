#include "graphics_3d.h"

//helper functions. forward declare
Vector3D to_screen_coords(Vector3D v);
void selection_sort(Polygon3D *a, uint16_t len);

//the perspective matrix
Matrix Perspective_matrix;


//camera rotation and position
Matrix camera_rot = get_identity();
Vector3D camera_pos;

//another buffer :D
uint16_t buffered_poly_amount = 0;
Polygon3D polygon_buffer[35];


void draw_poly3D(Polygon3D poly, uint16_t color)
{
	//apply camera transformation
	poly.p0 = camera_rot.multiply(poly.p0)-camera_pos;
	poly.p1 = camera_rot.multiply(poly.p1)-camera_pos;
	poly.p2 = camera_rot.multiply(poly.p2)-camera_pos;
	
	Vector3D poly_normal = (poly.p0 - poly.p2).cross(poly.p0 - poly.p1);
	float visible = poly_normal.dot(poly.p0-camera_pos);
	
	//this is backface-culling check for counter-clockwise winding of the polygon.
	if (visible < 0) return;
	
	
	//lets calculate distance value for depth-sorting algorithm
	//no need to put -camera_pos.data[n]*3 aftear each calculation since we do that earlier
	int16_t depth_x = (poly.p0.data[0] + poly.p1.data[0] + poly.p2.data[0]);
	int16_t depth_y = (poly.p0.data[1] + poly.p1.data[1] + poly.p2.data[1]);
	int16_t depth_z = -(poly.p0.data[2] + poly.p1.data[2] + poly.p2.data[2]);
	
	//1st we translate the 3d vectors (points) with perspective matrix, they have vals between -1 and 1.
	//then we translate those coordinates to screen coords
	poly.p0 = to_screen_coords(Perspective_matrix.multiply(poly.p0));
	poly.p1 = to_screen_coords(Perspective_matrix.multiply(poly.p1));
	poly.p2 = to_screen_coords(Perspective_matrix.multiply(poly.p2));
	
	
	//lets check z vals. if z is less than 0, it is behind camera and should not be drawn
	if (poly.p0.data[2] < 0) return;
	if (poly.p1.data[2] < 0) return;
	if (poly.p2.data[2] < 0) return;
	
	
	//store distance from camera to triangle center to p0's data[2], which used to represent 1st points z-val
	//we dont need that z-value anymore
	//if needed, just change to 3 which is w. remember to change that in sort func too
	//no need to take sqrt()
	poly.p0.data[2] = depth_x*depth_x + depth_y*depth_y + depth_z*depth_z;
	
	
	//color is color
	poly.color = color;
	
	polygon_buffer[buffered_poly_amount] = poly;
	buffered_poly_amount++;
}

void draw_poly_buffer()
{
	//lets sort polys by distance to the camera
	selection_sort(polygon_buffer, buffered_poly_amount);
	
	//loop through all polys and draw them to 2d poly buffer
	for (int32_t i = 0; i < buffered_poly_amount; i++)
	{
		draw_poly(polygon(polygon_buffer[i].p0.data[0], polygon_buffer[i].p0.data[1],
					polygon_buffer[i].p1.data[0], polygon_buffer[i].p1.data[1],
					polygon_buffer[i].p2.data[0], polygon_buffer[i].p2.data[1]),
					polygon_buffer[i].color
					);
	}
	
	//reset poly amount. ready for next frame
	buffered_poly_amount = 0;
}

//selectionsort for polygons. we need to sort these by depth
//std::sort works but it is ~5kb larger
void selection_sort(Polygon3D *a, uint16_t len)
{
	int32_t max = 0;
	Polygon3D temp;
	for (int32_t i = 0; i < len; i++)
	{
		max = i;
		for (int j = i+1; j < len; j++)
		{
			if (a[j].p0.data[2] > a[max].p0.data[2]) max = j;
		}
		//exchange a[i] with a[min]
		temp = a[i];
		a[i] = a[max];
		a[max] = temp;
	}
}

//sets perspective projection matrix
void set_perspective(float near, float far)
{
	for (int i = 0; i < 16; i++)
	{
		Perspective_matrix.data[i] = 0;
	}
	
	Perspective_matrix.data[0] = Perspective_matrix.data[5] = 1.0f;
	Perspective_matrix.data[10] = -(far / (far - near));
	Perspective_matrix.data[11] = -1;
	Perspective_matrix.data[14] = -(far * near) / (far - near);
}

void translate_camera(Vector3D vec)
{
	camera_pos = vec;
}


//basic matrix rotation of 3-axels. this is combined from 3 different matrix rotations
void rotate_camera(float x, float y, float z)
{
	//here be the dragons
	camera_rot.data[0] = lcd_cos(y)*lcd_cos(z);
	camera_rot.data[1] = -lcd_cos(y)*lcd_sin(z);
	camera_rot.data[2] = lcd_sin(y);
	camera_rot.data[3] = 0;
	
	camera_rot.data[4] = lcd_cos(x)*lcd_sin(z) + lcd_sin(x)*lcd_sin(y)*lcd_cos(z);
	camera_rot.data[5] = lcd_cos(x)*lcd_cos(z) - lcd_sin(x)*lcd_sin(y)*lcd_sin(z);
	camera_rot.data[6] = -lcd_sin(x)*lcd_cos(y);
	camera_rot.data[7] = 0;
	
	camera_rot.data[8]  = lcd_sin(x)*lcd_sin(z) - lcd_cos(x)*lcd_sin(y)*lcd_cos(z);
	camera_rot.data[9]  = lcd_sin(x)*lcd_cos(z) + lcd_cos(x)*lcd_sin(y)*lcd_sin(z);
	camera_rot.data[10] = lcd_cos(x)*lcd_cos(y);
	camera_rot.data[11] = 0;
	
	camera_rot.data[12] = 0;
	camera_rot.data[13] = 0;
	camera_rot.data[14] = 0;
	camera_rot.data[15] = 1;
}

//translates given 3d point to screen coordinates. you have to multiply the point with perspective matrix
//before giving it to this func
Vector3D to_screen_coords(Vector3D v)
{
	Vector3D t;
	
	t.data[0] = (SCREENX/2) + (v.data[0] / v.data[3] ) * (SCREENX/2);
	t.data[1] = (SCREENY/2) - (v.data[1] / v.data[3] ) * (SCREENY/2);
	t.data[2] = v.data[2];
	return t;
}