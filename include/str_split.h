#ifndef SPLIT_LIST_H
#define SPLIT_LIST_H

#include <stddef.h>

typedef struct {
    char **items;
    size_t cap;
    size_t len;
} str_split;

str_split *str_split_init(size_t cap);
void       str_split_clear(str_split *ss);
void       str_split_free(str_split *ss);

void       str_split_append(str_split *ss, char *item);

str_split *split(const char *line, const char *sep);

#endif
