#include <stdio.h>
#include <string.h>

#include "util.h"
#include "lexer.h"
#include "scanner.h"
#include "semantics.h"
#include "codegen.h"

static void show_help();
static bool exec_compiler(const char* filename);

bool opt_verbose_enabled;

int main(int argc, char const *argv[]) {
    // error -> show help
    if (argc <= 1) {
        error("error: no input files\n\n");
        show_help();
        return 1;
    }

    // simple way to parse the options and input files
    const char *file;
    for (size_t i = 1; i < argc; i++) {
        // option
        char const *cur = argv[i];
        if (cur[0] == '-') {
            if (is_str_equals(cur, "-h")) {
                show_help();
                return 0;
            }

            if (is_str_equals(cur, "-v")) {
                opt_verbose_enabled = true;
            }
        } else {
            // file
            file = cur;
        }
    }
    
    // invoke process
    if (exec_compiler(file))
        return 0;

    return 1;
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
static bool exec_compiler(const char* filename) {
    // open file
    FILE *f = fopen(filename, "r");

    // scan
    ast_prog *ast = scan(f);
    fclose(f);
    f = NULL;

    // close
    if (ast == NULL) {
        fprintf(stderr, "Failed to parse the source code\n");
        return false;
    }
    print_ast(ast);

    // semantics
    if (!validate(ast)) {
        free_ast(ast);
        return false;
    }

    // codegen
    const char *asm_name = "a.out.asm";
    FILE *fout = fopen(asm_name, "w");
    bool gen = generate_asm(ast, fout);
    if (gen) {
        printf("Generate ASM into: %s\n", asm_name);
    }

    fclose(fout);
    fout = NULL;

    // finish
    free_ast(ast);
    return gen;
}