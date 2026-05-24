cmake_minimum_required(VERSION 4.2)
project(CPUPhysicsEngine C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_WIN32_EXECUTABLE 1)

ADD_COMPILE_OPTIONS(/fp:fast)

enable_language(ASM_MASM)

add_executable(CPUPhysicsEngine
        main.c
        main.h
        Vector4.c
        Vector4.h
        Vector3.c
        Vector3.h
        Renderer.c
        Renderer.h
        Constants.c
        Constants.h
        AABB.c
        AABB.h
        Triangle.c
        Triangle.h
        WindowsFuckery.c
        WindowsFuckery.h
        Object.c
        Object.h
        ExpandableArray.c
        ExpandableArray.h
        invsqrt.asm
)
