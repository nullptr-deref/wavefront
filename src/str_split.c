#include "./../include/str_split.h"

#include "./../include/strutil.h"

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

StrSplit *str_split_init(size_t cap) {
    StrSplit *s = (StrSplit *)malloc(sizeof(StrSplit));
    s->items = (char **)malloc(cap * sizeof(char *));
    for (size_t i = 0; i < cap; i++) {
        // Nullifying for the sake of
        // proper str_split_clear work.
        s->items[i] = NULL;
    }
    s->len = 0;
    s->cap = cap;
    return s;
}

void str_split_free(StrSplit *ss) {
    for (size_t i = 0; i < ss->cap; i++) {
        if (ss->items[i] != NULL) {
            free(ss->items[i]);
        }
    }
    free(ss->items);
    free(ss);
}

void str_split_append(StrSplit *ss, char *item) {
    assert(ss->len < ss->cap);
    ss->items[ss->len++] = item;
}

void str_split_clear(StrSplit *ss) {
    ss->len = 0;
}

// Splits given string into substrings.
// Return StrSplit with unmodified string if no
// separators were found.
StrSplit *split(const char *line, const char *sep) {
    const size_t delim_count = count_delimiters(line, sep);
    if (delim_count == 0) {
        StrSplit *ss_single = str_split_init(1);
        char *copy = alloc_string(strlen(line));
        strcpy(copy, line);
        str_split_append(ss_single, copy);
        return ss_single;
    }
    const size_t words_count = delim_count + 1;
    const char *start = line;
    StrSplit *ss = str_split_init(words_count);
    for (size_t i = 0; i < words_count; i++) {
        size_t chunk_len = strcspn(start, sep);
        char *new_item = alloc_string_or_empty(chunk_len);
        memcpy(new_item, start, chunk_len);
        str_split_append(ss, new_item);
        start += chunk_len + 1;
        // If we replace the line above with the following 2 lines, then all empty
        // substrings are placed at the end of the list.
        // const size_t skip = strspn(start, sep);
        // start += skip;
    }
    return ss;
}

// Puts values into previously cleared string split object.
// Supposes that string split object was cleared via str_split_clear() beforehand.
void split_no_alloc(StrSplit *ss, const char *line, const char *sep) {
    assert(ss->len == 0);
    const size_t delim_count = count_delimiters(line, sep);
    if (delim_count == 0) {
        char *copy = alloc_string(strlen(line));
        str_split_append(ss, copy);
        return;
    }
    const size_t words_count = delim_count + 1;
    const char *start = line;
    for (size_t i = 0; i < words_count; i++) {
        const size_t chunk_len = strcspn(start, sep);
        if (ss->items[ss->len] != NULL) {
            const size_t old_len = strlen(ss->items[ss->len]);
            if (chunk_len >= old_len) {
                free(ss->items[ss->len]);
                ss->items[ss->len] = alloc_string_or_empty(chunk_len);
                memcpy(ss->items[ss->len], start, chunk_len);
            }
            else {
                memcpy(ss->items[ss->len], start, chunk_len);
                ss->items[ss->len][chunk_len] = '\0';
            }
            ss->len++;
        }
        start += chunk_len + 1;
        // If we replace the line above with the following 2 lines, then all empty
        // substrings are placed at the end of the list.
        // const size_t skip = strspn(start, sep);
        // start += skip;
    }
}
