#ifndef CPUPHYSICSENGINE_VECTOR3_H
#define CPUPHYSICSENGINE_VECTOR3_H

double *vector3X;
double *vector3Y;
double *vector3Z;

unsigned int vector3_cap;
unsigned int vector3_current;

struct Vector3Out {
    double *outX;
    double *outY;
    double *outZ;
};

void Vector3Init();

void Vector3Destroy();

unsigned int Vector3(double x, double y, double z);

// TODO, benchmark all these when done
void Vector3CrossProduct4(unsigned int a, unsigned int b, struct Vector3Out* out);

void Vector3RotateBy4(unsigned int a, unsigned int b, const struct Vector3Out *out);

void Vector3Midpoint4(unsigned int a, unsigned int b, struct Vector3Out* out);

void Vector3Distance4(unsigned int a, unsigned int b, struct Vector3Out* out); // 4 / 16 done :)

void Vector3Normalize(unsigned int a, struct Vector3Out* out);

void Vector3Length(unsigned int a, struct Vector3Out* out);

void Vector3LengthSquared(unsigned int a, struct Vector3Out* out);

void Vector3Add(unsigned int a, unsigned int b, struct Vector3Out* out);

void Vector3Subtract(unsigned int a, unsigned int b, struct Vector3Out* out);

void Vector3Multiply(unsigned int a, unsigned int b, struct Vector3Out* out);

void Vector3Divide(unsigned int a, unsigned int b, struct Vector3Out* out);

void Vector3AddScalar(unsigned int a, double b, struct Vector3Out* out);

void Vector3SubtractScalar(unsigned int a, double b, struct Vector3Out* out);

void Vector3MultiplyScalar(unsigned int a, unsigned int b, struct Vector3Out* out);

void Vector3DivideScalar(unsigned int a, unsigned int b, struct Vector3Out* out);

void Vector3EQ(unsigned int a, unsigned int b, struct Vector3Out* out);

#endif //CPUPHYSICSENGINE_VECTOR3_H