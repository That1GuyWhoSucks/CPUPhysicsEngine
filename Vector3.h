#ifndef CPUPHYSICSENGINE_VECTOR3_H
#define CPUPHYSICSENGINE_VECTOR3_H

#include "Constants.h"

double *vector3X_primary;
double *vector3X_secondary;
double *vector3X_velocity;
double *vector3Y_primary;
double *vector3Y_secondary;
double *vector3Y_velocity;
double *vector3Z_primary;
double *vector3Z_secondary;
double *vector3Z_velocity;

double *vector3Mass;

static const Vector3Index vector3_camera_pos_index = 0;

static const Vector3Index vector3_start = 1;

unsigned int vector3_cap; // if starting at vector3_start and incrementing by 4 it must always end nicely to prevent SO's
unsigned int vector3_cur;

void Vector3Init();

void Vector3Destroy();

Vector3Index Vector3(double x, double y, double z, double x_vec, double y_vec, double z_vec, double mass);

// void Vector3Subtract1(Vector3Index a, Vector3Index b, Vector3Index storage);
//
// void Vector3Add1(Vector3Index a, Vector3Index b, Vector3Index storage);
//
// void Vector3DivideScalar1(Vector3Index a, double b, Vector3Index storage);
//
// void Vector3CrossProduct1(Vector3Index a, Vector3Index b, Vector3Index storage);
//
// double Vector3Dot1(Vector3Index a, Vector3Index b);
//
// void Vector3Normalize1(Vector3Index a, Vector3Index storage);
//
// double Vector3Length1(Vector3Index a);

// void Vector3CrossProduct4(unsigned int a, unsigned int b, unsigned int storage);
//
// void Vector3RotateBy4(unsigned int a, unsigned int b, unsigned int storage);
//
// void Vector3Midpoint4(unsigned int a, unsigned int b, unsigned int storage);
//
// void Vector3Distance4(unsigned int a, unsigned int b, unsigned int storage); // 4 / 16 done :)
//
// void Vector3Normalize4(unsigned int a, unsigned int storage);
//
// void Vector3Length4(unsigned int a, unsigned int storage);
//
// void Vector3LengthSquared4(unsigned int a, unsigned int storage);
//
// void Vector3Add4(unsigned int a, unsigned int b, unsigned int storage);
//
// void Vector3Subtract4(unsigned int a, unsigned int b, unsigned int storage);
//
// void Vector3Multiply4(unsigned int a, unsigned int b, unsigned int storage);
//
// void Vector3Divide4(unsigned int a, unsigned int b, unsigned int storage);
//
// void Vector3AddScalar4(unsigned int a, double b, unsigned int storage);
//
// void Vector3SubtractScalar4(unsigned int a, double b, unsigned int storage);
//
// void Vector3MultiplyScalar4(unsigned int a, unsigned int b, unsigned int storage);
//
// void Vector3DivideScalar4(unsigned int a, unsigned int b, unsigned int storage);
//
// void Vector3EQ4(unsigned int a, unsigned int b, unsigned int storage);

#endif //CPUPHYSICSENGINE_VECTOR3_H