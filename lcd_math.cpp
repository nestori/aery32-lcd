/*
	By Nestori, 2013
	don't blame me when stuff burns
	I mean if
*/
#include "lcd_math.h"


//returns sine of given number, input has 1 degree accuracy
float lcd_sin(float z)
{
	int16_t x = z;
	
	if (x < 0) x = 360 - (-x)%360;
	
	x %=360;
	
	//use 90-value look-up table to get correct value
	if (x < 90)  return trigLUT[ (int16_t)x % 90 ];
	if (x < 180) return trigLUT[ 90 - (int16_t)x % 90 ];
	if (x < 270) return -trigLUT[ (int16_t)x % 90 ];
	return -trigLUT[ 90 - (int16_t)x % 90 ];
}


//returns cosine of given number, input has 1 degree accuracy
float lcd_cos(float x)
{
	return lcd_sin(x+90);
}

//returns distance to 0 of given number
int32_t lcd_abs(int32_t a)
{
	if (a < 0)
	{
		return -a;
	} else {
		return a;
	}
}

//returns smallest number out of given 3
int32_t find_min(int32_t a, int32_t b, int32_t c)
{
	int32_t temp = a;
	if (b < a) temp = b;
	if (c < temp) temp = c;
	return temp;
}

//returns largest number out of given 3
int32_t find_max(int32_t a, int32_t b, int32_t c)
{
	int32_t temp = a;
	if (b > a) temp = b;
	if (c > temp) temp = c;
	return temp;
}

//pseudorandom number generator code
//random seeds
uint32_t prng_seed1 = 1123477;
uint32_t prng_seed2 = 5583290;

//set seeds
void set_prng_seed(uint32_t v, uint32_t u)
{
	prng_seed1 = v;
	prng_seed2 = u;
}

//returns pseudorandom num
uint32_t prng()
{
	//by George Marsaglia
	prng_seed1 = 36969*(prng_seed1 & 65535) + (prng_seed1 >> 16);
	prng_seed2 = 18000*(prng_seed2 & 65535) + (prng_seed2 >> 16);
	return (prng_seed1 << 16) + prng_seed2;
}