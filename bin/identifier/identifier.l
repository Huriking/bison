%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

char *symbol_table[MAX];
int symbol_count = 0;

// List of C keywords to ignore
const char *keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do",
    "double", "else", "enum", "extern", "float", "for", "goto", "if",
    "inline", "int", "long", "register", "restrict", "return", "short",
    "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
    "unsigned", "void", "volatile", "while", "_Alignas", "_Alignof",
    "_Atomic", "_Bool", "_Complex", "_Generic", "_Imaginary", "_Noreturn",
    "_Static_assert", "_Thread_local"
};
int keyword_count = sizeof(keywords) / sizeof(keywords[0]);

int is_keyword(const char *id) {
    for (int i = 0; i < keyword_count; i++) {
        if (strcmp(keywords[i], id) == 0)
            return 1;
    }
    return 0;
}

int exists_in_table(const char *id) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i], id) == 0)
            return 1;
    }
    return 0;
}

void insert_symbol(const char *id) {
    if (!is_keyword(id) && !exists_in_table(id)) {
        symbol_table[symbol_count++] = strdup(id);
    }
}
%}

%%

[a-zA-Z_][a-zA-Z0-9_]*   { insert_symbol(yytext); }

[ \t\n]+                ;  // Skip whitespace

.                       ;  // Skip other characters

%%

int main() {
    printf("Enter C code (Ctrl+D or Ctrl+Z + Enter to end):\n");
    yylex();

    printf("\n--- Symbol Table (Excluding C Keywords) ---\n");
    for (int i = 0; i < symbol_count; i++) {
        printf("%s\n", symbol_table[i]);
    }

    return 0;
}

int yywrap() {
    return 1;
}
