// Token type
typedef enum {
    TOK_IDENTIFIER = 1,

    // keywords
    TOK_INT = 2,
    TOK_RETURN = 3,

    // symbols
    TOK_SEMI_COLON = 4,
    TOK_OPEN_PARENT = 5,
    TOK_CLOSE_PARENT = 6,
    TOK_OPEN_BRACKET = 7,
    TOK_CLOSE_BRACKET = 8,

    // numbers
    TOK_NUM = 9,

    // end of file
    TOK_EOF = 10
} token_type;

extern char identifier_str[256];

extern double int_val;

int get_token(FILE *f);