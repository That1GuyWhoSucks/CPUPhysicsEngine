#include "AABB.h"
#include "Constants.h"
#include "Triangle.h"
#include "Vector3.h"
#include "ExpandableArray.h"
#include "main.h"

// I hate collision detection
// Total rewrites: 3, soon to be 4

// reference material https://box2d.org/files/ErinCatto_DynamicBVH_GDC2019.pdf
void AABBInit() {
    // aabb_trees_cap = 2;
    // aabb_trees_cur = 0;
    // aabb_trees = ArrayInit(aabb_trees_cap, 1, sizeof(struct AABBTree), 8);
}

void AABBDestroy() {
    // for (int i=0; i<aabb_trees_cur; i++) {
    //     ArrayDestroy(aabb_trees[i].tree);
    //     ArrayDestroy(aabb_trees[i].flat);
    // }
    // ArrayDestroy(aabb_trees);
}

AABB_Tree_Index AABBRegisterNewTree() {
    // if (aabb_trees_cur == aabb_trees_cap) {
    //     ArrayResize((void **) &aabb_trees, &aabb_trees_cap, 2, 1, sizeof(struct AABBTree), 8);
    // }
    // aabb_trees[aabb_trees_cur] = (struct AABBTree) {
    //     NULL,
    //     100,
    //     0,
    //     NULL,
    //     255,
    //     0
    // };
    // aabb_trees[aabb_trees_cur].flat = ArrayInit(100, 2, sizeof(TriangleIndex), 8);
    // aabb_trees[aabb_trees_cur].tree = ArrayInit(255, 1, sizeof(struct AABBNodeData), 8);
    //
    // return aabb_trees_cur++;
}

void AABBRegister(AABB_Tree_Index a, TriangleIndex b) {
    // struct AABBTree tree = aabb_trees[a];
    // if (tree.flat_cur == tree.flat_cap) {
    //     ArrayResize((void **) &tree.flat, &tree.flat_cap, 2, 2, sizeof(TriangleIndex), 8);
    // }
    // if (tree.tree_cur > tree.tree_cap - 2) {
    //     ArrayResize((void **) &tree.tree, &tree.tree_cap, 2, 1, sizeof(struct AABBNodeData), 8);
    // }


}

// void AABBRegister(const TriangleIndex a) {
//     if (aabb_tree_cur > aabb_tree_cap - 2) { // if tree could overflow
//         ArrayResize((void **) &aabb_tree_data, &aabb_tree_cap, 2, 1, sizeof(struct AABBData), 32);
//     }
//     if (aabb_flat_cur == aabb_flat_cap) {
//         ArrayResize((void **) &aabb_flat_data, &aabb_flat_cap, 2, 2, sizeof(TriangleIndex), 32);
//     }
//     struct AABBData data = CreateAABBDataFromTriangle(a);
//     const AABBIndex new_triangle_index = aabb_tree_cur;
//     aabb_flat_data[aabb_flat_cur] = a;
//     aabb_flat_cur++;
//     aabb_tree_cur++;
//     if (aabb_flat_cur == 1) {
//         // place in
//         data.parent = 0;
//         aabb_tree_data[1] = data;
//         aabb_tree_data[0] = CreateAABBDataFromIndices(1, 1, AABB_RESERVED); // create parent node at root
//         aabb_tree_cur++;
//     } else if (aabb_flat_cur == 2) {
//         // if two then move middle node to be at position 0
//         data.parent = 0;
//         aabb_tree_data[2] = data;
//         aabb_tree_data[0] = CreateAABBDataFromIndices(1, 2, AABB_RESERVED); // update root node
//     } else {
//         aabb_tree_data[new_triangle_index] = data;
//         AABBIndex cur_node = 0;
//         while (1) {
//             const struct AABBData cur_data = aabb_tree_data[cur_node];
//             const unsigned int left_cost = Cost(cur_data.left, new_triangle_index);
//             const unsigned int right_cost = Cost(cur_data.right, new_triangle_index);
//             if (right_cost == AABB_RESERVED || left_cost == AABB_RESERVED) { // if left or right nodes empty, insert new parent to current node and new node
//                 aabb_tree_data[aabb_tree_cur] = CreateAABBDataFromIndices(new_triangle_index, cur_node, cur_data.parent);
//                 aabb_tree_data[new_triangle_index].parent = aabb_tree_cur;
//                 aabb_tree_data[cur_node].parent = aabb_tree_cur;
//                 struct AABBData grandparent = aabb_tree_data[cur_data.parent];
//                 if (grandparent.left == cur_node) {
//                     grandparent.left = aabb_tree_cur;
//                 } else {
//                     grandparent.right = aabb_tree_cur;
//                 }
//                 aabb_tree_cur++;
//                 break;
//             }
//             if (right_cost < left_cost) {
//                 cur_node = cur_data.right; // keep exploring right
//             } else {
//                 cur_node = cur_data.left; // keep exploring left
//             }
//         }
//     }
// }

// unsigned int Cost(const AABBIndex old_node, const AABBIndex new_node) {
//     if (old_node == AABB_RESERVED) {
//         return AABB_RESERVED;
//     }
//     const struct AABBData a_data = aabb_tree_data[old_node];
//     const struct AABBData b_data = aabb_tree_data[new_node];
//     int x = a_data.max_x - a_data.min_x;
//     int y = a_data.max_y - a_data.min_y;
//     int z = a_data.max_z - a_data.min_z;
//     const unsigned int cost = x * y * 2 + x * z * 2 + y * z * 2;
//     x = MaxInt(a_data.max_x, b_data.max_x) - MinInt(a_data.min_x, b_data.min_x);
//     y = MaxInt(a_data.max_y, b_data.max_y) - MinInt(a_data.min_y, b_data.min_y);
//     z = MaxInt(a_data.max_z, b_data.max_z) - MinInt(a_data.min_z, b_data.min_z);
//     return x * y * 2 + x * z * 2 + y * z * 2 - cost;
// }
//
// struct AABBData CreateAABBDataFromTriangle(const TriangleIndex a) {
//     const Vector3Index p0 = triangle_triangles[a].p0;
//     const Vector3Index p1 = triangle_triangles[a].p1;
//     const Vector3Index p2 = triangle_triangles[a].p2;
//     double p0_v = Abs(vector3X_velocity[p0]) * AABB_VELOCITY_MULTIPLIER;
//     double p1_v = Abs(vector3X_velocity[p1]) * AABB_VELOCITY_MULTIPLIER;
//     double p2_v = Abs(vector3X_velocity[p2]) * AABB_VELOCITY_MULTIPLIER;
//     const int max_X = Ceil(MaxDouble(
//         vector3X_primary[p0] + p0_v,
//         MaxDouble(
//             vector3X_primary[p1] + p1_v,
//             vector3X_primary[p2] + p2_v
//         )
//     ));
//     const int min_X = Floor(MinDouble(
//     vector3X_primary[p0] - p0_v,
//     MinDouble(
//         vector3X_primary[p1] - p1_v,
//         vector3X_primary[p2] - p2_v
//         )
//     ));
//
//     p0_v = Abs(vector3Y_velocity[p0]) * AABB_VELOCITY_MULTIPLIER;
//     p1_v = Abs(vector3Y_velocity[p1]) * AABB_VELOCITY_MULTIPLIER;
//     p2_v = Abs(vector3Y_velocity[p2]) * AABB_VELOCITY_MULTIPLIER;
//     const int max_Y = Ceil(MaxDouble(
//         vector3Y_primary[p0] + p0_v,
//         MaxDouble(
//             vector3Y_primary[p1] + p1_v,
//             vector3Y_primary[p2] + p2_v
//         )
//     ));
//     const int min_Y = Floor(MinDouble(
//     vector3Y_primary[p0] - p0_v,
//     MinDouble(
//         vector3Y_primary[p1] - p1_v,
//         vector3Y_primary[p2] - p2_v
//         )
//     ));
//
//     p0_v = Abs(vector3Z_velocity[p0]) * AABB_VELOCITY_MULTIPLIER;
//     p1_v = Abs(vector3Z_velocity[p1]) * AABB_VELOCITY_MULTIPLIER;
//     p2_v = Abs(vector3Z_velocity[p2]) * AABB_VELOCITY_MULTIPLIER;
//     const int max_Z = Ceil(MaxDouble(
//         vector3Z_primary[p0] + p0_v,
//         MaxDouble(
//             vector3Z_primary[p1] + p1_v,
//             vector3Z_primary[p2] + p2_v
//         )
//     ));
//     const int min_Z = Floor(MinDouble(
//     vector3Z_primary[p0] - p0_v,
//     MinDouble(
//         vector3Z_primary[p1] - p1_v,
//         vector3Z_primary[p2] - p2_v
//         )
//     ));
//     const struct AABBData ret = {max_X, max_Y, max_Z, min_X, min_Y, min_Z, AABB_RESERVED, AABB_RESERVED, AABB_RESERVED, a};
//     return ret;
// }
//
// struct AABBData CreateAABBDataFromIndices(const AABBIndex a, const AABBIndex b, const AABBIndex parent) {
//     const struct AABBData data_a = aabb_tree_data[a];
//     const struct AABBData data_b = aabb_tree_data[b];
//     const int max_x = data_a.max_x > data_b.max_x ? data_a.max_x : data_b.max_x;
//     const int min_x = data_a.min_x < data_b.min_x ? data_a.min_x : data_b.min_x;
//
//     const int max_y = data_a.max_y > data_b.max_y ? data_a.max_y : data_b.max_y;
//     const int min_y = data_a.min_y < data_b.min_y ? data_a.min_y : data_b.min_y;
//
//     const int max_z = data_a.max_z > data_b.max_z ? data_a.max_z : data_b.max_z;
//     const int min_z = data_a.min_z < data_b.min_z ? data_a.min_z : data_b.min_z;
//
//     const struct AABBData ret = {max_x, max_y, max_z, min_x, min_y, min_z,a,b, parent, AABB_RESERVED};
//     return ret;
// }
//
// void AABBTreeRefitData(const AABBIndex a) {
//     const struct AABBData data = aabb_tree_data[a];
//     aabb_tree_data[a] = CreateAABBDataFromIndices(data.left, data.right, a);
//     if (data.parent == AABB_RESERVED) {
//         return;
//     }
//     AABBTreeRefitData(data.parent);
// }
//

//
// TriangleIndex AABBCollideRecursive(const struct Vector3Data a, const AABBIndex b) {
//     const struct AABBData data = aabb_tree_data[b];
//     if (
//         a.x < data.min_x ||
//         a.x > data.max_x ||
//         a.y < data.min_y ||
//         a.y > data.max_y ||
//         a.z < data.min_z ||
//         a.z > data.max_z
//     ) { // a not in bounds of b
//         return AABB_RESERVED;
//     }
//     if (data.triangle != AABB_RESERVED) { // b is a leaf
//         // since b is leaf must now do expensive calculation
//         const Vector3Index p0 = triangle_triangles[data.triangle].p0;
//         const Vector3Index p1 = triangle_triangles[data.triangle].p1;
//         const Vector3Index p2 = triangle_triangles[data.triangle].p2;
//
//         if (p0 == a.index || p1 == a.index || p2 == a.index) return AABB_RESERVED;
//
//         const double p0x = vector3X_secondary[p0];
//         const double p1x = vector3X_secondary[p1];
//         const double p2x = vector3X_secondary[p2];
//         const double p0y = vector3Y_secondary[p0];
//         const double p1y = vector3Y_secondary[p1];
//         const double p2y = vector3Y_secondary[p2];
//         const double p0z = vector3Z_secondary[p0];
//         const double p1z = vector3Z_secondary[p1];
//         const double p2z = vector3Z_secondary[p2];
//
//         const double cx1 = p1x - p0x;
//         double cx2 = p2x - p0x;
//         const double cy1 = p1y - p0y;
//         double cy2 = p2y - p0y;
//         const double cz1 = p1z - p0z;
//         double cz2 = p2z - p0z;
//
//         double cx3 = cy1 * cz2 - cz1 * cy2;
//         double cy3 = cz1 * cx2 - cx1 * cz2;
//         double cz3 = cx1 * cy2 - cy1 * cx2;
//         cx2 = InvSqrt(cx3*cx3 + cy3*cy3 + cz3*cz3);
//
//         cx3 *= cx2;
//         cy3 *= cx2;
//         cz3 *= cx2;
//         // c3 has normalized normal
//         cx2 = a.x - (p0x + p1x + p2x) / 3;
//         cy2 = a.y - (p0y + p1y + p2y) / 3;
//         cz2 = a.z - (p0z + p1z + p2z) / 3;
//         // c2 has target - midpoint
//         double dist = cx3 * cx2 + cy3 * cy2 + cz3 * cz2;
//         // dist is dot product
//         if (dist >= -0.01 || dist < -COLLISION_DISTANCE) return AABB_RESERVED;
//         // if behind and within collision dist then correct
//         const double p0m = 1 / vector3Mass[p0];
//         const double p1m = 1 / vector3Mass[p1];
//         const double p2m = 1 / vector3Mass[p2];
//         const double p3m = 1 / vector3Mass[a.index];
//         const double factor = dist / (p0m + p1m + p2m + p3m);
//         cx2 = cx3 * factor;
//         cy2 = cy3 * factor;
//         cz2 = cz3 * factor;
//
//         vector3X_secondary[p0] += cx2 * p0m;
//         vector3X_secondary[p1] += cx2 * p1m;
//         vector3X_secondary[p2] += cx2 * p2m;
//         vector3X_secondary[a.index] -= cx2 * p3m;
//
//         vector3Y_secondary[p0] += cy2 * p0m;
//         vector3Y_secondary[p1] += cy2 * p1m;
//         vector3Y_secondary[p2] += cy2 * p2m;
//         vector3Y_secondary[a.index] -= cy2 * p3m;
//
//         vector3Z_secondary[p0] += cz2 * p0m;
//         vector3Z_secondary[p1] += cz2 * p1m;
//         vector3Z_secondary[p2] += cz2 * p2m;
//         vector3Z_secondary[a.index] -= cz2 * p3m;
//         return data.triangle;
//     }
//     unsigned int res = AABB_RESERVED;
//     if (data.left != AABB_RESERVED) {
//         res = AABBCollideRecursive(a, data.left);
//     }
//     if (data.right != AABB_RESERVED && res == AABB_RESERVED) {
//         res = AABBCollideRecursive(a, data.right);
//     }
//     return res;
// }
//
// TriangleIndex AABBCollide(const Vector3Index a) {
//     const struct Vector3Data n = {vector3X_secondary[a],vector3Y_secondary[a],vector3Z_secondary[a],a};
//     return AABBCollideRecursive(n, 0);
// }
//
//
// void AABBShake() {
//     const unsigned int old_cur = aabb_flat_cur;
//     aabb_tree_cur = 0;
//     aabb_flat_cur = 0;
//     for (int i=0; i<old_cur; i++) { // clone flat data into second half of array
//         aabb_flat_data[i + aabb_flat_cap] = aabb_flat_data[i];
//     }
//     for (int i=0; i<old_cur; i++) { // read from second half to place data in as this writes to first half
//         AABBRegister(aabb_flat_data[i + aabb_flat_cap]);
//     }
// }
//
// void CollisionConstraintApply() {
//     for (int i=0; i<aabb_flat_cur; i++) {
//         AABBCollide(aabb_flat_data[i]);
//     }
// }
