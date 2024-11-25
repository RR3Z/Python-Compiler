%{
  #include "nodes/parsing_tree.h"
  #include <string>
  #include <iostream>
  using namespace std;

  int yylex(void);
  int yyerror(const char *s);

  bool isFunc = false;
  struct ExprNode* exprTest; // TEMP
%}

%union {
    int intVal;
    float floatVal;
    string* stringVal;

    string* name;

    struct ExprNode* expressionNode;
    struct ExprListNode* expressionListNode;
    struct SlicingNode* slicingNode;
    struct StmtNode* stmtNode;
    struct StmtsListNode* stmtsListNode;
    struct FuncArgNode* funcArgNode;
    struct FuncArgsListNode* funcArgsListNode;
    struct FuncNode* funcNode;
    struct ClassNode* classNode;
    struct FileNode* fileNode;
    struct FileElementNode* fileElementNode;
    struct FileElementsListNode* fileElementsListNode;
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

%type <expressionNode>expr
%type <expressionNode>exprE
%type <expressionNode>target
%type <expressionNode>ifHeader
%type <expressionNode>forHeader
%type <expressionNode>funcHeader
%type <slicingNode>slicing
%type <funcArgNode>param
%type <stmtNode>stmt
%type <stmtNode>ifStmt
%type <stmtNode>whileStmt
%type <stmtNode>returnStmt
%type <stmtNode>exceptStmt
%type <stmtNode>forStmt
%type <stmtNode>tryStmt 
%type <stmtNode>assignStmt
%type <funcNode>funcDef
%type <classNode>classDef
%type <fileElementNode>topLevelStmt
%type <fileNode>program

%type <expressionListNode>exprList
%type <expressionListNode>exprListE
%type <expressionListNode>ifHeaderList
%type <expressionListNode>ifHeaderListE
%type <expressionListNode>forHeaderList
%type <expressionListNode>targetList
%type <expressionListNode>identifiers
%type <expressionListNode>identifiersE
%type <funcArgsListNode>paramsList
%type <funcArgsListNode>paramsListE
%type <funcArgsListNode>namedArgsList
%type <funcArgsListNode>funcArgs
%type <stmtsListNode>stmtsList
%type <stmtsListNode>elifStmtList
%type <stmtsListNode>exceptStmtList
%type <stmtsListNode>assignStmtTargetAssignList 
%type <stmtsListNode>suite
%type <fileElementsListNode>programStmtsList

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

program: programStmtsList { $$ = createFileNode($1); cout << "P: programStmtsList -> program" << endl; }
       | NEWLINE { $$ = nullptr; cout << "P: newLineList -> program" << endl; }
       ;

programStmtsList: topLevelStmt { $$ = createFileElementsListNode($1); cout << "P: topLevelStmt -> programStmtsList" << endl; }
                | stmt { $$ = createFileElementsListNode(createStmtFileElementNode($1)); cout << "P: stmt -> programStmtsList" << endl; }
                | programStmtsList topLevelStmt { $$ = addElementToFileElementsList($1, $2); cout << "P: programStmtsList topLevelStmt -> programStmtsList" << endl; }
                | programStmtsList stmt { $$ = addElementToFileElementsList($1, createStmtFileElementNode($2)); cout << "P: programStmtsList stmt -> programStmtsList" << endl; }
                ;

topLevelStmt: funcDef { $$ = createFuncDefFileElementNode($1); cout << "P: funcDef -> topLevelStmt" << endl; }
            | classDef { $$ = createClassDefFileElementNode($1); cout << "P: classDef -> topLevelStmt" << endl; }
            ;

stmt: assignStmt { $$ = $1; cout << "P: assignStmt -> stmt" << endl; }
    | returnStmt { $$ = $1; cout << "P: returnStmt -> stmt" << endl; }
    | ifStmt { $$ = $1; cout << "P: ifStmt -> stmt" << endl; }
    | forStmt { $$ = $1; cout << "P: forStmt -> stmt" << endl; }
    | whileStmt { $$ = $1; cout << "P: whileStmt -> stmt" << endl; }
    | tryStmt { $$ = $1; cout << "P: tryStmt -> stmt" << endl; }
    | expr NEWLINE { $$ = createStmtNodeFromExprNode($1); cout << "P: expr NEWLINE -> stmt" << endl; }
    | stmt NEWLINE { $$ = $1; cout << "P: stmt NEWLINE -> stmt" << endl; }
    ;

stmtsList: stmt { $$ = createStmtsListNode($1); cout << "P: stmt -> stmtsList" << endl; }
         | stmtsList stmt { $$ = addElementToStmtsList($1, $2); cout << "P: stmtsList stmt -> stmtsList" << endl; }
         ;
                  
classElement: funcDef { cout << "P: funcDef -> classElement" << endl; }
            | stmt { cout << "P: stmtWithoutReturn -> classElement" << endl; }
            ;

classElementsList: classElement { cout << "P: classElement -> classElementsList" << endl; }
                 | classElementsList classElement { cout << "P: classElementsList classElement -> classElementsList" << endl; }
                 ;

suite: NEWLINE INDENT stmtsList DEDENT { $$ = $3; cout << "P: NEWLINE INDENT stmtsList DEDENT -> suite" << endl; }
     ;

classSuite: NEWLINE INDENT classElementsList DEDENT { cout << "P: NEWLINE INDENT classElementsList DEDENT -> classSuite" << endl; }
          ;

// IF STATEMENT

ifStmt: IF expr ':' suite { $$ = createIfStmtNode($2, $4); cout << "P: IF expr ':' suite -> ifStmt" << endl; }
      | IF expr ':' suite ELSE ':' suite { $$ = createCompoundIfStmtNode(createIfStmtNode($2, $4), createElseStmtNode($7), nullptr); cout << "P: IF expr ':' suite ELSE ':' suite -> ifStmt" << endl; }
      | IF expr ':' suite elifStmtList { $$ = createCompoundIfStmtNode(createIfStmtNode($2, $4), nullptr, $5); cout << "P: IF expr ':' suite elifStmtList -> ifStmt" << endl; }
      | IF expr ':' suite elifStmtList ELSE ':' suite { $$ = createCompoundIfStmtNode(createIfStmtNode($2, $4), createElseStmtNode($8), $5); cout << "P: IF expr ':' suite elifStmtList ELSE ':' suite -> ifStmt" << endl; }
      ;

ifHeader: IF expr { $$ = createIfHeaderExprNode($2); cout << "P: IF expr -> ifHeader" << endl; }
        ;

ifHeaderList: ifHeader { $$ = createExprListNode($1); cout << "P: ifHeader -> ifHeaderList" << endl; }
            | ifHeaderList ifHeader { $$ = addElementToExprList($1, $2); cout << "P: ifHeaderList ifHeader -> ifHeaderList" << endl; }
            ;

ifHeaderListE: ifHeaderList { $$ = $1; cout << "P: ifHeaderList -> ifHeaderListE" << endl; }
             | /* empty */ { $$ = nullptr; cout << "P: /* empty */ -> ifHeaderListE" << endl; }
             ;

elifStmtList: ELIF expr ':' suite { $$ = createStmtsListNode(createElifStmtNode($2, $4)); cout << "P: ELIF expr ':' suite -> elifStmtList" << endl; }
            | elifStmtList ELIF expr ':' suite { $$ = addElementToStmtsList($1, createElifStmtNode($3, $5)); cout << "P: elifStmtList ELIF expr ':' suite -> elifStmtList" << endl; }
            ;

// FOR STATEMENT

forStmt: FOR targetList IN expr ':' suite { $$ = createForStmtNode($2, $4, $6); cout << "P: FOR targetList IN expr ':' suite -> forStmt" << endl; }
       | FOR targetList IN expr ':' suite ELSE ':' suite { $$ = createCompoundForStmtNode(createForStmtNode($2, $4, $6), createElseStmtNode($9)); cout << "P: FOR targetList IN expr ':' suite ELSE ':' suite -> forStmt" << endl; }
       ;

forHeader: FOR targetList IN expr { $$ = createForHeaderExprNode($2, $4); cout << "P: forHeader" << endl; }
         ;

forHeaderList: forHeader { $$ = createExprListNode($1); cout << "P: forHeader -> forHeaderList" << endl; }
             | forHeaderList forHeader { $$ = addElementToExprList($1, $2); cout << "P: forHeaderList forHeader -> forHeaderList" << endl; }
             ;

// WHILE STATEMENT

whileStmt: WHILE expr ':' suite { $$ = createWhileStmtNode($2, $4); cout << "P: WHILE expr ':' suite -> whileStmt" << endl; }
         | WHILE expr ':' suite ELSE ':' suite { $$ = createCompoundWhileStmtNode(createWhileStmtNode($2, $4), createElseStmtNode($7)); cout << "P: WHILE expr ':' suite ELSE ':' suite  -> whileStmt" << endl; }
         ;

// TRY STATEMENT

tryStmt: TRY ':' suite exceptStmtList { $$ = createCompoundTryStmtNode($3, $4, nullptr, nullptr);}
       | TRY ':' suite exceptStmtList ELSE ':' suite { $$ = createCompoundTryStmtNode($3, $4, createElseStmtNode($7), nullptr);}
       | TRY ':' suite exceptStmtList FINALLY ':' suite { $$ = createCompoundTryStmtNode($3, $4, nullptr, createFinallyStmtNode($7));}
       | TRY ':' suite exceptStmtList ELSE ':' suite FINALLY ':' suite  { $$ = createCompoundTryStmtNode($3, $4, createElseStmtNode($7), createFinallyStmtNode($10));}
       | TRY ':' suite FINALLY ':' suite { $$ = createCompoundTryStmtNode($3, nullptr, nullptr, createFinallyStmtNode($6));}
       ;

exceptStmt: EXCEPT ':' suite { $$ = createExceptStmtNode(nullptr, $3);}
          | EXCEPT expr ':' suite  { $$ = createExceptStmtNode($2, $4);}
          | EXCEPT expr AS identifier ':' suite { $$ = createExceptIdentifierStmtNode($2, createIdExprNode($4), $6);}
          ;

exceptStmtList: exceptStmt { $$ = createStmtsListNode($1);}
              | exceptStmtList exceptStmt { $$ = addElementToStmtsList($1, $2);}
              ;

// FUNCTION DEFINITION

funcDef: funcHeader ':' suite { 
                                isFunc = false; 
                                cout << "P: funcHeader ':' suite -> funcDef" << endl; 
                              }
       ;

funcHeader: DEF identifier '(' paramsListE ')' { 
                                                 $$ = createFuncHeaderExprNode(createIdExprNode($2), $4);
                                                 isFunc = true; 
                                                 cout << "P: DEF identifier '(' paramsListE ')' -> funcHeader" << endl; 
                                               }
          ;

param: identifier { $$ = createUnnamedFuncArgNode($1); cout << "P: identifier -> param" << endl; }
     | identifier '=' expr { $$ = createNamedFuncArgNode(createAssignStmtNode(createIdExprNode($1), $3)); cout << "P: identifier '=' expr -> param" << endl; }
     ;

paramsList: param { $$ = createParamsListNode($1); cout << "P: param -> paramsList" << endl; }
          | paramsList ',' param { $$ = addElementToParamsList($1, $3); cout << "P: paramsList ',' param -> paramsList" << endl; }
          ;

paramsListE: paramsList { $$ = $1; cout << "P: paramsList -> paramsListE" << endl; }
           | paramsList ',' {  $$ = $1; cout << "P: paramsList ',' -> paramsListE" << endl; }
           | /* empty */ { $$ = nullptr; cout << "P: /* empty */ -> paramsListE" << endl; }
           ;

// CLASS DEFINITION

classDef: CLASS identifier ':' classSuite  { cout << "P: CLASS identifier ':' classSuite -> classDef" << endl; }
        | CLASS identifier '(' identifiersE ')' ':' classSuite { cout << "P: CLASS identifier '(' identifiersE ')' ':' classSuite -> classDef" << endl; }
        ;

// ASSIGNMENT STATEMENT

assignStmt: assignStmtTargetAssignList '=' expr { $$ = createCompoundAssignStmtNode($1, $3); cout << "P: assignStmtTargetAssignList '=' expr -> assignStmt" << endl; }
          | identifier PLUS_ASSIGN expr { $$ = createPlusAssignStmtNode(createIdExprNode($1), $3); cout << "P: identifier PLUS_ASSIGN expr -> assignStmt" << endl; }
          | identifier MINUS_ASSIGN expr { $$ = createMinusAssignStmtNode(createIdExprNode($1), $3); cout << "P: identifier MINUS_ASSIGN expr -> assignStmt" << endl; }
          | identifier MUL_ASSIGN expr { $$ = createMulAssignStmtNode(createIdExprNode($1), $3); cout << "P: identifier MUL_ASSIGN expr -> assignStmt" << endl; }
          | identifier DIV_ASSIGN expr { $$ = createDivAssignStmtNode(createIdExprNode($1), $3); cout << "P: identifier DIV_ASSIGN expr -> assignStmt" << endl; }
          ;

assignStmtTargetAssignList: targetList { $$ = createStmtsListNode(createStmtNodeFromExprListNode($1)); cout << "P: targetList -> assignStmtTargetAssignList" << endl; }
                          | targetList ',' { $$ = createStmtsListNode(createStmtNodeFromExprListNode($1));  cout << "P: targetList ',' -> assignStmtTargetAssignList" << endl; }
                          | assignStmtTargetAssignList '=' targetList { $$ = addElementToStmtsList($1, createStmtNodeFromExprListNode($3)); cout << "P: assignStmtTargetAssignList '=' targetList -> assignStmtTargetAssignList" << endl; }
                          | assignStmtTargetAssignList '=' targetList ',' {  $$ = addElementToStmtsList($1, createStmtNodeFromExprListNode($3)); cout << "P: assignStmtTargetAssignList '=' targetList ',' -> assignStmtTargetAssignList" << endl; }
                          ;

// RETURN STATEMENT

returnStmt: RETURN exprListE NEWLINE { 
                                        if(!isFunc) { yyerror("syntax error (\'return\' outside function)"); }
                                        $$ = createReturnStmtNode($2);
                                        cout << "P: RETURN exprListE NEWLINE -> returnStmt" << endl; 
                                     }
          ;

// EXPRESSIONS

expr: expr '+' expr { $$ = createPlusExprNode($1, $3); exprTest = $$; cout << "P: expr '+' expr -> expr" << endl; }
    | expr '-' expr { $$ = createMinusExprNode($1, $3); exprTest = $$; cout << "P: expr '-' expr -> expr" << endl;}
    | expr '*' expr { $$ = createMulExprNode($1, $3); exprTest = $$;  cout << "P: expr '*' expr -> expr" << endl;}
    | expr '/' expr { $$ = createDivExprNode($1, $3); exprTest = $$;  cout << "P: expr '/' expr -> expr" << endl;}
    | expr AND expr { $$ = createAndLogicExprNode($1, $3); exprTest = $$; cout << "P: expr AND expr -> expr" << endl;}
    | expr '&' expr { $$ = createAndBitwiseExprNode($1, $3); exprTest = $$; cout << "P: expr '&' expr -> expr" << endl;}
    | expr OR expr { $$ = createOrLogicExprNode($1, $3); exprTest = $$;  cout << "P: expr OR expr -> expr" << endl;}
    | expr '|' expr { $$ = createOrBitwiseExprNode($1, $3); exprTest = $$; cout << "P: expr '|' expr -> expr" << endl;}
    | expr GT expr { $$ = createGreatExprNode($1, $3); exprTest = $$; cout << "P: expr GT expr -> expr" << endl;}
    | expr GE expr { $$ = createGreatEqualExprNode($1, $3); exprTest = $$; cout << "P: expr GE expr -> expr" << endl;}
    | expr LT expr { $$ = createLessExprNode($1, $3); exprTest = $$; cout << "P: expr LT expr -> expr" << endl;}
    | expr LE expr { $$ = createLessEqualExprNode($1, $3); exprTest = $$; cout << "P: expr LE expr -> expr" << endl;}
    | expr EQ expr { $$ = createEqualExprNode($1, $3); exprTest = $$; cout << "P: expr EQ expr -> expr" << endl;}
    | expr NE expr { $$ = createNotEqualExprNode($1, $3); exprTest = $$; cout << "P: expr NE expr -> expr" << endl; }
    | '+' expr %prec UPLUS { $$ = createUnaryPlusExprNode($2); exprTest = $$; cout << "P: '+' expr -> expr" << endl;}
    | '-' expr %prec UMINUS { $$ = createUnaryMinusExprNode($2); exprTest = $$; cout << "P: '-' expr -> expr" << endl; }
    | identifier ASSIGN_OP expr { $$ = createAssignOpExprNode(createIdExprNode($1), $3); exprTest = $$; cout << "P: identifier ASSIGN_OP expr -> expr" << endl; }
    | '(' expr ')' { $$ = createExprInParenthesesBracketsNode($2); exprTest = $$; cout << "P: '(' expr ')' -> expr" << endl; }
    | expr '[' expr ']' { $$ = createListAccessExprNode($1, $3); exprTest = $$; cout << "P: expr '[' expr ']' -> expr" << endl; }
    | '[' exprListE ']' { $$ = createListCreationExprNode($2); exprTest = $$; cout << "P: '[' exprListE ']' -> expr" << endl; }
    | expr '[' slicing ']' { $$ = createListAccessWithSlicingExprNode($1, $3); exprTest = $$; cout << "P: expr '[' slicing ']' -> expr" << endl; }
    | '[' expr forHeaderList ifHeaderListE ']' { $$ = createListComprehensionExprNode($2, $3, $4); exprTest = $$; cout << "P: '[' exprList forHeaderList ifHeaderListE ']' -> expr" << endl; }
    | LAMBDA paramsListE ':' expr %prec LAMBDA { $$ = createLambdaExprNode($2, $4); exprTest = $$; cout << "P: lambdaExpr -> expr" << endl; }
    | expr '(' funcArgs ')' { $$ = createFunctionCallExprNode($1, $3); exprTest = $$; cout << "P: expr '(' funcArgs ')' -> expr | FUNCTION CALL" << endl; }
    | expr '.' identifier '(' funcArgs ')' { $$ = createMethodCallExprNode($1, $3, $5); exprTest = $$; cout << "P: expr '.' identifier '(' funcArgs ')' -> expr | METHOD CALL" << endl; }
    | expr '.' identifier { $$ = createAttributeRefExprNode($1, createIdExprNode($3)); exprTest = $$; cout << "P: expr '.' identifier -> expr | ATTRIBUTE REF" << endl; }
    | INT_C { $$ = createIntConstExprNode($1); exprTest = $$; cout << "P: INT_C -> expr" << endl; }
    | FLOAT_C { $$ = createFloatConstExprNode($1); exprTest = $$; cout << "P: FLOAT_C -> expr" << endl; }
    | STRING_C { $$ = createStringConstExprNode($1); exprTest = $$; cout << "P: STRING_C -> expr" << endl; }
    | identifier { $$ = createIdExprNode($1); exprTest = $$; cout << "P: identifier -> expr" << endl; }
    | TRUE { $$ = createTrueConstExprNode(); exprTest = $$; cout << "P: TRUE -> expr" << endl; }
    | FALSE { $$ = createFalseConstExprNode(); exprTest = $$; cout << "P: FALSE -> expr" << endl; }
    | SELF { $$ = createSelfExprNode(); exprTest = $$; cout << "P: SELF -> expr" << endl; }
    | SUPER { $$ = createSuperExprNode(); exprTest = $$; cout << "P: SUPER -> expr" << endl; }
    ;

exprE: expr { $$ = $1; }
     | /* empty */ { $$ = nullptr; }
     ;

exprList: expr { $$ = createExprListNode($1); cout << "P: expr -> exprList" << endl; }
        | exprList ',' expr { $$ = addElementToExprList($1, $3); cout << "P: exprList ',' expr -> exprList" << endl; }
        ;

exprListE: exprList { $$ = $1; }
         | exprList ',' { $$ = $1; }
         | /* empty */ { $$ = nullptr; }
         ;

identifier: ID { $$ = $1; cout << "P: ID -> identifier" << endl; }
          | INT_TYPE { $$ = $1; cout << "P: INT_TYPE -> identifier" << endl; }
          | FLOAT_TYPE { $$ = $1; cout << "P: FLOAT_TYPE -> identifier" << endl; }
          | LIST_TYPE { $$ = $1; cout << "P: LIST_TYPE -> identifier" << endl; }
          | RANGE_TYPE { $$ = $1; cout << "P: RANGE_TYPE -> identifier" << endl; }
          | BOOL_TYPE { $$ = $1; cout << "P: BOOL_TYPE -> identifier" << endl; }
          | STR_TYPE { $$ = $1; cout << "P: STR_TYPE -> identifier" << endl; }
          ;

identifiers: identifier { $$ = createExprListNode(createIdExprNode($1)); cout << "P: identifier -> identifierList" << endl; }
           | identifiers ',' identifier  { $$ = addElementToExprList($1, createIdExprNode($3)); }
           ;

identifiersE: identifiers { $$ = $1; }
            | identifiers ',' { $$ = $1; }
            | /* empty */ { $$ = nullptr; }
            ;

target: identifier { $$ = createIdExprNode($1); cout << "P: identifier -> target" << endl; }
      | expr '[' expr ']' { $$ = createListAccessExprNode($1, $3); cout << "P: expr '[' expr ']' -> target" << endl; }
      | expr '[' slicing ']' { $$ = createListAccessWithSlicingExprNode($1, $3); cout << "P: expr '[' slicing ']' -> target" << endl; }
      | expr '.' identifier { $$ = createAttributeRefExprNode($1, createIdExprNode($3)); cout << "P: expr '.' identifier -> target" << endl; }
      ;

targetList: target {  $$ = createExprListNode($1); cout << "P: target -> targetList" << endl; }
          | targetList ',' target { $$ = addElementToExprList($1, $3); cout << "P: targetList , target -> targetList" << endl; }
          ;

slicing: exprE ':' exprE { $$ = createSlicingNode($1, $3, nullptr); cout << "P: exprE ':' exprE -> slicing" << endl; }
       | exprE ':' exprE ':' exprE { $$ = createSlicingNode($1, $3, $5); cout << "P: exprE ':' exprE ':' exprE -> slicing" << endl; }
       ;

namedArgsList: identifier '=' expr { $$ = createParamsListNode(createNamedFuncArgNode(createAssignStmtNode(createIdExprNode($1), $3))); }
             | namedArgsList ',' identifier '=' expr { $$ = addElementToParamsList($1, createNamedFuncArgNode(createAssignStmtNode(createIdExprNode($3), $5))); }
             ;

funcArgs: exprList { $$ = createFuncArgsListNodeFromExprList($1); }
        | namedArgsList { $$ = createFuncArgsListNodeFromFuncArgsList($1); }
        | exprList ',' { $$ = createFuncArgsListNodeFromExprList($1); }
        | namedArgsList ',' { $$ = createFuncArgsListNodeFromFuncArgsList($1); }
        | exprList ',' namedArgsList { $$ = createFuncArgsListNodeFromDifferentLists($1, $3); }
        | exprList ',' namedArgsList ',' { $$ = createFuncArgsListNodeFromDifferentLists($1, $3); }
        | /* empty */ { $$ = nullptr; }
        ;

%%

int yyerror(const char *errormsg)
{
      fprintf(stderr, "%s\n", errormsg);
      exit(1);
}