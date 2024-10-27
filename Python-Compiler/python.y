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
%token ASSIGN_OP
%token PLUS_ASSIGN MINUS_ASSIGN MUL_ASSIGN DIV_ASSIGN
%token IF ELSE ELIF
%token FOR WHILE IN
%token TRY FINALLY EXCEPT AS
%token DEF CLASS
%token RETURN LAMBDA

%right ASSIGN_OP '=' PLUS_ASSIGN MINUS_ASSIGN MUL_ASSIGN DIV_ASSIGN
%left LAMBDA
%left '+' '-'
%left '*' '/'
%left '|' '&' AND OR
%left GT GE LT LE EQ NE
%right UPLUS UMINUS

%start program

%%

program: stmtsList
       ;

stmt: compoundStmt
    | simpleStmt
    | stmt NEWLINE
    ;

stmtsList: stmt
         | stmtsList stmt
         ;

compoundStmt: ifStmt
            | forStmt
            | whileStmt
            | tryStmt
            | funcDef
            | classDef
            ;

simpleStmt: assignStmt
          | returnStmt
          ;

simpleStmtList: simpleStmt
              | simpleStmtList ';' simpleStmt
              ;

suite: NEWLINE INDENT stmtsList DEDENT
     | simpleStmtList NEWLINE
     | simpleStmtList ';' NEWLINE
     ;

// IF STATEMENT

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

// FOR STATEMENT

forStmt: FOR targetList IN expr ':' suite
       | FOR targetList IN expr ':' suite ELSE ':' suite
       ;

// WHILE STATEMENT

whileStmt: WHILE expr ':' suite
         | WHILE expr ':' suite ELSE ':' suite
         ;

// TRY STATEMENT

tryStmt: TRY ':' suite exceptStmtList
       | TRY ':' suite exceptStmtList ELSE ':' suite
       | TRY ':' suite exceptStmtList FINALLY ':' suite
       | TRY ':' suite exceptStmtList ELSE ':' suite FINALLY ':' suite
       | TRY ':' suite FINALLY ':' suite
       ;

exceptStmt: EXCEPT ':' suite
          | EXCEPT expr ':' suite
          | EXCEPT expr AS identifier ':' suite
          ;

exceptStmtList: exceptStmt
              | exceptStmtList exceptStmt
              ;

// FUNCTION DEFINITION

funcDef: DEF identifier '(' paramsListE ')' ':' suite
       ;

param: identifier
     | identifier '=' expr
     ;

paramsList: param
          | paramsList ',' param
          ;

paramsListE: paramsList
           | paramsList ','
           | /* empty */
           ;

// CLASS DEFINITION

classDef: CLASS identifier ':' suite
        | CLASS identifier '(' identifiersE ')' ':' suite
        ;

// ASSIGNMENT STATEMENT

assignStmt: assignStmtTargetAssignList '=' expr
          | identifier PLUS_ASSIGN expr
          | identifier MINUS_ASSIGN expr
          | identifier MUL_ASSIGN expr
          | identifier DIV_ASSIGN expr
          ;

assignStmtTargetAssignList: targetList
                          | targetList ','
                          | assignStmtTargetAssignList '=' targetList
                          | assignStmtTargetAssignList '=' targetList ','
                          ;

// RETURN STATEMENT
 
returnStmt: RETURN exprListE NEWLINE
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
    | '+' expr %prec UPLUS
    | '-' expr %prec UMINUS
    | '(' expr ')'
    | INT_C
    | FLOAT_C
    | STRING_C
    | TRUE
    | FALSE
    | identifier
    | identifier ASSIGN_OP expr
    | LAMBDA paramsListE ':' expr %prec LAMBDA
    ;

exprList: expr
        | exprList ',' expr
        ;

exprListE: exprList
         | exprList ','
         | /* empty */
         ;

identifier: ID
          ;

identifiers: identifier
           | identifiers ',' identifier
           ;

identifiersE: identifiers
            | identifiers ','
            | /* empty */
            ;

targetList: identifier
          | targetList ',' identifier
          ;

%%

int yyerror(const char *errormsg)
{
      fprintf(stderr, "%s\n", errormsg);
      exit(1);
}