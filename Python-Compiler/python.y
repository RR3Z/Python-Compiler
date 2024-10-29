%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <iostream>
  #include <string>
  using namespace std;

  int yylex(void);
  int yyerror(const char *s);
%}

%union {
    int intVal;
    float floatVal;
    string* stringVal;
}

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
%token INT_TYPE FLOAT_TYPE LIST_TYPE RANGE_TYPE BOOL_TYPE STR_TYPE

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

program: stmtsList { cout << "P: stmtsList -> program" << endl; }
       ;

stmt: compoundStmt { cout << "P: compoundStmt -> stmt" << endl; }
    | simpleStmt { cout << "P: simpleStmt -> stmt" << endl; }
    | expr NEWLINE { cout << "P: expr NEWLINE -> stmt" << endl; }
    | stmt NEWLINE { cout << "P: stmt NEWLINE -> stmt" << endl; }
    ;

stmtsList: stmt { cout << "P: stmt -> stmtsList" << endl; }
         | stmtsList stmt { cout << "P: stmtsList stmt -> stmtsList" << endl; }
         ;

compoundStmt: ifStmt { cout << "P: ifStmt -> compoundStmt" << endl; }
            | forStmt { cout << "P: forStmt -> compoundStmt" << endl; }
            | whileStmt
            | tryStmt
            | funcDef
            | classDef
            ;

simpleStmt: assignStmt { cout << "P: assignStmt -> simpleStmt" << endl; }
          | returnStmt
          ;

simpleStmtList: simpleStmt { cout << "P: simpleStmt -> simpleStmtList" << endl; }
              | simpleStmtList ';' simpleStmt { cout << "P: simpleStmtList ';' simpleStmt -> simpleStmtList" << endl; }
              ;

suite: NEWLINE INDENT stmtsList DEDENT { cout << "P: NEWLINE INDENT stmtsList DEDENT -> suite" << endl; }
     | simpleStmtList NEWLINE { cout << "P: simpleStmtList NEWLINE -> suite" << endl; }
     | simpleStmtList ';' NEWLINE { cout << "P: simpleStmtList ';' NEWLINE -> suite" << endl; }
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

forHeader: FOR targetList IN expr { cout << "P: forHeader" << endl; }
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

funcDef: DEF identifier '(' paramsListE ')' ':' suite { cout << "P: DEF identifier '(' paramsListE ')' ':' suite -> funcDef" << endl; }
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

classDef: CLASS identifier ':' suite  { cout << "P: CLASS identifier ':' suite -> classDef" << endl; }
        | CLASS identifier '(' identifiersE ')' ':' suite { cout << "P: CLASS identifier '(' identifiersE ')' ':' suite -> classDef" << endl; }
        ;

// ASSIGNMENT STATEMENT

assignStmt: assignStmtTargetAssignList '=' expr { cout << "P: assignStmtTargetAssignList '=' expr -> assignStmt" << endl; }
          | identifier PLUS_ASSIGN expr { cout << "P: identifier PLUS_ASSIGN expr -> assignStmt" << endl; }
          | identifier MINUS_ASSIGN expr { cout << "P: identifier MINUS_ASSIGN expr -> assignStmt" << endl; }
          | identifier MUL_ASSIGN expr { cout << "P: identifier MUL_ASSIGN expr -> assignStmt" << endl; }
          | identifier DIV_ASSIGN expr { cout << "P: identifier DIV_ASSIGN expr -> assignStmt" << endl; }
          ;

assignStmtTargetAssignList: targetList { cout << "P: targetList -> assignStmtTargetAssignList" << endl; }
                          | targetList ',' { cout << "P: targetList ',' -> assignStmtTargetAssignList" << endl; }
                          | assignStmtTargetAssignList '=' targetList { cout << "P: assignStmtTargetAssignList '=' targetList -> assignStmtTargetAssignList" << endl; }
                          | assignStmtTargetAssignList '=' targetList ',' { cout << "P: assignStmtTargetAssignList '=' targetList ',' -> assignStmtTargetAssignList" << endl; }
                          ;

// RETURN STATEMENT
 
returnStmt: RETURN exprListE NEWLINE { cout << "P: RETURN exprListE NEWLINE -> returnStmt" << endl; }
          ;

expr: expr '+' expr { cout << "P: expr '+' expr -> expr" << endl; }
    | expr '-' expr {cout << "P: expr '-' expr -> expr" << endl;}
    | expr '*' expr {cout << "P: expr '*' expr -> expr" << endl;}
    | expr '/' expr {cout << "P: expr '/' expr -> expr" << endl;}
    | expr AND expr {cout << "P: expr AND expr -> expr" << endl;}
    | expr '&' expr {cout << "P: expr '&' expr -> expr" << endl;}
    | expr OR expr {cout << "P: expr OR expr -> expr" << endl;}
    | expr '|' expr {cout << "P: expr '|' expr -> expr" << endl;}
    | expr GT expr {cout << "P: expr GT expr -> expr" << endl;}
    | expr GE expr {cout << "P: expr GE expr -> expr" << endl;}
    | expr LT expr {cout << "P: expr LT expr -> expr" << endl;}
    | expr LE expr {cout << "P: expr LE expr -> expr" << endl;}
    | expr EQ expr {cout << "P: expr EQ expr -> expr" << endl;}
    | expr NE expr {cout << "P: expr NE expr -> expr" << endl;}
    | '+' expr %prec UPLUS {cout << "P: '+' expr -> expr" << endl;}
    | '-' expr %prec UMINUS {cout << "P: '-' expr -> expr" << endl;}
    | '(' expr ')' {cout << "P: '(' expr ')' -> expr" << endl;}
    | INT_C {cout << "P: INT_C -> expr" << endl;}
    | FLOAT_C {cout << "P: FLOAT_C -> expr" << endl;}
    | STRING_C {cout << "P: STRING_C -> expr" << endl;}
    | TRUE {cout << "P: TRUE -> expr" << endl;}
    | FALSE {cout << "P: FALSE -> expr" << endl;}
    | identifier { cout << "P: identifier->expr" << endl; }
    | identifier ASSIGN_OP expr {cout << "P: identifier ASSIGN_OP expr -> expr" << endl;}
    | LAMBDA paramsListE ':' expr %prec LAMBDA { cout << "P: lambdaExpr -> expr" << endl; }
    | '[' exprListE ']' { cout << "P: '[' exprListE ']' -> expr" << endl; }
    | '[' exprList forHeaderList ifHeaderListE ']' { cout << "P: '[' exprList forHeaderList ifHeaderListE ']' -> expr" << endl; }
    | expr '[' expr ']' { cout << "P: expr '[' expr ']' -> expr" << endl; }
    | expr '[' arraySlice ']' { cout << "P: expr '[' arraySlice ']' -> expr" << endl; }
    | expr '(' funcArgs ')' { cout << "P: expr '(' funcArgs ')' -> expr" << endl; }
    | attributeRefList { cout << "P: attributeRefList -> expr" << endl; }
    | type '(' exprListE ')' { cout << "P: type '(' exprE ')' -> expr" << endl; }
    ;

type: INT_TYPE
    | FLOAT_TYPE
    | LIST_TYPE
    | RANGE_TYPE
    | BOOL_TYPE
    | STR_TYPE
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

targetList: identifier { cout << "P: identifier->targetList" << endl; }
          | targetList ',' identifier { cout << "P: targetList , identifier -> targetList" << endl; }
          ;

arraySlice: exprE ':' exprE
          | exprE ':' exprE ':' exprE
          ;

attributeRef: identifier '.' identifier { cout << "P: identifier '.' identifier -> attributeRef" << endl; }
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