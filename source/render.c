#include "common.h"
#include "render.h"
#include "video.h"

Renderer renderer;

void Renderer_Init(void) {
	// set values
	renderer.nearPlane        = 0.1;
	renderer.farPlane         = 1000;
	renderer.fov              = 70;
	renderer.aspect           = 640.0 / 480.0; // TODO: replace this
	renderer.camPos           = (FVec3) {0.0, 0.0, 0.0};
	renderer.camDir           = (FDir3) {0.0, 0.0, 0.0};
	renderer.viewMatrix[3][3] = 1.0f;
	renderer.projMatrix[2][3] = -1.0f;
	
	// calcualte projection matrix
	float tmp1 = 1.0f / tanf(renderer.fov * (float) PI / 180.0f * 0.5f);
	float tmp2 = 1.0f / (renderer.nearPlane - renderer.farPlane);
	renderer.projMatrix[0][0] = -(tmp1 / renderer.aspect);
	renderer.projMatrix[1][1] = tmp1;
	renderer.projMatrix[2][2] = (renderer.nearPlane + renderer.farPlane) * tmp2;
	renderer.projMatrix[3][2] = 2.0f * renderer.nearPlane * renderer.farPlane * tmp2;

	// set up OpenGL
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_CULL_FACE);
}

void Renderer_CalculateViewMatrix(void) {
	static float up[3];
	static float front[3];
	static float rotRadX, rotRadY, rotRadZ;

	rotRadX = renderer.camDir.pitch * (float) PI / 180.0f;
	rotRadY = renderer.camDir.yaw * -(float) PI / 180.0f;
	rotRadZ = renderer.camDir.roll * (float) PI / 180.0f;

	static float sinX, cosX;
	static float sinY, cosY;
	static float sinZ, cosZ;

	sinX     = sin(rotRadX);
	cosX     = cos(rotRadX);
	sinY     = sin(rotRadY);
	cosY     = cos(rotRadY);
	sinZ     = sin(rotRadZ);
	cosZ     = cos(rotRadZ);
	up[0]    = sinX * sinY * cosZ + cosY * sinZ;
	up[1]    = cosX * cosZ;
	up[2]    = -sinX * cosY * cosZ + sinY * sinZ;
	front[0] = cosX * -sinY;
	front[1] = sinX;
	front[2] = cosX * cosY;

	renderer.viewMatrix[0][0] =  front[1] * up[2] - front[2] * up[1];
	renderer.viewMatrix[1][0] =  front[2] * up[0] - front[0] * up[2];
	renderer.viewMatrix[2][0] =  front[0] * up[1] - front[1] * up[0];
	renderer.viewMatrix[3][0] =
		-(renderer.viewMatrix[0][0] * renderer.camPos.x + renderer.viewMatrix[1][0] *
		renderer.camPos.y + renderer.viewMatrix[2][0] * renderer.camPos.z);

	renderer.viewMatrix[0][1] =  up[0];
	renderer.viewMatrix[1][1] =  up[1];
	renderer.viewMatrix[2][1] =  up[2];
	renderer.viewMatrix[3][1] =
		-(up[0] * renderer.camPos.x + up[1] * renderer.camPos.y + up[2] * renderer.camPos.z);

	renderer.viewMatrix[0][2] = -front[0];
	renderer.viewMatrix[1][2] = -front[1];
	renderer.viewMatrix[2][2] = -front[2];
	renderer.viewMatrix[3][2] =
		front[0] * renderer.camPos.x + front[1] * renderer.camPos.y + front[2] *
		renderer.camPos.z;
}
