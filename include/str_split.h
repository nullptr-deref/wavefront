#ifndef SPLIT_LIST_H
#define SPLIT_LIST_H

#include <stddef.h>

typedef struct {
    char **items;
    size_t cap;
    size_t len;
} StrSplit;

StrSplit  *str_split_init(size_t cap);
void       str_split_clear(StrSplit *ss);
void       str_split_free(StrSplit *ss);

void       str_split_append(StrSplit *ss, char *item);

StrSplit *split(const char *line, const char *sep);
void split_no_alloc(StrSplit *ss, const char *line, const char *sep);

#endif
