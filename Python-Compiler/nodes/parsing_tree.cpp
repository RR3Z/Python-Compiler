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

ExprNode* createForHeaderExprNode(ExprListNode* targetList, ExprNode* expr) {
	ExprNode* node = new ExprNode();

	node->exprType = _FOR_HEADER;
	node->left = expr;
	node->list = targetList;
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

ExprNode* createLambdaExprNode(FuncArgsListNode* funcArgsListNode, ExprNode* expr) {
	ExprNode* node = new ExprNode();

	node->exprType = _LAMBDA;
	node->left = expr;
	node->funcArgs = funcArgsListNode;
	node->id = ID++;

	return node;
}

ExprNode* createMethodCallExprNode(ExprNode* expr, string* identifier, FuncArgsListNode* funcArgs) {
	ExprNode* node = new ExprNode();

	node->exprType = _METHOD_CALL;
	node->identifier = *identifier;
	node->left = expr;
	node->funcArgs = funcArgs;
	node->id = ID++;

	return node;
}

ExprNode* createFunctionCallExprNode(ExprNode* expr, FuncArgsListNode* funcArgs) {
	ExprNode* node = new ExprNode();

	node->exprType = _FUNCTION_CALL;
	node->left = expr;
	node->funcArgs = funcArgs;
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


StmtNode* createFinallyStmtNode(StmtsListNode* suite) {
	StmtNode* node = new StmtNode();

	node->stmtType = _FINALLY;
	node->suite = suite;
	node->id = ID++;

	return node;
}

StmtNode* createAssignStmtNode(ExprNode* identifier, ExprNode* expr) {
	StmtNode* node = new StmtNode();

	node->stmtType = _ASSIGN;
	node->leftExpr = identifier;
	node->rightExpr = expr;
	node->id = ID++;

	return node;
}

StmtNode* createWhileStmtNode(ExprNode* expr, StmtsListNode* suite) {
	StmtNode* node = new StmtNode();

	node->stmtType = _WHILE;
	node->expr = expr;
	node->suite = suite;
	node->id = ID++;

	return node;
}

StmtNode* createCompoundWhileStmtNode(StmtNode* whileStmt, StmtNode* elseStmt) {
	StmtNode* node = new StmtNode();

	node->stmtType = _COMPOUND_WHILE;
	node->leftNode = whileStmt;
	node->rightNode = elseStmt;
	node->id = ID++;

	return node;
}

StmtNode* createReturnStmtNode(ExprListNode* list) {
	StmtNode* node = new StmtNode();

	node->stmtType = _RETURN;
    node->exprList = list;
	node->id = ID++;

	return node;
}

StmtNode* createExceptStmtNode(ExprNode* expr, StmtsListNode* suite) {
	StmtNode* node = new StmtNode();

	node->stmtType = _EXCEPT;
	node->expr = expr;
	node->suite = suite;
	node->id = ID++;

	return node;
}

StmtNode* createExceptIdentifierStmtNode(ExprNode* expr, ExprNode* identifier, StmtsListNode* suite) {
	StmtNode* node = new StmtNode();

	node->stmtType = _IDENTIFIER_EXCEPT;
	node->expr = expr;
	node->identifier = identifier;
	node->suite = suite;
	node->id = ID++;

	return node;
}

StmtNode* createForStmtNode(ExprListNode* targetList, ExprNode* expr, StmtsListNode* suite) {
	StmtNode* node = new StmtNode();

	node->stmtType = _FOR;
	node->exprList = targetList;
	node->expr = expr;
	node->stmtsList = suite;
	node->id = ID++;

	return node;
}

StmtNode* createCompoundForStmtNode(StmtNode* forStmt, StmtNode* elseStmt) {
	StmtNode* node = new StmtNode();

	node->stmtType = _COMPOUND_FOR;
	node->leftNode = forStmt;
	node->rightNode = elseStmt;
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

/* ========== FUNC ARG ========== */

FuncArgNode* createUnnamedFuncArgNode(string* identifier) {
	FuncArgNode* node = new FuncArgNode();

	node->funcArgType = _UNNAMED;
	node->identifier = *identifier;
	node->id = ID++;

	return node;
}

FuncArgNode* createNamedFuncArgNode(StmtNode* assignStmt) {
	FuncArgNode* node = new FuncArgNode();

	node->funcArgType = _NAMED;
	node->assignStmt = assignStmt;
	node->id = ID++;

	return node;
}

/* ========== FUNC ARGS LIST ========== */

FuncArgsListNode* createParamsListNode(FuncArgNode* firstElement) {
	FuncArgsListNode* list = new FuncArgsListNode();

	list->first = firstElement;
	list->last = firstElement;
	list->id = ID++;

	return list;
}

FuncArgsListNode* createFuncArgsListNodeFromExprList(ExprListNode* exprList) {
	FuncArgsListNode* list = new FuncArgsListNode();

	list->exprList = exprList;
	list->id = ID++;

	return list;
}

FuncArgsListNode* createFuncArgsListNodeFromFuncArgsList(FuncArgsListNode* namedArgsList) {
	FuncArgsListNode* list = new FuncArgsListNode();

	list->namedArgsList = namedArgsList;
	list->id = ID++;

	return list;
}

FuncArgsListNode* createFuncArgsListNodeFromDifferentLists(ExprListNode* exprList, FuncArgsListNode* namedArgsList) {
	FuncArgsListNode* list = new FuncArgsListNode();

	list->exprList = exprList;
	list->namedArgsList = namedArgsList;
	list->id = ID++;

	return list;
}

FuncArgsListNode* addElementToParamsList(FuncArgsListNode* list, FuncArgNode* newElement) {
	list->last->next = newElement;
	list->last = newElement;

	return list;
}
