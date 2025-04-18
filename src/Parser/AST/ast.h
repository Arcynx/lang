#ifndef AST_H
#define AST_H

/* --- AST Node Types --- */

/* --- AST Node Types --- */

typedef enum
{
    AST_INT_LITERAL,
    AST_FLOAT_LITERAL,
    AST_STRING_LITERAL,
    AST_VARIABLE,
    AST_BINARY_EXPR,
    AST_UNARY_EXPR,
    AST_FUNCTION_CALL,
    AST_VARIABLE_DECLARATION,
    AST_FUNCTION_DECLARATION,
    AST_IF_STATEMENT,
    AST_FOR_LOOP,
    AST_WHILE_LOOP,
    AST_RETURN_STATEMENT,
    AST_PARAMETER,
    AST_USE,
    AST_EXTERN,
    AST_EOF,
} ASTNodeType;

/* ---- AST Node Base Struct --- */

typedef struct ASTNode
{
    ASTNodeType type;
    struct ASTNode* next;
} ASTNode;

/* --- Expression Nodes --- */

typedef struct
{
    ASTNode base;
    int value;
} ASTIntLiteral;

typedef struct
{
    ASTNode base;
    double value;
} ASTFloatLiteral;

typedef struct
{
    ASTNode base;
    char* value;
} ASTStringLiteral;

typedef struct
{
    ASTNode base;
    char* name;
} ASTVariable;

typedef struct
{
    ASTNode base;
    ASTNode* left;
    ASTNode* right;
    char op;
} ASTBinaryExpr;

typedef struct {
    ASTNode base;
    ASTNode* operand;
    char op;
} ASTUnaryExpr;

/* --- Statements --- */

typedef struct 
{
    ASTNode base;
    char* name;
    char* type;
    ASTNode* initalizer;
} ASTVariableDeclaration;

typedef struct 
{
    ASTNode base;
    char* name;
    char** parameters;
    int param_count;
    char* type;
    ASTNode* body;
} ASTFunctionDeclaration;

typedef struct 
{
    ASTNode base;
    ASTNode *condition;
    ASTNode *then_branch;
    ASTNode *else_branch;
} ASTIfStatement;

typedef struct 
{
    ASTNode base;
    ASTNode* initialization;
    ASTNode* end;
    ASTNode* increment;
    ASTNode* body; 
} ASTForLoop;

typedef struct
{
    ASTNode base;
    char* major;
    char* minor;
} ASTUseStatement;

typedef struct 
{
    ASTNode base;
    ASTNode* condition;
    ASTNode* body;
} ASTWhileLoop;

typedef struct 
{
    ASTNode base;
    ASTNode* expression;
} ASTReturnStatement;

typedef struct {
    ASTNode base;
    char* name;
    ASTNode** arguments;
    int arg_count;
} ASTFunctionCall;

typedef struct
{
    ASTNode base;
    char* type;
    char* name;
} ASTParameter;

typedef struct
{
    ASTNode base;
    char* file;
} ASTExternStatement;

typedef struct
{
    ASTNode base;
} ASTEndOfFile;

#endif