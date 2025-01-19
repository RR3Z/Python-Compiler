#pragma once

#include "./semantic.h"
#include "../nodes/nodes.h"
#include <iostream>
using namespace std;

/*
	Начало нашего дерева находится в FileNode - во что свернулся код на этапе грамматики.
	Что хранится в узлах, можно смотреть в файле nodes.h в папке nodes.

	1) Изменяем только "[]" и "=" на "[]=" (_COMPOUND_ASSIGN в transform для StmtNode)
	2) Добавляем модификатор доступа для элементов класса (функция defineAccessModifier)
	3) Проверка правильности присваивания (функция checkCompoundAssignForErrors)
	4) Подсчитываю количество передаваемых при вызове функции аргументов (_FUNCTION_CALL в transform для ExprNode)
*/

/* ========== FILE ========== */
void transformTree(FileNode* program) {
	if (program == nullptr || program->elementsList == nullptr) {
		//throw runtime_error("S: ERROR -> something went wrong in the previous stages (FileElementsListNode is unavailable)");
		return;
	}

	// FileNode состоит из множества FileElementNode (список FileElementsListNode)
	FileElementNode* programElement = program->elementsList->first;
	while (programElement != nullptr) {
		transform(programElement);
		programElement = programElement->next;
	}
}

void transform(FileElementNode* programElement) {
	if (programElement == nullptr) {
		throw runtime_error("S: ERROR->FileElementNode is unavailable");
	}
	
	switch (programElement->elementType)
	{
	case _CLASS_DEF:
		transform(programElement->classDef);
		break;
	case _FUNC_DEF:
		transform(programElement->funcDef);
		break;
	case _STMT:
		transform(programElement->stmt);
		break;
	}
}

/* ========== FUNCTION ========== */
void transform(FuncNode* funcDef) {
	if (funcDef == nullptr) {
		throw runtime_error("S: ERROR -> FuncNode is unavailable");
	}
	if (funcDef->identifier == nullptr) {
		throw runtime_error("S: ERROR -> FuncNode (id = " + to_string(funcDef->id) + ") has no identifier");
	}

	// В функции могут быть/не быть аргументы
	if (funcDef->args != nullptr) {
		transform(funcDef->args);
	}

	// В функции может быть/не быть тело (подразумевается кода внутри тела)
	if (funcDef->suite != nullptr) {
		transform(funcDef->suite);
	}
}

void transform(FuncArgsListNode* funcArgsList) {
	if (funcArgsList == nullptr) {
		throw runtime_error("S: ERROR -> FuncArgsListNode is unavailable");
	}

	// Неименованные аргументы
	ExprNode* funcArg = funcArgsList->exprList->first;
	while (funcArg != nullptr) {
		transform(funcArg);
		funcArg = funcArg->next;
	}

	// Именованные аргументы
	FuncArgNode* namedFuncArg = funcArgsList->first;
	while (namedFuncArg != nullptr) {
		transform(namedFuncArg);
		namedFuncArg = namedFuncArg->next;
	}
}

void transform(FuncArgNode* funcArg) {
	if (funcArg == nullptr) {
		throw runtime_error("S: ERROR -> FuncArgNode is unavailable");
	}
	
	switch (funcArg->funcArgType)
	{
		case _NAMED:
			transform(funcArg->assignStmt);
			break;
		case _VAR:
			transform(funcArg->var);
			break;
	}
}

/* ========== CLASS ========== */
void transform(ClassNode* classDef) {
	if (classDef == nullptr) {
		throw runtime_error("S: ERROR -> ClassNode is unavailable");
	}
	if (classDef->identifier == nullptr) {
		throw runtime_error("S: ERROR -> ClassNode (id = " + to_string(classDef->id) + ") has no identifier");
	}

	// В классе может быть/не быть тело (подразумевается кода внутри тела)
	if (classDef->suite != nullptr) {
		transform(classDef->suite);
	}

	// Имя родительского класса
	if (classDef->base != nullptr) {
		transform(classDef->base);
	}
}

void transform(ClassElementsListNode* classElementsList) {
	if (classElementsList == nullptr) {
		throw runtime_error("S: ERROR -> ClassElementsListNode is unavailable");
	}
	if (classElementsList->first == nullptr) {
		throw runtime_error("S: ERROR -> ClassElementsListNode (id = " + to_string(classElementsList->id) + ") first element in list is unavailable");
	}

	ClassElementNode* classElement = classElementsList->first;
	while (classElement != nullptr) {
		transform(classElement);
		classElement = classElement->next;
	}
}

void transform(ClassElementNode* classElement) {
	if (classElement == nullptr) {
		throw runtime_error("S: ERROR -> ClassElementNode is unavailable");
	}

	switch (classElement->elementType)
	{
		case _FUNCTION_DEF:
			if (classElement->funcDef != nullptr) {
				transform(classElement->funcDef);
				defineAccessModifier(classElement->funcDef);
			}
			break;
		case _STMT_NODE:
			if (classElement->stmt != nullptr) {
				transform(classElement->stmt);
				defineAccessModifier(classElement->stmt);
			}
			break;
	}
}

/* ========== STMTS ========== */
void transform(StmtsListNode* stmtsList) {
	if (stmtsList == nullptr) {
		throw runtime_error("S: ERROR -> StmtsListNode is unavailable");
		return;
	}
	if (stmtsList->first == nullptr) {
		throw runtime_error("S: ERROR -> StmtsListNode (id = " + to_string(stmtsList->id) + ") first element in list is unavailable");
		return;
	}
	
	StmtNode* stmtNode = stmtsList->first;
	while (stmtNode != nullptr) {
		transform(stmtNode);
		stmtNode = stmtNode->next;
	}
}

void transform(StmtNode* stmt) {
	if (stmt == nullptr) {
		throw runtime_error("S: ERROR->StmtNode is unavailable");
		return;
	}
	
	switch (stmt->stmtType) {
			// IF STMT
		case _IF:
			transform(stmt->expr); // condition
			transform(stmt->suite); // suite
			break;
		case _ELSE:
			transform(stmt->suite); // suite
			break;
		case _ELIF:
			transform(stmt->expr); // condition
			transform(stmt->suite); // suite
			break;
		case _COMPOUND_IF:
			transform(stmt->leftNode);	// ifStmt
			transform(stmt->rightNode);	// elseStmt
			transform(stmt->stmtsList);	// elifStmtsList
			break;

			// ASSIGN STMT
		case _ASSIGN:
			transform(stmt->leftExpr);
			transform(stmt->rightExpr);
			break;
		case _COMPOUND_ASSIGN: {
			StmtNode* target = stmt->stmtsList->first;
			ExprNode* value = stmt->list->first;

			while (target != nullptr) {
				if (target->list != nullptr && target->list->first->exprType == _LIST_ACCESS) {
					target->stmtType = _ASSIGN_AND_ACCESS;
					target->expr = target->list->last->right; // INDEX
					target->leftExpr = target->list->last->left; // ID
				}
				else {
					target->stmtType = _ASSIGN;
					target->leftExpr = target->list->last; // ID
				}
				target->rightExpr = value; // VALUE
				target->list = nullptr;

				transform(target);
			
				target = target->next;
			}
			stmt->list = nullptr;

			checkCompoundAssignForErrors(stmt);
			break;
		}
		case _ASSIGN_AND_ACCESS:
			transform(stmt->leftExpr);
			transform(stmt->rightExpr);
			transform(stmt->expr);
			break;

		case _MUL_ASSIGN:
			transform(stmt->leftExpr);
			transform(stmt->rightExpr);
			break;
		case _DIV_ASSIGN:
			transform(stmt->leftExpr);
			transform(stmt->rightExpr);
			break;
		case _MINUS_ASSIGN:
			transform(stmt->leftExpr);
			transform(stmt->rightExpr);
			break;
		case _PLUS_ASSIGN:
			transform(stmt->leftExpr);
			transform(stmt->rightExpr);
			break;

			// WHILE STMT
		case _WHILE:
			transform(stmt->expr); // condition
			transform(stmt->suite); // suite
			break;
		case _COMPOUND_WHILE:
			transform(stmt->leftNode);	// whileStmt
			transform(stmt->rightNode);	// elseStmt
			break;

			// FOR STMT
		case _FOR:
			transform(stmt->list);	// targetList из expr
			transform(stmt->expr);	// expr
			transform(stmt->suite);	// suite
			break;
		case _COMPOUND_FOR:
			transform(stmt->leftNode);	// forStmt
			transform(stmt->rightNode);	// elseStmt
			break;

			// RETURN STMT
		case _RETURN:
			transform(stmt->list); // exprList (возвращаемые значения)
			break;

			// TRY STMT
		case _TRY:
			transform(stmt->suite); // suite
			break;
		case _COMPOUND_TRY:
			transform(stmt->leftNode);	// elseStmt
			transform(stmt->rightNode);	// finnalyStmt
			transform(stmt->stmtsList); // exceptStmtsList
			transform(stmt->tryStmt);	// tryStmt
			break;
		case _EXCEPT:
			transform(stmt->expr); // что мы отлавливаем (expr)
			transform(stmt->suite); // suite
			break;
		case _IDENTIFIER_EXCEPT:
			transform(stmt->expr);		// что мы отлавливаем (expr)
			transform(stmt->identifier);  // собственное наименование того, что мы отлавливаем (expr)
			transform(stmt->suite); // suite
			break;
		case _FINALLY:
			transform(stmt->suite); // suite
			break;
		case _EXPR_STMT:
			transform(stmt->expr);
			break;
		case _EXPR_LIST_STMT:
			transform(stmt->list);
			break;
	}
}

/* ========== EXPRS ========== */

void transform(ExprListNode* exprsList) {
	if (exprsList == nullptr) {
		//cout << "S: ERROR -> ExprListNode is unavailable" << endl;
		return;
	}
	if (exprsList->first == nullptr) {
		//cout << "S: ERROR -> ExprListNode (id = " << exprsList->id << ") first element in list is unavailable" << endl;
		return;
	}

	ExprNode* exprNode = exprsList->first;
	while (exprNode != nullptr) {
		transform(exprNode);
		exprNode = exprNode->next;
	}
}

void transform(ExprNode* expr) {
	if (expr == nullptr) {
		cout << "S: ERROR -> ExprNode is unavailable" << endl;
		return;
	}
	
	switch (expr->exprType)
	{
		case _FUNCTION_CALL:
			expr->argsCount = 0;

			// Посчитать количество передаваемых аргументов
			if (expr->funcArgs != nullptr) {
				ExprNode* arg = expr->funcArgs->exprList->first;
				while (arg != nullptr) {
					expr->argsCount++;
					arg = arg->next;
				}
			}

			break;
	}
}


/* ========== ACCESS MODIFIER ========== */
/*
	Модификаторы доступа для элементов класса.

	1) Public
	Пояснение:
	Публичные свойства и методы доступны всем наследникам и создаются простым определением 
	их в теле класса без каких-либо особых префиксов. К ним можно обращаться как внутри объекта, так и снаружи.
	Как задается:
	Без _ в имени.

	2) Protected
	Пояснение:
	Это смесь между публичными и приватными.
	Подразумевается, что такие элементы доступны только внутри класса и подклассе, но не снаружи.
	По факту же, они доступны ото всюду (при обращении к ним).
	Как задается:
	Создается путем добавления одного подчеркивания (_) перед именем.

	3) Private
	Пояснение:
	Свойства и методы с приватным модификатором доступа доступны только внутри того класса, где они были определены.
	Как задается:
	В начале имени __.
*/

void defineAccessModifier(FuncNode* funcDef) {
	if (funcDef == nullptr) {
		throw runtime_error("S: ERROR->FuncNode is unavailable");
		return;
	}

	string funcIdentifier = funcDef->identifier->identifier;

	if (!funcIdentifier.empty()) {
		if (funcIdentifier.starts_with("__")) {
			funcDef->accessModifier = _PRIVATE;
		}
		else if (funcIdentifier.starts_with("_")) {
			funcDef->accessModifier = _PROTECTED;
		}
		else {
			funcDef->accessModifier = _PUBLIC;
		}
	}
}

void defineAccessModifier(StmtNode* stmt) {
	if (stmt == nullptr) {
		throw runtime_error("S: ERROR->StmtNode is unavailable");
		return;
	}

	StmtNode* stmtNode = stmt->stmtsList->first;

	while (stmtNode != nullptr) {
		if ((stmtNode->leftExpr->identifier).starts_with("__")) {
			stmtNode->accessModifier = _PRIVATE;
		}
		else if ((stmtNode->leftExpr->identifier).starts_with("_")) {
			stmtNode->accessModifier = _PROTECTED;
		}
		else {
			stmtNode->accessModifier = _PUBLIC;
		}

		stmtNode = stmtNode->next;
	}
}

/* ========== ERRORS ========== */
void checkCompoundAssignForErrors(StmtNode* stmt) {
	if (stmt == nullptr) {
		throw runtime_error("S: ERROR -> StmtNode is unavailable");
		return;
	}

	StmtNode* assignStmt = stmt->stmtsList->first;

	while (assignStmt != nullptr) {
		switch (assignStmt->leftExpr->exprType)
		{
			case _FALSE:
				// False = True
				throw runtime_error("S: Syntax Error -> cannot assign to False.");
				break;
			case _TRUE:
				// True = 15
				throw runtime_error("S: Syntax Error -> cannot assign to True.");
				break;
			case _INT_CONST:
				// 15 = True
				throw runtime_error("S: Syntax Error -> cannot assign to literal here.");
				break;
			case _FLOAT_CONST:
				// 15.0 = "a"
				throw runtime_error("S: Syntax Error -> cannot assign to literal here.");
				break;
			case _STRING_CONST:
				// "a" = 15.0
				throw runtime_error("S: Syntax Error -> cannot assign to literal here.");
				break;
			case _SLICING_LIST_ACCESS:
				// my_list[1:4] = 10
				throw runtime_error("S: Syntax Error -> must assign iterable to extended slice."); // TODO: RECHECK IT AFTER CODE GENERATION
				break;
			case _LIST_COMPREHENSION:
				//[x * 2 for x in my_list] = 5
				throw runtime_error("S: Syntax Error -> cannot assign to list comprehension.");
				break;
		}

		assignStmt = assignStmt->next;
	}
}
