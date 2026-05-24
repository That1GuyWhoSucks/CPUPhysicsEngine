#ifndef CPUPHYSICSENGINE_AABB_H
#define CPUPHYSICSENGINE_AABB_H

#include "Constants.h"
// TODO, redo again

// struct AABBNodeData {
//     int max_x;
//     int max_y;
//     int max_z;
//     int min_x;
//     int min_y;
//     int min_z;
//     AABB_Node_index left;
//     AABB_Node_index right;
//     AABB_Node_index parent;
//     TriangleIndex triangle;
// };
//
// struct AABBTree {
//     TriangleIndex *flat;
//     size_a flat_cap;
//     size_a flat_cur;
//     struct AABBNodeData *tree;
//     size_a tree_cap;
//     size_a tree_cur;
// };
//
// struct AABBTree *aabb_trees;
// size_a aabb_trees_cap;
// size_a aabb_trees_cur;

void AABBInit();

void AABBDestroy();

AABB_Tree_Index AABBRegisterNewTree();

void AABBRegister(AABB_Tree_Index a, TriangleIndex b);

TriangleIndex AABBCollide(Vector3Index a);

void AABBShake();

void CollisionConstraintApply();

#endif //CPUPHYSICSENGINE_AABB_H