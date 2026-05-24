#include <windows.h> // >:(

#include "Vector4.h"
#include "Vector3.h"
#include "Renderer.h"
#include "main.h"
#include "AABB.h"
#include "Object.h"
#include "Triangle.h"
#include "Constants.h"

HANDLE phys_thread;

void Init(HWND *hwnd, void *pixel_map) {
    LogInit();
    Vector3Init();
    Vector4Init();
    TriangleInit();
    RendererInit(pixel_map);
    AABBInit();
    working = 0;

    // create objs here
    // TODO, user facing scripting
    // ObjectCreateCube(0, 100, 100, 50, 50, 50, 1, 0xff00ff00);
    ObjectCreateSphere(0, 0, 250, 200, 200, 200, 6, 1, COLOR_RAND_PER_SETUP | COLOR_RAND_PER_TRI);

    phys_thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) PhysicsThread, hwnd, 0, NULL);

}

void Destroy() {
    TerminateThread(phys_thread, 0);
    CloseHandle(phys_thread);
    LogDestroy();
    Vector3Destroy();
    Vector4Destroy();
    TriangleDestroy();
    RendererDestroy();
}

void DoPhysics(double d_time) {
    // put calculated pre-statee
    // TODO SIMD this
    for (unsigned int i=vector3_start; i<vector3_cur; i++) {

        vector3X_secondary[i] = vector3X_primary[i] + vector3X_velocity[i] * d_time;
        vector3Y_secondary[i] = vector3Y_primary[i] + vector3Y_velocity[i] * d_time;
        vector3Z_secondary[i] = vector3Z_primary[i] + vector3Z_velocity[i] * d_time;
    }
    // do constraints for PBD
    for (unsigned int _=0; _<CONSTRAINT_ITERATIONS; _++) {
        // CollisionConstraintApply();
    }
    // after constraints have been solved put them to main for use next frame
    for (unsigned int i=vector3_start; i<vector3_cur; i++) {
        vector3X_velocity[i] = (vector3X_secondary[i] - vector3X_primary[i]) / d_time;
        vector3Y_velocity[i] = (vector3Y_secondary[i] - vector3Y_primary[i]) / d_time;
        vector3Z_velocity[i] = (vector3Z_secondary[i] - vector3Z_primary[i]) / d_time;
        vector3X_primary[i] = vector3X_secondary[i];
        vector3Y_primary[i] = vector3Y_secondary[i];
        vector3Z_primary[i] = vector3Z_secondary[i];
    }

    RendererApplyKeys(d_time); // apply camera movement
}

DWORD WINAPI PhysicsThread(const LPVOID *hwnd) {
    HWND hwn = *hwnd;
    const double d_s = 1. / PHYSICS_FRAME_RATE;
    const DWORD d_time = (DWORD) (1.0 / PHYSICS_FRAME_RATE) * 1000;
    LARGE_INTEGER StartingTime, EndingTime, Elapsed_Milleseconds;
    LARGE_INTEGER Frequency;

    QueryPerformanceFrequency(&Frequency);

    while (1) {
        if (SUICIDE_FLAG) {
            ExitThread(0);
        }
        QueryPerformanceCounter(&StartingTime);
        DoPhysics(d_s);
        while (working) {}
        working = 1;
        RendererRender();
        working = 0;
        QueryPerformanceCounter(&EndingTime);
        Elapsed_Milleseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
        Elapsed_Milleseconds.QuadPart *= 1000;
        Elapsed_Milleseconds.QuadPart /= Frequency.QuadPart;
        InvalidateRect(hwn, NULL, 0);
        PostMessage(hwn, WM_PAINT, 0, 0);
        const DWORD d = Elapsed_Milleseconds.LowPart;
        if (d < d_time) {
            Sleep(d_time - d);
        }
    }
    return 0;
}