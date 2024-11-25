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

ExprNode* createListComprehensionExprNode(ExprNode* expr, ExprListNode* forHeaderList, ExprListNode* ifHeaderListE);

ExprNode* createLambdaExprNode(FuncArgsListNode* funcArgsListNode, ExprNode* expr);

ExprNode* createMethodCallExprNode(ExprNode* expr, string* identifier, FuncArgsListNode* funcArgs);

ExprNode* createFunctionCallExprNode(ExprNode* expr, FuncArgsListNode* funcArgs);

/* ========== EXPRESSION LIST ========== */

ExprListNode* createExprListNode(ExprNode* firstElement);

ExprListNode* addElementToExprList(ExprListNode* list, ExprNode* newElement);

/* ========== SLICING ========== */

SlicingNode* createSlicingNode(ExprNode* start, ExprNode* end, ExprNode* step);

/* ========== STATEMENT ========== */

StmtNode* createCompoundIfStmtNode(StmtNode* ifStmt, StmtNode* elseStmt, StmtsListNode* elseStmtsList);

StmtNode* createIfStmtNode(ExprNode* expr, StmtsListNode* suite);

StmtNode* createElseStmtNode(StmtsListNode* suite);

StmtNode* createElifStmtNode(ExprNode* expr, StmtsListNode* suite);

StmtNode* createAssignStmtNode(ExprNode* identifier, ExprNode* expr);

StmtNode* createWhileStmtNode(ExprNode* expr, StmtsListNode* suite);

StmtNode* createCompoundWhileStmtNode(StmtNode* whileStmt, StmtNode* elseStmt);

StmtNode* createWhileStmtNode(ExprNode* expr, StmtsListNode* suite);

StmtNode* createReturnStmtNode(ExprListNode* list);

/* ========== STATEMENTS LIST ========== */

StmtsListNode* createStmtsListNode(StmtNode* firstElement);

StmtsListNode* addElementToStmtsList(StmtsListNode* list, StmtNode* newElement);

/* ========== FUNC ARG ========== */

FuncArgNode* createUnnamedFuncArgNode(string* identifier);

FuncArgNode* createNamedFuncArgNode(StmtNode* assignStmt);

/* ========== FUNC ARGS LIST ========== */

FuncArgsListNode* createParamsListNode(FuncArgNode* firstElement);

FuncArgsListNode* createFuncArgsListNodeFromExprList(ExprListNode* exprList);

FuncArgsListNode* createFuncArgsListNodeFromFuncArgsList(FuncArgsListNode* namedArgsList);

FuncArgsListNode* createFuncArgsListNodeFromDifferentLists(ExprListNode* exprList, FuncArgsListNode* namedArgsList);

FuncArgsListNode* addElementToParamsList(FuncArgsListNode* list, FuncArgNode* newElement);
