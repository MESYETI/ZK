#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "util.h"
#include "safe.h"
#include "common.h"

float RadToDeg(float rad) {
	return rad * 180 / PI;
}

float DegToRad(float deg) {
	return deg * (PI / 180);
}

float Lerp(float a, float b, float t) {
    return a * (1.0 - t) + (b * t);
}

float CosDeg(float deg) {
	return cos(DegToRad(deg));
}

float SinDeg(float deg) {
	return sin(DegToRad(deg));
}

char* NewString(const char* src) {
	char* ret = SafeMalloc(strlen(src) + 1);
	strcpy(ret, src);
	return ret;
}

void Log(const char* format, ...) { // most of this is taken from vsprintf(3)
	int     n    = 0;
	size_t  size = 0;
	char*   ret  = NULL;
	va_list ap;

	// Determine required size
	va_start(ap, format);
	n = vsnprintf(ret, size, format, ap);
	va_end(ap);

	if (n < 0) {
		return;
	}

	size = n + 1; // One extra byte for '\0'
	ret = (char*) SafeMalloc(size);
	if (ret == NULL) {
		return;
	}

	va_start(ap, format);
	n = vsnprintf(ret, size, format, ap);
	va_end(ap);

	if (n < 0) {
		free(ret);
	    return;
	}

	time_t     rawTime;
	struct tm* tm;
	
	time(&rawTime);
	tm = localtime(&rawTime);
	
	printf("[%.2d:%.2d:%.2d] %s\n", tm->tm_hour, tm->tm_min, tm->tm_sec, ret);
	free(ret);
}
