#!/bin/bash

FILES="src/main.c src/Lexer/lexer.c src/Parser/parser.c src/Compiler/compiler.c src/Parser/AST/ast.c"
CFLAGS="-Wall -Wextra -g"

gcc $CFLAGS -o build/flcc $FILES -I src/

./build/flcc

nasm -felf64 -o build/binary/out.o build/assembly/1.asm
nasm -felf64 -o build/binary/lib.o LIBS/std/io/lib.asm

ld -o build/binary/out build/binary/out.o build/binary/lib.o
rm build/binary/out.o build/binary/lib.o

echo "Program Output:"
./build/binary/out