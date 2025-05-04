%{
#include <stdio.h>

int char_count = 0;
int word_count = 0;
int sentence_count = 0;
int line_count = 0;
int tab_count = 0;
int number_count = 0;
%}

%%
[\t]+      { char_count ++; tab_count++;}
\n          { char_count++; line_count++; }
[.!?]       { char_count++; sentence_count++; }
[0-9]+      { char_count += yyleng; number_count++; word_count++; }
[A-Za-z]+   { char_count += yyleng; word_count++; }
.           { char_count++; }  // Any other character
%%

int main()
{
    printf("Enter text (Ctrl+D or Ctrl+Z + Enter to end):\n");
    yylex();

    printf("\n---- Statistics ----\n");
    printf("Characters: %d\n", char_count);
    printf("Words:      %d\n", word_count);
    printf("Sentences:  %d\n", sentence_count);
    printf("Lines:      %d\n", line_count);
    printf("Tabs:       %d\n", tab_count);
    printf("Numbers:    %d\n", number_count);
    return 0;
}

int yywrap() {
    return 1;
}
