#include "./../include/wavefront.h"

#include <stdio.h>

const char *files[3] = {
    "./tests/male.obj",
    "./tests/mustang.obj",
    "./tests/sphere.obj"
};

int main() {
    const char *filename = files[1];
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("%s: no such file or directory\n", filename);
        return 1;
    }
    WavefrontGeometry *g = wavefront_fread(f);
    printf("File %s: %ld vertices, %ld faces\n\n", filename, g->vertices_count, g->faces_count);
    if (g->objects_count > 0) {
        for (size_t i = 0; i < g->objects_count; i++) {
            printf("Object %ld:%s\n\toffset = %ld;\n\tlen = %ld\n", i, g->objects[i].name, g->objects[i].owned_faces.offset, g->objects[i].owned_faces.len);
        }
    }
    /*
    for (size_t i = 0; i < g->faces_count; i++) {
        for (size_t j = 0; j < g->faces[i].vertices_count; j++) {
            printf("%ld ", g->faces[i].vertices_data[j]);
        }
        puts("");
    }
    */
    fclose(f);
    wavefront_geometry_free(g);

    return 0;
}
