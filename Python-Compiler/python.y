%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <iostream>
  using namespace std;

  int yylex(void);
  int yyerror(const char *s);
%}

%token
LITERAL_INT LITERAL_FLOAT STRING
TRUE FALSE NONE ID

'=' '+' '-' '*' '/' '%'
PLUS_ASSIGN MINUS_ASSIGN MULT_ASSIGN
POW_ASSIGN DIV_ASSIGN MOD_ASSIGN
FLOOR_DIV UPLUS UMINUS POW

'<' '>' EQUAL NOT_EQUAL
LESSER_EQUAL GREATER_EQUAL

IF ELSE ELIF
FOR WHILE IN
TRY EXCEPT FINALLY
AS AND OR NOT

NEWLINE INDENT DEDENT

LAMBDA

%left '+' '-'
%left '*' '/' '%' FLOOR_DIV
%left UPLUS
%left UMINUS
%left AND OR NOT
%left '<' LESSER_EQUAL '>' GREATER_EQUAL NOT_EQUAL EQUAL
%left ','

%right POW
%right LAMBDA

%start program

%%

program: expr
	   | idList
       ;

varDeclaration: idList '=' exprList
			  | idListAssign '=' expr
			  ;

varList: varDeclaration
	   | varList ',' varDeclaration
	   ;

idList: ID
	  | idList ',' ID
	  ;

idListEmpty: idList
		   | idList ','
		   | /* EMPTY */
		   ;

idListAssign: idList '=' expr
			| idListAssign '=' expr
			;

expr: ID
	| LITERAL_INT 
	| LITERAL_FLOAT
	| STRING
	| TRUE
	| FALSE
	| NONE
	/*
	| expr '+' expr
	| expr '-' expr
	| expr '*' expr
	| expr '/' expr
	| expr FLOOR_DIV expr
	| expr '%' expr
	| expr POW expr
	| '+' expr %prec UPLUS
	| '-' expr %prec UMINUS
	| ID PLUS_ASSIGN expr
	| ID MINUS_ASSIGN expr
	| ID MULT_ASSIGN expr
	| ID POW_ASSIGN expr 
	| ID DIV_ASSIGN expr
	| ID MOD_ASSIGN expr
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
	| '(' exprListEmpty ')'
	| expr '(' exprListEmpty ')'
	| LAMBDA idListEmpty ':' expr
	*/
	;

exprList: expr
		| exprList ','
		| exprList ',' expr
		;

exprListEmpty: exprList
			 | /* EMPTY */
			 ;

exprNamed: expr
		 | expr AS ID
		 ;

slice: sliceDim ':' sliceDim ':' sliceDim
	 | sliceDim ':' sliceDim
	 ;

sliceDim: LITERAL_INT
	    | /* EMPTY */
		;

stmt: /* TODO */ 
	;

stmtList: /* TODO */
		;

suite: stmt
	 | NEWLINE INDENT stmtList DEDENT 
     ;

elseStmt: ELSE ':' stmt
		| ELSE ':' suite
		;

forStmt: FOR idList IN expr ':' suite
		| FOR idList IN expr ':' suite elseStmt
		;

whileStmt: WHILE expr ':' suite
		 | WHILE expr ':' suite elseStmt
		 ;

tryStmt: TRY ':' suite exceptStmtList
	   | TRY ':' suite exceptStmtList elseStmt
	   | TRY ':' suite exceptStmtList FINALLY ':' suite
	   | TRY ':' suite exceptStmtList elseStmt FINALLY ':' suite
	   | TRY ':' suite FINALLY ':' suite
	   ;

exceptStmt: EXCEPT ':' suite
		  | EXCEPT exprNamed ':' suite
		  ;

exceptStmtList: exceptStmt
			  | exceptStmtList exceptStmt
			  ;

ifStmt: IF expr ':' suite
	  | IF expr ':' suite elseStmt
	  | IF expr ':' suite elifStmtList
	  | IF expr ':' suite elifStmtList elseStmt
	  ;

elifStmt: ELIF ':' suite
		;

elifStmtList: elifStmt
			| elifStmtList elifStmt
			;

%%

int yyerror(const char *errormsg)
{
      fprintf(stderr, "%s\n", errormsg);
      exit(1);
}