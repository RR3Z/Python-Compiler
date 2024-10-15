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

%token NEWLINE
%token INDENT
%token DEDENT

%token ID
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
%token POW_ASSIGN
%token DIV_ASSIGN
%token MOD_ASSIGN

%token AND
%token OR
%token NOT

%left '+' '-'
%left '*' '/' '%' FLOOR_DIV
%left UPLUS
%left UMINUS
%left AND OR NOT
%left '<' LESSER_EQUAL '>' GREATER_EQUAL NOT_EQUAL EQUAL

%right POW
%right LAMBDA

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
	| expr POW expr
	| '+' expr %prec UPLUS
	| '-' expr %prec UMINUS
	| expr '=' expr
	| expr PLUS_ASSIGN expr
	| expr MINUS_ASSIGN expr
	| expr MULT_ASSIGN expr
	| expr POW_ASSIGN expr
	| expr DIV_ASSIGN expr
	| expr MOD_ASSIGN expr
	| expr AND expr
	| expr OR expr
	| NOT expr
	| expr '<' expr
	| expr LESSER_EQUAL expr
	| expr '>' expr
	| expr GREATER_EQUAL expr
	| expr NOT_EQUAL expr
	| expr EQUAL expr
	| ID '.' ID
	| '[' exprListEmpty ']'
	| '[' expr FOR ID IN expr ']'
	| expr '[' expr ']'
	| expr '[' slice ']'
	| '(' expr ')'
	| expr '(' exprListEmpty ')'
	| LAMBDA idListEmpty ':' expr
	;

exprList: expr
		| exprList ','
		| exprList ',' expr
		;

exprListEmpty: exprList
			 | /* EMPTY */
			 ;

stmt: /* TODO */ 
	;

stmtList: /* TODO */
		;

slice: sliceDim ':' sliceDim ':' sliceDim
	 | sliceDim ':' sliceDim
	 ;

sliceDim: expr
	    | /* EMPTY */
		;

idList: ID
	  | idList ','
	  | idList ',' ID
	  ;

idListEmpty: idList
		   | /* EMPTY */
		   ;

suite: stmt
	 | NEWLINE INDENT stmtList DEDENT
     ;

elseStmt: ELSE ':' stmt
		| ELSE ':' suite
		;

forStmt: FOR idList IN expr ':' suite elseStmt
		| FOR idList IN expr ':' suite
		;

whileStmt: WHILE expr ':' suite
		 | WHILE expr ':' suite elseStmt
		 ;

ifStmt: IF expr ':' expr
	  | IF expr ':' suite
	  | IF expr ':' suite elseStmt
	  | IF expr ':' suite elifStmtList
	  | IF expr ':' suite elifStmtList elseStmt
	  ;

elifStmt: ELIF ':' expr
		| ELIF ':' suite
		;

elifStmtList: elifStmt
			| elifStmtList elifStmt
			;

tryStmt: TRY ':' expr
	   | TRY ':' suite
	   | TRY ':' suite elseStmt
	   | TRY ':' suite exceptStmtList
	   | TRY ':' suite exceptStmtList elseStmt
	   | TRY ':' suite exceptStmtList finallyStmt
	   | TRY ':' suite exceptStmtList finallyStmt elseStmt
	   ;

exceptStmt: EXCEPT ':' expr
		  | EXCEPT ':' suite
		  ;

exceptStmtList: exceptStmt
			  | exceptStmtList exceptStmt
			  ;

finallyStmt: FINALLY ':' expr
           | FINALLY ':' suite
		   ;

%%

int yyerror(const char *errormsg)
{
      fprintf(stderr, "%s\n", errormsg);
      exit(1);
}