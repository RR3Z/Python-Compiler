#pragma once
#include "parsing_tree.h"

#pragma warning(disable: 6011)

int ID = 0;

/* ========== EXPRESSION ========== */

ExprNode* createIdExprNode(string *identifier) {
	ExprNode* node = new ExprNode();

	node->exprType = _IDENTIFIER;
	node->identifier = *identifier;
	node->id = ID++;

	return node;
}

ExprNode* createIntConstExprNode(int value) {
	ExprNode* node = new ExprNode();

	node->exprType = _INT_CONST;
	node->intVal = value;
	node->left = nullptr;
	node->right = nullptr;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createFloatConstExprNode(float value) {
	ExprNode* node = new ExprNode();

	node->exprType = _FLOAT_CONST;
	node->floatVal = value;
	node->left = nullptr;
	node->right = nullptr;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createStringConstExprNode(string* text) {
	ExprNode* node = new ExprNode();

	node->exprType = _STRING_CONST;
	node->stringVal = *text;
	node->left = nullptr;
	node->right = nullptr;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createTrueConstExprNode() {
	ExprNode* node = new ExprNode();

	node->exprType = _TRUE;
	node->boolVal = true;
	node->left = nullptr;
	node->right = nullptr;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createFalseConstExprNode() {
	ExprNode* node = new ExprNode();

	node->exprType = _FALSE;
	node->boolVal = false;
	node->left = nullptr;
	node->right = nullptr;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createPlusExprNode(ExprNode* leftOperand, ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _PLUS;
	node->left = leftOperand;
	node->right = rightOperand;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createMinusExprNode(ExprNode* leftOperand, ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _MINUS;
	node->left = leftOperand;
	node->right = rightOperand;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createMulExprNode(ExprNode* leftOperand, ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _MUL;
	node->left = leftOperand;
	node->right = rightOperand;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createDivExprNode(ExprNode* leftOperand, ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _DIV;
	node->left = leftOperand;
	node->right = rightOperand;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createAndLogicExprNode(ExprNode* leftOperand, ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _AND_LOGIC;
	node->left = leftOperand;
	node->right = rightOperand;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createAndBitwiseExprNode(ExprNode* leftOperand, ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _AND_BITWISE;
	node->left = leftOperand;
	node->right = rightOperand;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createOrLogicExprNode(ExprNode* leftOperand, ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _OR_LOGIC;
	node->left = leftOperand;
	node->right = rightOperand;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createOrBitwiseExprNode(ExprNode* leftOperand, ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _OR_BITWISE;
	node->left = leftOperand;
	node->right = rightOperand;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createGreatExprNode(ExprNode* leftOperand, ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _GREAT;
	node->left = leftOperand;
	node->right = rightOperand;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createGreatEqualExprNode(ExprNode* leftOperand, ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _GREAT_EQUAL;
	node->left = leftOperand;
	node->right = rightOperand;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createLessExprNode(ExprNode* leftOperand, ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _LESS;
	node->left = leftOperand;
	node->right = rightOperand;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createLessEqualExprNode(ExprNode* leftOperand, ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _LESS_EQUAL;
	node->left = leftOperand;
	node->right = rightOperand;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createEqualExprNode(ExprNode* leftOperand, ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _EQUAL;
	node->left = leftOperand;
	node->right = rightOperand;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createNotEqualExprNode(ExprNode* leftOperand, ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _NOT_EQUAL;
	node->left = leftOperand;
	node->right = rightOperand;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createUnaryPlusExprNode(ExprNode* value) {
	ExprNode* node = new ExprNode();

	node->exprType = _U_PLUS;
	node->left = nullptr;
	node->right = value;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createUnaryMinusExprNode(ExprNode* value) {
	ExprNode* node = new ExprNode();

	node->exprType = _U_MINUS;
	node->left = nullptr;
	node->right = value;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createSelfExprNode() {
	ExprNode* node = new ExprNode();

	node->exprType = _SELF;
	node->left = nullptr;
	node->right = nullptr;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createSuperExprNode() {
	ExprNode* node = new ExprNode();

	node->exprType = _SUPER;
	node->left = nullptr;
	node->right = nullptr;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createAssignOpExprNode(ExprNode* leftOperand, ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _ASSIGN_OP;
	node->left = leftOperand;
	node->right = rightOperand;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createExprInParenthesesBracketsNode(ExprNode* expression) {
	ExprNode* node = new ExprNode();

	node->exprType = _BRACKETS;
	node->left = expression;
	node->right = nullptr;
	node->next = nullptr;
	node->id = ID++;

	return node;
}

ExprNode* createAttributeRefExprNode(ExprNode* leftOperand, ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _ATTRIBUTE_REF;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

ExprNode* createListAccessExprNode(ExprNode* var, ExprNode* index) {
	ExprNode* node = new ExprNode();

	node->exprType = _LIST_ACCESS;
	node->left = var;
	node->right = index;
	node->id = ID++;

	return node;
}

ExprNode* createListCreationExprNode(ExprListNode* elements) {
	ExprNode* node = new ExprNode();

	node->exprType = _LIST_CREATION;
	node->list = elements;
	node->id = ID++;

	return node;
}

ExprNode* createListAccessWithSlicingExprNode(ExprNode* var, SlicingNode* slicing) {
	ExprNode* node = new ExprNode();

	node->exprType = _SLICING_LIST_ACCESS;
	node->left = var;
	node->slicing = slicing;
	node->id = ID++;

	return node;
}

ExprNode* createIfHeaderExprNode(ExprNode* expr) {
	ExprNode* node = new ExprNode();

	node->exprType = _IF_HEADER;
	node->left = expr;
	node->id = ID++;

	return node;
}

ExprNode* createForHeaderExprNode(TargetListNode* targetList, ExprNode* expr) {
	ExprNode* node = new ExprNode();

	node->exprType = _FOR_HEADER;
	node->left = expr;
	node->targetList = targetList;
	node->id = ID++;

	return node;
}

ExprNode* createListComprehensionExprNode(ExprNode* expr, ExprListNode* forHeaderList, ExprListNode* ifHeaderListE) {
	ExprNode* node = new ExprNode();

	node->exprType = _LIST_COMPREHENSION;
	node->left = expr;
	node->forHeaderList = forHeaderList;
	node->ifHeaderList = ifHeaderListE;
	node->id = ID++;

	return node;
}

/* ========== EXPRESSION LIST ========== */

ExprListNode* createExprListNode(ExprNode* firstElement) {
	ExprListNode* list = new ExprListNode();

	list->first = firstElement;
	list->last = firstElement;
	list->id = ID++;

	return list;
}

ExprListNode* addElementToExprList(ExprListNode* list, ExprNode* newElement) {
	list->last->next = newElement;
	list->last = newElement;

	return list;
}

/* ========== ARRAY SLICE ========== */

SlicingNode* createSlicingNode(ExprNode* start, ExprNode* end, ExprNode* step) {
	SlicingNode* ArraySlice = new SlicingNode();

	ArraySlice->start = start;
	ArraySlice->end = end;
	ArraySlice->step = step;

	return ArraySlice;
}

/* ========== IDENTIFIERS ========== */

IdentifierListNode* createIdentifierListNode(ExprNode* firstIdentifier) {
	IdentifierListNode* listId = new IdentifierListNode();

	listId->first = firstIdentifier;
	listId->last = firstIdentifier;
	listId->id = ID++;

	return listId;
}

IdentifierListNode* addElementToIdentifierList(IdentifierListNode* listId, ExprNode* newElement) {
	listId->last->next = newElement;
	listId->last = newElement;

	return listId;
}

/* ========== TARGET LIST ========== */

TargetListNode* createTargetListNode(ExprNode* firstElement) {
	TargetListNode* list = new TargetListNode();

	list->first = firstElement;
	list->last = firstElement;
	list->id = ID++;

	return list;
}

TargetListNode* addElementToTargetList(TargetListNode* list, ExprNode* newElement) {
	list->last->next = newElement;
	list->last = newElement;

	return list;
}

/* ========== PARAM LIST ========== */

ParamListNode* createParamListNode(ExprNode* firstIdentifier) {
	ParamListNode* list = new ParamListNode();

	list->first = firstIdentifier;
	list->last = firstIdentifier;
	list->id = ID++;

	return list;
}

ParamListNode* addElementToParamList(ParamListNode* list, ExprNode* newElement) {
	list->last->next = newElement;
	list->last = newElement;

	return list;
}

/* ========== STATEMENT ========== */

StmtNode* createCompoundIfStmtNode(StmtNode* ifStmt, StmtNode* elseStmt, StmtsListNode* elseStmtsList) {
	StmtNode* node = new StmtNode();

	node->stmtType = _COMPOUND_IF;
	node->leftNode = ifStmt;
	node->rightNode = elseStmt;
	node->stmtsList = elseStmtsList;
	node->id = ID++;

	return node;
}

StmtNode* createIfStmtNode(ExprNode* expr, StmtsListNode* suite) {
	StmtNode* node = new StmtNode();

	node->stmtType = _IF;
	node->expr = expr;
	node->suite = suite;
	node->id = ID++;

	return node;
}

StmtNode* createElseStmtNode(StmtsListNode* suite) {
	StmtNode* node = new StmtNode();

	node->stmtType = _ELSE;
	node->suite = suite;
	node->id = ID++;

	return node;
}

StmtNode* createElifStmtNode(ExprNode* expr, StmtsListNode* suite) {
	StmtNode* node = new StmtNode();

	node->stmtType = _ELIF;
	node->expr = expr;
	node->suite = suite;
	node->id = ID++;

	return node;
}

/* ========== STATEMENTS LIST ========== */

StmtsListNode* createStmtsListNode(StmtNode* firstElement) {
	StmtsListNode* list = new StmtsListNode();

	list->first = firstElement;
	list->last = firstElement;
	list->id = ID++;

	return list;
}

StmtsListNode* addElementToStmtsList(StmtsListNode* list, StmtNode* newElement) {
	list->last->next = newElement;
	list->last = newElement;

	return list;
}

