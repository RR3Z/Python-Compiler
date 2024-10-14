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

%start program

%%

program: 
       ;

expr:
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