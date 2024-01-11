#include "./../include/wavefront.h"

#include <stdio.h>

const char *files[2] = {
    "./tests/male.obj",
    "./tests/mustang.obj"
};

int main() {
    FILE *f = fopen(files[1], "r");
    if (f == NULL) {
        printf("%s: no such file or directory\n", files[1]);
        return 1;
    }
    WavefrontGeometry *g = wavefront_fread(f);
    const size_t vc = printf("Parsed %ld vertices and %ld faces from %s\n", g->vertices_count, g->faces_count, files[1]);
    fclose(f);
    wavefront_geometry_free(g);

    return 0;
}
