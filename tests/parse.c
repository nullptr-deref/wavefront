#include "./../include/wavefront.h"

#include <stdio.h>

const char *fname = "./tests/male.obj";
int main() {
    FILE *f = fopen(fname, "r");
    if (f == NULL) {
        printf("%s: no such file or directory\n", fname);
        return 1;
    }
    wavefront_geometry_t *g = wavefront_fread(f);
    const size_t vc = printf("Parsed %ld vertices and %ld faces from %s\n", g->vertices_count, g->faces_count, fname);
    fclose(f);
    wavefront_geometry_free(g);

    return 0;
}
