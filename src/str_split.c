#include "./../include/str_split.h"

#include "./../include/strutil.h"

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

str_split *str_split_init(size_t cap) {
    str_split *s = (str_split *)malloc(sizeof(str_split));
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

void str_split_free(str_split *ss) {
    if (ss->len > 0 && ss->items != NULL) {
        for (size_t i = 0; i < ss->len; i++)
            free(ss->items[i]);
        free(ss->items);
    }
    free(ss);
}

void str_split_append(str_split *ss, char *item) {
    assert(ss->len < ss->cap);
    if (ss->items[ss->len] != NULL) {
        const size_t old_len = strlen(ss->items[ss->len]);
        const size_t new_len = strlen(item);
        if (new_len > old_len) {
            free(ss->items[ss->len]);
        }
        else {
            memcpy(ss->items, item, new_len);
            ss->items[new_len] = '\0';
        }
    }
    ss->items[ss->len++] = item;
}

void str_split_clear(str_split *ss) {
    ss->len = 0;
}

// Splits given string into substrings.
// Return str_split with unmodified string if no
// separators were found.
str_split *split(const char *line, const char *sep) {
    const size_t delim_count = count_delimiters(line, sep);
    if (delim_count == 0) {
        str_split *ss_single = str_split_init(1);
        char *copy = alloc_string(strlen(line));
        strcpy(copy, line);
        str_split_append(ss_single, copy);
        return ss_single;
    }
    const size_t words_count = delim_count + 1;
    const char *start = line;
    str_split *ss = str_split_init(words_count);
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
