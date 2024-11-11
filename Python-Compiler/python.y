%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <iostream>
  #include <string>
  using namespace std;

  int yylex(void);
  int yyerror(const char *s);

  bool isFunc = false;
%}

%union {
    int intVal;
    float floatVal;
    string* stringVal;
}

%token INT_C FLOAT_C STRING_C ID TRUE FALSE
NEWLINE INDENT DEDENT
AND OR
GT GE LT LE EQ NE
ASSIGN_OP
PLUS_ASSIGN MINUS_ASSIGN MUL_ASSIGN DIV_ASSIGN
IF ELSE ELIF
FOR WHILE IN
TRY FINALLY EXCEPT AS
DEF CLASS SELF SUPER
RETURN LAMBDA
INT_TYPE FLOAT_TYPE LIST_TYPE RANGE_TYPE BOOL_TYPE STR_TYPE

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

program: programStmtsList { cout << "P: programStmtsList -> program" << endl; }
       | newLineList programStmtsList { cout << "P: newLineList programStmtsList -> program" << endl; }
       | newLineList { cout << "P: newLineList -> program" << endl; }
       ;

newLineList: NEWLINE
           | newLineList NEWLINE
           ;

programStmtsList: topLevelStmt { cout << "P: topLevelStmt -> programStmtsList" << endl; }
                | stmt { cout << "P: stmt -> programStmtsList" << endl; }
                | programStmtsList topLevelStmt { cout << "P: programStmtsList topLevelStmt -> programStmtsList" << endl; }
                | programStmtsList stmt { cout << "P: programStmtsList stmt -> programStmtsList" << endl; }
                ;

topLevelStmt: funcDef { cout << "P: funcDef -> topLevelStmt" << endl; }
            | classDef { cout << "P: classDef -> topLevelStmt" << endl; }
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
            | whileStmt { cout << "P: whileStmt -> compoundStmt" << endl; }
            | tryStmt { cout << "P: tryStmt -> compoundStmt" << endl; }
            ;

simpleStmt: assignStmt { cout << "P: assignStmt -> simpleStmt" << endl; }
          | returnStmt { cout << "P: returnStmt -> simpleStmt" << endl; }
          ;

suite: NEWLINE INDENT stmtsList DEDENT { cout << "P: NEWLINE INDENT stmtsList DEDENT -> suite" << endl; }
     ;

// IF STATEMENT

/*
    IF ... : ...
    IF ... : ... ELSE : ...
    IF ... : ... ELIF ... : ...
    IF ... : ... ELIF ... : ... ELSE : ...

    In this case, there may be several elif blocks, or there may not be
*/

ifStmt: ifHeader ':' suite { cout << "P: ifHeader ':' suite -> ifStmt" << endl; }
      | ifHeader ':' suite ELSE ':' suite { cout << "P: ifHeader ':' suite ELSE ':' suite -> ifStmt" << endl; }
      | ifHeader ':' suite elifStmtList { cout << "P: ifHeader ':' suite elifStmtList -> ifStmt" << endl; }
      | ifHeader ':' suite elifStmtList ELSE ':' suite { cout << "P: ifHeader ':' suite elifStmtList ELSE ':' suite -> ifStmt" << endl; }
      ;

ifHeader: IF expr { cout << "P: IF expr -> ifHeader" << endl; }
        ;

ifHeaderList: ifHeader { cout << "P: ifHeader -> ifHeaderList" << endl; }
            | ifHeaderList ifHeader { cout << "P: ifHeaderList ifHeader -> ifHeaderList" << endl; }
            ;

ifHeaderListE: ifHeaderList { cout << "P: ifHeaderList -> ifHeaderListE" << endl; }
             | /* empty */ { cout << "P: /* empty */ -> ifHeaderListE" << endl; }
             ;

elifStmtList: ELIF expr ':' suite { cout << "P: ELIF expr ':' suite -> elifStmtList" << endl; }
            | elifStmtList ELIF expr ':' suite { cout << "P: elifStmtList ELIF expr ':' suite -> elifStmtList" << endl; }
            ;

// FOR STATEMENT

forStmt: forHeader ':' suite { cout << "P: forHeader ':' suite -> forStmt" << endl; }
       | forHeader ':' suite ELSE ':' suite { cout << "P: forHeader ':' suite ELSE ':' suite -> forStmt" << endl; }
       ;

forHeader: FOR targetList IN expr { cout << "P: forHeader" << endl; }
         ;

forHeaderList: forHeader { cout << "P: forHeader -> forHeaderList" << endl; }
             | forHeaderList forHeader { cout << "P: forHeaderList forHeader -> forHeaderList" << endl; }
             ;

// WHILE STATEMENT

whileStmt: WHILE expr ':' suite { cout << "P: WHILE expr ':' suite -> whileStmt" << endl; }
         | WHILE expr ':' suite ELSE ':' suite { cout << "P: WHILE expr ':' suite ELSE ':' suite  -> whileStmt" << endl; }
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

funcDef: funcHeader ':' suite { 
                                isFunc = false; 
                                cout << "P: funcHeader ':' suite -> funcDef" << endl; 
                              }
       ;

funcHeader: DEF identifier '(' paramsListE ')' { 
                                                 isFunc = true; 
                                                 cout << "P: DEF identifier '(' paramsListE ')' -> funcHeader" << endl; 
                                               }
          ;

param: identifier { cout << "P: identifier -> param" << endl; }
     | identifier '=' expr { cout << "P: identifier '=' expr -> param" << endl; }
     ;

paramsList: param { cout << "P: param -> paramsList" << endl; }
          | paramsList ',' param { cout << "P: paramsList ',' param -> paramsList" << endl; }
          ;

paramsListE: paramsList { cout << "P: paramsList -> paramsListE" << endl; }
           | paramsList ',' { cout << "P: paramsList ',' -> paramsListE" << endl; }
           | /* empty */ { cout << "P: /* empty */ -> paramsListE" << endl; }
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
 
returnStmt: RETURN exprListE NEWLINE { 
                                        if(!isFunc) { yyerror("syntax error (\'return\' outside function)"); }
                                        cout << "P: RETURN exprListE NEWLINE -> returnStmt" << endl; 
                                     }
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
    | LAMBDA paramsListE ':' expr %prec LAMBDA { cout << "P: lambdaExpr -> expr" << endl; }
    | identifier ASSIGN_OP expr {cout << "P: identifier ASSIGN_OP expr -> expr" << endl;}
    | '(' expr ')' {cout << "P: '(' expr ')' -> expr" << endl;}
    | '[' exprListE ']' { cout << "P: '[' exprListE ']' -> expr" << endl; }
    | '[' exprList forHeaderList ifHeaderListE ']' { cout << "P: '[' exprList forHeaderList ifHeaderListE ']' -> expr" << endl; }
    | expr '[' expr ']' { cout << "P: expr '[' expr ']' -> expr" << endl; }
    | expr '[' arraySlice ']' { cout << "P: expr '[' arraySlice ']' -> expr" << endl; }
    | expr '(' funcArgs ')' { cout << "P: expr '(' funcArgs ')' -> expr | FUNCTION CALL" << endl; }
    | expr '.' identifier '(' funcArgs ')' { cout << "P: expr '.' identifier '(' funcArgs ')' -> expr | METHOD CALL" << endl; }
    | expr '.' identifier { cout << "P: expr '.' identifier -> expr | ATTRIBUTE REF" << endl; }
    | INT_C {cout << "P: INT_C -> expr" << endl;}
    | FLOAT_C {cout << "P: FLOAT_C -> expr" << endl;}
    | STRING_C {cout << "P: STRING_C -> expr" << endl;}
    | TRUE {cout << "P: TRUE -> expr" << endl;}
    | FALSE {cout << "P: FALSE -> expr" << endl;}
    | type { cout << "P: type -> expr" << endl; }
    | SELF { cout << "P: SELF -> expr" << endl; }
    | SUPER { cout << "P: SUPER -> expr" << endl; }
    ;

type: INT_TYPE { cout << "P: INT_TYPE -> type" << endl; }
    | FLOAT_TYPE { cout << "P: FLOAT_TYPE -> type" << endl; }
    | LIST_TYPE { cout << "P: LIST_TYPE -> type" << endl; }
    | RANGE_TYPE { cout << "P: RANGE_TYPE -> type" << endl; }
    | BOOL_TYPE { cout << "P: BOOL_TYPE -> type" << endl; }
    | STR_TYPE { cout << "P: STR_TYPE -> type" << endl; } 
    | identifier { cout << "P: identifier -> type" << endl; }
    ;

exprE: expr
     | /* empty */
     ;

exprList: expr { cout << "P: expr -> exprList" << endl; }
        | exprList ',' expr { cout << "P: exprList ',' expr -> exprList" << endl; }
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

target: identifier { cout << "P: identifier -> target" << endl; }
      | expr '[' expr ']' { cout << "P: expr '[' expr ']' -> target" << endl; }
      | expr '[' arraySlice ']' { cout << "P: expr '[' arraySlice ']' -> target" << endl; }
      | expr '.' identifier { cout << "P: expr '.' identifier -> target" << endl; }
      ;

targetList: target { cout << "P: target -> targetList" << endl; }
          | targetList ',' target { cout << "P: targetList , target -> targetList" << endl; }
          ;

arraySlice: exprE ':' exprE
          | exprE ':' exprE ':' exprE
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