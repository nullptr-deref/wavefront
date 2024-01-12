#include "./../include/wavefront.h"

#include <stdio.h>

const char *files[3] = {
    "./tests/male.obj",
    "./tests/mustang.obj",
    "./tests/sphere.obj"
};

int main() {
    FILE *f = fopen(files[1], "r");
    if (f == NULL) {
        printf("%s: no such file or directory\n", files[1]);
        return 1;
    }
    WavefrontGeometry *g = wavefront_fread(f);
    printf("Parsed %ld objects containing %ld vertices, %ld faces from %s\n", g->objects_count, g->vertices_count, g->faces_count, files[1]);
    for (size_t i = 0; i < g->objects_count; i++) {
        printf("Object %ld:%s\n\toffset = %ld;\n\tlen = %ld\n", i, g->objects[i].name, g->objects[i].owned_vertices.offset, g->objects[i].owned_vertices.len);
    }
    fclose(f);
    wavefront_geometry_free(g);

    return 0;
}
