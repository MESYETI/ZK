#include <SDL2/SDL.h>
#include <stdio.h>
#include "util.h"
#include "safe.h"
#include "model.h"
#include "video.h"

static uint8_t ReadByte(FILE* file) {
	int ret = fgetc(file);
	assert(ret != EOF);
	return ret;
}

static uint32_t Read32Bit(FILE* file) {
	uint32_t ret;
	uint8_t  bytes[4];

	assert(fread(&bytes, 4, 1, file) == 1);

	ret  = bytes[0];
	ret |= ((uint32_t) bytes[1]) << 8;
	ret |= ((uint32_t) bytes[2]) << 16;
	ret |= ((uint32_t) bytes[3]) << 24;

	return ret;
}
static float Read32BitF(FILE* file) {
	uint32_t ret;
	uint8_t  bytes[4];

	assert(fread(&bytes, 4, 1, file) == 1);

	ret  = bytes[0];
	ret |= ((uint32_t) bytes[1]) << 8;
	ret |= ((uint32_t) bytes[2]) << 16;
	ret |= ((uint32_t) bytes[3]) << 24;

	return *(float*)&ret;
}

#define EOFERR(p) Error("Hit end of file '%s' too early on line %u", (p), __LINE__)
void Model_Load(Model* model, const char* path) {
	FILE* file = fopen(path, "rb");

	if (file == NULL) {
		perror(path);
		Error("Failed to open '%s'", path);
	}

	char magic[3];

	if (fread(magic, 1, 3, file) != 3) EOFERR(path);
	if (magic[0] != 'Z' || magic[1] != 'K' || magic[2] != 'M') {
		Error("File '%s' is not a ZKM model", path);
	}

	int ver = fgetc(file);

	if (ver == EOF) EOFERR(path);
	if (ver != 0) {
		Error("Model '%s' is out of date or too new", path);
	}

	if (fread(&model->verticesNum, 4, 1, file) != 1) EOFERR(path);
	if (fread(&model->facesNum,    4, 1, file) != 1) EOFERR(path);
	// TODO: swap these ^ on big endian

	#ifndef NDEBUG
	printf("%u vertices, %u faces\n", model->verticesNum, model->facesNum);
	#endif

	model->vertices = SafeMalloc(model->verticesNum * sizeof(*model->vertices));
	model->faces    = SafeMalloc(model->facesNum * sizeof(*model->faces));

	for (uint32_t i = 0; i < model->verticesNum; ++i) {
		model->vertices[i].x = Read32BitF(file);
		model->vertices[i].y = Read32BitF(file);
		model->vertices[i].z = Read32BitF(file);
	}

	for (uint32_t i = 0; i < model->facesNum; ++i) {
		assert((model->faces[i].indices[0] = Read32Bit(file)) < model->verticesNum);
		assert((model->faces[i].indices[1] = Read32Bit(file)) < model->verticesNum);
		assert((model->faces[i].indices[2] = Read32Bit(file)) < model->verticesNum);
		model->faces[i].colour.r           = ReadByte(file);
		model->faces[i].colour.g           = ReadByte(file);
		model->faces[i].colour.b           = ReadByte(file);
	}
}

void Model_Free(Model* model) {
	free(model->vertices);
	free(model->faces);
}

void Model_Render(Model* model) {
	glBegin(GL_TRIANGLES);
	for (size_t i = 0; i < model->facesNum; ++ i) {
		ModelFace* face = &model->faces[i];

		glColor3ub(face->colour.r, face->colour.g, face->colour.b);

		#if 1
		int ci = (face->indices[0] * 0x10492851) ^ face->indices[1];
		uint8_t c[3] = {ci >> 16, ci >> 8, ci};
		glColor3ub(c[0], c[1], c[2]);
		#endif

		glVertex3f(
			model->vertices[face->indices[0]].x, model->vertices[face->indices[0]].y,
			model->vertices[face->indices[0]].z
		);
		glVertex3f(
			model->vertices[face->indices[1]].x, model->vertices[face->indices[1]].y,
			model->vertices[face->indices[1]].z
		);
		glVertex3f(
			model->vertices[face->indices[2]].x, model->vertices[face->indices[2]].y,
			model->vertices[face->indices[2]].z
		);
	}
	glEnd();
}
