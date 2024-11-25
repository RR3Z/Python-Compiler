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


/* ========== EXPRESSION LIST ========== */

ExprListNode* createExprListNode(ExprNode* firstElement);

ExprListNode* addElementToExprList(ExprListNode* list, ExprNode* newElement);

/* ========== SLICING ========== */

SlicingNode* createSlicingNode(ExprNode* start, ExprNode* end, ExprNode* step);

/* ========== IDENTIFIERS ========== */

IdentifierListNode* createIdentifierListNode(ExprNode* firstElement);

IdentifierListNode* addElementToIdentifierList(IdentifierListNode* listId, ExprNode* newElement);

/* ========== TARGET LIST ========== */

TargetListNode* createTargetListNode(ExprNode* firstElement);

TargetListNode* addElementToTargetList(TargetListNode* list, ExprNode* newElement);

/* ========== PARAM LIST ========== */

ParamListNode* createParamListNode(ExprNode* firstElement);

ParamListNode* addElementToParamList(ParamListNode* list, ExprNode* newElement);

/* ========== STATEMENT ========== */

StmtNode* createCompoundIfStmtNode(StmtNode* ifStmt, StmtNode* elseStmt, StmtsListNode* elseStmtsList);

StmtNode* createIfStmtNode(ExprNode* expr, StmtsListNode* suite);

StmtNode* createElseStmtNode(StmtsListNode* suite);

StmtNode* createElifStmtNode(ExprNode* expr, StmtsListNode* suite);

/* ========== STATEMENTS LIST ========== */

StmtsListNode* createStmtsListNode(StmtNode* firstElement);

StmtsListNode* addElementToStmtsList(StmtsListNode* list, StmtNode* newElement);
