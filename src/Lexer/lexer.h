#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* --- Token Types --- */

typedef enum {
    TOKEN_EOF, TOKEN_IDENTIFIER, TOKEN_INTEGER, TOKEN_FLOAT, TOKEN_STRING,
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN, TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
    TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS, TOKEN_SEMICOLON,
    TOKEN_COLON, TOKEN_SLASH, TOKEN_STAR, TOKEN_LESS, TOKEN_GREATER, TOKEN_EQUAL, TOKEN_EXCLAMATION,
    TOKEN_RIGHT_BRACKET, TOKEN_LEFT_BRACKET, TOKEN_AT,

    /* Keywords */
    TOKEN_FN, TOKEN_LET, TOKEN_USE, TOKEN_IF, 
    TOKEN_ELSE, TOKEN_WHILE, TOKEN_RET, TOKEN_FOR, TOKEN_EXTERN,
    TOKEN_TYPE_INT, TOKEN_TYPE_FLOAT, TOKEN_TYPE_STRING, TOKEN_TYPE_VOID
} TOKEN_TYPE;

typedef struct {
    TOKEN_TYPE type;
    char* value;
} Token;

typedef struct {
    Token* tokens;
    size_t count;
    size_t capacity;
} TokenList;

/* Function to tokenize source code */
TokenList tokenize(const char* source);

/* Function to check if a character is a digit */
int is_digit(char c);

/* Function to check if a character is an alpha character */
int is_alpha(char c);

#endif  /* LEXER_H */
