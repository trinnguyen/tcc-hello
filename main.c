#include <stdio.h>
#include <string.h>

#include "util.h"
#include "lexer.h"
#include "parser.h"
#include "semantics.h"
#include "codegen.h"
#include "external.h"

static void show_help();
static bool exec_compiler(const char* filename, const char *output);

bool opt_verbose_enabled;

int main(int argc, char const *argv[]) {
    // error -> show help
    if (argc <= 1) {
        error("error: no input files\n\n");
        show_help();
        return 1;
    }

    // simple way to parse the options and input files
    const char *file = NULL;
    const char *out = NULL;
    bool arg_has_out;
    for (size_t i = 1; i < argc; i++) {
        char const *cur = argv[i];

        // exp out
        if (arg_has_out) {
            if (cur[0] == '-') {
                error("Missing output file value\n");
                return 0;
            }

            out = cur;
            arg_has_out = false;
            continue;
        }

        // option
        if (cur[0] == '-') {
            if (is_str_equals(cur, "-h")) {
                show_help();
                return 0;
            }

            if (is_str_equals(cur, "-v")) {
                opt_verbose_enabled = true;
            }

            if (is_str_equals(cur, "-o")) {
                arg_has_out = true;
            }
        } else {
            file = cur;
        }
    }

    // verify
    if (file == NULL) {
        error("error: no input files\n");
        return 0;
    }

    if (out == NULL) {
        out = "a.out";
    }
    
    // invoke process
    if (exec_compiler(file, out))
        return 0;

    return 1;
}

static void show_help() {
    printf("tcc - A toy C compiler for ANSI C\n");
    printf("\n");

    printf("USAGE:\n");
    printf("\ttcc [options] filename ...\n");
    printf("\tOptions:\n");
    printf("\t\t-o <name>  Output filename. Default is a.out\n");
    printf("\t\t-h         Show this help information\n");
    printf("\t\t-v         Verbose output\n");

    printf("EXAMPLE:\n");
    printf("\ttcc hello.c\n");
}

/*
 * invoke compiler process:
 * lexer -> parser -> code generator (asm) -> assembler (nasm) -> linker (GNU ld)
 */
static bool exec_compiler(const char* filename, const char *output) {
    // open file
    FILE *f = fopen(filename, "r");

    // scan
    ast_prog *ast = parse(f);
    fclose(f);
    f = NULL;

    // close
    if (ast == NULL) {
        fprintf(stderr, "Failed to parse the source code\n");
        return false;
    }

    // print parser if need
    if (opt_verbose_enabled)
        print_ast(ast);

    // semantics
    if (!validate(ast)) {
        free_ast(ast);
        return false;
    }

    // codegen
    char asm_name[256];
    snprintf(asm_name, 256, "%s.s", output);
    FILE *fout = fopen(asm_name, "w");
    bool gen = generate_asm(ast, fout);
    free_ast(ast);

    if (gen) {
        printf("Generate ASM into: %s\n", asm_name);
    } else {
        error("Failed to generate ASM\n");
        return false;
    }

    fclose(fout);
    fout = NULL;

    // assembler
    char obj_fname[256];
    snprintf(obj_fname, 256, "%s.o", output);
    if (translate_asm_to_obj(asm_name, obj_fname)) {
        printf("Translate assembly code to object code: '%s' using system assembler (as)\n", obj_fname);
    } else {
        error("Failed to translate assembly code using system assembler (as)\n");
        return false;
    }

    // linker
    if (link_object_lib_exec(obj_fname, output)) {
        printf("Link object file to output: '%s' using system linker (ld)\n", output);
    } else {    
        printf("Failed to link object file using system linker (ld)\n");
    }

    return true;
}