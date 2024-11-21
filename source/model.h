#ifndef ZK_MODEL_H
#define ZK_MODEL_H

#include "types.h"
#include "render.h"

typedef struct {
	uint32_t vertices[3];
	Colour   colour;
} ModelFace;

typedef struct {
	FVec3*     vertices;
	ModelFace* faces;
	uint32_t   verticesNum;
	uint32_t   facesNum;
} Model;

void Model_Load(Model* model, const char* path);
void Model_Free(Model* model);
void Model_Render(Model* model);

#endif
