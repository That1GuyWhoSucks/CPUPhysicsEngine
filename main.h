#ifndef CPUPHYSICSENGINE_MAIN_H
#define CPUPHYSICSENGINE_MAIN_H

#include <windows.h> // >:(

void Init(HWND *hwnd, void *pixel_map);

void Destroy();

DWORD WINAPI PhysicsThread(const LPVOID *hwnd);

volatile char working;
#endif //CPUPHYSICSENGINE_MAIN_H