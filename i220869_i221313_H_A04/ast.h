#ifndef AST_H
#define AST_H

typedef enum { AST_STRING, AST_NUMBER, AST_BOOL, AST_NULL, AST_OBJECT, AST_ARRAY, AST_PAIR } ASTType;

typedef struct ASTNode {
    ASTType type;
    union {
        char *string_value;
        double number_value;
        int bool_value;
        struct {
            struct ASTNode *key;
            struct ASTNode *value;
        };
        struct ASTNode *child;
    };
    struct ASTNode *next; // for lists
} ASTNode;

ASTNode *make_string_node(char *s);
ASTNode *make_number_node(double n);
ASTNode *make_bool_node(int b);
ASTNode *make_null_node();
ASTNode *make_object_node(ASTNode *pairs);
ASTNode *make_key_value_node(char *key, ASTNode *value);
ASTNode *make_array_node(ASTNode *values);
ASTNode *make_pair_list_node(ASTNode *pair, ASTNode *rest);
ASTNode *make_value_list_node(ASTNode *val, ASTNode *rest);
void print_ast(ASTNode *node, int indent);

#endif
