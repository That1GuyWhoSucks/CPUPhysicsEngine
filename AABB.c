#include "AABB.h"
#include <malloc.h>
#include "Constants.h"
#include "Vector3.h"

// https://box2d.org/files/ErinCatto_DynamicBVH_GDC2019.pdf

void AABBInit() {

}

unsigned int AABB(unsigned int *triangles, unsigned int triangle_count, unsigned int unique_vertexes) {
    if (aabb_object_current == 0) {
        // if first obj alloc memory
    } else {
        // realloc obj tree and add space for the new shit
    }

    // get obj and triangle AABBs
    double maxX = vector3X[triangles[0]];
    double maxY = vector3Y[triangles[1]];
    double maxZ = vector3Z[triangles[2]];
    double minX = maxX;
    double minY = maxY;
    double minZ = maxZ;
    struct AABB_Box obj_box = {maxX, maxY, maxZ, minX, minY, minZ};
    for (int i=0; i<triangle_count; i++) {
        // get max and min indexes for all axes, max and min for all triangles
    }

}

void AABB_AddNodeToTree(struct Node *tree, unsigned int tree_size, char flag, unsigned int data, struct AABB_Box *box) {
    // find best sibling for new node

    // create new parent

    // walk back up the tree and refit AABBs
}

void AABB_Refit() {

}
