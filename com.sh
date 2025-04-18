#!/bin/bash

FILES="src/main.c src/Lexer/lexer.c src/Parser/parser.c src/Compiler/compiler.c src/Parser/AST/ast.c"
CFLAGS="-Wall -Wextra -g"

gcc $CFLAGS -o build/flcc $FILES -I src/

./build/flcc