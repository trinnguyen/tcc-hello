package parser

import scanner.Scanner
import scanner.Token
import scanner.TokenType

class Parser(scanner: Scanner) {
    val iterator = scanner.iterator()

    private lateinit var tok: Token

    fun parse(): SyntaxTree {
        nextTok()
        val decls = mutableListOf<FuncDecl>()
        while (!tok.isEOF) {
            decls.add(parseFunction())
        }
        return SyntaxTree(decls)
    }

    private fun parseFunction(): FuncDecl {
        // expect type
        val type: DataType = tok.toDataType()

        // expect id
        nextTok()
        val name = parseId()

        expectNextTok(TokenType.PARENT_OPEN)
        nextTok()
        val paramList = parseParameterList()
        expectTok(TokenType.PARENT_CLOSE)

        nextTok()
        val body: CompoundStmt = parseCompoundStmt()

        return FuncDecl(name, paramList, type, body)
    }

    private fun parseParameterList(): List<Parameter> {
        if (!isVarDataType(tok.type))
            return listOf()

        val list = mutableListOf<Parameter>()
        while (true) {
            list.add(parseParameter())
            nextTok()
            if (tok.type == TokenType.COMMA) {
                nextTok()
                continue
            } else {
                break
            }
        }

        return list
    }

    private fun parseParameter(): Parameter {
        val type: DataType = tok.toDataType()

        nextTok()
        val name: String = parseId()
        return Parameter(name, type)
    }

    private fun parseId(): String {
        expectTok(TokenType.ID)
        return tok.value!!
    }

    private fun parseCompoundStmt(): CompoundStmt {
        val stmts = mutableListOf<Stmt>()

        expectTok(TokenType.BRACKET_OPEN)

        nextTok()
        while (tok.type != TokenType.BRACKET_CLOSE) {
            stmts.add(parseStmt())
        }
        nextTok()

        return CompoundStmt(stmts)
    }

    private fun parseStmt(): Stmt {
        return when (tok.type) {
            TokenType.RETURN -> parseReturnStmt()
            else -> error("expected statement but ${actualTokMsg()}")
        }
    }

    private fun parseReturnStmt(): Stmt {
        expectTok(TokenType.RETURN)

        nextTok()
        val expr: Expr = parseExpr()

        expectNextTok(TokenType.SEMICOLON)
        nextTok()
        return ReturnStmt(expr)
    }

    private fun parseExpr(): Expr {
        return when (tok.type) {
            TokenType.CONST_INT -> IntConstExpr(tok.intVal)
            else -> error("expected expression but ${actualTokMsg()}")
        }
    }

    private fun expectNextTok(type: TokenType) {
        nextTok()
        expectTok(type)
    }

    private fun expectTok(type: TokenType) {
        if (tok.type != type) {
            error("expected $type but ${actualTokMsg()}")
        }
    }

    private fun actualTokMsg() = "actual ${tok.type} at ${tok.line}:${tok.column}"

    private fun isVarDataType(type: TokenType): Boolean = type == TokenType.INT

    private fun nextTok() {
        if (iterator.hasNext()) {
            tok = iterator.next()
            println(tok)
        } else {
            error("unexpected null token")
        }
    }
}