#pragma once
#include "dot_generation.h"
#include "DotHelpers.h"
#include <iostream>
using namespace std;

string generateDotFromExprNode(ExprNode* node) {
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
	case _ASSIGN_OP:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, ":=");
		dot += dotConnectionWithLabel(node->id, node->left->id, "id");
		dot += dotConnectionWithLabel(node->id, node->right->id, "value");
		break;
	case _BRACKETS:
		dot += generateDotFromExprNode(node->left);
		dot += dotLabel(node->id, "(expr)");
		dot += dotConnection(node->id, node->left->id);
		break;
	case _ATTRIBUTE_REF:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, "expr.identifier");
		dot += dotConnectionWithLabel(node->id, node->left->id, "id");
		dot += dotConnectionWithLabel(node->id, node->right->id, "attribute");
		break;
	case _LIST_ACCESS:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprNode(node->right);
		dot += dotLabel(node->id, "expr[expr]");
		dot += dotConnectionWithLabel(node->id, node->left->id, "id");
		dot += dotConnectionWithLabel(node->id, node->right->id, "index");
		break;
	case _LIST_CREATION:
		if (node->list != nullptr) {
			dot += dotLabel(node->id, "[exprList]");
			dot += generateDotFromExprListNode(node->list);
			dot += dotConnection(node->id, node->list->id);
		}
		else {
			dot += dotLabel(node->id, "[]");
		}
		break;
	case _SLICING_LIST_ACCESS:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromSlicingNode(node->slicing);
		dot += dotLabel(node->id, "id[start:end:step]");
		dot += dotConnectionWithLabel(node->id, node->left->id, "id");
		dot += dotConnectionWithLabel(node->id, node->slicing->id, "index");
		break;
	case _IF_HEADER:
		dot += generateDotFromExprNode(node->left);
		dot += dotLabel(node->id, "if expr");
		dot += dotConnectionWithLabel(node->id, node->left->id, "expr");
		break;
	case _FOR_HEADER:
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprListNode(node->list);
		dot += dotLabel(node->id, "for targetList in expr");
		dot += dotConnectionWithLabel(node->id, node->list->id, "targetList");
		dot += dotConnectionWithLabel(node->id, node->left->id, "expr");
		break;
	case _LIST_COMPREHENSION:
		dot += dotLabel(node->id, "List Comprehension");
		dot += generateDotFromExprNode(node->left);
		dot += generateDotFromExprListNode(node->forHeaderList);
		dot += dotConnectionWithLabel(node->id, node->left->id, "expr");
		dot += dotConnectionWithLabel(node->id, node->forHeaderList->id, "forHeaderList");
		if (node->ifHeaderList != nullptr) {
			dot += generateDotFromExprListNode(node->ifHeaderList);
			dot += dotConnectionWithLabel(node->id, node->ifHeaderList->id, "ifHeaderListE");
		}
		break;
	default:
		break;
	}
	
	return dot;
}

string generateDotFromExprListNode(ExprListNode* node) {
	string dot = "";

	if (node == nullptr) { return dot; }

	if (node->first != nullptr) {
		ExprNode* expr = node->first;

		dot += dotLabel(node->id, "exprList");
		dot += generateDotFromExprNode(expr);
		dot += dotConnection(node->id, expr->id);

		while (expr->next != nullptr) {
			dot += generateDotFromExprNode(expr->next);
			dot += dotConnection(node->id, expr->next->id);
			expr = expr->next;
		}
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
