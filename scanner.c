#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "util.h"

#include "scanner.h"

static void print_token(int t);

FILE *lf;
int cur_token;

static int next_token() {
    cur_token = get_token(lf);
    if (opt_verbose_enabled) {
        printf("\ttoken: ");
        print_token(cur_token);
    }

    return cur_token;
}

static bool expect(int tok) {
    bool res = cur_token == tok;
    if (!res) {
        fprintf(stderr, "Expected %s", token_name(tok));
    }

    return res;
}

static bool consume(int tok) {
    if (!(expect(tok))) {
        return false;    
    }

    next_token();
    return true;
}

static void free_node_stmt(void *pt) {
    if (pt != NULL) {
        node_stmt *stmt = (node_stmt *)pt;
    }
}

static void free_func_decl(void *pt) {
    if (pt != NULL) {
        node_func_decl *decl = (node_func_decl *)pt;
        free(decl->name);
        decl->name = NULL;

        if (decl->func_stmts != NULL) {
            list_free(decl->func_stmts);
            decl->func_stmts = NULL;
        }

        free(pt);
    }
}

static bool is_stmt() {
    return cur_token == TOK_RETURN;
}

node_stmt *parse_stmt() {
    if (!consume(TOK_RETURN)) {
        return NULL;
    }

    // expect number
    if (!consume(TOK_NUM)) {
        return NULL;
    }
    int val = lex_int_val;

    if (!consume(TOK_SEMI_COLON)) {
        return NULL;
    }

    node_stmt *stmt = malloc(sizeof(node_stmt));
    stmt->stmt_type = stmt_type_return;
    stmt->return_exprr.const_int = val;
    stmt->return_exprr.expr_type = expr_type_const_int;
    return stmt;
}

linked_list *parse_stmts() {
    if (!is_stmt())
        return NULL;

    linked_list *list = list_init(free_node_stmt);
    while (is_stmt()) {
        node_stmt *stmt = parse_stmt();
        if (stmt == NULL) {
            list_free(list);
            list = NULL;
            return NULL;
        }

        list_append(list, stmt);
    }
    
    return list;
}

static bool is_function() {
    return cur_token == TOK_INT || cur_token == TOK_VOID;
}

node_func_decl *parse_function() {
    if (!is_function()) {
        return NULL;
    }

    // return type
    data_type ret_type;
    if (!expect(TOK_INT)) {
        return NULL;
    }
    ret_type = data_type_int;
    next_token();
    
    // expect name
    if (!expect(TOK_IDENTIFIER)) {
        return NULL;
    }

    node_func_decl *decl = malloc(sizeof(node_func_decl));
    decl->return_type = ret_type;
    decl->name = strdup(lex_id_str);

    next_token();

    // expect open
    if (!consume(TOK_OPEN_PARENT)) {
        free_func_decl(decl);
        return NULL;
    }

    if (!consume(TOK_CLOSE_PARENT)) {
        free_func_decl(decl);
        return NULL;
    }


    if (!consume(TOK_OPEN_BRACKET)) {
        free_func_decl(decl);
        return NULL;
    }

    // parse stmts
    linked_list *stmts = parse_stmts();
    if (stmts == NULL) {
        free_func_decl(decl);
        return NULL;
    }

    decl->func_stmts = stmts;

    // close
    if (!consume(TOK_CLOSE_BRACKET)) {
        free_func_decl(decl);
        return NULL;
    }

    return decl;
}

linked_list *parse_functions() {
    if (!is_function())
        return NULL;
    
    linked_list *list = list_init(free_func_decl);
    while (is_function()) {
        node_func_decl *decl = parse_function();
        if (decl == NULL) {
            list_free(list);
            list = NULL;
            break;
        } else {
            list_append(list, decl);
        }
    }

    return list;
}

ast_prog *scan(FILE *f) {
    if (opt_verbose_enabled)
        info("---- Scanner\n");

    // cache file
    lf = f;

    ast_prog *ast = NULL;
    next_token();

    // expect functions
    linked_list *l = parse_functions();
    if (l != NULL) {
        if (!list_is_empty(l)) {
            ast = malloc(sizeof(ast_prog));
            ast->func_decls = l;
        } else {
            list_free(l);
        }
    }

    // clear cache
    lf = NULL;
    return ast;
}

void free_ast(ast_prog *node) {
    if (node != NULL) {
        if (node->func_decls != NULL) {
            list_free(node->func_decls);
        }

        free(node);
    }
}

static void print_token(int t) {
    printf("- %s", token_name(t));
    if (t == TOK_IDENTIFIER) {
        printf(": %s", lex_id_str);
    } else if (t == TOK_NUM) {
        printf(": %d", lex_int_val);
    }

    printf("\n");
}

static void print_stmt(node_stmt *stmt) {
    printf("<statement>:\n");
    switch (stmt->stmt_type) {
        case stmt_type_return:
            printf("\t return: %d", stmt->return_exprr.const_int);
            break;
        default:
            break;
    }
}

static void print_func_decl(node_func_decl *func) {
    printf("<func-decl>:\n");
    printf("\t ret: %s\n", func->return_type == data_type_int ? "int" : "");
    printf("\t name: %s\n", func->name);
    if (func->func_stmts != NULL) {
        list_node *node = func->func_stmts->head;
        while (node != NULL) {
            print_stmt(node->value);
            node = node->next;
        }
    }
}

void print_ast(ast_prog *ast) {
    if (ast->func_decls != NULL) {
        list_node *node = ast->func_decls->head;
        while (node != NULL) {
            print_func_decl(node->value);
            node = node->next;
        }
    }
}