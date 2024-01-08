#include "./../include/wavefront.h"

#include <stdio.h>

const char *fname = "./tests/male.obj";
int main() {
    FILE *f = fopen(fname, "r");
    if (f == NULL) {
        printf("%s: no such file or directory\n", fname);
        return 1;
    }
    wavefront_geometry_t *g;
    const size_t vc = wavefront_fread(g, f);
    printf("Parsed %ld vertices and %ld faces from %s\n", vc, g->faces_count, fname);
    fclose(f);

    return 0;
}
