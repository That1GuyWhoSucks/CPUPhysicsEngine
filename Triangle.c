#include "Triangle.h"
#include "ExpandableArray.h"
#include "Vector3.h"

TriangleIndex Triangle(const Vector3Index a, const Vector3Index b, const Vector3Index c, const Color color) {
    const TriangleIndex ret = triangle_cur;
    if (triangle_cur == triangle_cap) {
        ArrayResize((void **) &triangle_triangles, &triangle_cap, 2, 1, sizeof(struct TriangleData), 32);
    }
    const struct TriangleData tri = {a, b, c, color};
    triangle_triangles[ret] = tri;
    triangle_cur++;
    return ret;
}

void TriangleDestroy() {
    ArrayDestroy(triangle_triangles);
}

void TriangleInit() {
    triangle_cap = 100;
    triangle_cur = 0;
    triangle_triangles = ArrayInit(triangle_cap, 1, sizeof(struct TriangleData), 32);
}

void TriangleOrient(const TriangleIndex triangle, const Vector3Index pos) {
    if (TriangleIsBehind(triangle, pos) > 0) {
        const Vector3Index p0 = triangle_triangles[triangle].p0;
        triangle_triangles[triangle].p0 = triangle_triangles[triangle].p2;
        triangle_triangles[triangle].p2 = p0;
    }
}

double TriangleIsBehind(const unsigned int triangle, const unsigned int pos) {
    const unsigned int p0 = triangle_triangles[triangle].p0;
    const unsigned int p1 = triangle_triangles[triangle].p1;
    const unsigned int p2 = triangle_triangles[triangle].p2;

    const double p0x = vector3X_primary[p0];
    const double p1x = vector3X_primary[p1];
    const double p2x = vector3X_primary[p2];
    const double p3x = vector3X_primary[pos];
    const double p0y = vector3Y_primary[p0];
    const double p1y = vector3Y_primary[p1];
    const double p2y = vector3Y_primary[p2];
    const double p3y = vector3Y_primary[pos];
    const double p0z = vector3Z_primary[p0];
    const double p1z = vector3Z_primary[p1];
    const double p2z = vector3Z_primary[p2];
    const double p3z = vector3Z_primary[pos];

    const double cx1 = p1x - p0x;
    double cx2 = p2x - p0x;
    const double cy1 = p1y - p0y;
    double cy2 = p2y - p0y;
    const double cz1 = p1z - p0z;
    double cz2 = p2z - p0z;

    double cx3 = cy1 * cz2 - cz1 * cy2;
    double cy3 = cz1 * cx2 - cx1 * cz2;
    double cz3 = cx1 * cy2 - cy1 * cx2;
    cx2 = InvSqrt(cx3*cx3 + cy3*cy3 + cz3*cz3);

    cx3 *= cx2;
    cy3 *= cx2;
    cz3 *= cx2;
    // c3 has normalized normal
    cx2 = p3x - (p0x + p1x + p2x) / 3;
    cy2 = p3y - (p0y + p1y + p2y) / 3;
    cz2 = p3z - (p0z + p1z + p2z) / 3;
    // c2 has target - midpoint
    return cx3 * cx2 + cy3 * cy2 + cz3 * cz2;
    // return dot
}
