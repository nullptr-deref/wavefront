#include "wavefront.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const char GEOMETRY_VERTEX_MARKER = 'v';
const char GEOMETRY_FACE_MARKER = 'f';
const char COMMENT_MARKER = '#';

const size_t MESH_VERTEX_COUNT_DEFAULT = 1024;
const size_t LARGE_MESH_VERTEX_COUNT_DEFAULT = 8192;

size_t count_numbers(const char *line);
size_t trim_comment(char *line);

/* Reads geometry vertices from .obj file and stores it into tightly packed array.
 * Returns number of vertices read.
 */
size_t fread_wavefront_geometry(wavefront_geometry_t *geometry, FILE *restrict file, readmode_t mode) {
    const size_t LINEBUF_SIZE = 256;
    char *linebuf = (char *)malloc(LINEBUF_SIZE * sizeof(char));
    const size_t vertices_count_to_alloc = mode == LARGE
                ? LARGE_MESH_VERTEX_COUNT_DEFAULT
                : MESH_VERTEX_COUNT_DEFAULT;
    float *vertices = (float *)malloc(vertices_count_to_alloc * 4 * sizeof(float));
    face_t *faces = (face_t *)malloc((vertices_count_to_alloc / 3) * sizeof(face_t));
    size_t vw = 0;
    size_t fw = 0;
    while (!feof(file)) {
        char *read_buf = fgets(linebuf, LINEBUF_SIZE, file);
        if (read_buf == NULL) continue;
        if (linebuf[0] == COMMENT_MARKER) continue;
        const size_t len = trim_comment(linebuf);
        if (linebuf[0] == GEOMETRY_VERTEX_MARKER && !isalpha(linebuf[1])) {
            // Now line looks like "v %f %f %f [%f]" and we can parse it with scanf easily.
            float vertex[4] = { 0, 0, 0, 1.0 };
            const size_t vertex_count = count_numbers(linebuf);
            if (vertex_count == 4) {
                sscanf(linebuf, "%*s %f %f %f %f\n",
                        &vertex[0],
                        &vertex[1],
                        &vertex[2],
                        &vertex[3]);
            }
            else {
                sscanf(linebuf, "%*s %f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
            }
            // TODO: handle insufficient memory for holding geometry data (add check if some kind of realloc is needed).
            memcpy(&vertices[vw], vertex, 4*sizeof(float));
            vw += 4;
        }
        if (linebuf[0] == GEOMETRY_FACE_MARKER) {
            const size_t vertices_count = count_numbers(linebuf);
            idx_t *face_vertices = (idx_t *)malloc(vertices_count * sizeof(idx_t));
            size_t j = 0;
            char *caret = &linebuf[1]; // Discarding letter 'f' initially.
            char *rest;
            while (1) {
                const idx_t idx = strtol(caret, &rest, 10);
                if (idx == 0) { // No conversion could be performed.
                    break;
                }
                face_vertices[j++] = idx;
                caret = rest;
            }
            faces[fw].indices = face_vertices;
            faces[fw].vertices_count = j;
            fw++;
        }
    }
    free(linebuf);

    geometry->vertices = vertices;
    geometry->faces = faces;
    geometry->vertices_count = vw / 4;
    geometry->faces_count = fw;
    return vw / 4;
}

wavefront_geometry_t *wavefront_geometry_init() {
    return (wavefront_geometry_t *)malloc(sizeof(wavefront_geometry_t));
}

// Used to properly delete Wavefront geometry object allocated manually or
// using wavefront_geometry_init(...).
void wavefront_geometry_free(wavefront_geometry_t *geometry) {
    if (geometry->vertices != NULL) {
        free(geometry->vertices);
    }
    if (geometry->faces != NULL) {
        for (size_t i = 0; i < geometry->faces_count; i++) {
            free(geometry->faces[i].indices);
        }
        free(geometry->faces);
    }
    free(geometry);
}

size_t fread_wavefront_geometry_normal(wavefront_geometry_t *geometry, FILE *restrict file) {
    return fread_wavefront_geometry(geometry, file, NORMAL);
}

size_t fread_wavefront_geometry_large(wavefront_geometry_t *geometry, FILE *restrict file) {
    return fread_wavefront_geometry(geometry, file, LARGE);
}

// Counts number of numeric values in the string.
size_t count_numbers(const char *line) {
    size_t count = 0;
    size_t i = 0;
    while (i < strlen(line) - 1) {
        if (line[i] == ' ' && isdigit(line[i + 1])) {
            count++;
        }
        i++;
    }
    return count;
}

// Trims comment in the given line (assumed that comment starts with COMMENT_MARKER).
// Returns the length of the trimmed string.
size_t trim_comment(char *line) {
    for (size_t i = 0; i < strlen(line); i++) {
        if (line[i] == COMMENT_MARKER) {
            line[i] = '\0';
            break;
        }
    }
    return strlen(line);
}
