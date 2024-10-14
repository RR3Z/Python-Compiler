%{

  #include <stdio.h>
  #include <stdlib.h>
  #include <iostream>
  using namespace std;

  int yylex(void);
  int yyerror(const char *s);
%}

%token FOR
%token WHILE
%token IN
%token IF
%token ELSE
%token ELIF
%token TRY
%token EXCEPT
%token FINALLY

%token ID

%token NEWLINE

%token INDENT
%token DEDENT

%token INT
%token FLOAT
%token COMPLEX
%token TRUE
%token FALSE
%token STR
%token NONE

%token '='
%token PLUS_ASSIGN
%token MINUS_ASSIGN
%token MULT_ASSIGN
%token DEGREE_ASSIGN
%token DIV_ASSIGN
%token MOD_ASSIGN

%token AND
%token OR
%token NOT

%left '+' '-' 
%left '*' '/' '%' FLOOR_DIV
%left UPLUS
%left UMINUS
%right DEGREE

%left AND
%left OR
%left NOT

%left '<' LESSER_EQUAL '>' GREATER_EQUAL NOT_EQUAL EQUAL

%start program

%%

program: 
       ;

expr: ID 
	| INT 
	| FLOAT
	| COMPLEX
	| TRUE
	| FALSE
	| STR
	| NONE
	| expr '+' expr
	| expr '-' expr
	| expr '*' expr
	| expr '/' expr
	| expr FLOOR_DIV expr
	| expr '%' expr
	| expr DEGREE expr
	| '+' expr %prec UPLUS
	| '-' expr %prec UMINUS
	| expr '=' expr
	| expr PLUS_ASSIGN expr
	| expr MINUS_ASSIGN expr
	| expr MULT_ASSIGN expr
	| expr DEGREE_ASSIGN expr
	| expr DIV_ASSIGN expr
	| expr MOD_ASSIGN expr
	| expr AND expr
	| expr OR expr
	| expr NOT expr
	| expr '<' expr
	| expr LESSER_EQUAL expr
	| expr '>' expr
	| expr GREATER_EQUAL expr
	| expr NOT_EQUAL expr
	| expr EQUAL expr
	;

exprList: exprList ',' expr
		| expr
		;

exprListEmpty: exprList
			 |
			 ;

idList: ID
	  | idList ',' ID
	  ;

body: expr
	| NEWLINE INDENT exprList DEDENT
    ;

elseStmt: ELSE ':' expr
		| ELSE ':' body
		;

forStmt: FOR idList IN expr ':' body elseStmt
		| FOR idList IN expr ':' body
		;

whileStmt: WHILE expr ':' body elseStmt
		 | WHILE expr ':' body
		 ;

ifStmt: IF expr ':' expr
	  | IF expr ':' body
	  | IF expr ':' body elseStmt
	  | IF expr ':' body elifStmtList
	  | IF expr ':' body elifStmtList elseStmt
	  ;

elifStmt: ELIF ':' expr
		| ELIF ':' body
		;

elifStmtList: elifStmt
			| elifStmtList elifStmt
			;

tryStmt: TRY ':' expr
	   | TRY ':' body
	   | TRY ':' body elseStmt
	   | TRY ':' body exceptStmtList
	   | TRY ':' body exceptStmtList elseStmt
	   | TRY ':' body exceptStmtList finallyStmt
	   | TRY ':' body exceptStmtList finallyStmt elseStmt
	   ;

exceptStmt: EXCEPT ':' expr
		  | EXCEPT ':' body
		  ;

exceptStmtList: exceptStmt
			  | exceptStmtList exceptStmt
			  ;

finallyStmt: FINALLY ':' expr
           | FINALLY ':' body
		   ;

%%

int yyerror(const char *errormsg)
{
      fprintf(stderr, "%s\n", errormsg);
      exit(1);
}