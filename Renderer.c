#include "Renderer.h"
#include "Constants.h"
#include "Vector3.h"
#include "Vector4.h"

void RendererInit(void *pixel_map) {
    renderer_z_buffer = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(double));
    renderer_camera_pos_index = Vector3(0,0,0);
    renderer_camera_rot_index = Vector4(1,0,0,0);
    renderer_x_pos_index = Vector4(1, ROTATION_DELTA, 0, 0);
    renderer_x_neg_index = Vector4(1, -ROTATION_DELTA, 0, 0);
    renderer_y_pos_index = Vector4(1, 0, ROTATION_DELTA, 0);
    renderer_y_neg_index = Vector4(1, 0, -ROTATION_DELTA, 0);
    renderer_z_pos_index = Vector4(1, 0, 0, ROTATION_DELTA);
    renderer_z_neg_index = Vector4(1, 0, 0, -ROTATION_DELTA);
    renderer_buffer = pixel_map;
}

void RendererDestroy() {
    free(renderer_z_buffer);
}

// TODO, finish copying all this from v1
void RendererOnInput(const wchar_t input) {
    switch (input) {

    }
}

void RendererRender() {

}
