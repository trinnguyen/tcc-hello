package parser

import scanner.Token
import scanner.TokenType

fun Token.toDataType(): DataType {
    return when (this.type) {
        TokenType.INT -> DataType.Int
        TokenType.VOID -> DataType.Void
        else -> error("expected type int or void but actual ${this.type} at ${this.line}:${this.column}")
    }
}