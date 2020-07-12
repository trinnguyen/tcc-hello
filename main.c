#include <stdio.h>
#include <string.h>

#include "util.h"
#include "lexer.h"

static void show_help();
static void exec_compiler(const char* filename);

static bool verbose_enabled;

int main(int argc, char const *argv[]) {
    // error -> show help
    if (argc <= 1) {
        error("error: no input files\n\n");
        show_help();
        return 1;
    }

    // simple way to parse the options and input files
    char *file;
    for (size_t i = 1; i < argc; i++) {
        // option
        char const *cur = argv[i];
        if (cur[0] == '-') {
            if (is_str_equals(cur, "-h")) {
                show_help();
                return 0;
            }

            if (is_str_equals(cur, "-v")) {
                verbose_enabled = true;
            }
        } else {
            // file
            file = cur;
        }
    }
    
    // invoke process
    exec_compiler(file);

    return 0;
}

static void show_help() {
    printf("tcc - A toy C compiler for ANSI C\n");
    printf("\n");

    printf("USAGE:\n");
    printf("\ttcc [options] filename ...\n");
    printf("\tOptions:\n");
    printf("\t\t-h  Show this help information\n");
    printf("\t\t-v  Verbose output\n");

    printf("EXAMPLE:\n");
    printf("\ttcc hello.c\n");
}

/*
 * invoke compiler process:
 * lexer -> parser -> code generator (asm) -> assembler (nasm) -> linker (GNU ld)
 */
static void exec_compiler(const char* filename) {
    // open file
    FILE *f = fopen(filename, "r");

    int t = -1;
    // run lexer
    if (verbose_enabled)
        info("---- Lexer\n");
        
    while ((t = get_token(f)) != TOK_EOF) {
        if (verbose_enabled) {
            switch (t) {
                case TOK_IDENTIFIER:
                    printf("Identifier: %s\n", identifier_str);
                    break;
                case TOK_INT:
                    printf("Keyword: int\n");
                    break;
                case TOK_RETURN:
                    printf("Keyword: return\n");
                    break;
                case TOK_NUM:
                    printf("Number: %d\n", int_val);
                    break;
                case TOK_SEMI_COLON:
                    printf(";\n");
                    break;
                case TOK_OPEN_PARENT:
                    printf("(\n");
                    break;
                case TOK_CLOSE_PARENT:
                    printf(")\n");
                    break;
                case TOK_OPEN_BRACKET:
                    printf("{\n");
                    break;
                case TOK_CLOSE_BRACKET:
                    printf("}\n");
                    break;
                case TOK_EOF:
                    printf("End of file\n");
                    break;
                default:
                    printf("Unexpected token\n: %c", t);
                    break;
            }
        }
    }
    

    // close
    fclose(f);
}