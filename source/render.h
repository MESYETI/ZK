#ifndef PROJ_RENDER_H
#define PROJ_RENDER_H

#include "types.h"

typedef struct {
	float nearPlane;
	float farPlane;
	float projMatrix[4][4];
	float viewMatrix[4][4];
	float fov;
	float aspect;
	FVec3 camPos;
	FDir3 camDir;
} Renderer;

extern Renderer renderer;

void Renderer_Init(void);
void Renderer_CalculateViewMatrix(void);

#endif
