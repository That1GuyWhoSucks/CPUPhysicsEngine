#include "Vector4.h"
#include "Vector3.h"
#include "Renderer.h"
#include <windows.h>
#include "main.h"

int main() {
    Init(NULL, NULL);
}

void Init(const HWND *hwnd, void *pixel_map) {
    Vector3Init();
    Vector4Init();
    // RendererInit(pixel_map);
}

void Destroy() {
    Vector3Destroy();
    Vector4Destroy();
}