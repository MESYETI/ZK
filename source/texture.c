#include "stb.h"
#include "util.h"
#include "video.h"
#include "common.h"
#include "texture.h"

void Texture_LoadFile(const char* path) {
	int width, height, ch;

	uint8_t* data = stbi_load(path, &width, &height, &ch, 0);

	if (data == NULL) {
		Error("Failed to load %s: %s", path, stbi_failure_reason());
	}

	glTexImage2D(GL_TEXTURE_2D, 0, ch, width, height, 0, (ch == 3) ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    free(data);

	Log("Loaded %s (%dx%d@%dbpp)", path, width, height, ch * 8);
}
