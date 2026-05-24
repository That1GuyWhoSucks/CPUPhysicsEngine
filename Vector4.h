#ifndef CPUPHYSICSENGINE_VECTOR4_H
#define CPUPHYSICSENGINE_VECTOR4_H

// TODO currently vec4 is just camera stuff since softbodies don't really have a rotation.
// move this stuff to camera manager and have vec4 for rigidbodies

double *vector4W;
double *vector4X;
double *vector4Y;
double *vector4Z;

unsigned int vector4_cap;
unsigned int vector4_cur;

double camera_const;
static const unsigned int vector4_cam_rot_index = 0;
static const unsigned int vector4_cam_rot_conj_index = 1;

void Vector4Init();

void Vector4Destroy();

unsigned int Vector4(double w, double x, double y, double z);

void Vector4Multiplier(double x, double y, double z);

void Vector4TranslateCamera(double px, double py, double pz);

#endif //CPUPHYSICSENGINE_VECTOR4_H