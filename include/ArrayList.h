#include <stddef.h>

typedef struct {
    size_t  capacity;
    size_t  len;
    void   *data;
    size_t  item_size; // size of one element of the list in bytes
} ArrayList;

ArrayList array_list_init(size_t cap, size_t itemsz);
void array_list_free(ArrayList *restrict list);

void array_list_append(ArrayList *list, void *val);

// Returns the last element from the list.
// Caller owns the memory of the returned pointer and
// thus is responsible for freeing it afterwards.
void *array_list_pop(ArrayList *list);
