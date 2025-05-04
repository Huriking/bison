%{
	#include<math.h>
	#include "fact.tab.h"
%}

%%

[0-9]+		{yylval.inti=atoi(yytext); return NUMBER;}
\n		{return EOL;}
.			;
%%
	int yywrap(){
	return 1;
}