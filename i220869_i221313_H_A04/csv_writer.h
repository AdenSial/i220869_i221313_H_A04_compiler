#ifndef CSV_WRITER_H
#define CSV_WRITER_H

#include "ast.h"

typedef struct TableSchema {
    char *name;
    char **columns;
    int col_count;
    struct TableSchema *next;
} TableSchema;

typedef struct Row {
    char **values;
    struct Row *next;
} Row;

typedef struct Table {
    TableSchema *schema;
    Row *rows;
    struct Table *next;
} Table;

typedef struct SchemaList {
    Table *tables;
} SchemaList;

SchemaList *analyze_ast(ASTNode *root);
SchemaList *analyze_ast_named(ASTNode *root, const char *top_table_name);
void generate_csvs(SchemaList *schemas, const char *out_dir);
void free_schemas(SchemaList *schemas);

#endif
