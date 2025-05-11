#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "csv_writer.h"

static int global_id = 1;

typedef struct CSVFile {
    char *name;
    char **columns;
    int col_count;
    char ***rows;
    int row_count;
    int row_capacity;
    struct CSVFile *next;
} CSVFile;

static CSVFile *csv_files = NULL;

static void add_csv_column(CSVFile *file, const char *col_name) {
    for (int i = 0; i < file->col_count; i++)
        if (strcmp(file->columns[i], col_name) == 0)
            return;
    file->columns = realloc(file->columns, sizeof(char*) * (file->col_count + 1));
    file->columns[file->col_count++] = strdup(col_name);
}

static CSVFile *get_or_create_csv(const char *name) {
    for (CSVFile *f = csv_files; f; f = f->next)
        if (strcmp(f->name, name) == 0)
            return f;

    CSVFile *newf = calloc(1, sizeof(CSVFile));
    newf->name = strdup(name);
    newf->next = csv_files;
    csv_files = newf;
    return newf;
}

static void add_csv_row(CSVFile *file, char **values) {
    if (file->row_count == file->row_capacity) {
        file->row_capacity = file->row_capacity ? file->row_capacity * 2 : 4;
        file->rows = realloc(file->rows, sizeof(char**) * file->row_capacity);
    }
    file->rows[file->row_count++] = values;
}

static char *int_to_str(int val) {
    char *buf = malloc(16);
    snprintf(buf, 16, "%d", val);
    return buf;
}

static void process_node(ASTNode *node, const char *table_name, int parent_id, const char *parent_key);

static int process_object(ASTNode *node, const char *table_name, int parent_id, const char *parent_key) {
    CSVFile *file = get_or_create_csv(table_name);
    char **row = calloc(sizeof(char*), 64); // assume max 64 cols
    int row_id = global_id++;
    add_csv_column(file, "id");
    row[0] = int_to_str(row_id);

    if (parent_key) {
        char fk_name[64];
        snprintf(fk_name, sizeof(fk_name), "%s_id", parent_key);
        add_csv_column(file, fk_name);
        row[1] = int_to_str(parent_id);
    }

    int col_offset = parent_key ? 2 : 1;
    int i = col_offset;

    for (ASTNode *p = node->child; p; p = p->next) {
        const char *key = p->key->string_value;
        ASTNode *val = p->value;

        if (val->type == AST_OBJECT) {
            int fk_id = process_object(val, "users", 0, NULL); // user objects reused, common table
            char col[64];
            snprintf(col, sizeof(col), "%s_id", key);
            add_csv_column(file, col);
            row[i++] = int_to_str(fk_id);
        }
        else if (val->type == AST_ARRAY) {
            process_node(val, key, row_id, table_name);
        }
        else {
            add_csv_column(file, key);
            char *value = NULL;
            if (val->type == AST_STRING) value = strdup(val->string_value);
            else if (val->type == AST_NUMBER) {
                value = malloc(32);
                snprintf(value, 32, "%.2f", val->number_value);
            } else if (val->type == AST_BOOL)
                value = strdup(val->bool_value ? "true" : "false");
            else value = strdup("");

            row[i++] = value;
        }
    }

    add_csv_row(file, row);
    return row_id;
}

static void process_array(ASTNode *node, const char *key, int parent_id, const char *parent_table) {
    ASTNode *first = node->child;
    if (!first) return;

    if (first->type == AST_OBJECT) {
        int seq = 0;
        for (ASTNode *el = node->child; el; el = el->next, seq++) {
            CSVFile *file = get_or_create_csv(key);
            add_csv_column(file, "seq");
            char **row = calloc(sizeof(char*), 64);
            row[0] = int_to_str(parent_id);
            row[1] = int_to_str(seq);

            int fk = process_object(el, "users", 0, NULL); // reuse common object table if needed

            int i = 2;
            for (ASTNode *p = el->child; p; p = p->next) {
                const char *col = p->key->string_value;
                ASTNode *val = p->value;
                add_csv_column(file, col);
                if (val->type == AST_STRING) row[i++] = strdup(val->string_value);
                else if (val->type == AST_NUMBER) {
                    row[i] = malloc(32);
                    snprintf(row[i++], 32, "%.2f", val->number_value);
                } else if (val->type == AST_BOOL) row[i++] = strdup(val->bool_value ? "true" : "false");
                else row[i++] = strdup("");
            }

            add_csv_column(file, parent_table ? strcat(strdup(parent_table), "_id") : "parent_id");
            add_csv_row(file, row);
        }
    } else {
        CSVFile *file = get_or_create_csv(key);
        add_csv_column(file, parent_table ? strcat(strdup(parent_table), "_id") : "parent_id");
        add_csv_column(file, "index");
        add_csv_column(file, "value");

        int index = 0;
        for (ASTNode *el = node->child; el; el = el->next, index++) {
            char **row = calloc(sizeof(char*), 3);
            row[0] = int_to_str(parent_id);
            row[1] = int_to_str(index);
            if (el->type == AST_STRING) row[2] = strdup(el->string_value);
            else if (el->type == AST_NUMBER) {
                row[2] = malloc(32);
                snprintf(row[2], 32, "%.2f", el->number_value);
            } else if (el->type == AST_BOOL) row[2] = strdup(el->bool_value ? "true" : "false");
            else row[2] = strdup("");
            add_csv_row(file, row);
        }
    }
}

static void process_node(ASTNode *node, const char *table_name, int parent_id, const char *parent_key) {
    if (node->type == AST_OBJECT)
        process_object(node, table_name, parent_id, parent_key);
    else if (node->type == AST_ARRAY)
        process_array(node, table_name, parent_id, parent_key);
}

SchemaList *analyze_ast(ASTNode *root) {
    global_id = 1;
    process_node(root, "root", 0, NULL);
    return NULL;
}

void generate_csvs(SchemaList *schemas, const char *out_dir) {
    (void)schemas;
    mkdir(out_dir, 0777);
    for (CSVFile *f = csv_files; f; f = f->next) {
        char path[512];
        snprintf(path, sizeof(path), "%s/%s.csv", out_dir, f->name);
        FILE *fp = fopen(path, "w");
        if (!fp) { perror("fopen"); continue; }

        for (int i = 0; i < f->col_count; i++) {
            fprintf(fp, "%s%s", i ? "," : "", f->columns[i]);
        }
        fprintf(fp, "\n");

        for (int r = 0; r < f->row_count; r++) {
            for (int c = 0; c < f->col_count; c++) {
                if (c > 0) fprintf(fp, ",");
                char *v = f->rows[r][c];
                fprintf(fp, "%s", v ? v : "");
            }
            fprintf(fp, "\n");
        }

        fclose(fp);
        printf("Generated: %s\n", path);
    }
}

void free_schemas(SchemaList *schemas) {
    (void)schemas;
    // free CSVFile list (optional)
}