%{
#include "ast.h"
#include "parser.tab.h"
#include <string.h>
int line = 1, col = 1;
%}

%%

"true"      { yylval.boolean = 1; return TRUE; }
"false"     { yylval.boolean = 0; return FALSE; }
"null"      { return NULL_TOK; }

-?[0-9]+\.[0-9]+([eE][-+]?[0-9]+)? { yylval.number = atof(yytext); return NUMBER; }
-?[0-9]+                          { yylval.number = atof(yytext); return NUMBER; }

\"([^\\\"]|\\.)*\" {
    yylval.string = strdup(yytext);
    return STRING;
}

[ \t\r]+     { col += yyleng; }
\n           { line++; col = 1; }

":"          { return COLON; }
","          { return COMMA; }
"{"          { return LBRACE; }
"}"          { return RBRACE; }
"["          { return LBRACK; }
"]"          { return RBRACK; }

.            { fprintf(stderr, "Unexpected char '%c' at %d:%d\n", *yytext, line, col); exit(1); }

%%
int yywrap() { return 1; }
