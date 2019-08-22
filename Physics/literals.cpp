#include "literals.h"

// Distance
float operator "" _m(long double x)
{
	return (float)x;
}
float operator "" _m(unsigned long long x)
{
	return (float)x;
}

float operator "" _cm(long double x)
{
	return (float)x;
}
float operator "" _cm(unsigned long long x)
{
	return (float)x / 100.f;
}

float operator "" _Km(long double x)
{
	return (float)x * 1000;
}
float operator "" _Km(unsigned long long x)
{
	return (float)x * 1000;
}

// Angle
float ToRad(long double x)
{
	return (float)x * PI / 180.f;
}

float operator "" _deg(long double x)
{
	return ToRad(x);
}
float operator "" _deg(unsigned long long x)
{
	return ToRad((long double)x);
}