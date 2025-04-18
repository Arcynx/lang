#ifndef PARSER_H
#define PARSER_H

#include "Lexer/lexer.h"
#include "AST/ast.h"

/* Parses an array of tokens. */
ASTNode* parse(TokenList* tokens);

#endif