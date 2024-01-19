#ifndef STRUTIL_H
#define STRUTIL_H

#include <stddef.h>

size_t count_delimiters(const char *line, const char *delim);
size_t count_words(const char *line);
size_t count_numbers(const char *line);

int starts_with(const char *line, const char *prefix);

void trim_after(char *line, const char *trimmer);
void trim_trailing_spaces(char *line);

char *alloc_string(size_t len);
char *alloc_string_or_empty(size_t len);

#endif
