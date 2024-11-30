#include "stb.h"
#include "util.h"
#include "video.h"
#include "common.h"
#include "texture.h"

void Texture_LoadFile(int which, const char* path) {
	GLenum err;
	fprintf(stderr, "Getting errors\n");
	while ((err = glGetError()) != GL_NO_ERROR) {
		fprintf(stderr, "Error: %.4x\n", (int) err);
	}

	int width, height, comp;

	uint8_t* data = stbi_load(path, &width, &height, &comp, 4);

	if (data == NULL) {
		Error("Failed to load %s: %s", path, stbi_failure_reason());
	}

	glActiveTextureARB(GL_TEXTURE0_ARB + which);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	Log("Loaded %s to %d", path, which);

	fprintf(stderr, "Getting errors\n");
	while ((err = glGetError()) != GL_NO_ERROR) {
		fprintf(stderr, "Error: %.4x\n", (int) err);
	}
}
