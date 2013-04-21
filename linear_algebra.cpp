#include "linear_algebra.h"

Vector3D Vector3D::operator- (Vector3D param)
{
	Vector3D temp;
	temp.data[0] = data[0] - param.data[0];
	temp.data[1] = data[1] - param.data[1];
	temp.data[2] = data[2] - param.data[2];
	
	return temp;
}

Vector3D Vector3D::operator+ (Vector3D param)
{
	Vector3D temp;
	temp.data[0] = data[0] + param.data[0];
	temp.data[1] = data[1] + param.data[1];
	temp.data[2] = data[2] + param.data[2];
	
	return temp;
}

Matrix get_identity()
{
	Matrix t;
	t.data[0] = t.data[5] = t.data[10] = t.data[15] = 1;
	return t;
}