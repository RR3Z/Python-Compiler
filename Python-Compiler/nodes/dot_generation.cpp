#include "dot_generation.h"
#include "DotHelpers.h"
#include <iostream>
using namespace std;

string generateDotFromExprNode(struct ExprNode* node) {
	string dot = "";

	if (node == nullptr) { return dot; }

	switch (node->exprType)
	{
	case _IDENTIFIER:
		dot += dotLabel(node->id, node->identifier);
		break;
	case _INT_CONST:
		dot += dotLabel(node->id, to_string(node->intVal));
		break;
	case _FLOAT_CONST:
		dot += dotLabel(node->id, to_string(node->floatVal));
		break;
	case _STRING_CONST:
		dot += dotLabel(node->id, node->stringVal);
		break;
	case _TRUE:
		dot += dotLabel(node->id, "True");
		break;
	case _FALSE:
		dot += dotLabel(node->id, "False");
		break;
	case _PLUS:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, "+");
		dot += dotConnection(node->id, node->left->id);
		dot += dotConnection(node->id, node->right->id);
		break;
	case _MINUS:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, "-");
		dot += dotConnection(node->id, node->left->id);
		dot += dotConnection(node->id, node->right->id);
		break;
	case _MUL:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, "*");
		dot += dotConnection(node->id, node->left->id);
		dot += dotConnection(node->id, node->right->id);
		break;
	case _DIV:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, "/");
		dot += dotConnection(node->id, node->left->id);
		dot += dotConnection(node->id, node->right->id);
		break;
	case _AND_LOGIC:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, "AND");
		dot += dotConnection(node->id, node->left->id);
		dot += dotConnection(node->id, node->right->id);
		break;
	case _AND_BITWISE:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, "&");
		dot += dotConnection(node->id, node->left->id);
		dot += dotConnection(node->id, node->right->id);
		break;
	case _OR_LOGIC:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, "OR");
		dot += dotConnection(node->id, node->left->id);
		dot += dotConnection(node->id, node->right->id);
		break;
	case _OR_BITWISE:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, "|");
		dot += dotConnection(node->id, node->left->id);
		dot += dotConnection(node->id, node->right->id);
		break;
	case _GREAT:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, ">");
		dot += dotConnection(node->id, node->left->id);
		dot += dotConnection(node->id, node->right->id);
		break;
	case _GREAT_EQUAL:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, ">=");
		dot += dotConnection(node->id, node->left->id);
		dot += dotConnection(node->id, node->right->id);
		break;
	case _LESS:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, "<");
		dot += dotConnection(node->id, node->left->id);
		dot += dotConnection(node->id, node->right->id);
		break;
	case _LESS_EQUAL:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, "<=");
		dot += dotConnection(node->id, node->left->id);
		dot += dotConnection(node->id, node->right->id);
		break;
	case _EQUAL:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, "==");
		dot += dotConnection(node->id, node->left->id);
		dot += dotConnection(node->id, node->right->id);
		break;
	case _NOT_EQUAL:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, "!=");
		dot += dotConnection(node->id, node->left->id);
		dot += dotConnection(node->id, node->right->id);
		break;
	case _U_PLUS:
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, "Unary plus");
		dot += dotConnection(node->id, node->right->id);
		break; 
	case _U_MINUS:
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, "Unary minus");
		dot += dotConnection(node->id, node->right->id);
		break;
	case _SELF:
		dot += dotLabel(node->id, "self");
		break;
	case _SUPER:
		dot += dotLabel(node->id, "super");
		break;
	case _ASSIGN:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, ":=");
		dot += dotConnection(node->id, node->left->id);
		dot += dotConnection(node->id, node->right->id);
		break;
	case _BRACKETS:
		dot += generateDotFromExprNode(node->left);
		dot += dotLabel(node->id, "In Parantheses Brackets");
		dot += dotConnection(node->id, node->left->id);
		break;
	case _ATTRIBUTE_REF:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, ". (attribute ref)");
		dot += dotConnection(node->id, node->left->id);
		dot += dotConnection(node->id, node->right->id);
		break;
	case _UNKNOWN:
		break;
	default:
		break;
	}
	
	return dot;
}