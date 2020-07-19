#include <stdio.h>
#include "codegen.h"

FILE *cfout;

static void gen_header() {
    fprintf(cfout, "    global  _main\n");
    fprintf(cfout, "    section .text\n\n");
}

static void gen_expr(node_expr expr) {
    if (expr.expr_type == expr_type_const_int) {
        fprintf(cfout, "	push %d\n", expr.const_int);
    }
}

static void gen_stmt(node_stmt *stmt) {
    if (stmt->stmt_type == stmt_type_return) {

        // gen expr
        gen_expr(stmt->return_exprr);

        // pop last value to rax
        fprintf(cfout, "	pop	rax\n");
        fprintf(cfout, "	ret\n");
    }
}

static void gen_func(node_func_decl *decl) {
    // labe;
    fprintf(cfout, "_%s:\n", decl->name);

    // statement
    if (decl->func_stmts != NULL) {
        list_node *node = decl->func_stmts->head;
        while (node != NULL) {
            gen_stmt(node->value);
            node = node->next;
        }
    }
}

bool generate_asm(ast_prog *ast, FILE *fout) {
    // cache
    cfout = fout;

    // header
    gen_header();
    
    // functions
    list_node *node = ast->func_decls->head;
    while (node != NULL) {
        gen_func(node->value);
        node = node->next;
    }

    // clear
    cfout = NULL;
    return true;
}