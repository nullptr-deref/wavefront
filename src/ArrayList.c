#include "../include/ArrayList.h"

#include <assert.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

ArrayList array_list_init(size_t cap, size_t itemsz) {
    ArrayList al;
    al.capacity = cap;
    al.len = 0;
    al.item_size = itemsz;
    al.data = (void *)malloc(al.item_size * cap);
    return al;
}

void array_list_append(ArrayList *list, void *val) {
    assert(list->len + 1 <= list->capacity);
    memcpy(&list->data[list->len*list->item_size], val, list->item_size);
    ++list->len;
}

void *array_list_pop(ArrayList *list) {
    assert(list->len > 0);
    void *item = malloc(list->item_size);
    memcpy(item, &list->data[(list->len-1)*list->item_size], list->item_size);
    --list->len;

    return item;
}

void array_list_free(ArrayList *restrict list) {
    if (list != NULL) {
        if (list->data != NULL) {
            free(list->data);
        }
    }
}
