# tcc
Toy C compiler for (subset) C, target x64 Linux

## Overview

- Implement my own compiler for (subset) C language
- Hand-written all phases from scanning to code generation
    + Simple lexer
    + Recursive descent parser to AST
    + Generate x64 assembly code (Intel syntax) from AST
    + Invoke system assembler (as)
    + Invoke system linker (ld)
- Generate executable file (machine code) for Linux/macOS
- Usage of x64 assembly language (Intel syntax) and NASM assembler


## Usage
```
tcc - A toy C compiler for ANSI C

USAGE:
    tcc [options] filename ...
    Options:
        -o <name>  Output filename. Default is a.out
        -h         Show this help information
        -v         Verbose output
EXAMPLE:
    tcc hello.c
```

### Requirements
- Ensure **nasm** is listed in environment PATH
    + https://www.nasm.us

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
.section	__TEXT,__text
	.intel_syntax
	.globl	_main
_main:
	.cfi_startproc
	mov	eax, 1
	ret
	.cfi_endproc
```

### Related works
- An Incremental Approach to Compiler Construction: http://scheme2006.cs.uchicago.edu/11-ghuloum.pdf
- A series blog posts following the same approach but using OCaml and targeting x86: https://norasandler.com/2017/11/29/Write-a-Compiler.html
