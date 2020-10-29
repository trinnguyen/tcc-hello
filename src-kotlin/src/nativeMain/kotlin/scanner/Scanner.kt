package scanner

import kotlinx.cinterop.CPointer
import platform.posix.*

class Scanner (private val fin: CPointer<FILE>) : Iterable<Token> {
    override fun iterator(): Iterator<Token> {
        return ScannerIterator(fin)
    }

    class ScannerIterator(private val fin: CPointer<FILE>) : Iterator<Token> {

        private var line: Long = 1
        private var col: Long = 1
        private var raw: Int = Int.MIN_VALUE
        private var ch: Char = Char.MIN_VALUE
        private var isLookAhead: Boolean = false

        init {
            fseek(fin, 0, SEEK_SET)
        }

        private val keywordMap: Map<String, TokenType> = mapOf(
                "int" to TokenType.INT,
                "void" to TokenType.VOID,
                "return" to TokenType.RETURN,
        )

        private val symbolMap: Map<Char, TokenType> = mapOf(
                ';' to TokenType.SEMICOLON,
                ',' to TokenType.COMMA,
                '(' to TokenType.PARENT_OPEN,
                ')' to TokenType.PARENT_CLOSE,
                '{' to TokenType.BRACKET_OPEN,
                '}' to TokenType.BRACKET_CLOSE,
        )

        override fun hasNext(): Boolean {
            return raw != EOF
        }

        override fun next(): Token {

            // cache loc
            val cline = this.line
            val ccol = this.col
            nextChar()

            // skip while space
            while (ch.isWhitespace())
                nextChar()

            // check single symbol
            if (symbolMap.containsKey(ch))
                return Token(symbolMap.getValue(ch), cline, ccol)

            // check if alphabet
            if (ch.isLetter()) {

                var str = ""
                while (ch.isLetterOrDigit()) {
                    str += ch
                    nextChar()
                }
                isLookAhead = true

                // scan keywords (int, return)
                if (keywordMap.containsKey(str)) {
                    return Token(keywordMap.getValue(str), cline, ccol)
                }

                // scan ID
                return Token(TokenType.ID, cline, ccol, str)
            }

            // number
            if (ch.isDigit()) {
                var str = ""
                while (ch.isDigit()) {
                    str += ch
                    nextChar()
                }
                isLookAhead = true

                // number
                return Token(TokenType.CONST_INT, cline, ccol, null, str.toInt())
            }

            // EOF
            if (raw == EOF)
                return Token(TokenType.EOF, cline, ccol)

            // error
            error("error: unexpected token '${ch}' at $line:$col")
        }

        private fun nextChar() {
            if (isLookAhead) {
                isLookAhead = false
                return
            }

            raw = fgetc(fin)
            ch = raw.toChar()
            if (ch == '\n') {
                // reset
                line++
                col = 1
            } else {
                col ++
            }
        }
    }
}
