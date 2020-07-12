#include <stdio.h>
#include <string.h>

#include "util.h"

void error(const char *str) {
    fprintf(stderr, str);
}

void info(const char *str) {
    printf(str);
}

void reset_str(char *str) {
    if (str == NULL)
        return;

    size_t len = strlen(str);
    for (size_t i = 0; i < len; ++i) {
        str[i] = 0;
    }
}

bool is_str_equals(const char *str1, const char *str2) {
    return strcmp(str1, str2) == 0;
}