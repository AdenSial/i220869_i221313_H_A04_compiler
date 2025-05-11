#include <stdio.h>
#include "ast.h"
#include "csv_writer.h"

extern ASTNode *root;
extern int yyparse();

int main(int argc, char **argv) {
    if (yyparse() == 0) {
        printf("Parsing successful. AST:\n");
        print_ast(root, 0);

        printf("\nStarting semantic analysis...\n");
        SchemaList *schemas = analyze_ast(root);

        printf("\nGenerating CSV files...\n");
        generate_csvs(schemas, "./output");

        printf("\nCleaning up...\n");
        free_schemas(schemas);
    } else {
        fprintf(stderr, "Parsing failed.\n");
    }
    return 0;
}