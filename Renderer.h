#ifndef CPUPHYSICSENGINE_RENDERER_H
#define CPUPHYSICSENGINE_RENDERER_H

double *renderer_z_buffer;
unsigned char *renderer_buffer;
unsigned int renderer_camera_pos_index;
unsigned int renderer_camera_rot_index;
unsigned int renderer_x_pos_index;
unsigned int renderer_x_neg_index;
unsigned int renderer_y_pos_index;
unsigned int renderer_y_neg_index;
unsigned int renderer_z_pos_index;
unsigned int renderer_z_neg_index;

#include "wchar.h"

void RendererInit(void *pixel_map);

void RendererDestroy();

void RendererOnInput(wchar_t input);

void RendererRender();

#endif //CPUPHYSICSENGINE_RENDERER_H