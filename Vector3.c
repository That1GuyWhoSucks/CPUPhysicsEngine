#include "Vector3.h"
#include "Constants.h"
#include "ExpandableArray.h"

void Vector3Init() {
    vector3_cur = 0;
    vector3_cap = 4 * 100 + vector3_start;
    // 3 direction (x,y,z) * 3 types (primary, secondary, velocity) + mass
    vector3X_primary = ArrayInit(vector3_cap, 10, sizeof(double), 32);
    vector3X_secondary = vector3X_primary + vector3_cap;
    vector3X_velocity = vector3X_secondary + vector3_cap;
    vector3Y_primary = vector3X_velocity + vector3_cap;
    vector3Y_secondary = vector3Y_primary + vector3_cap;
    vector3Y_velocity = vector3Y_secondary + vector3_cap;
    vector3Z_primary = vector3Y_velocity + vector3_cap;
    vector3Z_secondary = vector3Z_primary + vector3_cap;
    vector3Z_velocity = vector3Z_secondary + vector3_cap;
    vector3Mass = vector3Z_velocity + vector3_cap;
    Vector3(0,0,0,0,0,0,0);

}

void Vector3Destroy() {
    ArrayDestroy(vector3X_primary);
}

Vector3Index Vector3(const double x, const double y, const double z, const double x_vec, const double y_vec, const double z_vec, const double mass) {
    const Vector3Index ret = vector3_cur;

    if (vector3_cur == vector3_cap) {
        // TODO not this
        ArrayResize((void **) &vector3X_primary, &vector3_cap, 2, 10, sizeof(double), 32);
        vector3X_secondary = vector3X_primary + vector3_cap;
        vector3X_velocity = vector3X_secondary + vector3_cap;
        vector3Y_primary = vector3X_velocity + vector3_cap;
        vector3Y_secondary = vector3Y_primary + vector3_cap;
        vector3Y_velocity = vector3Y_secondary + vector3_cap;
        vector3Z_primary = vector3Y_velocity + vector3_cap;
        vector3Z_secondary = vector3Z_primary + vector3_cap;
        vector3Z_velocity = vector3Z_secondary + vector3_cap;
        vector3Mass = vector3Z_velocity + vector3_cap;
    }
    vector3_cur++;
    vector3X_primary[ret] = x;
    vector3Y_primary[ret] = y;
    vector3Z_primary[ret] = z;
    vector3X_velocity[ret] = x_vec;
    vector3Y_velocity[ret] = y_vec;
    vector3Z_velocity[ret] = z_vec;
    vector3Mass[ret] = mass;
    return ret;
}

// void Vector3Subtract1(const Vector3Index a, const Vector3Index b, const Vector3Index storage) {
//     vector3X_primary[storage] = vector3X_primary[a] - vector3X_primary[b];
//     vector3Y_primary[storage] = vector3Y_primary[a] - vector3Y_primary[b];
//     vector3Z_primary[storage] = vector3Z_primary[a] - vector3Z_primary[b];
// }
//
// void Vector3Add1(const Vector3Index a, const Vector3Index b, const Vector3Index storage) {
//     vector3X_primary[storage] = vector3X_primary[a] + vector3X_primary[b];
//     vector3Y_primary[storage] = vector3Y_primary[a] + vector3Y_primary[b];
//     vector3Z_primary[storage] = vector3Z_primary[a] + vector3Z_primary[b];
// }
//
// void Vector3DivideScalar1(const Vector3Index a, const double b, const Vector3Index storage) {
//     vector3X_primary[storage] = vector3X_primary[a] / b;
//     vector3Y_primary[storage] = vector3Y_primary[a] / b;
//     vector3Z_primary[storage] = vector3Z_primary[a] / b;
// }
//
// void Vector3CrossProduct1(const Vector3Index a, const Vector3Index b, const Vector3Index storage) {
//     vector3X_primary[storage] = vector3Y_primary[a] * vector3Z_primary[b] - vector3Z_primary[a] * vector3Y_primary[b];
//     vector3Y_primary[storage] = vector3Z_primary[a] * vector3X_primary[b] - vector3X_primary[a] * vector3Z_primary[b];
//     vector3Z_primary[storage] = vector3X_primary[a] * vector3Y_primary[b] - vector3Y_primary[a] * vector3X_primary[b];
// }
//
// double Vector3Dot1(const Vector3Index a, const Vector3Index b) {
//     return vector3X_primary[a] * vector3X_primary[b] + vector3Y_primary[a] * vector3Y_primary[b] + vector3Z_primary[a] * vector3Z_primary[b];
// }
//
// void Vector3Normalize1(const Vector3Index a, const Vector3Index storage) {
//     const double length = Vector3Length1(a);
//     vector3X_primary[storage] = vector3X_primary[a] / length;
//     vector3Y_primary[storage] = vector3Y_primary[a] / length;
//     vector3Z_primary[storage] = vector3Z_primary[a] / length;
// }
//
// double Vector3Length1(const Vector3Index a) {
//     return sqrt(vector3X_primary[a] * vector3X_primary[a] + vector3Y_primary[a] * vector3Y_primary[a] + vector3Z_primary[a] * vector3Z_primary[a]);
// }
//
// // https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html#text=_mm_prefetch&ig_expand=5152,5153
// // http://const.me/articles/simd/simd.pdf
//
// void Vector3CrossProduct4(const Vector3Index a, const Vector3Index b, const Vector3Index storage) {
//     const __m256d ax = _mm256_load_pd(&vector3X_primary[a]);
//     const __m256d ay = _mm256_load_pd(&vector3Y_primary[a]);
//     const __m256d az = _mm256_load_pd(&vector3Z_primary[a]);
//
//     const __m256d bx = _mm256_load_pd(&vector3X_primary[b]);
//     const __m256d by = _mm256_load_pd(&vector3Y_primary[b]);
//     const __m256d bz = _mm256_load_pd(&vector3Z_primary[b]);
//
//     // ay * bz - az * by
//     // az * bx - ax * bz
//     // ax * by - ay * bx
//
//     __m256d cx = _mm256_mul_pd(ay, bz);
//     __m256d cy = _mm256_mul_pd(az, bx);
//     __m256d cz = _mm256_mul_pd(ax, by);
//
//     cx = _mm256_fnmadd_pd(az, by, cx);
//     cy = _mm256_fnmadd_pd(ax, bz, cy);
//     cz = _mm256_fnmadd_pd(ay, bx, cz);
//
//     _mm256_store_pd(&vector3X_primary[storage], cx);
//     _mm256_store_pd(&vector3Y_primary[storage], cy);
//     _mm256_store_pd(&vector3Z_primary[storage], cz);
// }
//
// void Vector3RotateBy4(const Vector3Index a, const Vector4Index b, const Vector3Index storage) {
//     // rotates 4 vector3 (a) by 1 quat (b)
//     // formula yoinked from some guy whom I blindly trust https://blog.molecular-matters.com/2013/05/24/a-faster-quaternion-vector-multiplication/
//     // p = position, v is vector part of quat, w is scalar part
//     // p’ = (v*v.dot(p) + v.cross(p)*(w))*2 + p*(w*w – v.dot(v))
//
//     const __m256d ax = _mm256_load_pd(&vector3X_primary[a]);
//     const __m256d ay = _mm256_load_pd(&vector3Y_primary[a]);
//     const __m256d az = _mm256_load_pd(&vector3Z_primary[a]);
//
//     __m256d bw = _mm256_set1_pd(vector4W[b]);
//     const __m256d bx = _mm256_set1_pd(vector4X[b]);
//     const __m256d by = _mm256_set1_pd(vector4Y[b]);
//     const __m256d bz = _mm256_set1_pd(vector4Z[b]);
//
//     // v.dot(p)
//     __m256d dot_vp = _mm256_mul_pd(ax, bx);
//     dot_vp = _mm256_fmadd_pd(ay, by, dot_vp);
//     dot_vp = _mm256_fmadd_pd(az, bz, dot_vp);
//
//     // v*v.dot(p)
//     __m256d cx = _mm256_mul_pd(bx, dot_vp);
//     __m256d cy = _mm256_mul_pd(by, dot_vp);
//     __m256d cz = _mm256_mul_pd(bz, dot_vp);
//
//     // v.cross(p)
//     dot_vp = _mm256_mul_pd(by, az);
//     dot_vp = _mm256_fnmadd_pd(bz, ay, dot_vp);
//     cx = _mm256_fmadd_pd(dot_vp, bw, cx);
//
//     dot_vp = _mm256_mul_pd(bz, ax);
//     dot_vp = _mm256_fnmadd_pd(bx, az, dot_vp);
//     cy = _mm256_fmadd_pd(dot_vp, bw, cy);
//
//     dot_vp = _mm256_mul_pd(bx, ay);
//     dot_vp = _mm256_fnmadd_pd(by, ax, dot_vp);
//     cz = _mm256_fmadd_pd(dot_vp, bw, cz);
//
//     // v*v.dot(p) + v.cross(p)*(w)
//
//     // (v*v.dot(p) + v.cross(p)*(w))*2
//     bw = _mm256_set1_pd(2.0);
//     cx = _mm256_mul_pd(cx, bw);
//     cy = _mm256_mul_pd(cy, bw);
//     cz = _mm256_mul_pd(cz, bw);
//
//     bw = _mm256_set1_pd(camera_const); // the funny const
//     // (v*v.dot(p) + v.cross(p)*(w))*2 + p*(w*w – v.dot(v))
//     cx = _mm256_fmadd_pd(ax, bw, cx);
//     cy = _mm256_fmadd_pd(ay, bw, cy);
//     cz = _mm256_fmadd_pd(az, bw, cz);
//
//     _mm256_store_pd(&vector3X_primary[storage], cx);
//     _mm256_store_pd(&vector3Y_primary[storage], cy);
//     _mm256_store_pd(&vector3Z_primary[storage], cz);
// }
//
// void Vector3Midpoint4(const Vector3Index a, const Vector3Index b, const Vector3Index storage) {
//     const __m256d ax = _mm256_load_pd(&vector3X_primary[a]);
//     const __m256d ay = _mm256_load_pd(&vector3Y_primary[a]);
//     const __m256d az = _mm256_load_pd(&vector3Z_primary[a]);
//
//     const __m256d scalar = _mm256_set1_pd(0.5);
//
//     __m256d bx = _mm256_load_pd(&vector3X_primary[b]);
//     __m256d by = _mm256_load_pd(&vector3Y_primary[b]);
//     __m256d bz = _mm256_load_pd(&vector3Z_primary[b]);
//
//     bx = _mm256_add_pd(ax, bx);
//     by = _mm256_add_pd(ay, by);
//     bz = _mm256_add_pd(az, bz);
//
//     bx = _mm256_mul_pd(bx, scalar);
//     by = _mm256_mul_pd(by, scalar);
//     bz = _mm256_mul_pd(bz, scalar);
//
//     _mm256_store_pd(&vector3X_primary[storage], bx);
//     _mm256_store_pd(&vector3Y_primary[storage], by);
//     _mm256_store_pd(&vector3Z_primary[storage], bz);
// }
//
// void Vector3Distance4(const Vector3Index a, const Vector3Index b, const Vector3Index storage) {
//     const __m256d ax = _mm256_load_pd(&vector3X_primary[a]);
//     const __m256d ay = _mm256_load_pd(&vector3Y_primary[a]);
//     __m256d az = _mm256_load_pd(&vector3Z_primary[a]);
//
//     __m256d bx = _mm256_load_pd(&vector3X_primary[b]);
//     __m256d by = _mm256_load_pd(&vector3Y_primary[b]);
//     __m256d bz = _mm256_load_pd(&vector3Z_primary[b]);
//
//     bx = _mm256_sub_pd(bx, ax);
//     by = _mm256_sub_pd(by, ay);
//     bz = _mm256_sub_pd(bz, az);
//
//     az = _mm256_mul_pd(bx, bx);
//     az = _mm256_fmadd_pd(by, by, az);
//     az = _mm256_fmadd_pd(bz, bz, az);
//     az = _mm256_invsqrt_pd(az);
//
//     bx = _mm256_mul_pd(bx, az);
//     by = _mm256_mul_pd(by, az);
//     bz = _mm256_mul_pd(bz, az);
//
//     _mm256_store_pd(&vector3X_primary[storage], bx);
//     _mm256_store_pd(&vector3Y_primary[storage], by);
//     _mm256_store_pd(&vector3Z_primary[storage], bz);
// }

