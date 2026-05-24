#ifndef CPUPHYSICSENGINE_TRIANGLE_H
#define CPUPHYSICSENGINE_TRIANGLE_H
// TODO, make this normal architecture dingus
struct TriangleData *triangle_triangles;
unsigned int triangle_cap;
unsigned int triangle_cur;

unsigned int Triangle(unsigned int a, unsigned int b, unsigned int c, unsigned int color);

void TriangleDestroy();

void TriangleInit();

void TriangleOrient(unsigned int triangle, unsigned int pos);

double TriangleIsBehind(unsigned int triangle, unsigned int pos);

#endif //CPUPHYSICSENGINE_TRIANGLE_H