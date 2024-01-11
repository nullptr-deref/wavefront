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
WavefrontGeometry *wavefront_fread(FILE *restrict file) {
    const size_t LINEBUF_SIZE = 256;
    char *linebuf = (char *)malloc(LINEBUF_SIZE * sizeof(char));

    WavefrontGeometry *g = wavefront_geometry_init();
    size_t vertices_lookup = 0;
    size_t faces_lookup = 0;
    // Looking up for how much memory we should allocate to store
    // all vertices/faces data.
    while (!feof(file)) {
        char *read_buf = fgets(linebuf, LINEBUF_SIZE, file);
        trim_after(linebuf, "\r\n");
        if (strlen(linebuf) > 2) {
            if (starts_with(linebuf, "v ")) {
                vertices_lookup++;
            }
            if (starts_with(linebuf, "f ")) {
                faces_lookup++;
            }
        }
    }
    rewind(file);

    g->vertices = (float *)malloc(vertices_lookup * 4 * sizeof(float));
    g->faces = (Face *)malloc(faces_lookup * sizeof(Face));
    size_t vw = 0;
    size_t fw = 0;

    StrSplit *face_vertex = str_split_init(3);

    while (!feof(file)) {
        char *read_buf = fgets(linebuf, LINEBUF_SIZE, file);
        if (read_buf == NULL) continue;
        trim_after(linebuf, "\r\n");
        const size_t new_len = trim_comment(linebuf);
        if (new_len < 2) continue;

        if (starts_with(linebuf, "v ")) {
            // Now line looks like "v %f %f %f [%f]" and we can parse it with scanf easily.
            float vertex[4] = { 0, 0, 0, 1.0 };
            const size_t coordinate_count = count_numbers(linebuf);
            if (coordinate_count == 4) {
                sscanf(linebuf, "%*s %f %f %f %f",
                        &vertex[0],
                        &vertex[1],
                        &vertex[2],
                        &vertex[3]);
            }
            else {
                sscanf(linebuf, "%*s %f %f %f", &vertex[0], &vertex[1], &vertex[2]);
            }
            memcpy(&g->vertices[vw], vertex, 4*sizeof(float));
            vw += 4;
        }

        if (starts_with(linebuf, "f ")) {
            Face face;
            const size_t vertices_count = count_words(&linebuf[2]); // Discarding letter 'f' initially.
            face.vertices = (idx_t *)malloc(vertices_count * 3 * sizeof(idx_t));
            StrSplit *splitted = split(&linebuf[2], " ");
            for (size_t i = 0; i < splitted->len; i++) {
                if (strlen(splitted->items[i]) != 0) {
                    str_split_clear(face_vertex);
                    split_no_alloc(face_vertex, splitted->items[i], "/");
                    for (size_t j = 0; j < face_vertex->len; j++) {
                        if (strlen(face_vertex->items[j]) == 0) {
                            face.vertices[i*3 + j] = 0;
                            continue;
                        }
                        face.vertices[i*3 + j] = atoll(face_vertex->items[j]);
                    }
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

WavefrontGeometry *wavefront_geometry_init() {
    WavefrontGeometry *g = (WavefrontGeometry *)malloc(sizeof(WavefrontGeometry));
    g->vertices = NULL;
    g->faces = NULL;
    return g;
}

void wavefront_geometry_free(WavefrontGeometry *geometry) {
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
