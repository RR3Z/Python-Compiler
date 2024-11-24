#include "parsing_tree.h"

#pragma warning(disable: 6011)

int ID = 0;

/* ========== EXPRESSION ========== */

struct ExprNode* createIdExprNode(string *identifier) {
	ExprNode* node = new ExprNode();

	node->exprType = _IDENTIFIER;
	node->identifier = *identifier;
	node->left = nullptr;
	node->right = nullptr;
	node->id = ID++;

	return node;
}

struct ExprNode* createIntConstExprNode(int value) {
	ExprNode* node = new ExprNode();

	node->exprType = _INT_CONST;
	node->intVal = value;
	node->left = nullptr;
	node->right = nullptr;
	node->id = ID++;

	return node;
}

struct ExprNode* createFloatConstExprNode(float value) {
	ExprNode* node = new ExprNode();

	node->exprType = _FLOAT_CONST;
	node->floatVal = value;
	node->left = nullptr;
	node->right = nullptr;
	node->id = ID++;

	return node;
}

struct ExprNode* createStringConstExprNode(string* text) {
	ExprNode* node = new ExprNode();

	node->exprType = _STRING_CONST;
	node->stringVal = *text;
	node->left = nullptr;
	node->right = nullptr;
	node->id = ID++;

	return node;
}

struct ExprNode* createTrueConstExprNode() {
	ExprNode* node = new ExprNode();

	node->exprType = _TRUE;
	node->boolVal = true;
	node->left = nullptr;
	node->right = nullptr;
	node->id = ID++;

	return node;
}

struct ExprNode* createFalseConstExprNode() {
	ExprNode* node = new ExprNode();

	node->exprType = _FALSE;
	node->boolVal = false;
	node->left = nullptr;
	node->right = nullptr;
	node->id = ID++;

	return node;
}

struct ExprNode* createPlusExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _PLUS;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createMinusExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _MINUS;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createMulExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _MUL;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createDivExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _DIV;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createAndLogicExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _AND_LOGIC;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createAndBitwiseExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _AND_BITWISE;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createOrLogicExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _OR_LOGIC;
		node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createOrBitwiseExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _OR_BITWISE;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createGreatExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _GREAT;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createGreatEqualExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _GREAT_EQUAL;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createLessExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _LESS;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createLessEqualExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _LESS_EQUAL;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createEqualExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _EQUAL;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createNotEqualExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	ExprNode* node = new ExprNode();

	node->exprType = _NOT_EQUAL;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createUnaryPlusExprNode(struct ExprNode* value) {
	ExprNode* node = new ExprNode();

	node->exprType = _U_PLUS;
	node->left = nullptr;
	node->right = value;
	node->id = ID++;

	return node;
}

struct ExprNode* createUnaryMinusExprNode(struct ExprNode* value) {
	ExprNode* node = new ExprNode();

	node->exprType = _U_MINUS;
	node->left = nullptr;
	node->right = value;
	node->id = ID++;

	return node;
}

struct ExprNode* createSelfExprNode() {
	ExprNode* node = new ExprNode();

	node->exprType = _SELF;
	node->left = nullptr;
	node->right = nullptr;
	node->id = ID++;

	return node;
}

struct ExprNode* createSuperExprNode() {
	ExprNode* node = new ExprNode();

	node->exprType = _SUPER;
	node->left = nullptr;
	node->right = nullptr;
	node->id = ID++;

	return node;
}