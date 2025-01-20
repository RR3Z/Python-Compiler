#pragma once
#include "nodes.h"

/* ========== EXPRESSION ========== */

ExprNode* createIdExprNode(string* identifier);

ExprNode* createIntConstExprNode(int value);

ExprNode* createFloatConstExprNode(float value);

ExprNode* createStringConstExprNode(string* text);

ExprNode* createTrueConstExprNode();

ExprNode* createFalseConstExprNode();

ExprNode* createSelfExprNode();

ExprNode* createSuperExprNode();

ExprNode* createPlusExprNode(ExprNode* leftOperand, ExprNode* rightOperand);

ExprNode* createUnaryPlusExprNode(ExprNode* value);

ExprNode* createMinusExprNode(ExprNode* leftOperand, ExprNode* rightOperand);

ExprNode* createUnaryMinusExprNode(ExprNode* value);

ExprNode* createMulExprNode(ExprNode* leftOperand, ExprNode* rightOperand);

ExprNode* createDivExprNode(ExprNode* leftOperand, ExprNode* rightOperand);

ExprNode* createGreatExprNode(ExprNode* leftOperand, ExprNode* rightOperand);

ExprNode* createGreatEqualExprNode(ExprNode* leftOperand, ExprNode* rightOperand);

ExprNode* createLessExprNode(ExprNode* leftOperand, ExprNode* rightOperand);

ExprNode* createLessEqualExprNode(ExprNode* leftOperand, ExprNode* rightOperand);

ExprNode* createEqualExprNode(ExprNode* leftOperand, ExprNode* rightOperand);

ExprNode* createNotEqualExprNode(ExprNode* leftOperand, ExprNode* rightOperand);

ExprNode* createAndLogicExprNode(ExprNode* leftOperand, ExprNode* rightOperand);

ExprNode* createAndLogicExprNode(ExprNode* leftOperand);

ExprNode* createAndBitwiseExprNode(ExprNode* leftOperand, ExprNode* rightOperand);

ExprNode* createOrLogicExprNode(ExprNode* leftOperand, ExprNode* rightOperand);

ExprNode* createOrBitwiseExprNode(ExprNode* leftOperand, ExprNode* rightOperand);

ExprNode* createAssignOpExprNode(ExprNode* leftOperand, ExprNode* rightOperand);

ExprNode* createExprInParenthesesBracketsNode(ExprNode* expression);

ExprNode* createAttributeRefExprNode(ExprNode* leftOperand, ExprNode* rightOperand);

ExprNode* createListAccessExprNode(ExprNode* var, ExprNode* index);

ExprNode* createListCreationExprNode(ExprListNode* elements);

ExprNode* createListAccessWithSlicingExprNode(ExprNode* var, SlicingNode* slicing);

ExprNode* createIfHeaderExprNode(ExprNode* expr);

ExprNode* createForHeaderExprNode(ExprListNode* targetList, ExprNode* expr);

ExprNode* createFuncHeaderExprNode(string* identifier, FuncArgsListNode* funcArgs);

ExprNode* createListComprehensionExprNode(ExprNode* expr, ExprListNode* forHeaderList, ExprListNode* ifHeaderListE);

ExprNode* createLambdaExprNode(FuncArgsListNode* funcArgsListNode, ExprNode* expr);

ExprNode* createMethodCallExprNode(ExprNode* expr, ExprNode* identifier, FuncArgsListNode* funcArgs);

ExprNode* createFunctionCallExprNode(ExprNode* expr, FuncArgsListNode* funcArgs);

/* ========== EXPRESSION LIST ========== */

ExprListNode* createExprListNode(ExprNode* firstElement);

ExprListNode* addElementToExprList(ExprListNode* list, ExprNode* newElement);

/* ========== SLICING ========== */

SlicingNode* createSlicingNode(ExprNode* start, ExprNode* end, ExprNode* step);

/* ========== STATEMENT ========== */

StmtNode* createCompoundIfStmtNode(StmtNode* ifStmt, StmtNode* elseStmt, StmtsListNode* elifStmtsList);

StmtNode* createIfStmtNode(ExprNode* expr, StmtsListNode* suite);

StmtNode* createElseStmtNode(StmtsListNode* suite);

StmtNode* createElifStmtNode(ExprNode* expr, StmtsListNode* suite);

StmtNode* createFinallyStmtNode(StmtsListNode* suite);

StmtNode* createAssignStmtNode(ExprNode* identifier, ExprNode* expr);

StmtNode* createCompoundAssignStmtNode(StmtsListNode* targetList, ExprListNode* exprList);

StmtNode* createPlusAssignStmtNode(ExprNode* identifier, ExprNode* expr);

StmtNode* createMinusAssignStmtNode(ExprNode* identifier, ExprNode* expr);

StmtNode* createMulAssignStmtNode(ExprNode* identifier, ExprNode* expr);

StmtNode* createDivAssignStmtNode(ExprNode* identifier, ExprNode* expr);

StmtNode* createWhileStmtNode(ExprNode* expr, StmtsListNode* suite);

StmtNode* createCompoundWhileStmtNode(StmtNode* whileStmt, StmtNode* elseStmt);

StmtNode* createWhileStmtNode(ExprNode* expr, StmtsListNode* suite);

StmtNode* createReturnStmtNode(ExprListNode* list);

StmtNode* createExceptStmtNode(ExprNode* expr, StmtsListNode* suite);

StmtNode* createExceptIdentifierStmtNode(ExprNode* expr, ExprNode* identifier, StmtsListNode* suite);

StmtNode* createForStmtNode(ExprListNode* targetList, ExprNode* expr, StmtsListNode* suite);

StmtNode* createCompoundForStmtNode(StmtNode* forStmt, StmtNode* elseStmt);

StmtNode* createCompoundTryStmtNode(StmtNode* tryStmt, StmtsListNode* exceptStmtsList, StmtNode* elseStmt, StmtNode* finallyStmt);

StmtNode* createStmtNodeFromExprNode(ExprNode* expr);

StmtNode* createStmtNodeFromExprListNode(ExprListNode* list);

StmtNode* createTryStmtNode(StmtsListNode* suite);

/* ========== STATEMENTS LIST ========== */

StmtsListNode* createStmtsListNode(StmtNode* firstElement);

StmtsListNode* addElementToStmtsList(StmtsListNode* list, StmtNode* newElement);

/* ========== CLASS ========== */

ClassNode* createClassNode(ExprNode* identifier, ClassElementsListNode* suite, ExprNode* parent);

/* ========== CLASS ELEMENT ========== */

ClassElementNode* createFuncDefClassElementNode(FuncNode* funcDef);

ClassElementNode* createStmtClassElementNode(StmtNode* stmt);

/* ========== CLASS ELEMENTS LIST ========== */

ClassElementsListNode* createClassElementsListNode(ClassElementNode* firstElement);

ClassElementsListNode* addElementToClassElementsList(ClassElementsListNode* list, ClassElementNode* newElement);

/* ========== FUNC ========== */

FuncNode* createFuncNode(ExprNode* identifier, FuncArgsListNode* args, StmtsListNode* suite);

/* ========== FUNC ARG ========== */

FuncArgNode* createVarFuncArgNode(ExprNode* var);

FuncArgNode* createNamedFuncArgNode(StmtNode* assignStmt);

/* ========== FUNC ARGS LIST ========== */

FuncArgsListNode* createFuncArgsListNode(FuncArgNode* firstElement);

FuncArgsListNode* createFuncArgsListNodeFromExprList(ExprListNode* exprList);

FuncArgsListNode* createFuncArgsListNodeFromFuncArgsList(FuncArgsListNode* namedArgsList);

FuncArgsListNode* createFuncArgsListNodeFromDifferentLists(ExprListNode* exprList, FuncArgsListNode* namedArgsList);

FuncArgsListNode* addElementToFuncArgsList(FuncArgsListNode* list, FuncArgNode* newElement);

/* ========== FILE ========== */

FileNode* createFileNode(FileElementsListNode* elementsList);

/* ========== FILE ELEMENT ========== */

FileElementNode* createFuncDefFileElementNode(FuncNode* funcDef);

FileElementNode* createClassDefFileElementNode(ClassNode* classDef);

FileElementNode* createStmtFileElementNode(StmtNode* stmt);

/* ========== FILE ELEMENTS LIST ========== */

FileElementsListNode* createFileElementsListNode(FileElementNode* fileElement);

FileElementsListNode* addElementToFileElementsList(FileElementsListNode* list, FileElementNode* newElement);
