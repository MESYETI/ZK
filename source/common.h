#ifndef PROJ_COMMON_H
#define PROJ_COMMON_H

#include <math.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define PI 3.14159265359

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	#define PLATFORM_WINDOWS
#elif defined(__APPLE__)
	#define PLATFORM_OSX
#elif defined(__linux__)
	#define PLATFORM_LINUX
#elif defined(__unix__)
	#define PLATFORM_UNIX
#endif

#endif
