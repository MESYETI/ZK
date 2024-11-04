#include <stdio.h>
#include "video.h"

Video video;

void Video_Init(void) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Failed to initialise SDL2: %s\n", SDL_GetError());
		exit(1);
	}

	assert(SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY
	) == 0);
	assert(SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_MAJOR_VERSION, 1
	) == 0);
	assert(SDL_GL_SetAttribute(
		SDL_GL_CONTEXT_MINOR_VERSION, 1
	) == 0);

	video.window = SDL_CreateWindow(
		"Not Elite", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480,
		SDL_WINDOW_OPENGL
	);

	if (video.window == NULL) {
		fprintf(stderr, "Failed to create window: %s\n", SDL_GetError());
		exit(1);
	}

	video.ctx = SDL_GL_CreateContext(video.window);
	SDL_GL_MakeCurrent(video.window, video.ctx);

	if (SDL_GL_SetSwapInterval(-1) == -1) {
		SDL_GL_SetSwapInterval(1);
	}

	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Video_Free(void) {
	SDL_GL_DeleteContext(video.ctx);
	SDL_DestroyWindow(video.window);
	SDL_Quit();
}
