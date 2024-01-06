#include "./../include/strutil.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

size_t count_delimiters(const char *line, const char *delim) {
    size_t count = 0;
    for (size_t i = 0; i < strlen(line); i++) {
        for (size_t j = 0; j < strlen(delim); j++) {
            if (line[i] == delim[j])
                count++;
        }
    }
    return count;
}

void trim_after(char *line, const char *trimmer) {
    char *found = strpbrk(line, trimmer);
    if (found != NULL) {
        *found = '\0';
    }
}

char *alloc_string(size_t len) {
    char *mem = (char *)malloc((1 + len) * sizeof(char));
    mem[len] = '\0';
    return mem;
}

char *alloc_string_or_empty(size_t len) {
    if (len == 0) {
        char *res = (char *)malloc(sizeof(char));
        res[0] = '\0';
        return res;
    }
    else return alloc_string(len);
}

size_t count_words(const char *line) {
    return count_delimiters(line, " ") + 1;
}

// Counts number of numeric values in the string.
size_t count_numbers(const char *line) {
    size_t count = 0;
    size_t i = 0;
    while (i < strlen(line) - 1) {
        if ((line[i] == ' ' || line[i] == '-') && isdigit(line[i + 1])) {
            count++;
        }
        i++;
    }
    return count;
}
