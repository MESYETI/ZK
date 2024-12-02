#include "common.h"
#include "render.h"
#include "video.h"
#include "model.h"
#include "texture.h"
#include "util.h"

Renderer renderer;

#if USE_KHR_DEBUG
    #if defined(GLAPIENTRY)
        #define GLDBGCB GLAPIENTRY
    #elif defined(APIENTRY)
        #define GLDBGCB APIENTRY
    #else
        #define GLDBGCB
    #endif
    static void GLDBGCB glcallback(GLenum src, GLenum type, GLuint id, GLenum sev, GLsizei l, const GLchar *m, const void *u) {
        (void)l; (void)u;
        char* sevstr;
        switch (sev) {
            case GL_DEBUG_SEVERITY_HIGH: sevstr = "error"; break;
            case GL_DEBUG_SEVERITY_MEDIUM: sevstr = "warning"; break;
            case GL_DEBUG_SEVERITY_LOW: sevstr = "warning"; break;
            #if 0
            case GL_DEBUG_SEVERITY_NOTIFICATION: sevstr = "debug message"; break;
            #endif
            default: return;
        }
        char* srcstr;
        switch (src) {
            case GL_DEBUG_SOURCE_API: srcstr = "API"; break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM: srcstr = "Windowing system"; break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER: srcstr = "Shader compiler"; break;
            case GL_DEBUG_SOURCE_THIRD_PARTY: srcstr = "Third-party"; break;
            case GL_DEBUG_SOURCE_APPLICATION: srcstr = "Application"; break;
            case GL_DEBUG_SOURCE_OTHER: srcstr = "Other"; break;
            default: srcstr = "Unknown"; break;
        }
        char* typestr;
        switch (type) {
            case GL_DEBUG_TYPE_ERROR: typestr = "error"; break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typestr = "deprecation"; break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: typestr = "undefined behavior"; break;
            case GL_DEBUG_TYPE_PORTABILITY: typestr = "portability"; break;
            case GL_DEBUG_TYPE_PERFORMANCE: typestr = "performance"; break;
            case GL_DEBUG_TYPE_MARKER: typestr = "marker"; break;
            case GL_DEBUG_TYPE_PUSH_GROUP: typestr = "push group"; break;
            case GL_DEBUG_TYPE_POP_GROUP: typestr = "pop group"; break;
            case GL_DEBUG_TYPE_OTHER: typestr = "other"; break;
            default: typestr = "unknown"; break;
        }
        Log("OpenGL %s 0x%08X (%s %s): %s", sevstr, id, srcstr, typestr, m);
    }
#endif

// temporary testing stuff
static Model model;
static GLuint texh;

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
    #if USE_KHR_DEBUG
        Log("Using GL_KHR_debug");
        glDebugMessageCallback(glcallback, NULL);
    #endif

	int maxTextureSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
	Log("Max texture size: %d", maxTextureSize);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

    // temporary testing stuff
	Model_Load(&model, "villager.zkm");
	Log("Loaded model");

    glGenTextures(1, &texh);
    glBindTexture(GL_TEXTURE_2D, texh);
	Texture_LoadFile("assets/textures.png");
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

void Renderer_RenderScene(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((float*) renderer.projMatrix);
	glMatrixMode(GL_MODELVIEW);
	Renderer_CalculateViewMatrix();
	glLoadMatrixf((float*) renderer.viewMatrix);

	//ModelRenderOpt opt;
	//opt.scale = 0.1;
	//Model_Render(&app.model, &opt);

    glBindTexture(GL_TEXTURE_2D, texh);
	glBegin(GL_TRIANGLE_FAN);
	    glTexCoord2f(0.0f, 1.0f); // Texture coords for lower left corner
	    glVertex3f(-0.5f, -0.5f, 1.0f);

	    glTexCoord2f(1.0f, 1.0f); // Texture coords for lower right corner
	    glVertex3f(0.5f, -0.5f, 1.0f);
	    
	    glTexCoord2f(1.0f, 0.0f); // Texture coords for upper right corner
	    glVertex3f(0.5f, 0.5f, 1.0f);
	    
	    glTexCoord2f(0.0f, 0.0f); // Texture coords for upper left corner
	    glVertex3f(-0.5f, 0.5f, 1.0f);
	glEnd();
}
void Renderer_FinishScene(void) {
	glFinish();
	SDL_GL_SwapWindow(video.window);
}

void Renderer_Free(void) {
	Model_Free(&model);
}
