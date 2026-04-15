#ifndef CPUPHYSICSENGINE_CONSTANTS_H
#define CPUPHYSICSENGINE_CONSTANTS_H

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define FOCAL_LENGTH 250
#define TRANSLATION_DELTA 10
#define ROTATION_DELTA 0.02
#define DO_LOG 1
#define COLLISION_DISTANCE 5

#include <immintrin.h>
#define SHUFFLE_R(a,b,c,d) _MM_SHUFFLE(d,c,b,a)

void Log(char *msg, char symb);

#endif //CPUPHYSICSENGINE_CONSTANTS_H