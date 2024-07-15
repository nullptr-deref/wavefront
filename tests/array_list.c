#include "../include/ArrayList.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    const size_t CAP = 10;
    ArrayList list = array_list_init(CAP, sizeof(int));
    for (int i = 0; i < 10; i++) {
        array_list_append(&list, (void *)(&i));
    }

    for (int i = 0; i < 10; i++) {
        int *val = array_list_pop(&list);
        printf("%d ", *val);
        free(val);
    }

    array_list_free(&list);

    return 0;
}
