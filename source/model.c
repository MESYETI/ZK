#include <SDL2/SDL.h>
#include <stdio.h>
#include "util.h"
#include "safe.h"
#include "model.h"
#include "video.h"

static uint8_t ReadByte(FILE* file) {
	uint8_t ret;
	assert(fread(&ret, 1, 1, file) == 1);
	return ret;
}

static uint32_t Read32Bit(FILE* file) {
	uint32_t ret;
	uint8_t  bytes[4];

	assert(fread(&bytes, 4, 1, file) == 4);

	ret  = bytes[0];
	ret |= ((uint32_t) bytes[1]) << 8;
	ret |= ((uint32_t) bytes[2]) << 16;
	ret |= ((uint32_t) bytes[3]) << 24;

	return ret;
}

void Model_Load(Model* model, const char* path) {
	FILE* file = fopen(path, "rb");

	if (file == NULL) {
		perror(path);
		Error("Failed to open '%s'", path);
	}

	bool isModel = true;

	if (ReadByte(file) != 'Z')            isModel = false;
	if (isModel && ReadByte(file) != 'K') isModel = false;
	if (isModel && ReadByte(file) != 'M') isModel = false;

	if (!isModel) {
		Error("File '%s' is not a ZKM model", path);
	}

	if (ReadByte(file) != 0) {
		Error("Model '%s' is out of date or too new", path);
	}

	model->verticesNum = Read32Bit(file);
	model->facesNum    = Read32Bit(file);

	printf("%u vertices, %u faces\n", model->verticesNum, model->facesNum);

	model->vertices = SafeMalloc(model->verticesNum * sizeof(FVec3));
	model->faces    = SafeMalloc(model->facesNum * sizeof(ModelFace));

	for (uint32_t i = 0; i < model->verticesNum; ++ i) {
		model->vertices[i].x = ((float) Read32Bit(file)) / 65536.0;
		model->vertices[i].y = ((float) Read32Bit(file)) / 65536.0;
		model->vertices[i].z = ((float) Read32Bit(file)) / 65536.0;
	}

	for (uint32_t i = 0; i < model->facesNum; ++ i) {
		model->faces[i].vertices[0] = Read32Bit(file);
		model->faces[i].vertices[1] = Read32Bit(file);
		model->faces[i].vertices[2] = Read32Bit(file);
		model->faces[i].colour.r    = ReadByte(file);
		model->faces[i].colour.g    = ReadByte(file);
		model->faces[i].colour.b    = ReadByte(file);
	}
}

void Model_Free(Model* model) {
	free(model->vertices);
	free(model->faces);
}

void Model_Render(Model* model) {
	for (size_t i = 0; i < model->facesNum; ++ i) {
		ModelFace* face = &model->faces[i];

		glBegin(GL_TRIANGLES);
		glColor3ub(face->colour.r, face->colour.g, face->colour.b);

		glVertex3f(
			model->vertices[face->vertices[0]].x, model->vertices[face->vertices[0]].y,
			model->vertices[face->vertices[0]].z
		);
		glVertex3f(
			model->vertices[face->vertices[1]].x, model->vertices[face->vertices[1]].y,
			model->vertices[face->vertices[1]].z
		);
		glVertex3f(
			model->vertices[face->vertices[2]].x, model->vertices[face->vertices[2]].y,
			model->vertices[face->vertices[2]].z
		);

		glEnd();
	}
}
