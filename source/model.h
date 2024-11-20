#ifndef ZK_MODEL_H
#define ZK_MODEL_H

#include "types.h"
#include "render.h"

typedef struct {
	FVec3  vertices[3];
	Colour colour;
} ModelFace;

typedef struct {
	FVec3*     vertices;
	ModelFace* faces;
} Model;
