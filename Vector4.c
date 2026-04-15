#include "Constants.h"
#include <immintrin.h>
#include "Vector4.h"
#include <malloc.h>

// TODO, basically uses same system as vector3 so same flaws
// maybe make it some kind of generic thing?
void Vector4Init() {
    vector4_cap = 100;
    vector4_current = 0;
    vector4W = _aligned_malloc(vector4_cap * 4 * sizeof(double), 32);
    vector4X = vector4W + vector4_cap;
    vector4Y = vector4X + vector4_cap;
    vector4Z = vector4Y + vector4_cap;
}

void Vector4Destroy() {
    _aligned_free(vector4W);
}
// NOTE, WAY less quats than vec3 so 100 should actually be fine
unsigned int Vector4(double w, double x, double y, double z) {
    if (vector4_current == vector4_cap) {
        vector4_cap *= 2;

        vector4W = _aligned_realloc(vector4W, vector4_cap * sizeof(double) * 4, 32);
        vector4X = vector4W + vector4_cap;
        vector4Y = vector4X + vector4_cap;
        vector4Z = vector4Y + vector4_cap;
    }

    const unsigned int ret = vector4_current;
    vector4X[ret] = x;
    vector4Y[ret] = y;
    vector4Z[ret] = z;
    vector4W[ret] = w;

    vector4_current++;
    return ret;
}

// NOTE, only for adjusting camera angle since that is only once quat change at a time
// vectors should use the dedicated vec3 func
void Vector4Multiplier(unsigned int a, unsigned int b, struct Vector4Out *out) {
    __m256d pa = _mm256_set1_pd(vector4W[a]); // [aw aw aw aw]
    __m256d pb = _mm256_setr_pd(vector4W[b], vector4X[b], vector4Y[b], vector4Z[b]); // [bw bx by bz]
    // __m256d sign = _mm256_set_pd(0.0, 0.0, 0.0,  0.0); // [bw bx by bz]
    // pb = _mm256_xor_pd(pb, sign);
    __m256d res = _mm256_mul_pd(pa, pb);

    pa = _mm256_set1_pd(vector4X[a]); // [ax ax ax ax]
    pb = _mm256_permute4x64_pd(pb, SHUFFLE_R(1,0,3,2)); // [bx bw bz by]
    __m256d sign = _mm256_setr_pd(-0.0, 0.0, -0.0, 0.0); // [-bx bw -bz by]
    pb = _mm256_xor_pd(pb, sign);
    res = _mm256_fmadd_pd(pa, pb, res);

    pa = _mm256_set1_pd(vector4Y[a]); // [ay ay ay ay]
    pb = _mm256_permute4x64_pd(pb, SHUFFLE_R(3,2,1,0)); // [by -bz bw -bx]
    sign = _mm256_setr_pd(-0.0, -0.0, 0.0, 0.0); // [-by bz bw -bx]
    pb = _mm256_xor_pd(pb, sign);
    res = _mm256_fmadd_pd(pa, pb, res);

    pa = _mm256_set1_pd(vector4Z[a]); // [az az az az]
    pb = _mm256_permute4x64_pd(pb, SHUFFLE_R(1,0,3,2)); // [bz -by -bx bw]
    sign = _mm256_setr_pd(-0.0, 0.0, -0.0, 0.0); // [-bz -by bx bw]
    pb = _mm256_xor_pd(pb, sign);
    res = _mm256_fmadd_pd(pa, pb, res);
    double vec[4];
    _mm256_storeu_pd(vec, res);
}

double Vector4CalculateFunnyConst(unsigned int b) {
    // w*w – v.dot(v)
    return vector4W[b]*vector4W[b] - (vector4X[b]*vector4X[b] + vector4Y[b]*vector4Y[b] + vector4Z[b]*vector4Z[b]);
}