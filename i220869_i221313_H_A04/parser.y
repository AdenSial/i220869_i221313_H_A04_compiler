%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

// Declare external lexer function and error handler
int yylex();
void yyerror(const char *s) { fprintf(stderr, "Syntax error: %s\n", s); exit(1); }

// Root of AST
ASTNode *root;
%}

/* Define types carried by tokens and non-terminals */
%union {
    double number;
    char *string;
    int boolean;
    ASTNode *node;
}

/* Token declarations with types */
%token <string> STRING
%token <number> NUMBER
%token <boolean> TRUE FALSE
%token NULL_TOK
%token COLON COMMA LBRACE RBRACE LBRACK RBRACK

/* Non-terminal types */
%type <node> value object array pair pair_list value_list

%%

json:
    value              { root = $1; }
    ;

value:
      STRING           { $$ = make_string_node($1); }
    | NUMBER           { $$ = make_number_node($1); }
    | TRUE             { $$ = make_bool_node(1); }
    | FALSE            { $$ = make_bool_node(0); }
    | NULL_TOK         { $$ = make_null_node(); }
    | object           { $$ = $1; }
    | array            { $$ = $1; }
    ;

object:
    LBRACE pair_list RBRACE { $$ = make_object_node($2); }
    ;

pair_list:
      pair                        { $$ = make_pair_list_node($1, NULL); }
    | pair COMMA pair_list        { $$ = make_pair_list_node($1, $3); }
    ;

pair:
    STRING COLON value { $$ = make_key_value_node($1, $3); }
    ;

array:
    LBRACK value_list RBRACK { $$ = make_array_node($2); }
    ;

value_list:
      value                        { $$ = make_value_list_node($1, NULL); }
    | value COMMA value_list       { $$ = make_value_list_node($1, $3); }
    ;

%%
