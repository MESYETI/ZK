#ifndef ZK_VIDEO_H
#define ZK_VIDEO_H

#include "common.h"
#include <SDL2/SDL.h>
#undef   main

#define GL_GLEXT_PROTOTYPES
#ifdef PLATFORM_OSX
	#include <OpenGL/gl.h>
	#include <OpenGL/glext.h>
#else
	#include <GL/gl.h>
	#include <GL/glext.h>
#endif

typedef struct {
	SDL_Window*   window;
	SDL_GLContext ctx;
} Video;

extern Video video;

void Video_Init(void);
void Video_Free(void);

#endif
