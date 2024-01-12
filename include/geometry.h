#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "range.h"

#include <stddef.h>

typedef size_t idx_t;
typedef size_t id_t;

typedef struct {
    idx_t  *vertices_data; // Array stores [v/vt/vn] tightly.
    size_t  vertices_count;
} Face;

typedef struct {
    char *name;
    id_t id;
    Range polygons;
} Group;

typedef struct {
    id_t     id;
    char    *name;
    Group   *groups;
    size_t   group_count;
    Range    owned_vertices;
} Object;

typedef struct {
    float    *vertices; // Array of consequent [x,y,z,w] structures.
    Face     *faces;
    Object   *objects;
    Group    *groups;
    size_t    vertices_count;
    size_t    faces_count;
    size_t    objects_count;
} WavefrontGeometry;

#endif
