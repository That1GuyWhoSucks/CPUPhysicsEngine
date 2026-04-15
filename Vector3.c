#include <immintrin.h>
#include "Vector3.h"
#include "Vector4.h"
#include <malloc.h>

void Vector3Init() {
    // TODO, either make this smart or just increase starting cap, spend way too much time on startup just reallocing
    vector3_cap = 100;
    vector3_current = 0;
    vector3X = _aligned_malloc(vector3_cap * 3 * sizeof(double), 32);
    vector3Y = vector3X + vector3_cap;
    vector3Z = vector3Y + vector3_cap;
}

void Vector3Destroy() {
    _aligned_free(vector3X);
}

// NOTE, because I'm lazy this cannot "unallocate" a vector. So don't forget to not use for temp values
// Or just make it actually able to do that
unsigned int Vector3(double x, double y, double z) {
    if (vector3_current == vector3_cap) {
        vector3_cap *= 2;

        vector3X = _aligned_realloc(vector3X, vector3_cap * 3 * sizeof(double), 32);
        vector3Y = vector3X + vector3_cap;
        vector3Z = vector3Y + vector3_cap;
    }

    const unsigned int ret = vector3_current;
    vector3X[ret] = x;
    vector3Y[ret] = y;
    vector3Z[ret] = z;

    vector3_current++;
    return ret;
}
// TODO, all current funcs assume A is an index to vector X/Y/Z but that cannot store temps
// Either make it able to store temps or allow it to just be a random pointer
void Vector3CrossProduct4(unsigned int a, unsigned int b, struct Vector3Out *out) {
    const __m256d ax = _mm256_load_pd(&vector3X[a]);
    const __m256d ay = _mm256_load_pd(&vector3Y[a]);
    const __m256d az = _mm256_load_pd(&vector3Z[a]);

    const __m256d bx = _mm256_load_pd(&vector3X[b]);
    const __m256d by = _mm256_load_pd(&vector3Y[b]);
    const __m256d bz = _mm256_load_pd(&vector3Z[b]);

    // ay * bz - az * by
    // az * bx - ax * bz
    // ax * by - ay * bx

    __m256d cx = _mm256_mul_pd(ay, bz);
    __m256d cy = _mm256_mul_pd(az, bx);
    __m256d cz = _mm256_mul_pd(ax, by);

    cx = _mm256_fnmadd_pd(az, by, cx);
    cy = _mm256_fnmadd_pd(ax, bz, cy);
    cz = _mm256_fnmadd_pd(ay, bx, cz);
    _mm256_store_pd(out->outX, cx);
    _mm256_store_pd(out->outY, cy);
    _mm256_store_pd(out->outZ, cz);
}

void Vector3RotateBy4(const unsigned int a, const unsigned int b, const struct Vector3Out *out) {
    // rotates 4 vector3 (a) by 1 quat (b)
    // formula yoinked from some guy whom i blindly trust https://blog.molecular-matters.com/2013/05/24/a-faster-quaternion-vector-multiplication/
    // p = position, v is vector part of quat, w is scalar part
    // p’ = (v*v.dot(p) + v.cross(p)*(w))*2 + p*(w*w – v.dot(v))

    const __m256d ax = _mm256_load_pd(&vector3X[a]);
    const __m256d ay = _mm256_load_pd(&vector3Y[a]);
    const __m256d az = _mm256_load_pd(&vector3Z[a]);

    __m256d bw = _mm256_set1_pd(vector4W[b]);
    const __m256d bx = _mm256_set1_pd(vector4X[b]);
    const __m256d by = _mm256_set1_pd(vector4Y[b]);
    const __m256d bz = _mm256_set1_pd(vector4Z[b]);

    // v.dot(p)
    __m256d dot_vp = _mm256_mul_pd(ax, bx);
    dot_vp = _mm256_fmadd_pd(ay, by, dot_vp);
    dot_vp = _mm256_fmadd_pd(az, bz, dot_vp);

    // v*v.dot(p)
    __m256d cx = _mm256_mul_pd(bx, dot_vp);
    __m256d cy = _mm256_mul_pd(by, dot_vp);
    __m256d cz = _mm256_mul_pd(bz, dot_vp);

    // v.cross(p)
    dot_vp = _mm256_mul_pd(by, az);
    dot_vp = _mm256_fnmadd_pd(bz, ay, dot_vp);
    cx = _mm256_fmadd_pd(dot_vp, bw, cx);

    dot_vp = _mm256_mul_pd(bz, ax);
    dot_vp = _mm256_fnmadd_pd(bx, az, dot_vp);
    cy = _mm256_fmadd_pd(dot_vp, bw, cy);

    dot_vp = _mm256_mul_pd(bx, ay);
    dot_vp = _mm256_fnmadd_pd(by, ax, dot_vp);
    cz = _mm256_fmadd_pd(dot_vp, bw, cz);

    // v*v.dot(p) + v.cross(p)*(w)

    // (v*v.dot(p) + v.cross(p)*(w))*2
    bw = _mm256_set1_pd(2.0);
    cx = _mm256_mul_pd(cx, bw);
    cy = _mm256_mul_pd(cy, bw);
    cz = _mm256_mul_pd(cz, bw);

    bw = _mm256_set1_pd(camera_const); // the funny const
    // (v*v.dot(p) + v.cross(p)*(w))*2 + p*(w*w – v.dot(v))
    cx = _mm256_fmadd_pd(ax, bw, cx);
    cy = _mm256_fmadd_pd(ay, bw, cy);
    cz = _mm256_fmadd_pd(az, bw, cz);

    _mm256_store_pd(out->outX, cx);
    _mm256_store_pd(out->outY, cy);
    _mm256_store_pd(out->outZ, cz);
}

void Vector3Midpoint4(unsigned int a, unsigned int b, struct Vector3Out* out) {
    const __m256d ax = _mm256_load_pd(&vector3X[a]);
    const __m256d ay = _mm256_load_pd(&vector3Y[a]);
    const __m256d az = _mm256_load_pd(&vector3Z[a]);

    const __m256d scalar = _mm256_set1_pd(0.5);

    __m256d bx = _mm256_load_pd(&vector3X[b]);
    __m256d by = _mm256_load_pd(&vector3Y[b]);
    __m256d bz = _mm256_load_pd(&vector3Z[b]);

    bx = _mm256_add_pd(ax, bx);
    by = _mm256_add_pd(ay, by);
    bz = _mm256_add_pd(az, bz);

    bx = _mm256_mul_pd(bx, scalar);
    by = _mm256_mul_pd(by, scalar);
    bz = _mm256_mul_pd(bz, scalar);

    _mm256_store_pd(out->outX, bx);
    _mm256_store_pd(out->outY, by);
    _mm256_store_pd(out->outZ, bz);
}

void Vector3Distance4(unsigned int a, unsigned int b, struct Vector3Out* out) {
    const __m256d ax = _mm256_load_pd(&vector3X[a]);
    const __m256d ay = _mm256_load_pd(&vector3Y[a]);
    __m256d az = _mm256_load_pd(&vector3Z[a]);

    __m256d bx = _mm256_load_pd(&vector3X[b]);
    __m256d by = _mm256_load_pd(&vector3Y[b]);
    __m256d bz = _mm256_load_pd(&vector3Z[b]);

    bx = _mm256_sub_pd(bx, ax);
    by = _mm256_sub_pd(by, ay);
    bz = _mm256_sub_pd(bz, az);

    az = _mm256_mul_pd(bx, bx);
    az = _mm256_fmadd_pd(by, by, az);
    az = _mm256_fmadd_pd(bz, bz, az);
    az = _mm256_sqrt_pd(az);

    bx = _mm256_div_pd(bx, az);
    by = _mm256_div_pd(by, az);
    bz = _mm256_div_pd(bz, az);

    _mm256_store_pd(out->outX, bx);
    _mm256_store_pd(out->outY, by);
    _mm256_store_pd(out->outZ, bz);
}

// struct Vector3Return4 Vector3Normalize4(unsigned int a, unsigned int b, unsigned int c, unsigned int d) {
//     int64_t indices[4] = {a, b, c, d};
//     __m256i index = _mm256_loadu_si256((__m256i const*) indices);
//     __m256d x = _mm256_i64gather_pd(vector3X, index, 8);
//     __m256d y = _mm256_i64gather_pd(vector3Y, index, 8);
//     __m256d z = _mm256_i64gather_pd(vector3Z, index, 8);
//     __m256d e = _mm256_mul_pd(x, x);
//     __m256d f = _mm256_mul_pd(y, y);
//     e = _mm256_add_pd(e, f);
//     f = _mm256_mul_pd(z, z);
//     e = _mm256_add_pd(e, f);
//     e = _mm256_sqrt_pd(e);
//     x = _mm256_div_pd(x, e);
//     y = _mm256_div_pd(y, e);
//     z = _mm256_div_pd(z, e);
//     struct Vector3Return4 g = {};
//     _mm256_storeu_pd(g.x, x);
//     _mm256_storeu_pd(g.y, y);
//     _mm256_storeu_pd(g.z, z);
//     return g;
// }
//
// struct DoubleReturn4 Vector3Length4(unsigned int a, unsigned int b, unsigned int c, unsigned int d) {
//     const int64_t indices[4] = {a, b, c, d};
//     struct DoubleReturn4 g = {};
//     const __m256i index = _mm256_loadu_epi64((__m256i const *) indices);
//     __m256d x = _mm256_i64gather_pd(vector3X, index, sizeof(double));
//     __m256d y = _mm256_i64gather_pd(vector3Y, index, sizeof(double));
//     __m256d z = _mm256_i64gather_pd(vector3Z, index, sizeof(double));
//     __m256d e = _mm256_mul_pd(x, x);
//     __m256d f = _mm256_mul_pd(y, y);
//
//     e = _mm256_add_pd(e, f);
//     f = _mm256_mul_pd(z, z);
//     e = _mm256_add_pd(e, f);
//     e = _mm256_sqrt_pd(e);
//     _mm256_storeu_pd(g.a, e);
//     return g;
// }
//
// void Vector3CrossProduct(unsigned int a, unsigned int b) {
//     // ay * bz - az * by
//     // az * bx - ax * bz
//     // ax * by - ay * bx
// }
//
// void Vector3RotateBy(unsigned int a, unsigned int b) {
//     // a * b as vec4 * a.conj
// }