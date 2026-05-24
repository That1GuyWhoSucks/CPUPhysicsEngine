#include "Renderer.h"
#include "Triangle.h"
#include "Vector3.h"
#include "Vector4.h"

void RendererInit(void *pixel_map) {
    keys_down = 0;
    renderer_z_buffer = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(double));
    renderer_buffer = pixel_map;
}

void RendererDestroy() {
    free(renderer_z_buffer);
}

void RendererOnKeyDown(const wchar_t input) {
    switch (input) {
        case 'W': {
            keys_down = keys_down | KEY_W;
            return;
        }
        case 'A': {
            keys_down = keys_down | KEY_A;
            return;
        }
        case 'S': {
            keys_down = keys_down | KEY_S;
            return;
        }
        case 'D': {
            keys_down = keys_down | KEY_D;
            return;
        }
        case ' ': {
            keys_down = keys_down | KEY_SPACE;
            return;
        }
        case 0x10: { // shift
            keys_down = keys_down | KEY_SHIFT;
            return;
        }
        case 'Q': {
            keys_down = keys_down | KEY_Q;
            return;
        }
        case 'E': {
            keys_down = keys_down | KEY_E;
            return;
        }
        case 0x1B: { // esc
            SUICIDE_FLAG = 1;
        }
        default: {}
    }
}

void RendererOnKeyUp(const wchar_t input) {
    switch (input) {
        case 'W': {
            keys_down = keys_down & ~KEY_W;
            return;
        }
        case 'A': {
            keys_down = keys_down & ~KEY_A;
            return;
        }
        case 'S': {
            keys_down = keys_down & ~KEY_S;
            return;
        }
        case 'D': {
            keys_down = keys_down & ~KEY_D;
            return;
        }
        case ' ': {
            keys_down = keys_down & ~KEY_SPACE;
            return;
        }
        case 0x10: {
            keys_down = keys_down & ~KEY_SHIFT;
            return;
        }
        case 'Q': {
            keys_down = keys_down & ~KEY_Q;
            return;
        }
        case 'E': {
            keys_down = keys_down & ~KEY_E;
        }
        default: {}
    }
}

void RendererOnMouse(const short x_delta, const short y_delta) {
    Vector4Multiplier(y_delta * CAMERA_SENSITIVITY, 0, 0);
    Vector4Multiplier(0,x_delta * CAMERA_SENSITIVITY,0);
}

double EdgeFunction(const double a_x, const double a_y, const double b_x, const double b_y, const double c_x, const double c_y) {
    return (c_x - a_x) * (b_y - a_y) - (c_y - a_y) * (b_x - a_x);
}

// AGHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH
void ProjectPoint(const unsigned int a) {
    __m256d ax = _mm256_load_pd(&vector3X_secondary[a]);
    __m256d ay = _mm256_load_pd(&vector3Y_secondary[a]);
    __m256d az = _mm256_load_pd(&vector3Z_secondary[a]);

    // a - camera pos
    // TODO cam is cold relative to rest of data
    __m256d bw = _mm256_set1_pd(vector3X_primary[vector3_camera_pos_index]);
    ax = _mm256_sub_pd(ax, bw); //
    bw = _mm256_set1_pd(vector3Y_primary[vector3_camera_pos_index]);
    ay = _mm256_sub_pd(ay, bw);
    bw = _mm256_set1_pd(vector3Z_primary[vector3_camera_pos_index]);
    az = _mm256_sub_pd(az, bw);

    // rotate by the conj
    // p = position, v is vector part of quat, w is scalar part, c = const that is stored
    // p’ = (v*v.dot(p) + v.cross(p)*(w))*2 + p*const
    bw = _mm256_set1_pd(vector4W[vector4_cam_rot_conj_index]);
    const __m256d bx = _mm256_set1_pd(vector4X[vector4_cam_rot_conj_index]);
    const __m256d by = _mm256_set1_pd(vector4Y[vector4_cam_rot_conj_index]);
    const __m256d bz = _mm256_set1_pd(vector4Z[vector4_cam_rot_conj_index]);

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

    _mm256_store_pd(&vector3X_secondary[a], cx);
    _mm256_store_pd(&vector3Y_secondary[a], cy);
    _mm256_store_pd(&vector3Z_secondary[a], cz);
}

void clamp(const double a, const double b, const double c, const unsigned int max, unsigned int *min_val, unsigned int *max_val) {
    if (a > b && a > c) {
        // a max
        *max_val = Ceil(MinDouble(MaxDouble(a, 0.0), max));
        *min_val = Floor(MaxDouble(0.0, MinDouble(b, c)));
    } else if (b > c) {
        // b max
        *max_val = Ceil(MinDouble(MaxDouble(b, 0.0), max));
        *min_val = Floor(MaxDouble(0.0, MinDouble(a, c)));
    } else {
        // c max
        *max_val = Ceil(MinDouble(MaxDouble(c, 0.0), max));
        *min_val = Floor(MaxDouble(0.0, MinDouble(a, b)));
    }
}

void RenderTriangleData(
    double p0_x, double p0_y, const double p0_z,
    double p1_x, double p1_y, const double p1_z,
    double p2_x, double p2_y, const double p2_z,
    Color color
) {
    double invZ = FOCAL_LENGTH / p0_z;
    p0_x = p0_x * invZ + SCREEN_WIDTH_HALF;
    p0_y = p0_y * invZ + SCREEN_HEIGHT_HALF;

    invZ = FOCAL_LENGTH / p1_z;
    p1_x = p1_x * invZ + SCREEN_WIDTH_HALF;
    p1_y = p1_y * invZ + SCREEN_HEIGHT_HALF;

    invZ = FOCAL_LENGTH / p2_z;
    p2_x = p2_x * invZ + SCREEN_WIDTH_HALF;
    p2_y = p2_y * invZ + SCREEN_HEIGHT_HALF;
    const double area = 1 / EdgeFunction(p0_x, p0_y, p1_x, p1_y, p2_x, p2_y);
    if (area < 0) return; // if normal away from camera then don't render
#if DO_LOG == 1
    const double wireframe_thresh = WIREFRAME_THICKNESS / area;
#endif

    unsigned int maxX;
    unsigned int minX;
    clamp(p0_x, p1_x, p2_x, SCREEN_WIDTH, &minX, &maxX);
    if (minX == maxX) return;
    unsigned int maxY;
    unsigned int minY;
    clamp(p0_y, p1_y, p2_y, SCREEN_HEIGHT, &minY, &maxY);
    if (minY == maxY) return;

    double w0_row = EdgeFunction(p1_x, p1_y, p2_x, p2_y, minX, minY);
    double w1_row = EdgeFunction(p2_x, p2_y, p0_x, p0_y, minX, minY);
    double w2_row = EdgeFunction(p0_x, p0_y, p1_x, p1_y, minX, minY);
    double z_row = (p0_z * w0_row + p1_z * w1_row + p2_z * w2_row) * area;

    const double w0_dx = p2_y - p1_y;
    const double w0_dy = -(p2_x - p1_x);
    const double w1_dx = p0_y - p2_y;
    const double w1_dy = -(p0_x - p2_x);
    const double w2_dx = p1_y - p0_y;
    const double w2_dy = -(p1_x - p0_x);
    const double z_dx = (p0_z * w0_dx + p1_z * w1_dx + p2_z * w2_dx) * area;
    const double z_dy = (p0_z * w0_dy + p1_z * w1_dy + p2_z * w2_dy) * area;
    // TODO, currently lots of checks to try and escape as early as possible.
    // this can be split into 2^6 unique functions that would minimize number of checks with only the initial switch statement and function call as overhead
    // but that sounds miserable so dont even think about until way later
    for (unsigned int y=minY; y<maxY; y++) {
        double w0 = w0_row;
        double w1 = w1_row;
        double w2 = w2_row;
        double z = z_row;
        if (
            (w0 < 0 && w0_dy < 0 && w0_dx < 0) ||
            (w1 < 0 && w1_dy < 0 && w1_dx < 0) ||
            (w2 < 0 && w2_dy < 0 && w2_dx < 0)
        ) break;
        // TODO, should be possible to SIMD this. Do cond * color + !cond * old_color for setting
        // might not work great at array, look into it
        for (unsigned int x=minX; x<maxX; x++) {
            if (
                (w0 < 0 && w0_dx < 0) ||
                (w1 < 0 && w1_dx < 0) ||
                (w2 < 0 && w2_dx < 0)
            ) break;
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                const unsigned int pidx = SCREEN_WIDTH * y + x;
                if (z < renderer_z_buffer[pidx]) {
                    renderer_z_buffer[pidx] = z;

#if DO_LOG == 1
                    if (w0 < wireframe_thresh || w1 < wireframe_thresh || w2 < wireframe_thresh) {
                        // wireframe
                        renderer_buffer[pidx] = COLOR_WHITE;
                    } else {
                        renderer_buffer[pidx] = color;
                    }
#else
                    renderer_buffer[pidx] = color;
#endif
                }
            }
            w0 += w0_dx;
            w1 += w1_dx;
            w2 += w2_dx;
            z += z_dx;
        }
        w0_row += w0_dy;
        w1_row += w1_dy;
        w2_row += w2_dy;
        z_row += z_dy;
    }
}

void Clip(const double nearX, const double nearY, const double nearZ, double *farX, double *farY, double *farZ) {
    const double c = (NEAR_PLANE - *farZ) / (nearZ - *farZ);
    *farX = (nearX - *farX) * c + *farX;
    *farY = (nearY - *farY) * c + *farY;
    *farZ = NEAR_PLANE;
}

void RenderTriangle(const unsigned int idx) {
    // https://www.desmos.com/3d/dp5j5y3flb
    // near plane clipping demo
    const Vector3Index p0 = triangle_triangles[idx].p0;
    const Vector3Index p1 = triangle_triangles[idx].p1;
    const Vector3Index p2 = triangle_triangles[idx].p2;
    const Color color = triangle_triangles[idx].c;

    double p0_x = vector3X_secondary[p0];
    double p1_x = vector3X_secondary[p1];
    double p2_x = vector3X_secondary[p2];
    double p0_y = vector3Y_secondary[p0];
    double p1_y = vector3Y_secondary[p1];
    double p2_y = vector3Y_secondary[p2];

    double p0_z = vector3Z_secondary[p0];
    double p1_z = vector3Z_secondary[p1];
    double p2_z = vector3Z_secondary[p2];
    switch ((p0_z > NEAR_PLANE) + (p1_z > NEAR_PLANE) * 2 + (p2_z > NEAR_PLANE) * 4) { // bit mask of near
        case 0: { // none near
            return;
        }
        // TODO probably neater way of doing this
        // if so make sure normals are maintained otherwise it doesn't work properly

        // also still inverts the colors on split triangles because its kinda cool. remove later
        case 1: { // p0 near
            Clip(p0_x, p0_y, p0_z, &p1_x, &p1_y, &p1_z);
            Clip(p0_x, p0_y, p0_z, &p2_x, &p2_y, &p2_z);
            RenderTriangleData(p0_x, p0_y, p0_z, p1_x, p1_y, p1_z, p2_x, p2_y, p2_z, color);
            break;
        }
        case 2: { // p1 near
            Clip(p1_x, p1_y, p1_z, &p0_x, &p0_y, &p0_z);
            Clip(p1_x, p1_y, p1_z, &p2_x, &p2_y, &p2_z);
            RenderTriangleData(p0_x, p0_y, p0_z, p1_x, p1_y, p1_z, p2_x, p2_y, p2_z, color);
            break;
        }
        case 3: { // p0, p1 near
            double p3_x = p2_x;
            double p3_y = p2_y;
            double p3_z = p2_z;
            Clip(p0_x, p0_y, p0_z, &p2_x, &p2_y, &p2_z); // p2 is p2 rel p0
            Clip(p1_x, p1_y, p1_z, &p3_x, &p3_y, &p3_z); // p3 is p2 rel p1
            RenderTriangleData(p0_x, p0_y, p0_z, p1_x, p1_y, p1_z, p2_x, p2_y, p2_z, color);
            RenderTriangleData(p2_x, p2_y, p2_z, p1_x, p1_y, p1_z, p3_x, p3_y, p3_z, ~color);
            break;
        }
        case 4: { // p2 near
            Clip(p2_x, p2_y, p2_z, &p0_x, &p0_y, &p0_z);
            Clip(p2_x, p2_y, p2_z, &p1_x, &p1_y, &p1_z);
            RenderTriangleData(p0_x, p0_y, p0_z, p1_x, p1_y, p1_z, p2_x, p2_y, p2_z, color);
            break;
        }
        case 5: { // p0, p2 near
            double p3_x = p1_x;
            double p3_y = p1_y;
            double p3_z = p1_z;
            Clip(p0_x, p0_y, p0_z, &p1_x, &p1_y, &p1_z); // p1 is p1 rel p0
            Clip(p2_x, p2_y, p2_z, &p3_x, &p3_y, &p3_z); // p3 is p1 rel p2
            RenderTriangleData(p0_x, p0_y, p0_z, p1_x, p1_y, p1_z, p2_x, p2_y, p2_z, color);
            RenderTriangleData(p1_x, p1_y, p1_z, p3_x, p3_y, p3_z, p2_x, p2_y, p2_z, ~color);
            break;
        }
        case 6: { // p1, p2 near
            double p3_x = p0_x;
            double p3_y = p0_y;
            double p3_z = p0_z;
            Clip(p1_x, p1_y, p1_z, &p0_x, &p0_y, &p0_z); // p0 is p0 rel p1
            Clip(p2_x, p2_y, p2_z, &p3_x, &p3_y, &p3_z); // p3 is p0 rel p2
            RenderTriangleData(p0_x, p0_y, p0_z, p1_x, p1_y, p1_z, p2_x, p2_y, p2_z, color);
            RenderTriangleData(p0_x,p0_y,p0_z,p2_x,p2_y,p2_z,p3_x,p3_y,p3_z, ~color);
            break;
        }
        case 7: { // p0, p1, p2 near
            RenderTriangleData(p0_x, p0_y, p0_z, p1_x, p1_y, p1_z, p2_x, p2_y, p2_z, color);
        }
        default: {};
    }
}

void RendererApplyKeys(const double d_time) {
    // TODO optimize
    const double px = (keys_down & KEY_A) * (-TRANSLATION_DELTA / KEY_A) + (keys_down & KEY_D) * (TRANSLATION_DELTA / KEY_D);
    const double py = (keys_down & KEY_SHIFT) * (-TRANSLATION_DELTA / KEY_SHIFT) + (keys_down & KEY_SPACE) * (TRANSLATION_DELTA / KEY_SPACE);
    const double pz = (keys_down & KEY_S) * (-TRANSLATION_DELTA / KEY_S) + (keys_down & KEY_W) * (TRANSLATION_DELTA / KEY_W);
    const double pr = (keys_down & KEY_E) * (-ROTATION_DELTA / KEY_E) + (keys_down & KEY_Q) * (ROTATION_DELTA / KEY_Q);
    if (px || py || pz) {
        Vector4TranslateCamera(px * d_time, py * d_time, pz * d_time);
    }
    if (pr) {
        Vector4Multiplier(0,0,pr * d_time);
    }
}

void RendererRender() {
    for (int i=0; i<SCREEN_WIDTH*SCREEN_HEIGHT; i++) {
        renderer_z_buffer[i] = (double) 0xefffffffffffffff; // all f breaks don't question it
        renderer_buffer[i] = 0;
    }

    for (unsigned int i=vector3_start; i<vector3_cur; i = i + 4) { // TODO just noting to self to remember this for later
        ProjectPoint(i);
    }
    for (int i=0; i<triangle_cur; i++) {
        // not a nice way to SIMD this one, too many conditionals
        RenderTriangle(i);
    }
}
