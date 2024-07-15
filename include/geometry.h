#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "range.h"

#include <stddef.h>

typedef size_t idx_t;
typedef size_t wavefront_id_t;

typedef struct {
    idx_t  *vertices_data; // Array stores [v/vt/vn] tightly.
    size_t  vertices_count;
} Face;

/* 
 * Group is a polygonal and free-form geometry statement.
 * Specifies the group name for the elements that follow it.
 */
typedef struct {
    wavefront_id_t  id;
    char           *name;
    ArrayList      *polygons; // Set of polygon ranges which belong to the group.
} Group;

void wavefront_register_group(WavefrontGeometry *geometry, const char *name);
wavefront_id_t wavefront_search_group(WavefrontGeometry *geometry, const char *name);
void group_free(Group *g);

/*
 * Object is a polygonal and free-form geometry statement.
 * It is optional; provides the object name for the geometry elements
 * that follow it.
 * Has no default name.
 */
typedef struct {
    wavefront_id_t id;
    char          *name;
    Range          owned_faces;
} Object;

typedef struct {
    float    *vertices; // Array of consequent [x,y,z,w] structures.
    Face     *faces;
    Object   *objects;
    Group    *groups;
    size_t    vertices_count;
    size_t    faces_count;
    size_t    objects_count;
    size_t    groups_count;
} WavefrontGeometry;

#endif
