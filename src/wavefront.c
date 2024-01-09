#include "./../include/wavefront.h"
#include "./../include/str_split.h"
#include "./../include/strutil.h"

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

const char GEOMETRY_VERTEX_MARKER = 'v';
const char GEOMETRY_FACE_MARKER = 'f';
const char COMMENT_MARKER = '#';

size_t trim_comment(char *line);

/* Reads geometry vertices from .obj file and stores it into tightly packed array.
 * Can cause realloc (or multiple) if called to read too big object file.
 * See wavefront_read_normal() and wavefront_read_large().
 *
 * Returns number of vertices read.
 */
wavefront_geometry_t * wavefront_fread(FILE *restrict file) {
    wavefront_geometry_t *g = wavefront_geometry_init();
    const size_t LINEBUF_SIZE = 256;
    char *linebuf = (char *)malloc(LINEBUF_SIZE * sizeof(char));
    size_t vertices_lookup = 0;
    size_t faces_lookup = 0;
    // Looking up for how much memory we should allocate to store
    // all vertices/faces data.
    while (!feof(file)) {
        char *read_buf = fgets(linebuf, LINEBUF_SIZE, file);
        if (strlen(linebuf) < 2) continue;
        if (starts_with(linebuf, "v ")) {
            vertices_lookup++;
        }
        if (starts_with(linebuf, "f ")) {
            faces_lookup++;
        }
    }
    rewind(file);
    g->vertices = (float *)malloc(vertices_lookup * 4 * sizeof(float));
    g->faces = (face_t *)malloc(faces_lookup * sizeof(face_t));
    size_t vw = 0;
    size_t fw = 0;

    str_split *face_vertex = str_split_init(3);

    while (!feof(file)) {
        char *read_buf = fgets(linebuf, LINEBUF_SIZE, file);
        if (read_buf == NULL) continue;
        if (linebuf[0] == COMMENT_MARKER) continue;
        const size_t len = trim_comment(linebuf);
        trim_after(linebuf, "\r\n");

        if (linebuf[0] == GEOMETRY_VERTEX_MARKER && !isalpha(linebuf[1])) {
            // Now line looks like "v %f %f %f [%f]" and we can parse it with scanf easily.
            float vertex[4] = { 0, 0, 0, 1.0 };
            const size_t coordinate_count = count_numbers(linebuf);
            if (coordinate_count == 4) {
                sscanf(linebuf, "%*s %f %f %f %f\n",
                        &vertex[0],
                        &vertex[1],
                        &vertex[2],
                        &vertex[3]);
            }
            else {
                sscanf(linebuf, "%*s %f %f %f\n", &vertex[0], &vertex[1], &vertex[2]);
            }
            memcpy(&g->vertices[vw], vertex, 4*sizeof(float));
            vw += 4;
        }

        if (linebuf[0] == GEOMETRY_FACE_MARKER) {
            face_t face;
            const size_t vertices_count = count_words(&linebuf[2]); // Discarding letter 'f' initially.
            face.vertices = (idx_t *)malloc(vertices_count * 3 * sizeof(idx_t));
            str_split *splitted = split(&linebuf[2], " ");
            for (size_t i = 0; i < splitted->len; i++) {
                if (strlen(splitted->items[i]) == 0) continue;
                for (size_t j = 0; j < vertex->len; j++) {
                    str_split_clear(vertex);
                    vertex = split(splitted->items[i], "/");
                    if (strlen(vertex->items[j]) == 0) {
                for (size_t j = 0; j < face_vertex->len; j++) {
                    if (strlen(face_vertex->items[j]) == 0) {
                        face.vertices[i*3 + j] = 0;
                        continue;
                    }
                    face.vertices[i*3 + j] = atoll(face_vertex->items[j]);
                }
            }
            g->faces[fw++] = face;
            str_split_free(splitted);
        }
    }
    free(linebuf);
    str_split_free(face_vertex);

    g->vertices_count = vertices_lookup;
    g->faces_count = faces_lookup;
    return g;
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
            free(geometry->faces[i].vertices);
        }
        free(geometry->faces);
    }
    free(geometry);
}

// Trims comment in the given line (assumed that comment starts with COMMENT_MARKER).
// Returns the length of the trimmed string.
size_t trim_comment(char *line) {
    const char trimmer[2] = { COMMENT_MARKER, '\0' };
    trim_after(line, trimmer);
    return strlen(line);
}
