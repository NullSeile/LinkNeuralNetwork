#pragma once

#ifndef PI
#define PI 3.1415926535f
#endif

// Distance
float operator "" _m(long double x);
float operator "" _m(unsigned long long x);

float operator "" _cm(long double x);
float operator "" _cm(unsigned long long x);

float operator "" _Km(long double x);
float operator "" _Km(unsigned long long x);

// Angle
float ToRad(long double x);
float operator "" _deg(long double x);
float operator "" _deg(unsigned long long x);

