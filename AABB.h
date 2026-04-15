#ifndef CPUPHYSICSENGINE_AABB_H
#define CPUPHYSICSENGINE_AABB_H
// TODO, this section is heavily WiP, I just really dont wanna work on it
struct AABB_Box {
    // vector3 index with all max/min vals for whatever box
    unsigned int min;
    unsigned int max;
};

// TODO, ughhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhhh
// this needs a rework, but so does this whole thing
struct Node {
    struct AABB_Box box;
    unsigned int parent;
    unsigned int left;
    unsigned int right;
    char flag; // either 0 for no data, 1 for triangle AABB, or 2 for triangle index
    unsigned int data; // either the actual triangle index, or the index of the triangle AABB depending on flag
};
unsigned int aabb_triangle_current; // current number of triangle AABB that exist
unsigned int aabb_object_current;
struct Node *aabb_objects; // object AABB tree
struct Node **aabb_triangles; // list of pointers to triangle AABB trees

// register an obj (array of triangles)
// add it to the obj collision AABB tree, which has a pointer to the triangle AAABB tree
// triangle AABB is same thing but for triangles, separate tree per object
// create new triangle AABB tree and add all triangles to it
// need to do a bunch of funny math with triangles to get their AABB since it extends behind them
// probably worth to precompute but it needs to be recalculated a shit ton if so
unsigned int AABB(unsigned int *triangles, unsigned int triangle_count);

void AABBInit();

#endif //CPUPHYSICSENGINE_AABB_H