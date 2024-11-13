%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <iostream>
  #include <string>
  using namespace std;

  #include "nodes/parsing_tree.h"

  int yylex(void);
  int yyerror(const char *s);

  bool isFunc = false;
  struct ExprNode* exprTest;
%}

%union {
    int intVal;
    float floatVal;
    string* stringVal;

    string* name;

    struct ExprNode* expression;
}

%token <intVal>INT_C
%token <stringVal>STRING_C
%token <floatVal>FLOAT_C
%token <name>ID
%token <name>INT_TYPE
%token <name>FLOAT_TYPE
%token <name>LIST_TYPE
%token <name>RANGE_TYPE
%token <name>BOOL_TYPE
%token <name>STR_TYPE

%type <name>identifier

%type <expression>expr

%token TRUE FALSE
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
       | NEWLINE { cout << "P: newLineList -> program" << endl; }
       ;

programStmtsList: topLevelStmt { cout << "P: topLevelStmt -> programStmtsList" << endl; }
                | stmt { cout << "P: stmt -> programStmtsList" << endl; }
                | programStmtsList topLevelStmt { cout << "P: programStmtsList topLevelStmt -> programStmtsList" << endl; }
                | programStmtsList stmt { cout << "P: programStmtsList stmt -> programStmtsList" << endl; }
                ;

topLevelStmt: funcDef { cout << "P: funcDef -> topLevelStmt" << endl; }
            | classDef { cout << "P: classDef -> topLevelStmt" << endl; }
            ;

stmt: assignStmt { cout << "P: assignStmt -> stmt" << endl; }
    | returnStmt { cout << "P: returnStmt -> stmt" << endl; }
    | ifStmt { cout << "P: ifStmt -> stmt" << endl; }
    | forStmt { cout << "P: forStmt -> stmt" << endl; }
    | whileStmt { cout << "P: whileStmt -> stmt" << endl; }
    | tryStmt { cout << "P: tryStmt -> stmt" << endl; }
    | expr NEWLINE { cout << "P: expr NEWLINE -> stmt" << endl; }
    | stmt NEWLINE { cout << "P: stmt NEWLINE -> stmt" << endl; }
    ;

stmtsList: stmt { cout << "P: stmt -> stmtsList" << endl; }
         | stmtsList stmt { cout << "P: stmtsList stmt -> stmtsList" << endl; }
         ;
                  
classElement: funcDef { cout << "P: funcDef -> classElement" << endl; }
            | stmt { cout << "P: stmtWithoutReturn -> classElement" << endl; }
            ;

classElementsList: classElement { cout << "P: classElement -> classElementsList" << endl; }
                 | classElementsList classElement { cout << "P: classElementsList classElement -> classElementsList" << endl; }
                 ;

suite: NEWLINE INDENT stmtsList DEDENT { cout << "P: NEWLINE INDENT stmtsList DEDENT -> suite" << endl; }
     ;

classSuite: NEWLINE INDENT classElementsList DEDENT { cout << "P: NEWLINE INDENT classElementsList DEDENT -> classSuite" << endl; }
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

classDef: CLASS identifier ':' classSuite  { cout << "P: CLASS identifier ':' classSuite -> classDef" << endl; }
        | CLASS identifier '(' identifiersE ')' ':' classSuite { cout << "P: CLASS identifier '(' identifiersE ')' ':' classSuite -> classDef" << endl; }
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

expr: expr '+' expr { $$ = createPlusExprNode($1, $3); exprTest = $$; cout << "P: expr '+' expr -> expr" << endl; }
    | expr '-' expr { $$ = createMinusExprNode($1, $3); exprTest = $$; cout << "P: expr '-' expr -> expr" << endl;}
    | expr '*' expr { $$ = createMulExprNode($1, $3); exprTest = $$;  cout << "P: expr '*' expr -> expr" << endl;}
    | expr '/' expr { $$ = createDivExprNode($1, $3); exprTest = $$;  cout << "P: expr '/' expr -> expr" << endl;}
    | expr AND expr { $$ = createAndLogicExprNode($1, $3); exprTest = $$; cout << "P: expr AND expr -> expr" << endl;}
    | expr '&' expr { $$ = createAndBitwiseExprNode($1, $3); exprTest = $$; cout << "P: expr '&' expr -> expr" << endl;}
    | expr OR expr { $$ = createOrLogicExprNode($1, $3); exprTest = $$;  cout << "P: expr OR expr -> expr" << endl;}
    | expr '|' expr { $$ = createOrBitwiseExprNode($1, $3); exprTest = $$; cout << "P: expr '|' expr -> expr" << endl;}
    | expr GT expr { $$ = createGreatExprNode($1, $3); exprTest = $$; cout << "P: expr GT expr -> expr" << endl;}
    | expr GE expr { $$ = createGreateEqualExprNode($1, $3); exprTest = $$; cout << "P: expr GE expr -> expr" << endl;}
    | expr LT expr { $$ = createLessExprNode($1, $3); exprTest = $$; cout << "P: expr LT expr -> expr" << endl;}
    | expr LE expr { $$ = createLessEqualExprNode($1, $3); exprTest = $$; cout << "P: expr LE expr -> expr" << endl;}
    | expr EQ expr { $$ = createEqualExprNode($1, $3); exprTest = $$; cout << "P: expr EQ expr -> expr" << endl;}
    | expr NE expr { $$ = createNotEqualExprNode($1, $3); exprTest = $$; cout << "P: expr NE expr -> expr" << endl; }
    | '+' expr %prec UPLUS { $$ = createUnaryPlusExprNode($2); exprTest = $$; cout << "P: '+' expr -> expr" << endl;}
    | '-' expr %prec UMINUS { $$ = createUnaryMinusExprNode($2); exprTest = $$; cout << "P: '-' expr -> expr" << endl;}
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
    | INT_C { $$ = createIntConstExprNode($1); exprTest = $$; cout << "P: INT_C -> expr" << endl; }
    | FLOAT_C { $$ = createFloatConstExprNode($1); exprTest = $$; cout << "P: FLOAT_C -> expr" << endl;}
    | STRING_C { $$ = createStringConstExprNode($1); exprTest = $$; cout << "P: STRING_C -> expr" << endl;}
    | identifier { $$ = createIdExprNode($1); exprTest = $$; cout << "P: identifier -> expr" << endl; }
    | TRUE { $$ = createTrueConstExprNode(); exprTest = $$; cout << "P: TRUE -> expr" << endl;}
    | FALSE { $$ = createFalseConstExprNode(); exprTest = $$; cout << "P: FALSE -> expr" << endl;}
    | SELF { $$ = createSelfExprNode(); exprTest = $$; cout << "P: SELF -> expr" << endl; }
    | SUPER { $$ = createSuperExprNode(); exprTest = $$; cout << "P: SUPER -> expr" << endl; }
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

identifier: ID { $$ = $1; cout << "P: ID -> identifier" << endl; }
          | INT_TYPE { $$ = $1; cout << "P: INT_TYPE -> identifier" << endl; }
          | FLOAT_TYPE { $$ = $1; cout << "P: FLOAT_TYPE -> identifier" << endl; }
          | LIST_TYPE { $$ = $1; cout << "P: LIST_TYPE -> identifier" << endl; }
          | RANGE_TYPE { $$ = $1; cout << "P: RANGE_TYPE -> identifier" << endl; }
          | BOOL_TYPE { $$ = $1; cout << "P: BOOL_TYPE -> identifier" << endl; }
          | STR_TYPE { $$ = $1; cout << "P: STR_TYPE -> identifier" << endl; }
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