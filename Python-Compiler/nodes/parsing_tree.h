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

ExprNode* createAssignExprNode(ExprNode* leftOperand, ExprNode* rightOperand);

ExprNode* createExprInParenthesesBracketsNode(ExprNode* expression);

ExprNode* createAttributeRefExprNode(ExprNode* leftOperand, ExprNode* rightOperand);

ExprNode* createListAccessExprNode(ExprNode* name, ExprNode* element);

/* ========== EXPRESSION LIST ========== */

ExprListNode* createExprListNode(ExprNode* firstElement);

ExprListNode* addExprToExprList(ExprListNode* list, ExprNode* newElement);

/* ========== ARRAY SLICE ========== */

ArraySliceNode* createArraySliceNode(ExprNode* start, ExprNode* end, ExprNode* step);
