#include <stdio.h>
#include "intern.h"

ast_prog *parse(FILE *f);

void print_ast(ast_prog *ast);

void free_ast(ast_prog *node);

void print_ast(ast_prog *ast);