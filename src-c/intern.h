#ifndef _INTERN_
#define _INTERN_

#include "list.h"

typedef enum {
    expr_type_const_int
} node_expr_type;

typedef enum {
    stmt_type_return
} node_stmt_type;

typedef enum {
    data_type_int,
    data_type_void
} data_type;

typedef struct {
    node_expr_type expr_type;
    union {
        int const_int;
    };    
} node_expr;

typedef struct {
    node_stmt_type stmt_type;
    union {
        node_expr return_exprr;
    };    
} node_stmt;

typedef struct {
    data_type return_type;
    char *name;
    linked_list *func_stmts;
} node_func_decl;

typedef struct {
    linked_list *func_decls;
} ast_prog;

extern bool opt_verbose_enabled;

#endif