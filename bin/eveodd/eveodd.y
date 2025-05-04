%{
	#include<stdio.h>
	#include<math.h>
	#include<string.h>
	int yylex(void);
	int isEven(int num);
%}

%union{
	int inti;
}

%token<inti> NUMBER;
%token EOL;

%left NUMBER;

%%
	input:
		NUMBER EOL		{if(isEven($1))
							printf("Given number is a even number");
						 else
						 	printf("Given number is a odd number");
						}
		;
%%

int isEven(int num){
	int result;
	int i=2;
		result=0;
	if(num%2==0)
		result=1;
	return result;
}

void yyerror(void){
;}

int main(void){
	printf("Enter the number to be evaluated:");
	return yyparse();
}