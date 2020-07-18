#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "lexer.h"
#include "util.h"

char lex_id_str[256];

int lex_int_val;

static bool is_letter_digit(char c) {
    return isalnum(c) || c == '_';
}

int get_token(FILE *f) {
    static char c = ' ';
    
    // skip space
    while (isspace(c))
        c = fgetc(f);

    // start with keywords/identifiers
    if (isalpha(c) || c == '_') {
        reset_str(lex_id_str);

        int idx = 0;
        lex_id_str [idx ++] = c;
        while (is_letter_digit(c = fgetc(f))) {
            lex_id_str [idx ++] = c;
        }

        // check keywords
        if (is_str_equals(lex_id_str, "int")) {
            return TOK_INT;
        }

        if (is_str_equals(lex_id_str, "return")) {
            return TOK_RETURN;
        }

        // return id with val in lex_id_str
        return TOK_IDENTIFIER;
    }

    // parse number
    if (isdigit(c)) {
        char str_num[64];
        int idx = 0;
        str_num[idx++] = c;
        while (isdigit(c = fgetc(f))) {
            str_num[idx++] = c;
        }

        // parse
        lex_int_val = atoi(str_num);
        return TOK_NUM;
    }

    // end of file
    if (c == EOF)
        return TOK_EOF;

    // symbols
    int t = c;
    if (c == ';')
        t = TOK_SEMI_COLON;

    if (c == '(')
        t = TOK_OPEN_PARENT;

    if (c == ')')
        t = TOK_CLOSE_PARENT;

    if (c == '{')
        t = TOK_OPEN_BRACKET;

    if (c == '}')
        t = TOK_CLOSE_BRACKET;

    c = fgetc(f);
    return t;
}

const char *token_name(int t) {
    switch (t) {
        case TOK_IDENTIFIER:
            return "identifier";
        case TOK_INT:
            return "int";
        case TOK_RETURN:
            return "return";
        case TOK_NUM:
            return "number";
        case TOK_SEMI_COLON:
            return ";";
        case TOK_OPEN_PARENT:
            return "(";
        case TOK_CLOSE_PARENT:
            return ")";
        case TOK_OPEN_BRACKET:
            return "{";
        case TOK_CLOSE_BRACKET:
            return "}";
            break;
        case TOK_EOF:
            return "EOF";
            break;
        default:
            return "";
    }
}