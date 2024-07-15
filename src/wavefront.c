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
    size_t vertices_total_count = 0;
    size_t faces_total_count = 0;

    size_t objects_total_count = 0;
    size_t current_obj_id = 0;
    size_t current_face_offset = 0;
    size_t prev_face_offset = 0;
    // Looking up for how much memory we should allocate to store
    // all vertices/faces data.
    while (!feof(file)) {
        char *read_buf = fgets(linebuf, LINEBUF_SIZE, file);
        trim_after(read_buf, "\r\n");
        read_buf = first_nonspace(read_buf); // trimming tabs/spaces/etc at the start of the line (affects further parsing).
        if (strlen(read_buf) > 2) {
            if (starts_with(read_buf, "v ")) {
                vertices_total_count++;
            }
            if (starts_with(read_buf, "f ")) {
                faces_total_count++;
            }
            if (starts_with(read_buf, "o ")) {
                objects_total_count++;
            }
        }
    }
    rewind(file);

    g->objects = objects_total_count > 0
        ? (Object *)malloc(objects_total_count * sizeof(Object))
        : NULL;
    g->objects_count = objects_total_count;

    // TODO: safety check to protect against allocating 0 memory for vertices/faces.
    g->vertices = (float *)malloc(vertices_total_count * 4 * sizeof(float));
    g->faces = (Face *)malloc(faces_total_count * sizeof(Face));

    size_t vw = 0;
    size_t fw = 0;

    StrSplit *face_vertex = NULL;

    // Actual parsing
    while (!feof(file)) {
        char *read_buf = fgets(linebuf, LINEBUF_SIZE, file);
        if (read_buf == NULL) continue;
        const size_t new_len = trim_comment(read_buf);
        trim_after(read_buf, "\r\n");
        trim_trailing_spaces(read_buf);
        read_buf = first_nonspace(read_buf); // trimming tabs/spaces/etc at the start of the line (affects further parsing).
        if (strlen(read_buf) < 2) continue;

        if (starts_with(read_buf, "v ")) {
            // Now line looks like "v %f %f %f [%f]" and we can parse it with scanf easily.
            float vertex[4] = { 0, 0, 0, 1.0 };
            const size_t coordinate_count = count_numbers(read_buf);
            if (coordinate_count == 4) {
                sscanf(read_buf, "%*s %f %f %f %f",
                        &vertex[0],
                        &vertex[1],
                        &vertex[2],
                        &vertex[3]);
            }
            else {
                sscanf(read_buf, "%*s %f %f %f", &vertex[0], &vertex[1], &vertex[2]);
            }
            memcpy(&g->vertices[vw], vertex, 4*sizeof(float));
            vw += 4;
        }

        if (starts_with(read_buf, "f ")) {
            Face face;
            const size_t vertices_count = count_words(&read_buf[2]); // Discarding letter 'f' initially.
            face.vertices_data = (idx_t *)malloc(vertices_count * 3 * sizeof(idx_t));
            face.vertices_count = vertices_count;
            StrSplit *splitted = split(&read_buf[2], " ");
            for (size_t i = 0; i < splitted->len; i++) {
                if (strlen(splitted->items[i]) >= 1) {
                    if (face_vertex == NULL) {
                        face_vertex = split(splitted->items[i], "/");
                    }
                    else {
                        str_split_clear(face_vertex);
                        split_no_alloc(face_vertex, splitted->items[i], "/");
                    }
                    for (size_t j = 0; j < face_vertex->len; j++) {
                        if (strlen(face_vertex->items[j]) == 0) {
                            face.vertices_data[i*3 + j] = 0;
                        }
                        else face.vertices_data[i*3 + j] = atoll(face_vertex->items[j]);
                    }
                }
            }
            g->faces[fw++] = face;
            current_face_offset++;
            str_split_free(splitted);
        }

        if (starts_with(read_buf, "o ")) {
            const size_t name_beginning = 2 + strspn(&read_buf[2], " ");
            const size_t name_len = strlen(&read_buf[name_beginning]);
            g->objects[current_obj_id].name = alloc_string(name_len);
            memcpy(g->objects[current_obj_id].name, &read_buf[name_beginning], name_len);
            g->objects[current_obj_id].id = current_obj_id;
            g->objects[current_obj_id].owned_faces.offset = current_face_offset;
            if (current_obj_id != 0) {
                g->objects[current_obj_id - 1].owned_faces.len = current_face_offset - prev_face_offset;
            }
            if (current_obj_id == objects_total_count - 1) {
                g->objects[current_obj_id].owned_faces.len = faces_total_count - current_face_offset;
            }
            prev_face_offset = current_face_offset;
            current_obj_id++;
        }
    }
    free(linebuf);
    str_split_free(face_vertex);

    g->vertices_count = vertices_total_count;
    g->faces_count = faces_total_count;
    return g;
}

WavefrontGeometry *wavefront_geometry_init() {
    WavefrontGeometry *g = (WavefrontGeometry *)malloc(sizeof(WavefrontGeometry));
    g->vertices = NULL;
    g->faces = NULL;
    g->objects = NULL;
    g->groups = NULL;
    return g;
}

void wavefront_geometry_free(WavefrontGeometry *geometry) {
    if (geometry->vertices != NULL) {
        free(geometry->vertices);
    }
    if (geometry->faces != NULL) {
        for (size_t i = 0; i < geometry->faces_count; i++) {
            free(geometry->faces[i].vertices_data);
        }
        free(geometry->faces);
    }
    if (geometry->objects != NULL) {
        for (size_t i = 0; i < geometry->objects_count; i++) {
            free(geometry->objects[i].name);
        }
        free(geometry->objects);
    }
    if (geometry->groups != NULL) {
        free(geometry->groups);
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
