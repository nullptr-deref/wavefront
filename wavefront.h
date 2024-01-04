#ifndef WAVEFRONT_H
#define WAVEFRONT_H

#include <stdint.h>
#include <stdio.h>

typedef size_t idx_t;
typedef enum {
    NORMAL,
    LARGE
} readmode_t;

typedef struct {
    idx_t  *indices;
    size_t  vertices_count;
} face_t;

// Struct holding vertices data.
// Data is stored as an tightly packed arrays.
typedef struct {
    float  *vertices;
    face_t *faces;
    size_t  vertices_count;
    size_t faces_count;
} wavefront_geometry_t;

wavefront_geometry_t *wavefront_geometry_init();
void wavefront_geometry_free(wavefront_geometry_t *geometry);

size_t fread_wavefront_geometry(wavefront_geometry_t *geometry, FILE *restrict file, readmode_t mode);
size_t fread_wavefront_geometry_normal(wavefront_geometry_t *geometry, FILE *restrict file);
size_t fread_wavefront_geometry_large(wavefront_geometry_t *geometry, FILE *restrict file);

#endif
