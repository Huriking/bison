%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int yylex(void);
/* No yyerror function since we assume valid input */
%}

%union {
    double dval;
}

%token <dval> NUMBER
%token PLUS MINUS TIMES DIVIDE POWER
%token LEFT_PAREN RIGHT_PAREN
%token SIN COS TAN

%type <dval> expression term factor power unary function

%left PLUS MINUS
%left TIMES DIVIDE
%right POWER
%left UMINUS

%%
calculation:
    expression              { printf("= %.10g\n", $1); exit(0); }
    ;

expression:
    expression PLUS term    { $$ = $1 + $3; }
    | expression MINUS term { $$ = $1 - $3; }
    | term                  { $$ = $1; }
    ;

term:
    term TIMES factor       { $$ = $1 * $3; }
    | term DIVIDE factor    { $$ = $1 / $3; } /* No division by zero check */
    | factor                { $$ = $1; }
    ;

factor:
    power                   { $$ = $1; }
    | MINUS factor %prec UMINUS { $$ = -$2; }
    ;

power:
    unary                   { $$ = $1; }
    | unary POWER factor    { $$ = pow($1, $3); }
    ;

unary:
    function                { $$ = $1; }
    | LEFT_PAREN expression RIGHT_PAREN { $$ = $2; }
    | NUMBER                { $$ = $1; }
    ;

function:
    SIN LEFT_PAREN expression RIGHT_PAREN    { $$ = sin($3); }
    | COS LEFT_PAREN expression RIGHT_PAREN  { $$ = cos($3); }
    | TAN LEFT_PAREN expression RIGHT_PAREN  { $$ = tan($3); }
    ;

%%

/* Simplified error function (minimal) */
void yyerror(char *s) {
    /* Do nothing since we assume input is correct */
}

int main(void) {
    printf("Scientific Calculator\n");
    printf("Enter a single expression:\n");
    return yyparse();
}