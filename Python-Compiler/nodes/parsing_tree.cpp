#include "parsing_tree.h"

int ID = 0;

/* ========== EXPRESSION ========== */

struct ExprNode* createIdExprNode(string id) {
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

struct ExprNode* createPlusExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand) {
	struct ExprNode* node = (struct ExprNode*)malloc(sizeof(struct ExprNode));

	node->type = _PLUS;
	node->left = leftOperand;
	node->right = rightOperand;
	node->id = ID++;

	return node;
}