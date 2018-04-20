#ifndef __FASTMATH_H_
#define __FASTMATH_H_

#define SINARRAYSIZE 1024
#define SINARRAYSCALE 32767

#define M_TWOPI 6.2831853071796f
#define M_PI    3.1415926535898f

extern short int sinDataI16[];
void InitSinArray(void);
float fastSin(float x);

extern float Rad2Deg(float x);
extern float Deg2Rad(float x);
extern float Round(float x);

#endif

