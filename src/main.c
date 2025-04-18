#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Lexer/lexer.h"
#include "Parser/parser.h"
#include "Parser/AST/ast.h"
#include "Compiler/compiler.h"
#include <awe/util.h>

char* readFile(const char* filename);
void writeFile(const char *filename, const char *content);

char* itoa(int num);

/* Main Function */

int main(int argc, char *argv[])
{
    /* Handle flags */

    int opt;
    char *output_file = "build/tokens/debug.tokens";

    while ((opt = getopt(argc, argv, "o:")) != -1) 
    {
        switch (opt) 
        {
            case 'o':
                output_file = optarg; 
                printf("Output file: %s\n", output_file);
                break;
            case '?': /* Unknown */
                fprintf(stderr, "Usage: %s [-o output_file]\n", argv[0]);
                return 1;
        }
    }

    char* file = readFile("Tests/1.fl");

    if (file == NULL)
    {
        perror("File not found!");
        exit(1);
    }

    TokenList tokens = tokenize(file);

    free(file);

    if (!tokens.tokens)
    {
        fprintf(stderr, "Failed to tokenize input.\n");
        return 1;
    }

    printf("Writing tokens to:  %s\n", output_file);
    char buf[16384] = {0};
    const char* seperator = " ";
    const char* seperator_lines = "\n";
    const char* end_msg = "END OF TOKENS";

    for (size_t i = 0; i < tokens.count; i++)
    {
        char* type_str = itoa(tokens.tokens[i].type);
        if (!type_str)
            continue;

        strncat(buf, type_str, sizeof(buf) - strlen(buf) - 1);

        if ((i + 1) % 10 == 0)
            strncat(buf, seperator_lines, sizeof(buf) - strlen(buf) - 1);
        else
            strncat(buf, seperator, sizeof(buf) - strlen(buf) - 1);
    }
    printf("Wrote tokens to:  %s\n", output_file);

    strncat(buf, end_msg, sizeof(buf) - strlen(buf) - 1);

    writeFile(output_file, buf);

    printf("Wrote tokens to:  %s\n", output_file);

    ASTNode* ast = parse(&tokens);

    compile(ast);

    for (size_t i = 0; i < tokens.count; i++)
    {
        free(tokens.tokens[i].value);
    }
    free(tokens.tokens);

    return 0;
}

char* readFile(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file)
        return NULL;

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = malloc(size + 1);
    if (!buffer)
    {
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, size, file);
    buffer[size] = '\0';
    fclose(file);

    return buffer;
}

void writeFile(const char *filename, const char *content) 
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

char* itoa(int num)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "%d", num);
    return strdup(buf);
}