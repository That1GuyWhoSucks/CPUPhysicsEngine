#ifndef CPUPHYSICSENGINE_TRIANGLE_H
#define CPUPHYSICSENGINE_TRIANGLE_H

unsigned int *triangles; // [p1,p2,p3][]...
unsigned int triangle_cap; // max amount of triangles can currently hold
unsigned int triangle_current; // current triangle count
// figure out if worth precomputing
//unsigned int *triangle_normal; // array of indexes to vector3 X/Y/Z that hold the normal vector for the given triangle

unsigned int Triangle(unsigned int a, unsigned int b, unsigned int c);

void TriangleInit();

void TriangleOrient(unsigned int triangle, double center[3]);

void TriangleCalcNormal(unsigned int triangle);

#endif //CPUPHYSICSENGINE_TRIANGLE_H