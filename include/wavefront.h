#ifndef WAVEFRONT_H
#define WAVEFRONT_H

#include "geometry.h"

#include <stdio.h>

typedef size_t idx_t;

WavefrontGeometry *wavefront_geometry_init();
void wavefront_geometry_free(WavefrontGeometry *geometry);

WavefrontGeometry * wavefront_fread(FILE *restrict file);

#endif
