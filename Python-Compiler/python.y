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

program: assignmentStmt
       ;

expr: ID
    ;

assignmentStmt: targetAssignList expr
              ;

targetAssignList: targetList '='
                | targetAssignList '=' targetList '='
                ;

target: ID
      | '(' targetListEmpty ')'
      | '[' targetListEmpty ']'
      ;

targetList: target
          | targetList ',' target
          ;

targetListEmpty: targetList
               | targetList ','
               | /* empty */
               ;

%%

int yyerror(const char *errormsg)
{
      fprintf(stderr, "%s\n", errormsg);
      exit(1);
}