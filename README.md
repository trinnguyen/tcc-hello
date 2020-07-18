# tcc
Toy compiler for (subset) C, target x64 macOS and Linux

## Overview

- Implement my own compiler for (subset) C language
- Hand-written all phases from scanner to code generation
- Generate executable file (machine code) for Linux/macOS
- Invoke assembler, linker
- Usage of x64 assembly language (Intel syntax) and NASM assembler
- Follow the methodology in paper "An Incremental Approach to Compiler Construction"
- Use ANSI C grammar from the book ["C Programming Language, 2nd Edition"](https://www.pearson.com/us/higher-education/program/Kernighan-C-Programming-Language-2nd-Edition/PGM54487.html)
- The compiler is developed in C


### Related works
- A series blog posts following the same approach but using OCaml and targeting x86: https://norasandler.com/2017/11/29/Write-a-Compiler.html

## Incremental steps

### Step 1. Integers
- Goals
    + Initialize compiler phases
    + Scanner and parser for function with return statement, scalar number expression (integer)
    + Generate x64 assembly from AST
    + Invoke assembler (NASM) to compiler object file
    + Invoke built-in OS linker (GNU ld) to generate executable file

- EBNF Grammar

```
<prog>          := <func-decl>+
<func-decl>     := 'int' <id> '(' ')' '{' <statement>* '}'
<id>            := (letter | '_') (<letter> | '_' | <digit>)*
<letter>        := [a-zA-Z]
<digit>         := [0-9]
<statement>     := 'return' <num> ';'
<num>           := <digit>+
```

- Input: `int1.c`
```c
int main() {
    return 1;
}
```

- Output: `int1` on macOS and Linux (Fedora)

- Phases:

Lexer -> Parser -> Semantics Analyzer -> Assembly code generator -> Assembler -> Linker

- Sample assembly code:
```s
# use with GNU as
.section	__TEXT,__text
	.intel_syntax
	.globl	_main
_main:
	.cfi_startproc
	mov	eax, 1
	ret
	.cfi_endproc
```