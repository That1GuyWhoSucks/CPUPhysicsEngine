#include <immintrin.h>

#include "Constants.h"
#include "Vector4.h"
#include "ExpandableArray.h"
#include "Vector3.h"

void Vector4Init() {
    vector4_cap = 2;
    vector4_cur = 0;
    camera_const = 1;
    vector4W = ArrayInit(vector4_cap, 4, sizeof(double), 32);
    vector4X = vector4W + vector4_cap;
    vector4Y = vector4X + vector4_cap;
    vector4Z = vector4Y + vector4_cap;
    Vector4(1,0,0,0); // cam
    Vector4(1,0,0,0); // cam conj
}

void Vector4Destroy() {
    ArrayDestroy(vector4W);
}

unsigned int Vector4(const double w, const double x, const double y, const double z) {
    if (vector4_cur == vector4_cap) {
        ArrayResize((void **) &vector4W, &vector4_cap, 2, 4, sizeof(double), 32);
        vector4X = vector4W + vector4_cap;
        vector4Y = vector4X + vector4_cap;
        vector4Z = vector4Y + vector4_cap;
    }

    const unsigned int ret = vector4_cur;
    vector4X[ret] = x;
    vector4Y[ret] = y;
    vector4Z[ret] = z;
    vector4W[ret] = w;

    vector4_cur++;
    return ret;
}

// https://lisyarus.github.io/blog/posts/introduction-to-quaternions.html

// NOTE, only for adjusting camera angle since that is only once quat change at a time
// vectors should use the dedicated vec3 func
void Vector4Multiplier(const double x, const double y, const double z) {
    __m256d pa = _mm256_set1_pd(vector4W[vector4_cam_rot_index]); // [aw aw aw aw]
    __m256d pb = _mm256_setr_pd(1, x,y,z); // [bw bx by bz]
    // __m256d sign = _mm256_set_pd(0.0, 0.0, 0.0,  0.0); // [bw bx by bz]
    // pb = _mm256_xor_pd(pb, sign);
    __m256d res = _mm256_mul_pd(pa, pb);

    pa = _mm256_set1_pd(vector4X[vector4_cam_rot_index]); // [ax ax ax ax]
    pb = _mm256_permute4x64_pd(pb, SHUFFLE_R(1,0,3,2)); // [bx bw bz by]
    __m256d sign = _mm256_setr_pd(-0.0, 0.0, -0.0, 0.0); // [-bx bw -bz by]
    pb = _mm256_xor_pd(pb, sign);
    res = _mm256_fmadd_pd(pa, pb, res);

    pa = _mm256_set1_pd(vector4Y[vector4_cam_rot_index]); // [ay ay ay ay]
    pb = _mm256_permute4x64_pd(pb, SHUFFLE_R(3,2,1,0)); // [by -bz bw -bx]
    sign = _mm256_setr_pd(-0.0, -0.0, 0.0, 0.0); // [-by bz bw -bx]
    pb = _mm256_xor_pd(pb, sign);
    res = _mm256_fmadd_pd(pa, pb, res);

    pa = _mm256_set1_pd(vector4Z[vector4_cam_rot_index]); // [az az az az]
    pb = _mm256_permute4x64_pd(pb, SHUFFLE_R(1,0,3,2)); // [bz -by -bx bw]
    sign = _mm256_setr_pd(-0.0, 0.0, -0.0, 0.0); // [-bz -by bx bw]
    pb = _mm256_xor_pd(pb, sign);
    res = _mm256_fmadd_pd(pa, pb, res);

    // normalize
    pb = _mm256_mul_pd(res, res); // [w^2 x^2 y^2 z^2]
    pa = _mm256_permute4x64_pd(pb, SHUFFLE_R(2,3,0,1)); // [y^2 z^2 w^2 x^2]
    pb = _mm256_add_pd(pb, pa); // [w+y x+z y+w z+x]

    pb = _mm256_hadd_pd(pb, pb); // [w+y+x+z ...]
    pb = _mm256_invsqrt_pd(pb);
    pb = _mm256_permute4x64_pd(pb, SHUFFLE_R(0,0,0,0));
    res = _mm256_mul_pd(res, pb);

    // store
    double vec[4];
    _mm256_store_pd(vec, res);

    vector4W[vector4_cam_rot_index] = vec[0];
    vector4X[vector4_cam_rot_index] = vec[1];
    vector4Y[vector4_cam_rot_index] = vec[2];
    vector4Z[vector4_cam_rot_index] = vec[3];

    vector4W[vector4_cam_rot_conj_index] = vec[0];
    vector4X[vector4_cam_rot_conj_index] = -vec[1];
    vector4Y[vector4_cam_rot_conj_index] = -vec[2];
    vector4Z[vector4_cam_rot_conj_index] = -vec[3];

    // w*w – v.dot(v)
    camera_const = vector4W[vector4_cam_rot_index]*vector4W[vector4_cam_rot_index] - (vector4X[vector4_cam_rot_index]*vector4X[vector4_cam_rot_index] + vector4Y[vector4_cam_rot_index]*vector4Y[vector4_cam_rot_index] + vector4Z[vector4_cam_rot_index]*vector4Z[vector4_cam_rot_index]);
}

void Vector4TranslateCamera(double px, double py, double pz) {
    // p = position, v is vector part of quat, w is scalar part
    // p’ = (v*v.dot(p) + v.cross(p)*(w))*2 + p*(w*w – v.dot(v))

    double dot = vector4X[vector4_cam_rot_index] * px + vector4Y[vector4_cam_rot_index] * py + vector4Z[vector4_cam_rot_index] * pz;
    double cx = (dot * vector4X[vector4_cam_rot_index] + (vector4Y[vector4_cam_rot_index] * pz - vector4Z[vector4_cam_rot_index] * py) * vector4W[vector4_cam_rot_index]) * 2 + px * camera_const;
    double cy = (dot * vector4Y[vector4_cam_rot_index] + (vector4Z[vector4_cam_rot_index] * px - vector4X[vector4_cam_rot_index] * pz) * vector4W[vector4_cam_rot_index]) * 2 + py * camera_const;
    double cz = (dot * vector4Z[vector4_cam_rot_index] + (vector4X[vector4_cam_rot_index] * py - vector4Y[vector4_cam_rot_index] * px) * vector4W[vector4_cam_rot_index]) * 2 + pz * camera_const;
    vector3X_primary[vector3_camera_pos_index] += cx;
    vector3Y_primary[vector3_camera_pos_index] += cy;
    vector3Z_primary[vector3_camera_pos_index] += cz;
}