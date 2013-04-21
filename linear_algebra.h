/*
	By Nestori, 2013
	don't blame me when stuff burns
	I mean if
*/
#ifndef REDUCEDLINEARALGEBRA_H
#define REDUCEDLINEARALGEBRA_H

//for uint32_t etc
#include <stdint.h>

/*
	Reduced Linear Algebra stuff here
	it is "reduced" because it is not complete linear algebra with vectors/matrices by any means
	just what is barely enough for 3d rendering
*/

struct Vector3D{
	Vector3D()
	{
		data[0] = data[1] = data[2] = 0;
		data[3] = 1;
	}
	
	Vector3D(float a, float b, float c)
	{
		data[0] = a; data[1] = b; data[2] = c;
		data[3] = 1;
	}
	
	//dot product of 2 vecs
	float dot(Vector3D b) const
	{
		//we will leave the 4th data, "w" out
		return (data[0] * b.data[0] +
				data[1] * b.data[1] +
				data[2] * b.data[2]);
	}
	
	//cross product of 2 vecs
	Vector3D cross(Vector3D b) const
	{
		Vector3D t;
		t.data[0] = (data[1] * b.data[2] - data[2] * b.data[1]);
		t.data[1] = (data[2] * b.data[0] - data[0] * b.data[2]);
		t.data[2] = (data[0] * b.data[1] - data[1] * b.data[0]);
		//cross product is not defined for 4d vectors so we leave this out
		//also this is vector3d struct so xyz and "w" is just hanging out
		t.data[3] = 1;
		return t;
	}
	
	Vector3D operator- (Vector3D param);
	Vector3D operator+ (Vector3D param);
	
	float data[4];
};


/*
	matrix has the data arranged as so:
			colums
	rows	data[0], data[4], data[08], data[12]
			data[1], data[5], data[09], data[13]
			data[2], data[6], data[10], data[14]
			data[3], data[7], data[11], data[15]
*/
struct Matrix{
	Matrix()
	{
		for (int32_t i = 0; i < 16; i++)
		{
			data[i] = 0;
		}
	}
	//returns multiplied matrix
	Matrix multiply(Matrix b) const
	{
		Matrix t;
		for (int32_t row = 0; row < 4; row++)
		{
			for (int32_t col = 0; col < 4; col++)
			{
				t.data[row+col*4] = data[row]    * b.data[col*4] +
									data[row+4]  * b.data[col*4+1] +
									data[row+8]  * b.data[col*4+2] +
									data[row+12] * b.data[col*4+3];
			}
		}
		return t;
	}
	
	//returns multiplied vector
	Vector3D multiply(Vector3D b) const
	{
		Vector3D t;
		for (int32_t row = 0; row < 4; row++)
		{
			t.data[row] =	data[row*4]    * b.data[0] +
							data[row*4+1]  * b.data[1] +
							data[row*4+2]  * b.data[2] +
							data[row*4+3]  * b.data[3];
		}
		return t;
	}
	
	float data[16];
};

Matrix get_identity();


#endif