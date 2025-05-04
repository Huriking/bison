%{
	#include <stdio.h>
	#include "fact.tab.h"
	
	int yylex(void);
	int factorial(int num);
%}

%union{
	int inti;
	}
	
%token<inti> NUMBER;
%token EOL;
%left NUMBER;

%%
input:
	NUMBER EOL		{printf("The factorial is:%ld",factorial($1));}
	;
%%

int factorial(int num){
	int i;
	long int fact=1;
	if(num<0)
	return 0;
	if(num==0)
	return 1;
	for(i=1;i<=num;i++){
	fact=fact*i;
	}
	return fact;
}

void yyerror(char* s){
;
}

int main(){
	printf("\nEnter the number to be evaluated:\n");
	return yyparse();
}