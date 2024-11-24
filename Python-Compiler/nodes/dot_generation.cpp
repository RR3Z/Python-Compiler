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
		dot += dotConnectionWithLabel(node->id, node->left->id, "id");
		dot += dotConnectionWithLabel(node->id, node->right->id, "value");
		break;
	case _BRACKETS:
		dot += generateDotFromExprNode(node->left);
		dot += dotLabel(node->id, "In Parantheses Brackets\n( (expr) )");
		dot += dotConnection(node->id, node->left->id);
		break;
	case _ATTRIBUTE_REF:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, "Attribute ref\n(expr.identifier)");
		dot += dotConnectionWithLabel(node->id, node->left->id, "id");
		dot += dotConnectionWithLabel(node->id, node->right->id, "attribute");
		break;
	case _LIST_ACCESS:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, "List Access\n(expr[expr])");
		dot += dotConnectionWithLabel(node->id, node->left->id, "id");
		dot += dotConnectionWithLabel(node->id, node->right->id, "index");
		break;
	case _LIST_CREATION:
		if (node->list != nullptr) {
			dot += dotLabel(node->id, "[exprList]");

			if (node->list->first != nullptr) {
				ExprNode* expr = node->list->first;

				dot += generateDotFromExprNode(expr);
				dot += dotConnection(node->id, expr->id);

				while (expr->next != nullptr) {
					dot += generateDotFromExprNode(expr->next);
					dot += dotConnection(node->id, expr->next->id);
					expr = expr->next;
				}
			}
		}
		else {
			dot += dotLabel(node->id, "[]");
		}
		break;
	case _SLICING:
		dot += generateDotFromSlicingNode(node->slicing);
		break;
	case _UNKNOWN:
		break;
	default:
		break;
	}
	
	return dot;
}

string generateDotFromSlicingNode(SlicingNode* node) {
	string dot = "";

	if (node == nullptr) { return dot; }

	dot += dotLabel(node->id, "slicing\n(start:end:step)");
	if (node->start != nullptr) {
		dot += generateDotFromExprNode(node->start);
		dot += dotConnectionWithLabel(node->id, node->start->id, "start");
	}

	if (node->end != nullptr) {
		dot += generateDotFromExprNode(node->end);
		dot += dotConnectionWithLabel(node->id, node->end->id, "end");
	}

	if (node->step != nullptr) {
		dot += generateDotFromExprNode(node->step);
		dot += dotConnectionWithLabel(node->id, node->step->id, "step");
	}

	return dot;
}