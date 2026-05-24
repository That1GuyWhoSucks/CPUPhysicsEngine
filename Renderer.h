#ifndef CPUPHYSICSENGINE_RENDERER_H
#define CPUPHYSICSENGINE_RENDERER_H

#include "Constants.h"

int keys_down;
double *renderer_z_buffer;
Color *renderer_buffer;

void RendererInit(void *pixel_map);

void RendererDestroy();

void RendererOnKeyDown(unsigned short input);

void RendererOnKeyUp(unsigned short input);

void RendererOnMouse(short x_delta, short y_delta);

void RendererApplyKeys(double d_time);

void RendererRender();

#endif //CPUPHYSICSENGINE_RENDERER_H