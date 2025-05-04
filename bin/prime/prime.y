%{
	#include<stdio.h>
	#include<math.h>
	#include<string.h>
	int yylex(void);
	int isPrime(int num);
%}

%union{
	int inti;
}

%token<inti> NUMBER;
%token EOL;

%left NUMBER;

%%
	input:
		NUMBER EOL		{if(isPrime($1))
							printf("Given number is a prime");
						 else
						 	printf("Given number is not a prime");
						}
		;
%%

int isPrime(int num){
	int result;
	int i=2;
	for(i=2;i<num;i++){
	if(num%i==0){
		result=0;
		return result;
	}
	}
	result=1;
	return result;
}

void yyerror(void){
;}

int main(void){
	printf("Enter the number to be evaluated:");
	return yyparse();
}