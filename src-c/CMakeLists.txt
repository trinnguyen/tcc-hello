cmake_minimum_required(VERSION 3.10)

# set the project name
project(tcc)

# add the executable
add_executable(tcc main.c util.c lexer.c parser.c list.c semantics.c codegen.c external.c)