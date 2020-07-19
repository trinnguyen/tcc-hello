#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "util.h"

void error(const char *str) {
    fprintf(stderr, "%s", str);
}

void info(const char *str) {
    printf("%s", str);
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

bool get_full_path(char *buf, const char *cmd) {
    char *path = getenv("PATH");
    char *token = strtok(path, ":");
    while (token != NULL) {
        snprintf(buf, 1024, "%s/%s", token, cmd);
        if (access(buf, F_OK) == 0) {
            return true;
        }

        token = strtok(NULL, ":");
    }

    return false;
}

bool exec_cmd(const char *cmd, const char *args) {
    char buf[1024];
    if (cmd[0] == '/') {
        strncpy(buf, cmd, sizeof(buf));
    } else {
        if (!get_full_path(buf, cmd)) {
            fprintf(stderr, "Failed to find tool: %s", cmd);
            return false;
        }
    }

    char tmp[1024];
    snprintf(tmp, 1024, "%s %s", buf, args);
    printf("Executing: %s\n", tmp);
    FILE *f = popen(cmd, "r");
    if (f != NULL) {
        int ret = fclose(f);
        return ret == 0;
    }

    return false;
}