#include <stdio.h>

#include "util.h"
#include "external.h"

static char is_macos() {
    #ifdef __APPLE__
        return true;
    #endif

    return false;
}

static char is_linux() {
    #ifdef __linux__
        return true;
    #endif

    return false;
}

bool translate_asm_to_obj(const char* src, const char* dst) {
    const char *format = is_macos() ? "macho64" : (is_linux() ? "elf64" : NULL);
    if (format == NULL)
        return false;

    char args[1024];
    snprintf(args, 1024, "%s -f %s -o %s", src, format, dst);
    return exec_cmd("nasm", args);
}

bool link_object_lib_exec(const char* src, const char* dst) {
    char args[1024];
    snprintf(args, 1024, "%s -lc -o %s", src, dst);
    return exec_cmd("/usr/bin/ld", args);
}