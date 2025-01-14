#pragma once

#include "./semantic.h"
#include "../nodes/nodes.h"
#include <iostream>
using namespace std;

/*
	Начало нашего дерева находится в FileNode - во что свернулся код на этапе грамматики.
	Что хранится в узлах, можно смотреть в файле nodes.h в папке nodes.

	Изменяем только "[]" и "=" на "[]="
*/

/* ========== FILE ========== */
void transformTree(FileNode* program) {
	if (program == nullptr || program->elementsList == nullptr) {
		cout << "S: ERROR -> file is empty (FileNode is unavailable) or something went wrong in the previous stages (FileElementsListNode is unavailable)" << endl;
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
		cout << "S: ERROR -> FileElementNode is unavailable" << endl;
		return;
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
		cout << "S: ERROR -> FuncNode is unavailable" << endl;
		return;
	}
	if (funcDef->identifier == nullptr) {
		cout << "S: ERROR -> FuncNode (id = " << funcDef->id << ") has no identifier" << endl;
		return;
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
		cout << "S: ERROR -> FuncArgsListNode is unavailable" << endl;
		return;
	}

	// Для неименованных аргументов (ExprList из ExprNode*) ничего делать не надо

	FuncArgNode* namedFuncArg = funcArgsList->first;
	if (namedFuncArg != nullptr) {
		while (namedFuncArg != nullptr) {
			transform(namedFuncArg);
			namedFuncArg = namedFuncArg->next;
		}
	}
}

void transform(FuncArgNode* funcArg) {
	if (funcArg == nullptr) {
		cout << "S: ERROR -> FuncArgNode is unavailable" << endl;
		return;
	}
	
	switch (funcArg->funcArgType)
	{
	case _NAMED:
		transform(funcArg->assignStmt);
		break;
	case _VAR:
		// transform(funcArg->var); // expr, который не надо никак преобразовывать
		break;
	}
}

/* ========== CLASS ========== */
void transform(ClassNode* classDef) {
	if (classDef == nullptr) {
		cout << "S: ERROR -> FuncNode is unavailable" << endl;
		return;
	}
	if (classDef->identifier == nullptr) {
		cout << "S: ERROR -> FuncNode (id = " << classDef->id << ") has no identifier" << endl;
		return;
	}

	// В классе может быть/не быть тело (подразумевается кода внутри тела)
	if (classDef->suite != nullptr) {
		transform(classDef->suite);
	}

	// Проверять/преобразовать родительский класс (base) нам не надо
}

void transform(ClassElementsListNode* classElementsList) {
	if (classElementsList == nullptr) {
		cout << "S: ERROR -> ClassElementsListNode is unavailable" << endl;
		return;
	}
	if (classElementsList->first == nullptr) {
		cout << "S: ERROR -> ClassElementsListNode (id = " << classElementsList->id << ") first element in list is unavailable" << endl;
		return;
	}

	ClassElementNode* classElement = classElementsList->first;
	while (classElement != nullptr) {
		transform(classElement);
		classElement = classElement->next;
	}
}

void transform(ClassElementNode* classElement) {
	if (classElement == nullptr) {
		cout << "S: ERROR -> ClassElementNode is unavailable" << endl;
		return;
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
		cout << "S: ERROR -> StmtsListNode is unavailable" << endl;
		return;
	}
	if (stmtsList->first == nullptr) {
		cout << "S: ERROR -> StmtsListNode (id = " << stmtsList->id << ") first element in list is unavailable" << endl;
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
		cout << "S: ERROR -> StmtNode is unavailable" << endl;
		return;
	}

	// EXPR STMT
	/*
		А надо ли что-то изменять в expr?
		Егор: нет, не надо. AssignOp (как expr) работает иначе чем Assign (как Stmt).

		if a[0][0]:=5:
		SyntaxError: cannot use assignment expressions with subscript
		НЕЛЬЗЯ таким образом задавать значение элементу списка (массива). Соответственно, не надо ничего изменять.
	*/
	
	switch (stmt->stmtType) {
		// IF STMT
	case _IF:
		//transform(stmt->expr); // condition (expr, который нам не надо изменять)
		transform(stmt->suite); // suite
		break;
	case _ELSE:
		transform(stmt->suite); // suite
		break;
	case _ELIF:
		//transform(stmt->expr); // condition (expr, который нам не надо изменять)
		transform(stmt->suite); // suite
		break;
	case _COMPOUND_IF:
		transform(stmt->leftNode);	// ifStmt
		transform(stmt->rightNode);	// elseStmt
		transform(stmt->stmtsList);	// elifStmtsList
		break;

		// ASSIGN STMT
	case _ASSIGN:
		// НИЧЕГО НЕ НАДО ДЕЛАТЬ
		break;
	case _COMPOUND_ASSIGN: {
		// TODO: Проверка на ошибки

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
			
			target = target->next;
		}

		break;
	}

	case _MUL_ASSIGN:
		// НИЧЕГО НЕ НАДО ДЕЛАТЬ
		break;
	case _DIV_ASSIGN:
		// НИЧЕГО НЕ НАДО ДЕЛАТЬ
		break;
	case _MINUS_ASSIGN:
		// НИЧЕГО НЕ НАДО ДЕЛАТЬ
		break;
	case _PLUS_ASSIGN:
		// НИЧЕГО НЕ НАДО ДЕЛАТЬ
		break;

		// WHILE STMT
	case _WHILE:
		//transform(stmt->expr); // condition (expr, который нам не надо изменять)
		transform(stmt->suite); // suite
		break;
	case _COMPOUND_WHILE:
		transform(stmt->leftNode);	// whileStmt
		transform(stmt->rightNode);	// elseStmt
		break;

		// FOR STMT
	case _FOR:
		//transform(stmt->list);	// targetList из expr, который нам не надо изменять
		//transform(stmt->expr);	// expr, который нам не надо изменять
		transform(stmt->suite);	// suite
		break;
	case _COMPOUND_FOR:
		transform(stmt->leftNode);	// forStmt
		transform(stmt->rightNode);	// elseStmt
		break;

		// RETURN STMT
	case _RETURN:
		//transform(stmt->list); // exprList (возвращаемые значения, т.е. expr, которые нам не надо изменять)
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
		//transform(stmt->expr); // что мы отлавливаем (expr, который нам не надо изменять)
		transform(stmt->suite); // suite
		break;
	case _IDENTIFIER_EXCEPT:
		//transform(stmt->expr);		// что мы отлавливаем (expr, который нам не надо изменять)
		//transform(stmt->identifier);  // собственное наименование того, что мы отлавливаем (expr, который нам не надо изменять)
		transform(stmt->suite); // suite
		break;
	case _FINALLY:
		transform(stmt->suite); // suite
		break;
	}
}

/* ========== EXPRS ========== */
/*
void transform(ExprListNode* exprsList) {
	if (exprsList == nullptr) {
		cout << "S: ERROR -> ExprListNode is unavailable" << endl;
		return;
	}
	if (exprsList->first == nullptr) {
		cout << "S: ERROR -> ExprListNode (id = " << exprsList->id << ") first element in list is unavailable" << endl;
		return;
	}

	//ExprNode* exprNode = exprsList->first;
	//while (exprNode != nullptr) {
	//	transform(exprNode);
	//	exprNode = exprNode->next;
	//}
}

void transform(ExprNode* expr) {
	if (expr == nullptr) {
		cout << "S: ERROR -> ExprNode is unavailable" << endl;
		return;
	}
	
	//switch (expr->exprType)
	//{
	//case _ASSIGN_OP:
	//	break;
	//}
}
*/

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
		cout << "S: ERROR -> FuncNode is unavailable" << endl;
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
		cout << "S: ERROR -> StmtNode is unavailable" << endl;
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