#include "parsing_tree.h"

#pragma warning(disable: 6011)

int ID = 0;

/* ========== EXPRESSION ========== */

struct ExprNode* createIdExprNode(string* id) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _IDENTIFIER;
	node->identifier = id;
	node->left = NULL;
	node->right = NULL;
	node->id = ID++;

	return node;
}

struct ExprNode* createIntConstExprNode(int value) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _INT_CONST;
	node->intVal = value;
	node->left = NULL;
	node->right = NULL;
	node->id = ID++;

	return node;
}

struct ExprNode* createFloatConstExprNode(float value) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _FLOAT_CONST;
	node->floatVal = value;
	node->left = NULL;
	node->right = NULL;
	node->id = ID++;

	return node;
}

struct ExprNode* createStringConstExprNode(string* text) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _STRING_CONST;
	node->stringVal = *text;
	node->left = NULL;
	node->right = NULL;
	node->id = ID++;

	return node;
}

struct ExprNode* createTrueConstExprNode() {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _TRUE;
	node->boolVal = true;
	node->left = NULL;
	node->right = NULL;
	node->id = ID++;

	return node;
}

struct ExprNode* createFalseConstExprNode() {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _FALSE;
	node->boolVal = false;
	node->left = NULL;
	node->right = NULL;
	node->id = ID++;

	return node;
}

struct ExprNode* createPlusExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _PLUS;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createMinusExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _MINUS;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createMulExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _MUL;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createDivExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _DIV;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createAndLogicExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _AND_LOGIC;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createAndBitwiseExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _AND_BITWISE;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createOrLogicExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _OR_LOGIC;
		node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}


struct ExprNode* createOrBitwiseExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _OR_BITWISE;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createGreatExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _GREAT;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createGreateEqualExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _GREAT_EQUAL;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createLessExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _LESS;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createLessEqualExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _LESS_EQUAL;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createEqualExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _EQUAL;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createNotEqualExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _NOT_EQUAL;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}

struct ExprNode* createUnaryPlusExprNode(struct ExprNode* value) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _U_PLUS;
	node->left = NULL;
	node->right = value;
	node->id = ID++;

	return node;
}

struct ExprNode* createUnaryMinusExprNode(struct ExprNode* value) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _U_MINUS;
	node->left = NULL;
	node->right = value;
	node->id = ID++;

	return node;
}

struct ExprNode* createSelfExprNode() {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _SELF;
	node->left = NULL;
	node->right = NULL;
	node->id = ID++;

	return node;
}

struct ExprNode* createSuperExprNode() {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _SUPER;
	node->left = NULL;
	node->right = NULL;
	node->id = ID++;

	return node;
}