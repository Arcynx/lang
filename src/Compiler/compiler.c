#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Parser/AST/ast.h"
#include "Lexer/lexer.h"

char* header = "; --- Generated by Flouride Compiler ---\n\n";

char* data_section = NULL;
char* text_section = NULL;
char* top_section = NULL;

int string_initalizer = 2147483647;

void compile_block(ASTNode *block);
void addInstruction(const char* instruction);
void writeASM(const char* filename, const char* content);

#define AST_RET_STATEMENT 123

/* Compiler */

int compile(ASTNode *node)
{
    char instructionbuf[256];

    text_section = malloc(256);
    text_section[0] = '\0';

    addInstruction("section .text\n\n"
               "global _start\n\n"
               "_start:\n"
               "call main\n"
               "call exit\n");

    data_section = malloc(256);
    data_section[0] = '\0';

    addData("section .data\n");

    printf("----------------------------------\nCompiling...\n");

    top_section = malloc(256);
    top_section[0] = '\0';
    int i = 0;

    while (node != NULL && node->type != NULL)
    {
        if (node->type == AST_FUNCTION_DECLARATION)
        {
            ASTFunctionDeclaration *func = (ASTFunctionDeclaration *)node;
            printf("Found Function Declaration:\n   Name: \"%s\",\n", func->name);
            printf("   Parameters: %d\n", func->param_count);
            printf("   Block:\n");

            memset(instructionbuf, 0, sizeof(instructionbuf));
            sprintf(instructionbuf, "%s:\n", func->name);
            addInstruction(instructionbuf);

            ASTNode *block = func -> body;
            compile_block(block);
        }
        if (node->type == AST_USE)
        {
            ASTUseStatement *use = (ASTUseStatement*)node;

            memset(instructionbuf, 0, sizeof(instructionbuf));
            sprintf(instructionbuf, "%%include \"LIBS/%s/%s/lib.inc\"\n", use->major, use->minor);
            addTop(instructionbuf);
        }

        i++;

        printf("AST INDEX: \"%d\" With Type Of: \"%d\"\n", i, node->type);

        node = node->next;
    }

    size_t size = strlen(header) + strlen(data_section) + strlen(text_section) + 1;
    char* end_buf = malloc(size + 128);
    if (end_buf == NULL)
    {
        perror("Failed to allocate memory");
        return;
    }

    end_buf[0] = '\0';

    strcat(end_buf, header);
    strcat(end_buf, top_section);
    strcat(end_buf, data_section);
    strcat(end_buf, text_section);

    writeASM("build/assembly/1.asm", end_buf);

    printf("----------------------------------\nEND OF COMPILING!.\n");

    return 0;
}

void compile_block(ASTNode *block)
{
    if (block == NULL)
    {
        return;
    }

    printf("--- BLOCK BEGIN ---\n");

    while (block != NULL && block->type != NULL)
    {
        if (block->type == AST_RET_STATEMENT)
        {
            handleReturnStatement(block);
            return;
        }

        switch (block->type)
        {
            case AST_RETURN_STATEMENT:
            {
                ASTReturnStatement *retState = (ASTReturnStatement*) block;
                handleReturnStatement(retState);
                break;
            }

            case AST_FUNCTION_CALL:
            {
                ASTFunctionCall* call = (ASTFunctionCall*) block;
                handleFunctionCall(call);
                break;
            }

            case AST_IF_STATEMENT:
            {
                ASTIfStatement* ifStatement = (ASTIfStatement*) block;
                handleIfStatement(ifStatement);
                break;
            }
        
            default:
                break;
        }

        block = block->next;
    }

    printf("--- BLOCK END ---\n");
}

void handleIfStatement(ASTIfStatement* ifStatement)
{
    char str_ident[1028];
    char instructionbuf[1028];

    ifStatement->condition;
}

void handleReturnStatement(ASTReturnStatement* ret)
{
    char str_ident[1028];
    char instructionbuf[1028];

    if (ret->expression == NULL)
        return;

    ASTBinaryExpr* expr = (ASTBinaryExpr*)ret->expression;
        if (expr == NULL)
            return;

    switch (ret->expression->type) 
    {
    case AST_STRING_LITERAL: 
    {
        ASTStringLiteral* str_lit = (ASTStringLiteral*) ret->expression;
        string_initalizer--;
        memset(str_ident, 0, sizeof(str_ident));
        memset(instructionbuf, 0, sizeof(instructionbuf));
        sprintf(str_ident, "FLOURIDE_STRING_%d", string_initalizer);
        if (strlen(str_ident) >= sizeof(str_ident))
            return;
        sprintf(instructionbuf, "%s db \"%s\", 0\n", str_ident, str_lit->value);
        if (strlen(instructionbuf) >= sizeof(instructionbuf))
            return;
        addData(instructionbuf);
                        
        printf("        STRING: \"%s\"\n", str_lit->value);
        memset(instructionbuf, 0, sizeof(instructionbuf));
        sprintf(instructionbuf, "lea rax, [%s]\n", str_ident);
        addInstruction(instructionbuf);

        break;
    }
    case AST_INT_LITERAL: 
    {
        ASTIntLiteral* int_lit = (ASTIntLiteral*) ret->expression;
        printf("        INT: %d\n", int_lit->value);

        memset(instructionbuf, 0, sizeof(instructionbuf));
        snprintf(instructionbuf, sizeof(instructionbuf), "mov rax, %d\nret\n", int_lit->value);
        addInstruction(instructionbuf);

        break;
    }
    case AST_FLOAT_LITERAL:
    {
        ASTFloatLiteral* float_lit = (ASTFloatLiteral*) ret->expression;
        printf("        FLOAT: %f\n", float_lit->value);
        break;
    }
    case AST_FUNCTION_CALL:
    {
        ASTFunctionCall* call = (ASTFunctionCall*) ret->expression;
        handleFunctionCall(call);
        break;
    }
    default:
        printf("        UNKNOWN (type %d)\n", ret->expression->type);
        break;
    }
}

void handleFunctionCall(ASTFunctionCall* call) 
{
    char str_ident[1028];
    char instructionbuf[1028];

    printf("    Running Function: \"%s\"\n", call->name);

    /* Function Arguments */
    for (int i = 0; i < call->arg_count; i++) 
    {
        printf("    arg type: \n");
        
        /* Implement Argument Logic */

        switch (call->arguments[i]->type)
        {
            case AST_STRING_LITERAL :
            {
                ASTStringLiteral* str_lit = (ASTStringLiteral*) call->arguments[i];
                string_initalizer--;
                memset(str_ident, 0, sizeof(str_ident));
                memset(instructionbuf, 0, sizeof(instructionbuf));
                sprintf(str_ident, "FLOURIDE_STRING_%d", string_initalizer);
                sprintf(instructionbuf, "%s db \"%s\", 0\n", str_ident, str_lit->value);
                addData(instructionbuf);

                memset(instructionbuf, 0, sizeof(instructionbuf));
                sprintf(instructionbuf, "lea rax, [%s]\n", str_ident);
                addInstruction(instructionbuf);
                break;
            }
            case AST_INT_LITERAL:
            {
                ASTIntLiteral* int_lit = (ASTIntLiteral*) call->arguments[i];

                memset(instructionbuf, 0, sizeof(instructionbuf));
                sprintf(instructionbuf, "mov rax, %d\n", int_lit);
                addInstruction(instructionbuf);
                break;
            }
            case AST_FLOAT_LITERAL:
            {
                ASTFloatLiteral* float_lit = (ASTFloatLiteral*) call->arguments[i];

                memset(instructionbuf, 0, sizeof(instructionbuf));
                sprintf(instructionbuf, "mov rax, %f\n", float_lit);
                addInstruction(instructionbuf);
                break;
            }
            default:
            {
                break;
            }
        }
    }

    memset(instructionbuf, 0, sizeof(instructionbuf));
    sprintf(instructionbuf, "call %s\n", call->name);
    addInstruction(instructionbuf);
    printf("(FUNC) DONE\n");
}

void addInstruction(const char* instruction)
{
    if (instruction == NULL)
    {
        fprintf(stderr, "Tried to add NULL instruction!\n");
        exit(1);
    }

    if (text_section == NULL) 
    {
        text_section = malloc(256);
        if (text_section == NULL) 
        {
            perror("Failed to allocate memory for text_section");
            exit(1);
        }
        text_section[0] = '\0';
    }

    size_t text_len = strlen(text_section);
    size_t instr_len = strlen(instruction);

    size_t new_size = text_len + instr_len + (strlen(instruction) * 2);
    char* temp = realloc(text_section, new_size);
    if (temp == NULL)
    {
        perror("Failed to realloc text_section");
        exit(1);
    }

    text_section = temp;

    strcat(text_section, instruction);
}

void addData(const char* data)
{
    if (data == NULL)
    {
        fprintf(stderr, "Tried to add NULL instruction!\n");
        exit(1);
    }

    if (data_section == NULL) 
    {
        data_section = malloc(256);
        if (data_section == NULL) 
        {
            perror("Failed to allocate memory for data_section");
            exit(1);
        }
        data_section[0] = '\0';
    }

    size_t text_len = strlen(data_section);
    size_t instr_len = strlen(data);

    size_t new_size = text_len + instr_len + (strlen(data) * 2);
    char* temp = realloc(data_section, new_size);
    if (temp == NULL)
    {
        perror("Failed to realloc data_section");
        exit(1);
    }

    data_section = temp;

    strcat(data_section, data);
}

void addTop(const char* data)
{
    if (data == NULL)
    {
        fprintf(stderr, "Tried to add NULL instruction!\n");
        exit(1);
    }

    if (top_section == NULL) 
    {
        top_section = malloc(256);
        if (top_section == NULL) 
        {
            perror("Failed to allocate memory for top_section");
            exit(1);
        }
        top_section[0] = '\0';
    }

    size_t text_len = strlen(top_section);
    size_t instr_len = strlen(data);

    size_t new_size = text_len + instr_len;
    char* temp = realloc(top_section, new_size + 1);
    if (temp == NULL)
    {
        perror("Failed to realloc top_section");
        exit(1);
    }

    top_section = temp;

    strcat(top_section, data);
}

void writeASM(const char *filename, const char *content) 
{
    FILE *file = fopen(filename, "w");

    if (!file) 
    {
        perror("Failed to open file");
        return;
    }

    fprintf(file, "%s", content); 

    fclose(file);
}