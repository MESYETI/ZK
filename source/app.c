#include "app.h"
#include "util.h"
#include "render.h"

App app;

void App_Init(void) {
	app.running = true;
	Video_Init();
	Renderer_Init();
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

		if (keys[SDL_SCANCODE_W]) moveZ += 1.0f;
		if (keys[SDL_SCANCODE_A]) moveX -= 1.0f;
		if (keys[SDL_SCANCODE_S]) moveZ -= 1.0f;
		if (keys[SDL_SCANCODE_D]) moveX += 1.0f;

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
		renderer.camPos.z += moveZ * speed * app.delta;
	}

	glColor3b(0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((float*) renderer.projMatrix);
	glMatrixMode(GL_MODELVIEW);
	Renderer_CalculateViewMatrix();
	glLoadMatrixf((float*) renderer.viewMatrix);

	glBegin(GL_TRIANGLES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(-0.5f, -0.5f, 1.0f);
	glColor3f(0.0, 1.0, 0.0);
	glVertex3f( 0.5f, -0.5f, 1.0f);
	glColor3f(0.0, 0.0, 1.0);
	glVertex3f( 0.0f,  0.5f, 1.0f);
	glEnd();

	glFinish();
	SDL_GL_SwapWindow(video.window);

	oldFrameTime = newFrameTime;
}

void App_Free(void) {
	Video_Free();
}

