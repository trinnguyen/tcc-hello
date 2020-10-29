package scanner

data class Token(val type: TokenType, val line: Long, val column: Long, val value: String? = null, val intVal: Int = 0) {
    val isEOF = type == TokenType.EOF
}