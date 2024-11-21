#ifndef BK_UTIL_H
#define BK_UTIL_H

#include <math.h>
#include <stdio.h>

#define FUNCTION_POINTER(TYPE, NAME, ...) TYPE (*NAME)(__VA_ARGS__)
#define UNUSED(V) (void) V
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

float   RadToDeg(float rad);
float   DegToRad(float deg);
float   CosDeg(float deg);
float   SinDeg(float deg);
float   Lerp(float a, float b, float t);
char*   NewString(const char* src);
void    Log(const char* format, ...);
void    Error(const char* format, ...);

#endif
