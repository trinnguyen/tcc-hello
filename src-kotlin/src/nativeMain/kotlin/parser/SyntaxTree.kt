package parser

data class SyntaxTree(val funcDecls: List<FuncDecl>) {
}

data class FuncDecl(val name: String, val parameters: List<Parameter>, val returnType: DataType, val cmpStmt: CompoundStmt)

data class CompoundStmt(val stmts: List<Stmt>) {
}

open class Stmt {
}

open class Expr {
}

data class ReturnStmt(val expr: Expr) : Stmt() {
}

data class IntConstExpr(val intVal: Int) : Expr()

data class Parameter(val name: String, val type: DataType)

enum class DataType {
    Int,
    Void
}