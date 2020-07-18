#include <string.h>
#include "semantics.h"

bool validate_function(node_func_decl *decl) {
    // ensure having return statement
    if (decl->return_type != data_type_void) {
        if (decl->func_stmts == NULL || list_is_empty(decl->func_stmts)) {
            fprintf(stderr, "Missing return statement\n");
            return false;
        }

        bool has_return = false;
        list_node *node = decl->func_stmts->head;
        while (node != NULL) {
            node_stmt *stmt = node->value;
            if (stmt->stmt_type == stmt_type_return) {
                has_return = true;
                break;
            }

            node = node->next;
        }

        // check
        if (!has_return) {
            fprintf(stderr, "Missing return statement\n");
            return false;
        }
    }

    return true;
}

bool validate_functions(linked_list *decls) {
    list_node *node = decls->head;
    bool has_main = false;
    bool has_err = false;
    while (node != NULL) {
        node_func_decl *decl = node->value;

        if (strcmp(decl->name, "main") == 0) {
            has_main = true;
        }

        if (!validate_function(decl)) {
            has_err = true;
        }

        node = node->next;
    }

    // ensure main
    if (!has_main) {
        fprintf(stderr, "Missing main function\n");
        return false;
    }

    if (has_err)
        return false;

    return true;
}

bool validate(ast_prog *ast) {
    if (ast->func_decls == NULL || list_is_empty(ast->func_decls)) {
        fprintf(stderr, "Missing function\n");
        return false;
    }

    if (!validate_functions(ast->func_decls)) {
        return false;
    }

    return true;
}