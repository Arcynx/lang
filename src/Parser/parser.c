#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "parser.h"
#include "Lexer/lexer.h"
#include "AST/ast.h"

ASTNode* parse_expression(TokenList* tokens, int* index);
void error(const char* reason);
ASTNode* parse_statement(TokenList* tokens, int* index);

int expect(TokenList* tokens, int* index, int num_expected, ...) 
{
    (*index)++;
    if (*index >= tokens->count) 
    {
        error("Syntax Error: Unexpected end of input");
        return 0;
    }
    
    int current_type = tokens->tokens[*index].type;

    va_list args;
    va_start(args, num_expected);

    int expected;

    for (int i = 0; i < num_expected; i++) 
    {
        expected = va_arg(args, int);
        if (current_type == expected) 
        {
            va_end(args);
            printf("Syntax: Parsing token at \"%s\", Token: %d, Expected: %d\n",
                tokens->tokens[*index].value, (*index) + 0, expected);
            return 1;
        }
    }

    va_end(args);
    printf("Syntax Error: Unexpected token at \"%s\", Token: %d, Expected: %d\n",
       tokens->tokens[*index].value, (*index) + 0, expected);
    exit(1);
    return 0;
}

int expect_noquit(TokenList* tokens, int* index, int num_expected, ...) 
{
    int current_type = tokens->tokens[*index].type;

    va_list args;
    va_start(args, num_expected);

    for (int i = 0; i < num_expected; i++) 
    {
        int expected = va_arg(args, int);
        if (current_type == expected) 
        {
            va_end(args);
            (*index)++;
            return 1;
        }
    }

    va_end(args);
    return 0;
}

/* --- Value Parsing --- */

ASTNode* parse_integer (TokenList* tokens, int* index)
{
    ASTIntLiteral* node = malloc(sizeof(ASTIntLiteral));
    node->base.type = AST_INT_LITERAL;
    node->value = atoi(tokens->tokens[*index].value);
    return (ASTNode*)node;
}

ASTNode* parse_float (TokenList* tokens, int* index)
{
    ASTFloatLiteral* node = malloc(sizeof(ASTFloatLiteral));
    node->base.type = AST_FLOAT_LITERAL;
    node->value = atof(tokens->tokens[*index].value);
    return (ASTNode*)node;
}

ASTNode* parse_string (TokenList* tokens, int* index)
{
    ASTStringLiteral* node = malloc(sizeof(ASTStringLiteral));
    node->base.type = AST_STRING_LITERAL;
    node->value = strdup(tokens->tokens[*index].value);
    return (ASTNode*)node;
}

/* --- Expression Parsing --- */

ASTNode* parse_function_call(TokenList* tokens, int* index) 
{
    Token name_token = tokens->tokens[*index];
    if (name_token.type != TOKEN_IDENTIFIER)
        error("Expected function name before '->'");

    expect(tokens, index, 1, TOKEN_MINUS);
    expect(tokens, index, 1, TOKEN_GREATER);

    (*index)++;

    ASTNode** args = NULL;
    int arg_count = 0;

    if (expect_noquit(tokens, index, 1, TOKEN_LEFT_PAREN))
    {
        (*index)--;
        expect(tokens, index, 1, TOKEN_RIGHT_PAREN);
        printf("NULL\n");
    }
    else
        while (*index < tokens->count && tokens->tokens[*index].type != TOKEN_SEMICOLON) 
        {
            ASTNode* arg = parse_expression(tokens, index);

            args = realloc(args, sizeof(ASTNode*) * (arg_count + 1));
            if (!args) error("Memory allocation failed during function arguments parsing");
            args[arg_count++] = arg;

            if (tokens->tokens[*index].type == TOKEN_COMMA)
                (*index)++;
            else
                break;
        }


    expect(tokens, index, 1, TOKEN_SEMICOLON);

    ASTFunctionCall* call = malloc(sizeof(ASTFunctionCall));
    call->base.type = AST_FUNCTION_CALL;
    call->name = strdup(name_token.value);
    call->arg_count = arg_count;
    call->arguments = args;

    printf("2\n");

    return (ASTNode*)call;
}

ASTNode* parse_primary(TokenList* tokens, int* index) 
{
    Token token = tokens->tokens[*index];

    if (token.type == TOKEN_INTEGER) 
    {
        return parse_integer(tokens, index);
    }
    else if (token.type == TOKEN_FLOAT) 
    {
        return parse_float(tokens, index);
    }
    else if (token.type == TOKEN_STRING)
    {
        return parse_string(tokens, index);
    }
    else if (token.type == TOKEN_IDENTIFIER) 
    {
        if (*index + 2 < tokens->count &&
            tokens->tokens[*index + 1].type == TOKEN_MINUS &&
            tokens->tokens[*index + 2].type == TOKEN_GREATER) 
        {
            return parse_function_call(tokens, index);
        }

        ASTVariable* varNode = malloc(sizeof(ASTVariable));
        varNode->base.type = AST_VARIABLE;
        varNode->name = strdup(token.value);
        (*index)++;
        printf("(PRIME 1) Parsing statement at index %d\n", *index);
        return (ASTNode*)varNode;
    }
    else if (token.type == TOKEN_LEFT_PAREN)
    {
        (*index)++;
        printf("(PRIME 2) Parsing statement at index %d\n", *index);
        ASTNode* expr = parse_expression(tokens, index);
        expect(tokens, index, 1, TOKEN_RIGHT_PAREN);
        return expr;
    }
    else if (token.type == 0)
    {
        ASTEndOfFile* end_of_file = malloc(sizeof(ASTEndOfFile));
        end_of_file->base.type = AST_EOF;

        return NULL;
    }

    printf("Unexpected token in expression at \"%s\", Token: %d.\n", token.value, (*index) + 1);
    exit(1);
    return NULL;
}

ASTNode* parse_expression(TokenList* tokens, int* index) 
{
    ASTNode* left = parse_primary(tokens, index);

    while (*index < tokens->count) 
    {
        Token token = tokens->tokens[*index];

        if (token.type == TOKEN_PLUS || token.type == TOKEN_MINUS || 
            token.type == TOKEN_STAR || token.type == TOKEN_SLASH) 
        {
            ASTBinaryExpr* node = malloc(sizeof(ASTBinaryExpr));
            node->base.type = AST_BINARY_EXPR;
            node->left = left;
            node->op = token.value[0];
            (*index)++;
            printf("(EXPR) Parsing statement at index %d\n", *index);
            node->right = parse_primary(tokens, index);
            if (node->right->type == AST_EOF)
                return node->right;
            left = (ASTNode*)node;
        }
        else if (token.type == 0)
            return NULL;
        else break;
    }

    return left;
}

/* --- Statement Parsing --- */

ASTNode* parse_block(TokenList* tokens, int* index)
{
    ASTNode* first = NULL;
    ASTNode* current = NULL;

    while (*index < tokens->count && tokens->tokens[(*index) + 1].type != TOKEN_RIGHT_BRACE) 
    {
        printf("INDEX 0 OF BLOCK\n");
        (*index)++;
        if (tokens->tokens[*index].value == NULL || tokens->tokens[*index].type == NULL)
            error("Unexpected end of input");

        printf("INDEX 1 OF BLOCK\n");

        ASTNode* stmt = parse_statement(tokens, index);
        if (tokens->tokens[*index].value == NULL || tokens->tokens[*index].type == NULL)
            error("Unexpected end of input");
        if (stmt == NULL)
            continue;

        if (!first)
            first = stmt;
        else
            current->next = stmt;

        printf("INDEX 2 OF BLOCK\n");

        current = stmt;
        if (tokens->tokens[*index].value == NULL || tokens->tokens[*index].type == NULL)
            error("Unexpected end of input");
    }
    printf("END OF BLOCK\n");

    return first;
}

ASTNode* parse_function_declaration(TokenList* tokens, int* index)
{
    (*index)++;
    printf("(FN) Parsing statement at index %d\n", *index);

    Token name = tokens->tokens[*index];
    if (name.type != TOKEN_IDENTIFIER)
        error("Expected function name after 'fn'");


    char* type = "auto"; /* Default function type */

    expect(tokens, index, 1, TOKEN_LEFT_PAREN);
    (*index)++;
    printf("(FN2) Parsing statement at index %d\n", *index);

    ASTParameter* params = NULL;
    int param_count = 0;

    while (tokens->tokens[*index].type != TOKEN_RIGHT_PAREN)
    {
        if (tokens->tokens[*index].type == TOKEN_LEFT_PAREN)
        {
            printf("Double paren at token: '%d'\n", tokens->tokens[*index].type, *index);
            exit(1);
        }

        if (tokens->tokens[*index].type != TOKEN_IDENTIFIER)
        {
            printf("Expected parameter name instead got: '%d' at token: '%d'\n", tokens->tokens[*index].type, *index);
            exit(1);
        }

        params = realloc(params, sizeof(ASTParameter) * (param_count + 1));

        params[param_count].base.type = AST_PARAMETER;
        params[param_count].name = tokens->tokens[*index].value;
        (*index)++;
        printf("(FN3) Parsing statement at index %d 8 with token id: %d\n", *index, tokens->tokens[*index].type);

        if (expect_noquit(tokens, index, 1, TOKEN_COLON))
        {
            (*index)++;
            printf("(FN4) Parsing statement at index %d 80\n", *index);
            params[param_count].type = tokens->tokens[*index].value;
            (*index)++;
            printf("(FN5) Parsing statement at index %d 90\n", *index);
        }
        else
        {
            params[param_count].type = "auto";
        }

        if (tokens->tokens[*index].type == TOKEN_COMMA)
            (*index)++;
            printf("Parsing statement at index %d 8\n", *index);

        param_count++;
    }
    (*index)++;
    printf("(FN PARAM FINISHED) Parsing statement at index %d\n", *index);

    if (expect_noquit(tokens, index, 1, TOKEN_COLON))
    {
        (*index)--;
        printf("Parsing statement at index %d 8\n", *index);
        expect(tokens, index, 4, TOKEN_TYPE_INT, TOKEN_TYPE_FLOAT, TOKEN_TYPE_STRING, TOKEN_TYPE_VOID);
        type = tokens->tokens[*index - 1].value;
    }

    expect(tokens, index, 1, TOKEN_LEFT_BRACE);

    ASTNode* body = parse_block(tokens, index);

    expect(tokens, index, 1, TOKEN_RIGHT_BRACE);

    (*index)++;

    printf("(FN BLOCK FINISHED) Parsing statement at index %d\n", *index);

    ASTFunctionDeclaration* func = malloc(sizeof(ASTFunctionDeclaration));
    func->base.type = AST_FUNCTION_DECLARATION;
    func->name = strdup(name.value);
    func->param_count = param_count;
    func->parameters = params;
    func->body = body;
    func->type = type;

    free(params);

    return (ASTNode*)func;
}

ASTNode* parse_variable_declaration(TokenList* tokens, int* index) 
{
    char* type = "auto";
    ASTNode* initializer = NULL;
    (*index)++;
    printf("Parsing statement at index %d 7\n", *index);
    Token name = tokens->tokens[*index];
    (*index)++;
    printf("Parsing statement at index %d 7\n", *index);
    if (expect_noquit(tokens, index, 1, TOKEN_EQUAL))
        initializer = parse_expression(tokens, index);

    if (expect_noquit(tokens, index, 1, TOKEN_COLON)) 
    {
        (*index)++;
        type = tokens->tokens[*index].value;
    }


    expect(tokens, index, 1, TOKEN_SEMICOLON);
    
    ASTVariableDeclaration* varDecl = malloc(sizeof(ASTVariableDeclaration));
    varDecl->base.type = AST_VARIABLE_DECLARATION;
    varDecl->name = strdup(name.value);
    varDecl->initalizer = initializer;
    varDecl->type = type;
    return (ASTNode*)varDecl;
}

ASTNode* parse_if_statement(TokenList* tokens, int* index) 
{
    (*index)++;
    printf("Parsing statement at index %d 6\n", *index);

    expect(tokens, index, 1, TOKEN_MINUS);
    expect(tokens, index, 1, TOKEN_GREATER);

    ASTNode* condition = parse_expression(tokens, index);
    ASTNode* then_branch = parse_block(tokens, index);
    ASTNode* else_branch = NULL;
    
    if (strcmp(tokens->tokens[*index].value, "else") == 0)
    {
        (*index)++;
        else_branch = parse_block(tokens, index);
    }

    expect(tokens, index, 1, TOKEN_SEMICOLON);
    
    ASTIfStatement* ifStmt = malloc(sizeof(ASTIfStatement));
    ifStmt->base.type = AST_IF_STATEMENT;
    ifStmt->condition = condition;
    ifStmt->then_branch = then_branch;
    ifStmt->else_branch = else_branch;
    return (ASTNode*)ifStmt;
}

ASTNode* parse_while_loop(TokenList* tokens, int* index)
{
    (*index)++;
    printf("Parsing statement at index %d 5\n", *index);
    expect(tokens, index, 1, TOKEN_MINUS);
    expect(tokens, index, 1, TOKEN_GREATER);

    ASTNode* condition = parse_expression(tokens, index);

    expect(tokens, index, 1, TOKEN_LEFT_BRACE);

    ASTNode* body = parse_block(tokens, index);
    expect(tokens, index, 1, TOKEN_SEMICOLON);

    ASTWhileLoop* whileLoop = malloc(sizeof(ASTWhileLoop));
    whileLoop->base.type = AST_WHILE_LOOP;
    whileLoop->condition = condition;
    whileLoop->body = body;
    return (ASTNode*)whileLoop;
}

ASTNode* parse_for_loop(TokenList* tokens, int* index)
{
    (*index)++;
    printf("Parsing statement at index %d 4\n", *index);
    
    expect(tokens, index, 1, TOKEN_MINUS);
    expect(tokens, index, 1, TOKEN_GREATER);

    ASTNode* init = parse_expression(tokens, index);
    expect(tokens, index, 1, TOKEN_DOT);
    expect(tokens, index, 1, TOKEN_DOT);
    ASTNode* end = parse_expression(tokens, index);

    expect(tokens, index, 1, TOKEN_LEFT_BRACE);
    ASTNode* body = parse_block(tokens, index);
    expect(tokens, index, 1, TOKEN_RIGHT_BRACE);
    expect(tokens, index, 1, TOKEN_SEMICOLON);

    ASTForLoop* forLoop = malloc(sizeof(ASTForLoop));
    forLoop->base.type = AST_FOR_LOOP;
    forLoop->initialization = init;
    forLoop->end = end;
    forLoop->body = body;
    return (ASTNode*)forLoop;
}

ASTNode* parse_return_statement(TokenList* tokens, int* index)
{
    printf("(RET) Parsing statement at index %d\n", *index);
    ASTReturnStatement* ret = malloc(sizeof(ASTReturnStatement));
    ret->base.type = AST_RETURN_STATEMENT;

    expect(tokens, index, 1, TOKEN_MINUS);
    expect(tokens, index, 1, TOKEN_GREATER);

    (*index)++;
    printf("(RET) Parsing statement at index %d\n", *index);
    ret->expression = parse_expression(tokens, index);
    expect(tokens, index, 1, TOKEN_SEMICOLON);

    return (ASTNode*) ret;
}

ASTNode* parse_use_statement(TokenList* tokens, int* index)
{
    (*index)++;
    printf("Parsing statement at index %d 2\n", *index);

    ASTUseStatement* use = malloc(sizeof(ASTUseStatement));
    use->base.type = AST_USE;
    use->major = tokens->tokens[*index].value;

    expect(tokens, index, 1, TOKEN_COLON);
    expect(tokens, index, 1, TOKEN_COLON);

    (*index)++;
    use->minor = tokens->tokens[*index].value;
    (*index)++;
    printf("Parsing statement at index %d 2\n", *index);

    return (ASTNode*)use;
}

ASTNode* parse_extern_statement(TokenList* tokens, int* index)
{
    (*index)++;
    printf("Parsing statement at index %d 1\n", *index);

    ASTExternStatement* lib = malloc(sizeof(ASTExternStatement));

    lib->base.type = AST_EXTERN;
    lib->file = tokens->tokens[*index].value;
    (*index)++;

    expect(tokens, index, 1, TOKEN_SEMICOLON);

    return (ASTNode*)lib;
}

ASTNode* parse_statement(TokenList* tokens, int* index)
{
    printf("Parsing statement at index %d 0\n", *index);

    TOKEN_TYPE type = tokens->tokens[*index].type;

    if (tokens->count < *index + 1)
    {
        error("OUT OF BOUNDS!");
    }    
    else if (type == TOKEN_USE)
        return parse_use_statement(tokens, index);
    else if (type == TOKEN_IF)
        return parse_if_statement(tokens, index);
    else if (type == TOKEN_FN)
        return parse_function_declaration(tokens, index);
    else if (type == TOKEN_LET)
        return parse_variable_declaration(tokens, index);
    else if (type == TOKEN_WHILE)
        return parse_while_loop(tokens, index);
    else if (type == TOKEN_FOR)
        return parse_for_loop(tokens, index);
    else if (type == TOKEN_RET)
        return parse_return_statement(tokens, index);
    else if (type == TOKEN_EXTERN)
        return parse_extern_statement(tokens, index);
    else if (type == TOKEN_IDENTIFIER) 
    {
        printf("IDENT\n");
        if (*index + 2 < tokens->count &&
            tokens->tokens[*index + 1].type == TOKEN_MINUS &&
            tokens->tokens[*index + 2].type == TOKEN_GREATER)
        {
            printf("Func\n");
            return parse_function_call(tokens, index);
        }
    }
    else
    {
        return parse_expression(tokens, index);
    }
}

ASTNode* parse(TokenList* tokens) 
{
    int index = 0;
    ASTNode* first = NULL;
    ASTNode* current = NULL;

    while (index < tokens->count + 1) 
    {
        ASTNode* stmt = parse_statement(tokens, &index);
        if (stmt == NULL) break;

        if (!first)
            first = stmt;
        else
            current->next = stmt;
        current = stmt;
    }
    
    return first;
}

/* --- ERROR HANDLING --- */

void error(const char* reason)
{
    printf("Error: %s\n", reason);
    exit(1);
}
