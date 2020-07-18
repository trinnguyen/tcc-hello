#include <stdbool.h>

void error(const char *str);

void info(const char *str);

void reset_str(char *str);

bool is_str_equals(const char *str1, const char *str2);

bool exec_cmd(const char *cmd);