#include "stb.h"
#include "util.h"
#include "video.h"
#include "common.h"
#include "texture.h"

void Texture_LoadFile(int which, const char* path) {
	int width, height, comp;

	uint8_t* data = stbi_load(path, &width, &height, &comp, 4);

	if (data == NULL) {
		Error("Failed to load %s: %s", path, stbi_failure_reason());
	}

	glActiveTextureARB(which);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	Log("Loaded %s to %d", path, which);
}
