#include "dot_generation.h"
#include "DotHelpers.h"

string generateDotFromExprNode(struct ExprNode* node) {
	string dot = "";

	switch (node->type)
	{
	case _IDENTIFIER:
		dot += dotLabel(node->id, node->identifier);
		break;
	case _INT_CONST:
		dot += dotLabel(node->id, to_string(node->intVal));
		break;
	case _PLUS:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, "+");
		dot += dotConnection(node->id, node->left->id);
		dot += dotConnection(node->id, node->right->id);
		break;
	}

	return dot;
}