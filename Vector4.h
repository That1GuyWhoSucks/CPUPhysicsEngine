#ifndef CPUPHYSICSENGINE_VECTOR4_H
#define CPUPHYSICSENGINE_VECTOR4_H

double *vector4W;
double *vector4X;
double *vector4Y;
double *vector4Z;
double camera_const;

unsigned int vector4_cap;
unsigned int vector4_current;

struct Vector4Out {
    double *out_W, *out_X, *out_Y, *out_Z;
};

void Vector4Init();

void Vector4Destroy();

unsigned int Vector4(double w, double x, double y, double z);

void Vector4Multiplier(unsigned int a, unsigned int b, struct Vector4Out *out);

double Vector4CalculateFunnyConst(unsigned int b);

#endif //CPUPHYSICSENGINE_VECTOR4_H