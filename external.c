#include <stdio.h>

#include "util.h"
#include "external.h"

bool translate_asm_to_obj(const char* src, const char* dst) {
    char cmd[1024];
    snprintf(cmd, 1024, "/usr/bin/as -dynamic %s -o %s", src, dst);
    return exec_cmd(cmd);
}

bool link_object_lib_exec(const char* src, const char* dst) {
    char cmd[1024];
    snprintf(cmd, 1024, "/usr/bin/ld %s -lc -o %s", src, dst);
    return exec_cmd(cmd);
}