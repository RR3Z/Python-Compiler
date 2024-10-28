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
%nonassoc LAMBDA
%left OR
%left AND
%right NOT
%left GT GE LT LE EQ NE
%left '|'
%left '&'
%left '+' '-'
%left '*' '/'
%right UPLUS UMINUS
%left '.'
%nonassoc '(' ')'
%nonassoc '[' ']'

%start program

%%

program: stmtsList
       ;

stmt: compoundStmt
    | simpleStmt
    | expr NEWLINE
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
    IF ... : ... ELIF ... : ...
    IF ... : ... ELIF ... : ... ELSE : ...

    In this case, there may be several elif blocks, or there may not be
*/

ifStmt: ifHeader ':' suite
      | ifHeader ':' suite ELSE ':' suite
      | ifHeader ':' suite elifStmtList
      | ifHeader ':' suite elifStmtList ELSE ':' suite
      ;

ifHeader: IF expr
        ;

ifHeaderList: ifHeader
            | ifHeaderList ifHeader
            ;

ifHeaderListE: ifHeaderList
             | /* empty */
             ;

elifStmtList: ELIF expr ':' suite
            | elifStmtList ELIF expr ':' suite
            ;

// FOR STATEMENT

forStmt: forHeader ':' suite
       | forHeader ':' suite ELSE ':' suite
       ;

forHeader: FOR targetList IN expr
         ;

forHeaderList: forHeader
             | forHeaderList forHeader
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
    | expr '*' expr {cout << "P: expr '*' expr" << endl;}
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
    | '(' expr ')' {cout<<"P: '(' expr ')'"<<endl;}
    | INT_C
    | FLOAT_C
    | STRING_C
    | TRUE
    | FALSE
    | identifier {cout<<"P: identifier"<<endl;}
    | identifier ASSIGN_OP expr
    | LAMBDA paramsListE ':' expr %prec LAMBDA
    | '[' exprListE ']'
    | '[' exprList forHeaderList ifHeaderListE ']'
    | expr '[' expr ']'
    | expr '[' arraySlice ']'
    | expr '(' funcArgs ')'
    | attributeRefList
    ;

exprE: expr
     | /* empty */
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

arraySlice: exprE ':' exprE
          | exprE ':' exprE ':' exprE
          ;

attributeRef: identifier '.' identifier
            ;

attributeRefList: attributeRef
                | attributeRefList '.' identifier
                ;

namedArgsList: identifier '=' expr
             | namedArgsList ',' identifier '=' expr
             ;

funcArgs: exprList
        | namedArgsList
        | exprList ','
        | namedArgsList ','
        | exprList ',' namedArgsList
        | exprList ',' namedArgsList ','
        | /* empty */
        ;

%%

int yyerror(const char *errormsg)
{
      fprintf(stderr, "%s\n", errormsg);
      exit(1);
}