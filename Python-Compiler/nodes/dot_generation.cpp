#pragma once
#include "dot_generation.h"
#include "DotHelpers.h"
#include <iostream>
using namespace std;

string generateDotFromRoot(FileNode* root) {
	string dot = "";
	if (root == nullptr) { return dot; }

	if (root->elementsList->first != nullptr) {
		FileElementNode* element = root->elementsList->first;

		dot += dotLabel(root->id, "ROOT");
		dot += generateDotFromFileElementNode(root->id, element);

		while (element->next != nullptr) {
			dot += generateDotFromFileElementNode(root->id, element->next);
			element = element->next;
		}
	}

	return dot;
}

string generateDotFromFileElementNode(int parentId, FileElementNode* node) {
	string dot = "";
	if (node == nullptr) { return dot; }

	switch (node->elementType)
	{
		case _CLASS_DEF:
			dot += generateDotFromClassNode(node->classDef);
			dot += dotConnection(parentId, node->classDef->id);
			break;
		case _FUNC_DEF:
			dot += generateDotFromFuncNode(node->funcDef);
			dot += dotConnection(parentId, node->funcDef->id);
			break;
		case _STMT:
			dot += generateDotFromStmtNode(node->stmt);
			dot += dotConnection(parentId, node->stmt->id);
			break;
		default:
			break;
	}

	return dot;
}

string generateDotFromClassNode(ClassNode* node) {
	string dot = "";
	if (node == nullptr) { return dot; }

	dot += dotLabel(node->id, "class\\nName: " + node->identifier->identifier);

	// Родительский класс (одиночное наследование)
	if (node->base != nullptr) {
		dot += generateDotFromExprNode(node->base);
		dot += dotConnectionWithLabel(node->id, node->base->id, "base");
	}

	// Тело класса
	dot += generateDotFromClassElementsListNode(node->suite);
	dot += dotConnection(node->id, node->suite->id);

	return dot;
}

string generateDotFromFuncNode(FuncNode* node) {
	string dot = "";
	if (node == nullptr) { return dot; }

	dot += dotLabel(node->id, "def\\nName: " + node->identifier->identifier);

	// Аргументы функции
	if (node->args != nullptr) {
		dot += dotLabel(node->args->id, "args");
		dot += generateDotFromFuncArgsListNode(node->args->id, node->args);
		dot += dotConnection(node->id, node->args->id);
	}

	// Тело функции
	dot += dotLabel(node->suite->id, "suite");
	dot += generateDotFromStmtsListNode(node->suite->id, node->suite);
	dot += dotConnection(node->id, node->suite->id);

	return dot;
}

string generateDotFromStmtNode(StmtNode* node) {
	string dot = "";
	if (node == nullptr) { return dot; }

	switch (node->stmtType)
	{
		case _IF:
			dot += dotLabel(node->id, "IF stmt");
			// Condition
			dot += generateDotFromExprNode(node->expr);
			dot += dotConnectionWithLabel(node->id, node->expr->id, "condition");
			// Suite
			dot += dotLabel(node->suite->id, "suite");
			dot += generateDotFromStmtsListNode(node->suite->id, node->suite);
			dot += dotConnection(node->id, node->suite->id);
			break;
		case _ELSE:
			dot += dotLabel(node->id, "ELSE stmt");
			// Suite
			dot += dotLabel(node->suite->id, "suite");
			dot += generateDotFromStmtsListNode(node->suite->id, node->suite);
			dot += dotConnection(node->id, node->suite->id);
			break;
		case _ELIF:
			dot += dotLabel(node->id, "ELIF stmt");
			// Condition
			dot += generateDotFromExprNode(node->expr);
			dot += dotConnectionWithLabel(node->id, node->expr->id, "condition");
			// Suite
			dot += dotLabel(node->suite->id, "suite");
			dot += generateDotFromStmtsListNode(node->suite->id, node->suite);
			dot += dotConnection(node->id, node->suite->id);
			break;
		case _COMPOUND_IF:
			dot += dotLabel(node->id, "Compound\\nIF stmt");
			// IF stmt
			dot += generateDotFromStmtNode(node->leftNode);
			dot += dotConnection(node->id, node->leftNode->id);
			// ELIF stmt
			if (node->stmtsList != nullptr) {
				dot += generateDotFromStmtsListNode(node->id, node->stmtsList);
			}
			// ELSE STMT
			if (node->rightNode != nullptr) {
				dot += generateDotFromStmtNode(node->rightNode);
				dot += dotConnection(node->id, node->rightNode->id);
			}
			break;
		case _WHILE:
			dot += dotLabel(node->id, "WHILE stmt");
			// Condition
			dot += generateDotFromExprNode(node->expr);
			dot += dotConnectionWithLabel(node->id, node->expr->id, "condition");
			// Suite
			dot += dotLabel(node->suite->id, "suite");
			dot += generateDotFromStmtsListNode(node->suite->id, node->suite);
			dot += dotConnection(node->id, node->suite->id);
			break;
		case _COMPOUND_WHILE:
			dot += dotLabel(node->id, "Compound\\nWHILE stmt");
			// WHILE stmt
			dot += generateDotFromStmtNode(node->leftNode);
			dot += dotConnection(node->id, node->leftNode->id);
			// ELSE STMT
			dot += generateDotFromStmtNode(node->rightNode);
			dot += dotConnection(node->id, node->rightNode->id);
			break;
		case _FOR:
			dot += dotLabel(node->id, "FOR stmt");
			// Target List
			dot += dotLabel(node->list->id, "Target List");
			dot += generateDotFromExprListNode(node->list->id, node->list);
			dot += dotConnection(node->id, node->list->id);
			// Expression
			dot += generateDotFromExprNode(node->expr);
			dot += dotConnectionWithLabel(node->id, node->expr->id, "expr");
			// Suite
			dot += dotLabel(node->suite->id, "suite");
			dot += generateDotFromStmtsListNode(node->suite->id, node->suite);
			dot += dotConnection(node->id, node->suite->id);
			break;
		case _COMPOUND_FOR:
			dot += dotLabel(node->id, "Compound\\nFOR stmt");
			// FOR stmt
			dot += generateDotFromStmtNode(node->leftNode);
			dot += dotConnection(node->id, node->leftNode->id);
			// ELSE STMT
			dot += generateDotFromStmtNode(node->rightNode);
			dot += dotConnection(node->id, node->rightNode->id);
			break;
		case _EXCEPT:
			dot += dotLabel(node->id, "EXCEPT stmt");
			// Expression
			if (node->expr != nullptr) {
				dot += generateDotFromExprNode(node->expr);
				dot += dotConnectionWithLabel(node->id, node->expr->id, "expr");
			}
			// Suite
			dot += dotLabel(node->suite->id, "suite");
			dot += generateDotFromStmtsListNode(node->suite->id, node->suite);
			dot += dotConnection(node->id, node->suite->id);
			break;
		case _IDENTIFIER_EXCEPT:
			dot += dotLabel(node->id, "EXCEPT stmt");
			// Expression
			dot += generateDotFromExprNode(node->expr);
			dot += dotConnectionWithLabel(node->id, node->expr->id, "expr");
			// Identifier (except expr as IDENTIFIER)
			dot += generateDotFromExprNode(node->identifier);
			dot += dotConnectionWithLabel(node->id, node->identifier->id, "as expr");
			// Suite
			dot += dotLabel(node->suite->id, "suite");
			dot += generateDotFromStmtsListNode(node->suite->id, node->suite);
			dot += dotConnection(node->id, node->suite->id);
			break;
		case _FINALLY:
			dot += dotLabel(node->id, "FINALLY stmt");
			// Suite
			dot += dotLabel(node->suite->id, "suite");
			dot += generateDotFromStmtsListNode(node->suite->id, node->suite);
			dot += dotConnection(node->id, node->suite->id);
			break;
		case _TRY:
			dot += dotLabel(node->id, "TRY stmt");
			// Suite
			dot += dotLabel(node->suite->id, "suite");
			dot += generateDotFromStmtsListNode(node->suite->id, node->suite);
			dot += dotConnection(node->id, node->suite->id);
			break;
		case _COMPOUND_TRY:
			dot += dotLabel(node->id, "Compound\\nTRY stmt");
			// TRY stmt
			dot += generateDotFromStmtNode(node->tryStmt);
			dot += dotConnection(node->id, node->tryStmt->id);
			// EXCEPT stmt/stmts
			if (node->stmtsList != nullptr) {
				dot += generateDotFromStmtsListNode(-1,node->stmtsList);
				dot += dotConnection(node->id, node->stmtsList->id);
			}
			// ELSE stmt
			if (node->leftNode != nullptr) {
				dot += generateDotFromStmtNode(node->leftNode);
				dot += dotConnection(node->id, node->leftNode->id);
			}
			// FINALY stmt
			if (node->rightNode != nullptr) {
				dot += generateDotFromStmtNode(node->rightNode);
				dot += dotConnection(node->id, node->rightNode->id);
			}
			break;
		case _ASSIGN:
			dot += dotLabel(node->id, "Assign stmt");
			// Identifier
			dot += generateDotFromExprNode(node->leftExpr);
			dot += dotConnection(node->id, node->leftExpr->id);
			// Expression
			dot += generateDotFromExprNode(node->rightExpr);
			dot += dotConnection(node->id, node->rightExpr->id);
			break;
		case _COMPOUND_ASSIGN:
			dot += dotLabel(node->id, "Compound\\nAssign stmt");
			// Target List
			dot += generateDotFromStmtsListNode(node->id,node->stmtsList);
			// Expression
			dot += generateDotFromExprListNode(-1, node->list);
			dot += dotConnection(node->id, node->list->id);
			break;
		case _PLUS_ASSIGN:
			dot += dotLabel(node->id, "+= stmt");
			// Identifier
			dot += generateDotFromExprNode(node->leftExpr);
			dot += dotConnection(node->id, node->leftExpr->id);
			// Expression
			dot += generateDotFromExprNode(node->rightExpr);
			dot += dotConnection(node->id, node->rightExpr->id);
			break;
		case _MINUS_ASSIGN:
			dot += dotLabel(node->id, "-= stmt");
			// Identifier
			dot += generateDotFromExprNode(node->leftExpr);
			dot += dotConnection(node->id, node->leftExpr->id);
			// Expression
			dot += generateDotFromExprNode(node->rightExpr);
			dot += dotConnection(node->id, node->rightExpr->id);
			break;
		case _MUL_ASSIGN:
			dot += dotLabel(node->id, "*= stmt");
			// Identifier
			dot += generateDotFromExprNode(node->leftExpr);
			dot += dotConnection(node->id, node->leftExpr->id);
			// Expression
			dot += generateDotFromExprNode(node->rightExpr);
			dot += dotConnection(node->id, node->rightExpr->id);
			break;
		case _DIV_ASSIGN:
			dot += dotLabel(node->id, "/= stmt");
			// Identifier
			dot += generateDotFromExprNode(node->leftExpr);
			dot += dotConnection(node->id, node->leftExpr->id);
			// Expression
			dot += generateDotFromExprNode(node->rightExpr);
			dot += dotConnection(node->id, node->rightExpr->id);
			break;
		case _EXPR_STMT:
			dot += dotLabel(node->id, "EXPR stmt");
			// Expression
			dot += generateDotFromExprNode(node->expr);
			dot += dotConnection(node->id, node->expr->id);
			break;
		case _EXPR_LIST_STMT:
			dot += dotLabel(node->id, "Assign\\nTarget List");
			// Target List
			dot += generateDotFromExprListNode(node->id, node->list);
			break;
		case _RETURN:
			dot += dotLabel(node->id, "RETURN stmt");
			// Return values
			dot += generateDotFromExprListNode(node->id, node->list);
			break;
		default:
			break;
	}

	return dot;
}

string generateDotFromStmtsListNode(int parentId, StmtsListNode* node) {
	string dot = "";
	if (node == nullptr) { return dot; }

	if (node->first != nullptr) {
		StmtNode* element = node->first;

		if (parentId > 0) {
			dot += generateDotFromStmtNode(element);
			dot += dotConnection(parentId, element->id);

			while (element->next != nullptr) {
				dot += generateDotFromStmtNode(element->next);
				dot += dotConnection(parentId, element->next->id);
				element = element->next;
			}
		}
		else {
			dot += dotLabel(node->id, "Stmts List");
			dot += generateDotFromStmtNode(element);
			dot += dotConnection(node->id, element->id);

			while (element->next != nullptr) {
				dot += generateDotFromStmtNode(element->next);
				dot += dotConnection(node->id, element->next->id);
				element = element->next;
			}
		}	
	}

	return dot;
}

string generateDotFromFuncArgsListNode(int parentId, FuncArgsListNode* node) {
	string dot = "";
	if (node == nullptr) { return dot; }

	if (node->first != nullptr) {
		FuncArgNode* element = node->first;

		if (parentId > 0) {
			dot += generateDotFromFuncArgNode(parentId, element);

			while (element->next != nullptr) {
				dot += generateDotFromFuncArgNode(parentId, element->next);
				element = element->next;
			}
		}
		else {
			dot += dotLabel(node->id, "Func Args List");
			dot += generateDotFromFuncArgNode(node->id, element);
			dot += dotConnection(node->id, element->id);

			while (element->next != nullptr) {
				dot += generateDotFromFuncArgNode(node->id, element->next);
				dot += dotConnection(node->id, element->next->id);
				element = element->next;
			}
		}
	}

	if (node->namedArgsList != nullptr) {
		if (parentId > 0) {
			dot += generateDotFromFuncArgsListNode(parentId, node->namedArgsList);
			dot += dotConnection(parentId, node->namedArgsList->id);
		}
		else {
			dot += dotLabel(node->id, "Func Args List");
			dot += generateDotFromFuncArgsListNode(node->id, node->namedArgsList);
			dot += dotConnection(node->id, node->namedArgsList->id);
		}
	}

	if (node->exprList != nullptr) {
		if (parentId > 0) {
			dot += generateDotFromExprListNode(parentId, node->exprList);
		}
		else {
			dot += dotLabel(node->id, "Func Args List");
			dot += generateDotFromExprListNode(node->id, node->exprList);
		}
	}

	return dot;
}

string generateDotFromFuncArgNode(int parentId, FuncArgNode* node) {
	string dot = "";
	if (node == nullptr) { return dot; }

	switch (node->funcArgType)
	{
		case _NAMED:
			dot += generateDotFromStmtNode(node->assignStmt);
			dot += dotConnection(parentId, node->assignStmt->id);
			break;
		case _VAR:
			dot += generateDotFromExprNode(node->var);
			dot += dotConnection(parentId, node->var->id);
			break;
		default:
			break;
	}

	return dot;
}

string generateDotFromClassElementsListNode(ClassElementsListNode* node) {
	string dot = "";
	if (node == nullptr) { return dot; }

	if (node->first != nullptr) {
		ClassElementNode* element = node->first;

		dot += dotLabel(node->id, "suite");
		dot += generateDotFromClassElementNode(node->id, element);

		while (element->next != nullptr) {
			dot += generateDotFromClassElementNode(node->id, element->next);
			element = element->next;
		}
	}

	return dot;
}

string generateDotFromClassElementNode(int parentId, ClassElementNode* node) {
	string dot = "";
	if (node == nullptr) { return dot; }

	switch (node->elementType)
	{
		case _FUNCTION_DEF:
			dot += generateDotFromFuncNode(node->funcDef);
			dot += dotConnection(parentId, node->funcDef->id);
			break;
		case _STMT_NODE:
			dot += generateDotFromStmtNode(node->stmt);
			dot += dotConnection(parentId, node->stmt->id);
			break;
		default:
			break;
	}

	return dot;
}

string generateDotFromExprNode(ExprNode* node) {
	string dot = "";

	if (node == nullptr) { return dot; }

	switch (node->exprType)
	{
		case _PLUS:
			dot += dotLabel(node->id, "+");
			dot += generateDotFromExprNode(node->left);
			dot += dotConnection(node->id, node->left->id);
			dot += generateDotFromExprNode(node->right);
			dot += dotConnection(node->id, node->right->id);
			break;
		case _MINUS:
			dot += dotLabel(node->id, "-");
			dot += generateDotFromExprNode(node->left);
			dot += dotConnection(node->id, node->left->id);
			dot += generateDotFromExprNode(node->right);
			dot += dotConnection(node->id, node->right->id);
			break;
		case _MUL:
			dot += dotLabel(node->id, "*");
			dot += generateDotFromExprNode(node->left);
			dot += dotConnection(node->id, node->left->id);
			dot += generateDotFromExprNode(node->right);
			dot += dotConnection(node->id, node->right->id);
			break;
		case _DIV:
			dot += dotLabel(node->id, "/");
			dot += generateDotFromExprNode(node->left);
			dot += dotConnection(node->id, node->left->id);
			dot += generateDotFromExprNode(node->right);
			dot += dotConnection(node->id, node->right->id);
			break;
		case _AND_LOGIC:
			dot += dotLabel(node->id, "and");
			dot += generateDotFromExprNode(node->left);
			dot += dotConnection(node->id, node->left->id);
			dot += generateDotFromExprNode(node->right);
			dot += dotConnection(node->id, node->right->id);
			break;
		case _AND_BITWISE:
			dot += dotLabel(node->id, "&");
			dot += generateDotFromExprNode(node->left);
			dot += dotConnection(node->id, node->left->id);
			dot += generateDotFromExprNode(node->right);
			dot += dotConnection(node->id, node->right->id);
			break;
		case _OR_LOGIC:
			dot += dotLabel(node->id, "or");
			dot += generateDotFromExprNode(node->left);
			dot += dotConnection(node->id, node->left->id);
			dot += generateDotFromExprNode(node->right);
			dot += dotConnection(node->id, node->right->id);
			break;
		case _OR_BITWISE:
			dot += dotLabel(node->id, "|");
			dot += generateDotFromExprNode(node->left);
			dot += dotConnection(node->id, node->left->id);
			dot += generateDotFromExprNode(node->right);
			dot += dotConnection(node->id, node->right->id);
			break;
		case _GREAT:
			dot += dotLabel(node->id, ">");
			dot += generateDotFromExprNode(node->left);
			dot += dotConnection(node->id, node->left->id);
			dot += generateDotFromExprNode(node->right);
			dot += dotConnection(node->id, node->right->id);
			break;
		case _GREAT_EQUAL:
			dot += dotLabel(node->id, ">=");
			dot += generateDotFromExprNode(node->left);
			dot += dotConnection(node->id, node->left->id);
			dot += generateDotFromExprNode(node->right);
			dot += dotConnection(node->id, node->right->id);
			break;
		case _LESS:
			dot += dotLabel(node->id, "<");
			dot += generateDotFromExprNode(node->left);
			dot += dotConnection(node->id, node->left->id);
			dot += generateDotFromExprNode(node->right);
			dot += dotConnection(node->id, node->right->id);
			break;
		case _LESS_EQUAL:
			dot += dotLabel(node->id, "<=");
			dot += generateDotFromExprNode(node->left);
			dot += dotConnection(node->id, node->left->id);
			dot += generateDotFromExprNode(node->right);
			dot += dotConnection(node->id, node->right->id);
			break;
		case _EQUAL:
			dot += dotLabel(node->id, "==");
			dot += generateDotFromExprNode(node->left);
			dot += dotConnection(node->id, node->left->id);
			dot += generateDotFromExprNode(node->right);
			dot += dotConnection(node->id, node->right->id);
			break;
		case _NOT_EQUAL:
			dot += dotLabel(node->id, "!=");
			dot += generateDotFromExprNode(node->left);
			dot += dotConnection(node->id, node->left->id);
			dot += generateDotFromExprNode(node->right);
			dot += dotConnection(node->id, node->right->id);
			break;
		case _U_PLUS:
			dot += dotLabel(node->id, "UNAR_PLUS");
			dot += generateDotFromExprNode(node->right);
			dot += dotConnection(node->id, node->right->id);
			break;
		case _U_MINUS:
			dot += dotLabel(node->id, "UNAR_MINUS");
			dot += generateDotFromExprNode(node->right);
			dot += dotConnection(node->id, node->right->id);
			break;
		case _LAMBDA:
			dot += dotLabel(node->id, "LAMBDA");
			// PARAMS
			dot += dotLabel(node->funcArgs->id, "lambda params");
			dot += generateDotFromFuncArgsListNode(node->funcArgs->id, node->funcArgs);
			dot += dotConnection(node->id, node->funcArgs->id); 
			// EXPRESSION
			dot += generateDotFromExprNode(node->left);
			dot += dotConnection(node->id, node->left->id);
			break;
		case _ASSIGN_OP:
			dot += dotLabel(node->id, ":=");
			dot += generateDotFromExprNode(node->left);
			dot += dotConnection(node->id, node->left->id);
			dot += generateDotFromExprNode(node->right);
			dot += dotConnection(node->id, node->right->id);
			break;
		case _SUPER:
			dot += dotLabel(node->id, "super");
			break;
		case _SELF:
			dot += dotLabel(node->id, "self");
			break;
		case _FALSE:
			dot += dotLabel(node->id, "False");
			break;
		case _TRUE:
			dot += dotLabel(node->id, "True");
			break;
		case _INT_CONST:
			dot += dotLabel(node->id, to_string(node->intVal));
			break;
		case _FLOAT_CONST:
			dot += dotLabel(node->id, to_string(node->floatVal));
			break;
		case _STRING_CONST:
			dot += dotLabel(node->id, "\'\'" + node->stringVal + "\'\'");
			break;
		case _ATTRIBUTE_REF:
			dot += dotLabel(node->id, "Attribute ref");
			dot += generateDotFromExprNode(node->left);
			dot += dotConnectionWithLabel(node->id, node->left->id, "expr");
			dot += generateDotFromExprNode(node->right);
			dot += dotConnectionWithLabel(node->id, node->right->id, "attribute");
			break;
		case _METHOD_CALL:
			dot += dotLabel(node->id, "Method call");
			// EXPRESSION
			dot += generateDotFromExprNode(node->left);
			dot += dotConnectionWithLabel(node->id, node->left->id, "classId");
			// IDENTIFIER
			dot += generateDotFromExprNode(node->right);
			dot += dotConnectionWithLabel(node->id, node->right->id, "funcId");
			// PARAMS
			dot += dotLabel(node->funcArgs->id, "params");
			dot += generateDotFromFuncArgsListNode(node->funcArgs->id, node->funcArgs);
			dot += dotConnection(node->id, node->funcArgs->id); 
			break;
		case _FUNCTION_CALL:
			dot += dotLabel(node->id, "Function call");
			// EXPRESSION
			dot += generateDotFromExprNode(node->left);
			dot += dotConnectionWithLabel(node->id, node->left->id, "id");
			// PARAMS
			if (node->funcArgs != nullptr) {
				dot += dotLabel(node->funcArgs->id, "params");
				dot += generateDotFromFuncArgsListNode(node->funcArgs->id, node->funcArgs);
				dot += dotConnection(node->id, node->funcArgs->id);
			}
			break;
		case _SLICING_LIST_ACCESS:
			dot += generateDotFromExprNode(node->left);
			dot += generateDotFromSlicingNode(node->slicing);
			dot += dotLabel(node->id, "id[start:end:step]");
			dot += dotConnectionWithLabel(node->id, node->left->id, "id");
			dot += dotConnectionWithLabel(node->id, node->slicing->id, "index");
			break;
		case _LIST_ACCESS:
			dot += dotLabel(node->id, "expr[expr]\nlist access");
			dot += generateDotFromExprNode(node->left);
			dot += dotConnectionWithLabel(node->id, node->left->id, "expr");
			dot += generateDotFromExprNode(node->right);
			dot += dotConnectionWithLabel(node->id, node->right->id, "index");
			break;
		case _LIST_COMPREHENSION:
			dot += dotLabel(node->id, "List Comprehension");
			dot += generateDotFromExprNode(node->left);
			dot += generateDotFromExprListNode(node->id, node->forHeaderList);
			dot += dotConnectionWithLabel(node->id, node->left->id, "expr");
			if (node->ifHeaderList != nullptr) {
				dot += generateDotFromExprListNode(node->id, node->ifHeaderList);
				dot += dotConnectionWithLabel(node->id, node->ifHeaderList->id, "ifHeaderList");
			}
			break;
		case _LIST_CREATION:
			if (node->list != nullptr) {
				dot += dotLabel(node->id, "[exprList]");
				dot += generateDotFromExprListNode(node->id, node->list);
			}
			else {
				dot += dotLabel(node->id, "[]");
			}
			break;
		case _BRACKETS:
			dot += dotLabel(node->id, "(expr)"); 
			dot += generateDotFromExprNode(node->left);
			dot += dotConnection(node->id, node->left->id);
			break;
		case _IDENTIFIER:
			dot += dotLabel(node->id, node->identifier);
			break;
		case _IF_HEADER:
			dot += dotLabel(node->id, "if header");
			dot += generateDotFromExprNode(node->left);
			dot += dotConnectionWithLabel(node->id, node->left->id, "expr");
			break;
		case _FOR_HEADER:
			dot += dotLabel(node->id, "for header");
			dot += generateDotFromExprNode(node->left);
			dot += dotConnectionWithLabel(node->id, node->left->id, "expr");
			dot += generateDotFromExprListNode(node->id, node->list);
			break;
		case _FUNC_HEADER:
			break;
		default:
			break;
	}
	
	return dot;
}

string generateDotFromExprListNode(int parentId, ExprListNode* node) {
	string dot = "";

	if (node == nullptr) { return dot; }

	if (node->first != nullptr) {
		ExprNode* expr = node->first;

		if (parentId > 0) {
			dot += generateDotFromExprNode(expr);
			dot += dotConnection(parentId, expr->id);

			while (expr->next != nullptr) {
				dot += generateDotFromExprNode(expr->next);
				dot += dotConnection(parentId, expr->next->id);
				expr = expr->next;
			}
		}
		else {
			dot += dotLabel(node->id, "Expr List");
			dot += generateDotFromExprNode(expr);
			dot += dotConnection(node->id, expr->id);

			while (expr->next != nullptr) {
				dot += generateDotFromExprNode(expr->next);
				dot += dotConnection(node->id, expr->next->id);
				expr = expr->next;
			}
		}
	}

	return dot;
}

string generateDotFromSlicingNode(SlicingNode* node) {
	string dot = "";

	if (node == nullptr) { return dot; }

	dot += dotLabel(node->id, "slicing\\n(start:end:step)");

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
