#ifndef CPUPHYSICSENGINE_CONSTANTS_H
#define CPUPHYSICSENGINE_CONSTANTS_H

#include <immintrin.h>

#define SHUFFLE_R(a,b,c,d) _MM_SHUFFLE(d,c,b,a)
#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 1000
#define SCREEN_WIDTH_HALF (SCREEN_WIDTH / 2.)
#define SCREEN_HEIGHT_HALF (SCREEN_HEIGHT / 2.)
#define SCREEN_WIDTH_MIN_1 (SCREEN_WIDTH - 1)
#define SCREEN_HEIGHT_MIN_1 (SCREEN_HEIGHT - 1)
#define FOCAL_LENGTH 250
#define NEAR_PLANE (1. / 128)
#define TRANSLATION_DELTA 16.
#define ROTATION_DELTA 0.5
#define CAMERA_SENSITIVITY (1. / 1024)
#define TRANSLATION_DELTA_NEG (-TRANSLATION_DELTA)
#define ROTATION_DELTA_NEG (-ROTATION_DELTA)
#define DO_LOG 1
#define COLLISION_DISTANCE 5
#define VECTOR4_MULTIPLIER_X 0b001
#define VECTOR4_MULTIPLIER_Y 0b010
#define VECTOR4_MULTIPLIER_Z 0b100
#define PHYSICS_FRAME_RATE 60
#define WIREFRAME_THICKNESS (1. / 32)
#define CONSTRAINT_ITERATIONS 10
#define COLOR_BLACK  0xff000000
#define COLOR_RED    0xffff0000
#define COLOR_GREEN  0xff00ff00
#define COLOR_BLUE   0xff0000ff
#define COLOR_YELLOW 0xffffff00
#define COLOR_PURPLE 0xffff00ff
#define COLOR_CYAN   0xff00ffff
#define COLOR_WHITE  0xffffffff
#define COLOR_RAND_PER_FRAME 0x00000000 // rand every frame
#define COLOR_RAND_PER_SETUP 0x01000000 // rand color on setup
#define COLOR_RAND_PER_TRI   0x02000000 // rand color per triangle
#define COLOR_RAND_PER_OBJ   0x04000000 // same rand for entire obj
#define AABB_RESERVED 0xffffffff // TODO make this depend on sizeof size_a
#define FRAMES_BETWEEN_REFITS 5
#define AABB_VELOCITY_MULTIPLIER (FRAMES_BETWEEN_REFITS * 2)
#define KEY_W 0b00000001
#define KEY_A 0b00000010
#define KEY_S 0b00000100
#define KEY_D 0b00001000
#define KEY_Q 0b00010000
#define KEY_E 0b00100000
#define KEY_SPACE 0b01000000
#define KEY_SHIFT 0b10000000

typedef unsigned char u_char;
typedef unsigned int u_int;

typedef unsigned int size_a;
typedef size_a Vector3Index;
typedef size_a Vector4Index;
typedef size_a TriangleIndex;
typedef size_a AABB_Tree_Index;
typedef size_a AABB_Node_index;

typedef u_int Color;

u_char SUICIDE_FLAG; // something something global state bad

struct TriangleData { // TODO remove
    Vector3Index p0;
    Vector3Index p1;
    Vector3Index p2;
    Color c;
};

struct Vector3Data {
    double x;
    double y;
    double z;
    size_a index;
};

void Log(const char *msg, char size);

void LogInit();

void LogDestroy();

unsigned int Random();

void MemCpy(void *dest, const void *src, size_a bytes);

int MinInt(int a, int b);

int MaxInt(int a, int b);

double MinDouble(double a, double b);

double MaxDouble(double a, double b);

int Ceil(double a);

int Floor(double a);

void MemSet(void *dest, u_char val, u_int bytes);

double Abs(double a);

unsigned int Pow(u_int a, u_int b);

u_int MaxUInt(u_int a, u_int b);

u_int MinUInt(u_int a, u_int b);

extern float InvSqrt(float a); // casting to float is way faster lmao

#endif //CPUPHYSICSENGINE_CONSTANTS_H