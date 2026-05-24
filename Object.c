#include "Object.h"
#include "AABB.h"
#include "Triangle.h"
#include "Vector3.h"
#include "Constants.h"

// helper func for creating sphere
Vector3Index getMid(
    Vector3Index a,
    Vector3Index b,
    unsigned int *current_new_point,
    unsigned int *new_points,
    unsigned int *current_vertex,
    Vector3Index *new_vertices,
    const double mass
) {
    if (a > b) {
        const unsigned int c = b;
        b = a;
        a = c;
    }
    for (unsigned int i=0; i<*current_new_point; i = i + 3) {
        if (new_points[i] == a && new_points[i + 1] == b) {
            return new_points[i + 2];
        }
    }
    double x = (vector3X_primary[a] + vector3X_primary[b]) / 2;
    double y = (vector3Y_primary[a] + vector3Y_primary[b]) / 2;
    double z = (vector3Z_primary[a] + vector3Z_primary[b]) / 2;
    const double inv_norm = InvSqrt(x * x + y * y + z * z);
    x *= inv_norm;
    y *= inv_norm;
    z *= inv_norm;
    const unsigned int idx = Vector3(x, y, z, 0, 0, 0, mass);
    new_points[*current_new_point + 0] = a;
    new_points[*current_new_point + 1] = b;
    new_points[*current_new_point + 2] = idx;
    new_vertices[*current_vertex] = idx;
    (*current_vertex)++;
    *current_new_point = *current_new_point + 3;
    return idx;
}
// TODO make these not suck
void ObjectCreateSphere(const double x_pos, const double y_pos, const double z_pos, const double scale_x, const double scale_y, const double scale_z, const unsigned int vertex_scale, const double mass, unsigned int color) {
    const unsigned int triangle_count = 8 * Pow(4, vertex_scale);
    const unsigned int vertices_count = 2 + 4 * Pow(4, vertex_scale);
    // thank you mebot
    TriangleIndex *triangle_indexes = malloc(triangle_count * sizeof(unsigned int));
    Vector3Index *vertex_indices = malloc(vertices_count * sizeof(unsigned int));
    unsigned int *past_midpoints = malloc(sizeof(unsigned int) * 3 * vertices_count);
    if (triangle_indexes == NULL || vertex_indices == NULL || past_midpoints == NULL) {
        free(triangle_indexes);
        free(vertex_indices);
        free(past_midpoints);
        return;
    }
    char do_rand = 0;
    if (color == (COLOR_RAND_PER_SETUP | COLOR_RAND_PER_OBJ)) {
        color = Random();
    } else if (color == (COLOR_RAND_PER_SETUP | COLOR_RAND_PER_TRI)) {
        do_rand = 1;
    }

    unsigned int triangle_cnt = 8;
    unsigned int vertex_cnt = 6;
    const Vector3Index p0 = Vector3(1,0,0,0,0,0,mass);
    { // setup
        const Vector3Index p1 = Vector3(-1,0,0,0,0,0,mass);
        const Vector3Index p2 = Vector3(0,1,0,0,0,0,mass);
        const Vector3Index p3 = Vector3(0,-1,0,0,0,0,mass);
        const Vector3Index p4 = Vector3(0,0,1,0,0,0,mass);
        const Vector3Index p5 = Vector3(0,0,-1,0,0,0,mass);

        vertex_indices[0] = p0;
        vertex_indices[1] = p1;
        vertex_indices[2] = p2;
        vertex_indices[3] = p3;
        vertex_indices[4] = p4;
        vertex_indices[5] = p5;

        if (do_rand) {
            triangle_indexes[0] = Triangle(p0, p2, p4, color);
            color = Random();
            triangle_indexes[1] = Triangle(p0, p2, p5, color);
            color = Random();
            triangle_indexes[2] = Triangle(p0, p3, p4, color);
            color = Random();
            triangle_indexes[3] = Triangle(p0, p3, p5, color);
            color = Random();
            triangle_indexes[4] = Triangle(p1, p2, p4, color);
            color = Random();
            triangle_indexes[5] = Triangle(p1, p2, p5, color);
            color = Random();
            triangle_indexes[6] = Triangle(p1, p3, p4, color);
            color = Random();
            triangle_indexes[7] = Triangle(p1, p3, p5, color);
            color = Random();
        } else {
            triangle_indexes[0] = Triangle(p0, p2, p4, color);
            triangle_indexes[1] = Triangle(p0, p2, p5, color);
            triangle_indexes[2] = Triangle(p0, p3, p4, color);
            triangle_indexes[3] = Triangle(p0, p3, p5, color);
            triangle_indexes[4] = Triangle(p1, p2, p4, color);
            triangle_indexes[5] = Triangle(p1, p2, p5, color);
            triangle_indexes[6] = Triangle(p1, p3, p4, color);
            triangle_indexes[7] = Triangle(p1, p3, p5, color);
        }

    }
    for (int i=0; i<vertex_scale; i++) {
        unsigned int current_new_points = 0;
        for (int j=0; j<8*Pow(4, i); j++) {

            const unsigned int face = triangle_indexes[j];
            const Vector3Index m01 = getMid(triangle_triangles[face].p0, triangle_triangles[face].p1, &current_new_points, past_midpoints, &vertex_cnt, vertex_indices, mass);
            const Vector3Index m02 = getMid(triangle_triangles[face].p0, triangle_triangles[face].p2, &current_new_points, past_midpoints, &vertex_cnt, vertex_indices, mass);
            const Vector3Index m12 = getMid(triangle_triangles[face].p1, triangle_triangles[face].p2, &current_new_points, past_midpoints, &vertex_cnt, vertex_indices, mass);

            // add new triangles
            if (do_rand) {color = Random();}
            triangle_indexes[triangle_cnt] = Triangle(triangle_triangles[face].p1, m01, m12, color);
            triangle_cnt++;
            if (do_rand) {color = Random();}
            triangle_indexes[triangle_cnt] = Triangle(triangle_triangles[face].p2, m02, m12, color);
            triangle_cnt++;
            if (do_rand) {color = Random();}
            triangle_indexes[triangle_cnt] = Triangle(m01, m02, m12, color);
            triangle_cnt++;
            // modify old triangle to be smaller
            triangle_triangles[face].p1 = m01;
            triangle_triangles[face].p2 = m02;
        }
    }

    // move unit "sphere" according to inputs
    for (int i=0; i<vertex_cnt; i++) {
        vector3X_primary[vertex_indices[i]] = vector3X_primary[vertex_indices[i]] * scale_x + x_pos;
        vector3Y_primary[vertex_indices[i]] = vector3Y_primary[vertex_indices[i]] * scale_y + y_pos;
        vector3Z_primary[vertex_indices[i]] = vector3Z_primary[vertex_indices[i]] * scale_z + z_pos;
    }

    vector3X_secondary[p0] = x_pos;
    vector3Y_secondary[p0] = y_pos;
    vector3Z_secondary[p0] = z_pos;
    const Vector3Index center_index = p0 + vector3_cap;
    for (int i=0; i<triangle_cnt; i++) {
        TriangleOrient(triangle_indexes[i], center_index);
    }

    free(triangle_indexes);
    free(vertex_indices);
    free(past_midpoints);
}

void ObjectCreateCube(const double x_pos, const double y_pos, const double z_pos, const double scale_x, const double scale_y, const double scale_z, const double mass, const unsigned int color) {
    const Vector3Index p0 = Vector3(-scale_x + x_pos,-scale_y + y_pos,-scale_z + z_pos, 0, 0, 0, mass);
    const Vector3Index p1 = Vector3( scale_x + x_pos,-scale_y + y_pos,-scale_z + z_pos, 0, 0, 0, mass);
    const Vector3Index p2 = Vector3(-scale_x + x_pos, scale_y + y_pos,-scale_z + z_pos, 0, 0, 0, mass);
    const Vector3Index p3 = Vector3( scale_x + x_pos, scale_y + y_pos,-scale_z + z_pos, 0, 0, 0, mass);
    const Vector3Index p4 = Vector3(-scale_x + x_pos,-scale_y + y_pos, scale_z + z_pos, 0, 0, 0, mass);
    const Vector3Index p5 = Vector3( scale_x + x_pos,-scale_y + y_pos, scale_z + z_pos, 0, 0, 0, mass);
    const Vector3Index p6 = Vector3(-scale_x + x_pos, scale_y + y_pos, scale_z + z_pos, 0, 0, 0, mass);
    const Vector3Index p7 = Vector3( scale_x + x_pos, scale_y + y_pos, scale_z + z_pos, 0, 0, 0, mass);

    vector3X_secondary[p0] = x_pos;
    vector3Y_secondary[p0] = y_pos;
    vector3Z_secondary[p0] = z_pos;
    const Vector3Index center = p0 + vector3_cap;

    TriangleIndex id = Triangle(p0, p1, p2, color);
    TriangleOrient(id, center);

    id = Triangle(p3, p1, p2, color);
    TriangleOrient(id, center);

    id = Triangle(p4, p5, p6, color);
    TriangleOrient(id, center);

    id = Triangle(p7, p5, p6, color);
    TriangleOrient(id, center);

    id = Triangle(p2, p6, p7, color);
    TriangleOrient(id, center);

    id = Triangle(p2, p3, p7, color);
    TriangleOrient(id, center);

    id = Triangle(p0, p4, p5, color);
    TriangleOrient(id, center);

    id = Triangle(p0, p1, p5, color);
    TriangleOrient(id, center);

    id = Triangle(p1, p5, p7, color);
    TriangleOrient(id, center);

    id = Triangle(p1, p3, p7, color);
    TriangleOrient(id, center);

    id = Triangle(p0, p2, p6, color);
    TriangleOrient(id, center);

    id = Triangle(p0, p4, p6, color);
    TriangleOrient(id, center);
}

void ObjectFromFile(const char* file_name) {
    // https://opengameart.org/
    // .obj and .fbx maybe
    // https://kixor.net/dev/objloader/
    // https://en.wikipedia.org/wiki/Wavefront_.obj_file
}
