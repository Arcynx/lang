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

/* Adds a new token to a tokenlist */

void add_token(TokenList* list, TOKEN_TYPE type, const char* value) 
{
    if (list->count >= list->capacity) {
        size_t new_capacity = (list->capacity == 0) ? 8 : list->capacity * 2;
        Token* new_tokens = realloc(list->tokens, new_capacity * sizeof(Token));
        if (!new_tokens) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
        list->tokens = new_tokens;
        list->capacity = new_capacity;
    }

    list->tokens[list->count].type = type;
    list->tokens[list->count].value = value ? strdup(value) : NULL;
    list->count++;
}

TOKEN_TYPE match_keyword(const char* word) 
{
    if (strcmp(word, "fn") == 0) return TOKEN_FN;
    if (strcmp(word, "let") == 0) return TOKEN_LET;
    if (strcmp(word, "int") == 0) return TOKEN_TYPE_INT;
    if (strcmp(word, "float") == 0) return TOKEN_TYPE_FLOAT;
    if (strcmp(word, "string") == 0) return TOKEN_TYPE_STRING;
    if (strcmp(word, "use") == 0) return TOKEN_USE;
    if (strcmp(word, "if") == 0) return TOKEN_IF;
    if (strcmp(word, "else") == 0) return TOKEN_ELSE;
    if (strcmp(word, "while") == 0) return TOKEN_WHILE;
    if (strcmp(word, "ret") == 0) return TOKEN_RET;
    if (strcmp(word, "for") == 0) return TOKEN_FOR;
    if (strcmp(word, "extern") == 0) return TOKEN_EXTERN;
    if (strcmp(word, "void") == 0) return TOKEN_TYPE_VOID;
    return TOKEN_IDENTIFIER;
}


TokenList tokenize(const char* source) 
{
    TokenList tokens = { NULL, 0, 0 };
    size_t length = strlen(source);

    for (size_t i = 0; i < length; i++) 
    {
        char c = source[i];

        switch (c) {
            case '(': add_token(&tokens, TOKEN_LEFT_PAREN, "("); break;
            case ')': add_token(&tokens, TOKEN_RIGHT_PAREN, ")"); break;
            case '{': add_token(&tokens, TOKEN_LEFT_BRACE, "{"); break;
            case '}': add_token(&tokens, TOKEN_RIGHT_BRACE, "}"); break;
            case ',': add_token(&tokens, TOKEN_COMMA, ","); break;
            case '.': add_token(&tokens, TOKEN_DOT, "."); break;
            case '-': add_token(&tokens, TOKEN_MINUS, "-"); break;
            case '+': add_token(&tokens, TOKEN_PLUS, "+"); break;
            case ';': add_token(&tokens, TOKEN_SEMICOLON, ";"); break;
            case ':': add_token(&tokens, TOKEN_COLON, ":"); break;
            case '/': 
                if (source[i+1] == '*') 
                {
                    i += 2;
                    while (source[i] != '\0' && !(source[i] == '*' && source[i+1] == '/'))
                            i++;
                    if (source[i] != '\0') i++;
                } 
                else 
                    add_token(&tokens, TOKEN_SLASH, "/");
                break;
            case '*': add_token(&tokens, TOKEN_STAR, "*"); break;
            case '<': add_token(&tokens, TOKEN_LESS, "<"); break;
            case '>': add_token(&tokens, TOKEN_GREATER, ">"); break;
            case '=': add_token(&tokens, TOKEN_EQUAL, "="); break;
            case '!': add_token(&tokens, TOKEN_EXCLAMATION, "!"); break;
            case '[': add_token(&tokens, TOKEN_LEFT_BRACKET, "["); break;
            case ']': add_token(&tokens, TOKEN_RIGHT_BRACKET, "]"); break;
            case '@': add_token(&tokens, TOKEN_AT, "@"); break;
            case ' ':
            case '\n':
            case '\t':
                /* Skip whitespaces */
                break;
            default:
                if (isdigit(c)) 
                {
                    size_t start = i;

                    while (isdigit(source[i])) i++;

                    if (source[i] == '.' && isdigit(source[i + 1])) 
                    {
                        i++;
                        while (isdigit(source[i])) i++;
                        add_token(&tokens, TOKEN_FLOAT, strndup(&source[start], i - start));
                    } else
                        add_token(&tokens, TOKEN_INTEGER, strndup(&source[start], i - start));

                    i--;
                } 

                else if (isalpha(c) || c == '_') 
                {
                    size_t start = i;
                    while (isalnum(source[i]) || source[i] == '_') i++;

                    size_t len = i - start;
                    char* word = strndup(&source[start], len);

                    TOKEN_TYPE type = match_keyword(word);
                    add_token(&tokens, type, word);

                    free(word);
                    i--;
                } 

                else if (c == '"') 
                {
                    size_t start = ++i;
                    while (source[i] != '"' && source[i] != '\0') i++;
                    if (source[i] == '"') 
                        add_token(&tokens, TOKEN_STRING, strndup(&source[start], i - start));
                    else
                        fprintf(stderr, "Unterminated string\n");
                } else
                    fprintf(stderr, "Unknown character: %c\n", c);
        }
    }

    add_token(&tokens, TOKEN_EOF, NULL);
    return tokens;
}
