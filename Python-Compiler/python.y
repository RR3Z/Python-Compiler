%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <iostream>
  using namespace std;

  int yylex(void);
  int yyerror(const char *s);
%}

%token INT_C FLOAT_C STRING_C ID TRUE FALSE
%token NEWLINE INDENT DEDENT
%token AND OR
%token GT GE LT LE EQ NE
%token ASSIGN_OP UMINUS ':'
%token IF ELSE ELIF
%token TEST /*TODO: REMOVE IT*/

%left '='
%left '|' '&' AND OR
%left GT GE LT LE EQ NE
%left '+' '-'
%left '*' '/'
%right ASSIGN_OP
%right UMINUS

%start program

%%

program: stmtsList
       ;

stmt: compoundStmt
    | simpleStmt
    ;

stmtsList: stmt
         | stmtsList stmt
         ;

compoundStmt: ifStmt
            | expr NEWLINE
            ;

simpleStmt: assignmentStmt
          ;

simpleStmtList: simpleStmt
              | simpleStmtList ';' simpleStmt
              ;

suite: NEWLINE INDENT stmtsList DEDENT
     ;

// Start of IF STATEMENT

/*
    IF ... : ...
    IF ... : ... ELSE : ...
    IF ... : ... ELIF : ...
    IF ... : ... ELIF : ... ELSE : ...

    In this case, there may be several elif blocks, or there may not be
*/

ifStmt: IF expr ':' suite
      | IF expr ':' suite ELSE ':' suite
      | IF expr ':' suite elifStmtList
      | IF expr ':' suite elifStmtList ELSE ':' suite
      ;

elifStmtList: ELIF ':' suite
            | elifStmtList ELIF ':' suite
            ;

// Start of ASSIGNMENT STATEMENT

assignmentStmt: TEST
              ;


expr: expr '+' expr
    | expr '-' expr
    | expr '*' expr
    | expr '/' expr
    | expr AND expr
    | expr '&' expr
    | expr OR expr
    | expr '|' expr
    | expr GT expr
    | expr GE expr
    | expr LT expr
    | expr LE expr
    | expr EQ expr
    | expr NE expr
    | ID ASSIGN_OP expr
    | '(' expr ')'
    | '-' expr %prec UMINUS
    | INT_C
    | FLOAT_C
    | STRING_C
    | TRUE
    | FALSE
    | ID
    ;

%%

int yyerror(const char *errormsg)
{
      fprintf(stderr, "%s\n", errormsg);
      exit(1);
}