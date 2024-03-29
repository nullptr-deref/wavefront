#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <stddef.h>

typedef size_t idx_t;

typedef struct {
    idx_t  *vertices; // Array stores [v/vt/vn] tightly.
    size_t  vertices_count;
} face_t;

// Struct holding vertices data.
// Data is stored as an tightly packed arrays.
typedef struct {
    float  *vertices;
    face_t *faces;
    size_t  vertices_count;
    size_t  faces_count;
} wavefront_geometry_t;

#endif
