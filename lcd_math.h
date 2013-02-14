/*
	By Nestori, 2013
	don't blame me when stuff burns
	I mean if
*/
#ifndef MATHSTUFF_H
#define MATHSTUFF_H
#include "lcd_defines.h"
#include "lut.h"

float lcd_sin(float x);
float lcd_cos(float x);
int32_t lcd_abs(int32_t a);
int32_t find_min(int32_t a, int32_t b, int32_t c);
int32_t find_max(int32_t a, int32_t b, int32_t c);

//pseudorandom generator
extern uint32_t prng_seed1;
extern uint32_t prng_seed2;
void set_prng_seed(uint32_t v, uint32_t u);
uint32_t prng();

#endif