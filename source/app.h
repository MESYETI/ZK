#ifndef ZK_APP_H
#define ZK_APP_H

#include "video.h"
#include "common.h"

typedef struct {
	bool  running;
	float delta;
} App;

extern App app;

void App_Init(void);
void App_Update(void);
void App_Free(void);

#endif
