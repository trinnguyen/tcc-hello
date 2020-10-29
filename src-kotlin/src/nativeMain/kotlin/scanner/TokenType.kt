package scanner

enum class TokenType {
    // keywords
    INT,
    VOID,
    RETURN,

    // ID
    ID,
    EOF,

    // symbols
    PARENT_OPEN,
    PARENT_CLOSE,
    BRACKET_OPEN,
    BRACKET_CLOSE,
    COMMA,
    SEMICOLON,

    // numbers
    CONST_INT
}
