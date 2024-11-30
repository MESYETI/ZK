#include "app.h"
#include "util.h"
#include "render.h"
#include "texture.h"

App app;

void App_Init(void) {
	app.running = true;
	Video_Init();
	Renderer_Init();

	Model_Load(&app.model, "villager.zkm");
	Log("Loaded model");

	Texture_LoadFile(0, "assets/textures.png");
}

void App_Update(void) {
	static uint64_t oldFrameTime = 0;

	uint64_t newFrameTime  = SDL_GetTicks64();
	uint64_t frameTimeDiff = newFrameTime - oldFrameTime;
	app.delta              = frameTimeDiff / 1000.0f;

	static const float sensitivity = 7.5f;

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_QUIT: app.running = false; break;
			case SDL_MOUSEMOTION: {
				renderer.camDir.yaw   += (float) e.motion.xrel * sensitivity * app.delta;
				renderer.camDir.pitch -= (float) e.motion.yrel * sensitivity * app.delta;
				break;
			}
		}
	}

	const uint8_t* keys = SDL_GetKeyboardState(NULL);

	{
		float moveX = 0.0f;
		float moveZ = 0.0f;
		float moveY = 0.0f;

		if (keys[SDL_SCANCODE_W]) moveZ += 1.0f;
		if (keys[SDL_SCANCODE_A]) moveX -= 1.0f;
		if (keys[SDL_SCANCODE_S]) moveZ -= 1.0f;
		if (keys[SDL_SCANCODE_D]) moveX += 1.0f;
		if (keys[SDL_SCANCODE_SPACE]) moveY += 1.0f;
		if (keys[SDL_SCANCODE_LSHIFT]) moveY -= 1.0f;

		float mul      = atan2f(fabs(moveX), fabs(moveZ));
		mul            = fabs(1 / (cosf(mul) + sinf(mul)));
		moveX         *= mul;
		moveZ         *= mul;
		float yrotrad  = renderer.camDir.yaw * (float) PI / 180.0f;
		float tmp[4];

		tmp[0]  = sinf(yrotrad);
		tmp[1]  = cosf(yrotrad);
		tmp[2]  = tmp[1] * moveZ;
		tmp[3]  = -tmp[0] * moveX;
		tmp[0] *= moveZ;
		tmp[1] *= moveX;
		moveX   = tmp[0] + tmp[1];
		moveZ   = tmp[2] + tmp[3];

		float speed = 2.0; // units per second
		renderer.camPos.x += moveX * speed * app.delta;
		renderer.camPos.y += moveY * speed * app.delta;
		renderer.camPos.z += moveZ * speed * app.delta;
	}

	glColor3ub(0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((float*) renderer.projMatrix);
	glMatrixMode(GL_MODELVIEW);
	Renderer_CalculateViewMatrix();
	glLoadMatrixf((float*) renderer.viewMatrix);

	ModelRenderOpt opt;
	opt.scale = 0.1;

	//Model_Render(&app.model, &opt);
	GLenum err;
	fprintf(stderr, "Getting errors\n");
	while ((err = glGetError()) != GL_NO_ERROR) {
		fprintf(stderr, "Error: %.4x\n", (int) err);
	}

	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2f(0,0); // Texture coords for lower left corner
	glVertex3f(-0.5, -0.5, 1.0);

	glTexCoord2f(1,   0); // Texture coords for lower right corner
	glVertex3f(0.5,  -0.5, 1.0);
	
	glTexCoord2f(1,   1); // Texture coords for upper right corner
	glVertex3f(0.5,   0.5, 1.0);
	
	glTexCoord2f(0,   1); // Texture coords for upper left corner
	glVertex3f(-0.5,  0.5, 1.0);
	glEnd();

	glFinish();
	fprintf(stderr, "Getting errors\n");
	while ((err = glGetError()) != GL_NO_ERROR) {
		fprintf(stderr, "Error: %.4x\n", (int) err);
	}
	SDL_GL_SwapWindow(video.window);

	oldFrameTime = newFrameTime;
}

void App_Free(void) {
	Model_Free(&app.model);
	Video_Free();
}

