#ifndef ZK_APP_H
#define ZK_APP_H

#include "video.h"
#include "model.h"
#include "common.h"

typedef struct {
	bool  running;
	float delta;
	Model model;
} App;

extern App app;

void App_Init(void);
void App_Update(void);
void App_Free(void);

#endif
