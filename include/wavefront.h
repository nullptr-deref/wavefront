#ifndef WAVEFRONT_H
#define WAVEFRONT_H

#include "geometry.h"

#include <stdio.h>

typedef size_t idx_t;

wavefront_geometry_t *wavefront_geometry_init();
void wavefront_geometry_free(wavefront_geometry_t *geometry);

size_t wavefront_fread(wavefront_geometry_t *geometry, FILE *restrict file);

#endif
