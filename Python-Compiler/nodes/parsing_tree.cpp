#include "parsing_tree.h"

#pragma warning(disable: 6011)

int ID = 0;

/* ========== EXPRESSION ========== */

ExprNode* createIdExprNode(string *identifier) {
	ExprNode* node = new ExprNode();

	node->exprType = _IDENTIFIER;
	node->identifier = *identifier;
	node->left = nullptr;
	node->right = nullptr;
	node->next = nullptr;
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

ExprNode* createAssignExprNode(ExprNode* leftOperand, ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _ASSIGN;
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
	node->next = nullptr;
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

ExprNode* createExprWithSlicingNode(ExprNode* var, SlicingNode* slicing) {
	ExprNode* node = new ExprNode();

	node->exprType = _SLICING_ACCESS;
	node->left = var;
	node->slicing = slicing;
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

ExprListNode* addExprToExprList(ExprListNode* list, ExprNode* newElement) {
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

	listId->firstId = firstIdentifier;
	listId->lastId = firstIdentifier;
	listId->id = ID++;

	return listId;
}

IdentifierListNode* addIdentifierToIdentifierList(IdentifierListNode* listId, ExprNode* newElement) {
	listId->lastId->next = newElement;
	listId->lastId = newElement;

	return listId;
}
