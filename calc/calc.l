%{
#include <stdlib.h>
#include <math.h>
#include "calc.tab.h"
%}

%%
[0-9]+\.?[0-9]*([eE][-+]?[0-9]+)?  { yylval.dval = atof(yytext); return NUMBER; }
"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return TIMES; }
"/"                     { return DIVIDE; }
"^"                     { return POWER; }
"("                     { return LEFT_PAREN; }
")"                     { return RIGHT_PAREN; }
"sin"                   { return SIN; }
"cos"                   { return COS; }
"tan"                   { return TAN; }
"pi"                    { yylval.dval = M_PI; return NUMBER; }
[ \t\n]                 { /* ignore whitespace */ }
.                       { /* ignore any other character */ }
%%

int yywrap(void) {
    return 1;
}