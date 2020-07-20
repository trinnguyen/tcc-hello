#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
    char buf[1024];
    snprintf(buf, 1024, "-o %s %s", dst, src);
    return exec_cmd("/usr/bin/as", buf);
}

bool link_object_lib_exec(const char* src, const char* dst) {
    char *crt = getenv("TCC_RT_OBJ");
    if (crt == NULL) {
        error("Missing TCC_RT_OBJ environment variable\n");
        return false;
    }

    char buf[1024];
    snprintf(buf, 1024, "-static -o %s %s %s", dst, src, crt);
    return exec_cmd("/usr/bin/ld", buf);
}