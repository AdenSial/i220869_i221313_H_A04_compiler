#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

ASTNode *new_node(ASTType type) {
    ASTNode *n = calloc(1, sizeof(ASTNode));
    n->type = type;
    return n;
}

ASTNode *make_string_node(char *s) {
    ASTNode *n = new_node(AST_STRING);
    n->string_value = strdup(s);
    return n;
}
ASTNode *make_number_node(double n) {
    ASTNode *node = new_node(AST_NUMBER);
    node->number_value = n;
    return node;
}
ASTNode *make_bool_node(int b) {
    ASTNode *n = new_node(AST_BOOL);
    n->bool_value = b;
    return n;
}
ASTNode *make_null_node() {
    return new_node(AST_NULL);
}
ASTNode *make_key_value_node(char *key, ASTNode *val) {
    ASTNode *n = new_node(AST_PAIR);
    n->key = make_string_node(key);
    n->value = val;
    return n;
}
ASTNode *make_object_node(ASTNode *pairs) {
    ASTNode *n = new_node(AST_OBJECT);
    n->child = pairs;
    return n;
}
ASTNode *make_array_node(ASTNode *values) {
    ASTNode *n = new_node(AST_ARRAY);
    n->child = values;
    return n;
}
ASTNode *make_pair_list_node(ASTNode *pair, ASTNode *rest) {
    pair->next = rest;
    return pair;
}
ASTNode *make_value_list_node(ASTNode *val, ASTNode *rest) {
    val->next = rest;
    return val;
}

void print_indent(int indent) {
    for (int i = 0; i < indent; i++) printf("  ");
}
void print_ast(ASTNode *node, int indent) {
    if (!node) return;
    print_indent(indent);
    switch (node->type) {
        case AST_STRING: printf("STRING: %s\n", node->string_value); break;
        case AST_NUMBER: printf("NUMBER: %lf\n", node->number_value); break;
        case AST_BOOL:   printf("BOOL: %s\n", node->bool_value ? "true" : "false"); break;
        case AST_NULL:   printf("NULL\n"); break;
        case AST_ARRAY:
            printf("ARRAY\n");
            for (ASTNode *c = node->child; c; c = c->next) print_ast(c, indent + 1);
            break;
        case AST_OBJECT:
            printf("OBJECT\n");
            for (ASTNode *p = node->child; p; p = p->next) print_ast(p, indent + 1);
            break;
        case AST_PAIR:
            printf("PAIR\n");
            print_ast(node->key, indent + 1);
            print_ast(node->value, indent + 1);
            break;
    }
}
