#pragma once
#include "./semantic.h"
#include <iostream>
using namespace std;

std::map<std::string, Class*> classesList;

// ========= Функции для начала заполнения таблиц =========

// Стартовая функция
void fillTables(FileNode* program) {
	Class* entryClass = new Class();
	entryClass->name = "__PROGRAM__";
	classesList[entryClass->name] = entryClass;

	entryClass->pushOrFindConstant(*Constant::UTF8("Code"));
	entryClass->number = entryClass->pushOrFindConstant(*Constant::Class(entryClass->pushOrFindConstant(*Constant::UTF8(entryClass->name))));

	// Добавление функций из RTL
	addRTLToClass(entryClass);

	int parentClassName = entryClass->pushOrFindConstant(*Constant::UTF8("java/lang/Object"));
	entryClass->parentNumber = entryClass->pushOrFindConstant(*Constant::Class(parentClassName));

	// Создание main как точки входа в программу
	Method* mainMethod = new Method();
	mainMethod->name = "main";
	mainMethod->descriptor = "([Ljava/lang/String;)V";
	mainMethod->nameNumber = entryClass->pushOrFindConstant(*Constant::UTF8(mainMethod->name));
	mainMethod->descriptorNumber = entryClass->pushOrFindConstant(*Constant::UTF8("([Ljava/lang/String;)V"));
	mainMethod->number = entryClass->pushOrFindMethodRef(entryClass->name, mainMethod->name, "([Ljava/lang/String;)V");
	mainMethod->localVars.push_back("args");
	mainMethod->suite = nullptr;
	entryClass->methods[mainMethod->name] = mainMethod;

	// Разбор всех элементов программы
	if (program != nullptr && program->elementsList != nullptr) {
		FileElementNode* programElement = program->elementsList->first;

		while (programElement != nullptr) {
			switch (programElement->elementType)
			{
				case _CLASS_DEF:
					//fillTable(programElement->classDef);
					break;
				case _FUNC_DEF:
					fillMethodTable(entryClass, programElement->funcDef);
					break;
				case _STMT:
					if (programElement->stmt->stmtType == _COMPOUND_ASSIGN && programElement->stmt->stmtsList != nullptr) {
						fillFieldTable(entryClass, programElement->stmt->stmtsList);
					}

					if (mainMethod->suite == nullptr) mainMethod->suite = new StmtsListNode();

					if (mainMethod->suite->first != nullptr) {
						mainMethod->suite->last->next = programElement->stmt;
						mainMethod->suite->last = programElement->stmt;
					}
					else {
						mainMethod->suite->first = programElement->stmt;
						mainMethod->suite->last = programElement->stmt;
					}

					fillMethodTable(entryClass, mainMethod, programElement->stmt);
					
					break;
			}

			programElement = programElement->next;
		}
	}
}

// TODO: классы
void fillTables(ClassNode* classDef) {
	// Создание нового класса
	Class* newClass = new Class();
	// Добавление класса в глобальную таблицу
	newClass->name = classDef->identifier->identifier;
	classesList[newClass->name] = newClass;

	// Добавление констант в класс
	newClass->pushOrFindConstant(*Constant::UTF8("Code"));
	newClass->number = newClass->pushOrFindConstant(*Constant::Class(newClass->pushOrFindConstant(*Constant::UTF8(newClass->name))));

	// Добавление RTL в класс
	addRTLToClass(newClass);

	// Родительский класс
	if (classDef->base != nullptr) {
		if (classesList.find(classDef->base->stringVal) != classesList.end()) {
			newClass->parent = classesList[classDef->base->stringVal];
		}
		else {
			throw runtime_error("S: ERROR -> parent class " + classDef->base->stringVal + " for class " + classDef->identifier->stringVal + " not found");
		}
	}

	// По идее не может быть пустым (всегда отлавливается на уровне парсера)
	if (classDef->suite != nullptr && classDef->suite->first != nullptr) {
		ClassElementNode* classElement = classDef->suite->first;
		while (classElement != nullptr) {
			switch (classElement->elementType)
			{
				case _FUNCTION_DEF:
					//fillMethodTable(newClass, classElement->funcDef);
					break;
				case _STMT_NODE:
					break;
			}

			classElement = classElement->next;
		}
	}

	// TODO: Конструктор по умолчанию (от java/lang/Object)
	//newClass->pushOrFindConstant(*Constant::UTF8("<init>"));
}

// ========= Заполнение таблиц методов =========

// TODO: именованные аргументы + дескриптор
void fillMethodTable(Class* clazz, FuncNode* funcDef) {
	checkMethodNameForErrors(funcDef);

	Method* method = new Method();
	method->name = funcDef->identifier->identifier;
	method->nameNumber = clazz->pushOrFindConstant(*Constant::UTF8(method->name));
	// Модификатор доступа - AccessModifier -> AccessFlag
	switch (funcDef->accessModifier)
	{
		case _PRIVATE:
			method->accessModifier = PRIVATE;
			break;
		case _PROTECTED:
			method->accessModifier = PROTECTED;
			break;
		case _UNKNOWN: // Если нет модификатора доступа, то по дефолту public.
		case _PUBLIC:
			method->accessModifier = PUBLIC;
			break;
	}

	// Аргументы метода
	int paramsCounter = 0;
	if (funcDef->args != nullptr) {
		// Обычные аргументы (a,b,c,...)
		ExprNode* arg = funcDef->args->exprList->first;
		while (arg != nullptr) {
			paramsCounter++;
			method->localVars.push_back(arg->identifier);

			arg = arg->next;
		}

		// TODO: Именованные аргументы (d=1,e=2,...)
		/*
		FuncArgNode* namedArg = funcDef->args->namedArgsList->first;
		while (namedArg != nullptr) {
			paramsCounter++;

			//TODO: записать значения и константы в таблицы
			method->localVars.push_back(namedArg->assignStmt->leftExpr->identifier);
			// TODO: записать в expr node индекс аругмента в методе ???

			namedArg = namedArg->next;
		}
		*/
	}
	method->paramsCount = paramsCounter;

	// Составление дескриптора
	string methodReturnType = defineMethodReturnType(method);
	string methodDescriptor = generateMethodDescriptor(paramsCounter, methodReturnType);
	method->descriptor = methodDescriptor;
	method->descriptorNumber = clazz->pushOrFindConstant(*Constant::UTF8(methodDescriptor));

	method->number = clazz->pushOrFindMethodRef(clazz->name, method->name, methodDescriptor);
	funcDef->idSemantic = method->number;

	// Ссылки на super класс __BASE__
	method->baseClassNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
	method->baseConstructorNumber = clazz->pushOrFindMethodRef("__BASE__", "<init>", "()V");
	clazz->methods[method->name] = method;

	// Генерация таблиц для тела функции
	method->suite = funcDef->suite;
	fillMethodTable(clazz, method, method->suite);
}

void fillMethodTable(Class* clazz, Method* method, StmtsListNode* stmts) {
	if (stmts != nullptr) {
		StmtNode* stmt = stmts->first;
		while (stmt != nullptr) {
			fillMethodTable(clazz, method, stmt);

			if (stmt->stmtType == StmtType::_RETURN) {
				stmt->next = nullptr;
				if(method->suite == stmts) method->suite->last = stmt;
			}
			
			stmt = stmt->next;
		}
	}
}

// TODO
void fillMethodTable(Class* clazz, Method* method, StmtNode* stmt) {
	switch (stmt->stmtType)
	{
		case _ASSIGN:
			if (!stmt->leftExpr->identifier.empty()) {
				if (find(method->localVars.begin(), method->localVars.end(), stmt->leftExpr->identifier) != method->localVars.end()) {
					stmt->leftExpr->paramLocalVarNum = findElementIndexInVector(method->localVars, stmt->leftExpr->identifier);
				}

				// Если переменная не является полем класса, добавляем ее в качестве локальной переменной текущей функции
				if (clazz->fields.find(stmt->leftExpr->identifier) == clazz->fields.end()) {
					method->localVars.push_back(stmt->leftExpr->identifier);
					stmt->leftExpr->paramLocalVarNum = findElementIndexInVector(method->localVars, stmt->leftExpr->identifier);
				}
				else {
					stmt->number = clazz->fields[stmt->leftExpr->identifier]->number;
				}
			}

			fillMethodTable(clazz, method, stmt->leftExpr);
			fillMethodTable(clazz, method, stmt->rightExpr);
			break;
		case _COMPOUND_ASSIGN:
			if (stmt->stmtsList != nullptr) {
				StmtNode* assignStmt = stmt->stmtsList->first;
				while (assignStmt != nullptr) {
					fillMethodTable(clazz, method, assignStmt);
					assignStmt = assignStmt->next;
				}
			}
			break;

		case _IF:
		case _ELIF:
			// condition
			if (stmt->expr != nullptr) {
				checkConditionForErrors(clazz, method, stmt->expr, "IF");
				fillMethodTable(clazz, method, stmt->expr);
			}
			if (stmt->suite != nullptr) fillMethodTable(clazz, method, stmt->suite); // suite
			stmt->boolFieldMethodRef = clazz->pushOrFindFieldRef("__BASE__", "__bVal", "Z");
			break;
		case _ELSE:
			if (stmt->suite != nullptr) fillMethodTable(clazz, method, stmt->suite); // suite
			break;
		case _COMPOUND_IF:
			fillMethodTable(clazz, method, stmt->leftNode);
			if (stmt->rightNode != nullptr) fillMethodTable(clazz, method, stmt->rightNode);
			if (stmt->stmtsList != nullptr) fillMethodTable(clazz, method, stmt->stmtsList);
			stmt->boolFieldMethodRef = clazz->pushOrFindFieldRef("__BASE__", "__bVal", "Z");
			break;

		case _FOR:
			// TODO
			break;
		case _WHILE:
			// condition
			if (stmt->expr != nullptr) {
				checkConditionForErrors(clazz, method, stmt->expr, "WHILE");
				fillMethodTable(clazz, method, stmt->expr);
			}

			// suite
			if (stmt->suite != nullptr) fillMethodTable(clazz, method, stmt->suite);

			stmt->boolFieldMethodRef = clazz->pushOrFindFieldRef("__BASE__", "__bVal", "Z");
			break;

		case _RETURN:
			// TODO: множественное возвращение
			if (stmt->list != nullptr) {
				ExprNode* expr = stmt->list->first;

				checkReturnValue(clazz, method, expr);
				fillMethodTable(clazz, method, expr);
			}
			break;
		case _EXPR_STMT:
			fillMethodTable(clazz, method, stmt->expr);
			break;
		case _EXPR_LIST_STMT:
			break;
		case _ASSIGN_AND_ACCESS:
			fillMethodTable(clazz, method, stmt->leftExpr);
			fillMethodTable(clazz, method, stmt->rightExpr);
			fillMethodTable(clazz, method, stmt->expr);
			stmt->number = clazz->pushOrFindMethodRef("__BASE__", "__member_access_assign__", "(L__BASE__;L__BASE__;)L__BASE__;");
			break;
	}
}

// TODO
void fillMethodTable(Class* clazz, Method* method, ExprNode* expr) {
	if (expr == nullptr) return;
	
	switch (expr->exprType)
	{
		case _INT_CONST:
			expr->valueNumber = clazz->pushOrFindConstant(*Constant::Integer(expr->intVal));
			expr->classNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "<init>", "(I)V");
			break;
		case _TRUE:
		case _FALSE:
			expr->valueNumber = clazz->pushOrFindConstant(*Constant::Integer(expr->boolVal));
			expr->classNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "<init>", "(Z)V");
			break;
		case _STRING_CONST:
			expr->valueNumber = clazz->pushOrFindConstant(*Constant::String(clazz->pushOrFindConstant(*Constant::UTF8(expr->stringVal))));
			expr->classNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "<init>", "(Ljava/lang/String;)V");
			break;
		case _FLOAT_CONST:
			expr->valueNumber = clazz->pushOrFindConstant(*Constant::Float(expr->floatVal));
			expr->classNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "<init>", "(F)V");
			break;
		case _IDENTIFIER:
			expr->valueNumber = clazz->pushOrFindConstant(*Constant::String(clazz->pushOrFindConstant(*Constant::UTF8(expr->identifier))));
			expr->classNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "<init>", "(Ljava/lang/String;)V");
			expr->paramLocalVarNum = findElementIndexInVector(method->localVars, expr->identifier);
			break;
		case _PLUS:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__add__", "(L__BASE__;)L__BASE__;");
			break;
		case _MINUS:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__sub__", "(L__BASE__;)L__BASE__;");
			break;
		case _MUL:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__mul__", "(L__BASE__;)L__BASE__;");
			break;
		case _DIV:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__div__", "(L__BASE__;)L__BASE__;");
			break;
		case _EQUAL:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__eql__", "(L__BASE__;)L__BASE__;");
			break;
		case _NOT_EQUAL:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__not_eql__", "(L__BASE__;)L__BASE__;");
			break;
		case _LESS:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__les__", "(L__BASE__;)L__BASE__;");
			break;
		case _LESS_EQUAL:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__les_or_eql__", "(L__BASE__;)L__BASE__;");
			break;
		case _GREAT:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__greater__", "(L__BASE__;)L__BASE__;");
			break;
		case _GREAT_EQUAL:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__greater_or_eql__", "(L__BASE__;)L__BASE__;");
			break;
		case _U_PLUS:
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__unary_plus__", "()L__BASE__;");
			break;
		case _U_MINUS:
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__unary_minus__", "()L__BASE__;");
			break;
		case _BRACKETS:
			fillMethodTable(clazz, method, expr->left);
			break;
		case _FUNCTION_CALL:
			// Проверка на существование метода
			isMethodExists(clazz, method, expr);
			
			if (expr->funcArgs != nullptr) {
				checkFunctionCallParams(clazz, method, expr);

				ExprNode* arg = expr->funcArgs->exprList->first;
				while (arg != nullptr) {
					fillMethodTable(clazz, method, arg);
					arg = arg->next;
				}
			}

			// Определяем функцию (RTL или обычная)
			expr->number = defineMethodRefByExprNode(clazz, method, expr);
			
			break;
		case _LIST_CREATION:
			if (expr->list != nullptr) {
				ExprNode* listElement = expr->list->first;
				while (listElement != nullptr) {
					fillMethodTable(clazz, method, listElement);
					listElement = listElement->next;
				}
			}
			expr->arrayListClassNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("java/util/ArrayList"))));
			expr->arrayListConstructorMethodRef = clazz->pushOrFindMethodRef("java/util/ArrayList", "<init>", "(Ljava/util/Collection;)V");
			expr->listConstructorMethodRef = clazz->pushOrFindMethodRef("java/util/Arrays", "asList", "([Ljava/lang/Object;)Ljava/util/List;");
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "<init>", "(Ljava/util/ArrayList;)V");
			expr->classNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
			break;
		case _LIST_ACCESS:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__member_access__", "(L__BASE__;)L__BASE__;");
			break;
		case _AND_LOGIC:
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__logical_and__", "(L__BASE__;)L__BASE__;");
			expr->classNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
			expr->booleanFieldRef = clazz->pushOrFindFieldRef("__BASE__", "__bVal", "Z");
			expr->booleanInitMethodRef = clazz->pushOrFindMethodRef("__BASE__", "<init>", "(Z)V");
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			break;
		case _OR_LOGIC:
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__logical_or__", "(L__BASE__;)L__BASE__;");
			expr->classNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
			expr->booleanFieldRef = clazz->pushOrFindFieldRef("__BASE__", "__bVal", "Z");
			expr->booleanInitMethodRef = clazz->pushOrFindMethodRef("__BASE__", "<init>", "(Z)V");
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			break;
		case _NOT:
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__not__", ("()L__BASE__;"));
			fillMethodTable(clazz, method, expr->left);
			break;
	}
}

// ========= Заполнение таблиц полей =========

void fillFieldTable(Class* clazz, StmtsListNode* compoundAssign) {
	if (compoundAssign->first != nullptr) {
		StmtNode* assignStmt = compoundAssign->first;
		while (assignStmt != nullptr) {
			fillFieldTable(clazz, assignStmt);
			assignStmt = assignStmt->next;
		}
	}
}

void fillFieldTable(Class* clazz, StmtNode* assignStmt) {
	Field* field = new Field();

	// Имя
	field->name = assignStmt->leftExpr->identifier;
	field->nameNumber = clazz->pushOrFindConstant(*Constant::UTF8(field->name));
	field->nameNode = assignStmt->leftExpr;

	// Модификатор доступа - AccessModifier -> AccessFlag
	switch (assignStmt->accessModifier) {
	case _PRIVATE:
		field->accessModifier = PRIVATE;
		break;
	case _PROTECTED:
		field->accessModifier = PROTECTED;
		break;
	case _UNKNOWN: // Если нет модификатора доступа, то по умолчанию public.
	case _PUBLIC:
		field->accessModifier = PUBLIC;
		break;
	}

	// Дескриптор
	field->descriptor = "L__BASE__;";
	field->descriptorNumber = clazz->pushOrFindConstant(*Constant::UTF8(field->descriptor));

	// FieldRef
	field->number = clazz->pushOrFindFieldRef(clazz->name, field->name, field->descriptor);
	assignStmt->number = field->number;

	clazz->fields[field->name] = field;
}

// ========= RTL =========

void addRTLToClass(Class* clazz) {
	// Загрузка базового класса
	clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"));
	clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));

	// Методы для вывода данных в консоль
	clazz->pushOrFindMethodRef("__BASE__", "print", "(L__BASE__;)V");
	clazz->pushOrFindMethodRef("__BASE__", "print", "()V");

	// Методы для получения данных из консоли
	clazz->pushOrFindMethodRef("__BASE__", "input", "(L__BASE__;)L__BASE__;");
	clazz->pushOrFindMethodRef("__BASE__", "input", "()L__BASE__;");
}

// ========= Функции проверок =========

void checkFunctionCallParams(Class* clazz, Method* method, ExprNode* expr) {
	if (expr != nullptr && expr->exprType == _FUNCTION_CALL && expr->funcArgs != nullptr) {
		// 1) Проверка на существование передаваемых аргументов
		ExprNode* arg = expr->funcArgs->exprList->first;
		while (arg != nullptr) {
			if (find(method->localVars.begin(), method->localVars.end(), arg->identifier) == method->localVars.end() && clazz->fields.find(arg->identifier) == clazz->fields.end() && arg->exprType == _IDENTIFIER) {
				throw runtime_error("S: ERROR -> variable \"" + arg->identifier + "\" is not defined. Function call \"" + expr->left->identifier + "\".");
			}

			arg = arg->next;
		}

		// 2) Сравнение количества передаваемых аргументов с количеством требуемых
		// Для RTL функций
		if (checkRTLFunctionCallParams(expr)) return;
		// Для собственной функции
		if (clazz->methods.find(expr->left->identifier) != clazz->methods.end() && expr->argsCount != clazz->methods[expr->left->identifier]->paramsCount) {
			throw runtime_error("S: ERROR -> function \"" + expr->left->identifier + "\" takes " + to_string(clazz->methods[expr->left->identifier]->paramsCount) +
				" arguments but " + to_string(expr->argsCount) + " was given");
		}
	}
}

bool checkRTLFunctionCallParams(ExprNode* expr) {
	if (expr->left->identifier == "print") {
		if (expr->argsCount > 1) {
			throw runtime_error("S: ERROR -> function \"" + expr->left->identifier + "\" takes 1 or 0 arguments but " + to_string(expr->argsCount) + " was given");
		}
		return true;
	}

	if (expr->left->identifier == "input") {
		if (expr->argsCount > 1) {
			throw runtime_error("S: ERROR -> function \"" + expr->left->identifier + "\" takes 1 or 0 arguments but " + to_string(expr->argsCount) + " was given");
		}
		return true;
	}

	return false;
}

void isMethodExists(Class* clazz, Method* method, ExprNode* functionCall) {
	if (functionCall != nullptr && functionCall->exprType == _FUNCTION_CALL) {
		if (clazz->methods.find(functionCall->left->identifier) == clazz->methods.end()) {
			if (isRTLMethodExists(clazz, functionCall) || method->name == functionCall->left->identifier) return;
			throw runtime_error("S: ERROR -> trying call unknown function \"" + functionCall->left->identifier + "\"");
		}
	}
} 

bool isRTLMethodExists(Class* clazz, ExprNode* functionCall) {
	if (functionCall->left->identifier == "print") {
		int printFirstMethodRefNumber = clazz->findMethodRef("__BASE__", "print", "(L__BASE__;)V");
		int printSecondMethodRefNumber = clazz->findMethodRef("__BASE__", "print", "()V");
		if (printFirstMethodRefNumber == -1 && printSecondMethodRefNumber == -1) {
			throw runtime_error("S: ERROR -> trying call unknown function " + functionCall->left->identifier);
		}
		return true;
	}

	if (functionCall->left->identifier == "input") {
		int inputFirstMethodRefNumber = clazz->findMethodRef("__BASE__", "input", "(L__BASE__;)L__BASE__;");
		int inputSecondMethodRefNumber = clazz->findMethodRef("__BASE__", "input", "()L__BASE__;");
		if (inputFirstMethodRefNumber == -1 && inputSecondMethodRefNumber == -1) {
			throw runtime_error("S: ERROR -> trying call unknown function " + functionCall->left->identifier);
		}
		return true;
	}

	return false;
}

void checkReturnValue(Class* clazz, Method* method, ExprNode* expr) {
	if (expr != nullptr && expr->exprType == _IDENTIFIER) {
		// Method local vars
		if (find(method->localVars.begin(), method->localVars.end(), expr->identifier) != method->localVars.end()) return;
		// Fields
		if (clazz->fields.find(expr->identifier) != clazz->fields.end()) return;

		throw runtime_error("S: ERROR -> return value \"" + expr->identifier + "\" is not defined. Function name \"" + method->name + "\".");
	}
}

void checkMethodNameForErrors(FuncNode* funcDef) {
	if (funcDef != nullptr) {
		if (funcDef->identifier->identifier == "print") {
			throw runtime_error("S: ERROR -> Changes to the signature of the \"print\" function!");
		}

		if (funcDef->identifier->identifier == "input") {
			throw runtime_error("S: ERROR -> Changes to the signature of the \"input\" function!");
		}

		if (funcDef->identifier->identifier == "main") {
			throw runtime_error("S: ERROR -> Changes to the signature of the \"main\" function!");
		}
	}
}

void checkConditionForErrors(Class* clazz, Method* method, ExprNode* condition, string stmtType) {
	if (condition == nullptr) throw runtime_error("S: ERROR -> No condition for IF.");

	ExprNode* cond = condition;
	if (condition->exprType == _BRACKETS) cond = condition->left;

	switch (condition->exprType)
	{
		case _IDENTIFIER:
			if (!cond->identifier.empty() &&
				find(method->localVars.begin(), method->localVars.end(), cond->identifier) == method->localVars.end() &&
				clazz->fields.find(cond->identifier) == clazz->fields.end())
			{
				throw runtime_error("S: ERROR -> Unknown variable \"" + cond->identifier + "\" in " + stmtType + " condition");
			}
			break;
		case _EQUAL:
		case _GREAT_EQUAL:
		case _GREAT:
		case _LESS:
		case _LESS_EQUAL:
			if (cond->left != nullptr &&
				find(method->localVars.begin(), method->localVars.end(), cond->left->identifier) == method->localVars.end() &&
				clazz->fields.find(cond->left->identifier) == clazz->fields.end())
			{
				throw runtime_error("S: ERROR -> Unknown variable \"" + cond->left->identifier + "\" in " + stmtType + " condition");
			}
			break;
	}
}

// ========= Вспомогательные функции =========

// TODO: в качестве параметров могут передаваться и другие классы, для которых будет другой дескриптор
string generateMethodDescriptor(int paramsNumber, string returnValueDescriptor) {
	string descriptor = "(";
	
	// Заполнение параметров
	for (int i = 0; i < paramsNumber; i++) {
		descriptor += "L__BASE__;";
	}
	// Возвращаемое значение
	descriptor += ")" + returnValueDescriptor;

	return descriptor;
}

// TODO: реализовать множественное возвращение (сейчас можно вернуть только один элемент)
string defineMethodReturnType(Method* method) {
	vector<ExprNode*> returnValues = {};

	if (method->suite == nullptr) return "V";

	StmtNode* suiteStmt = method->suite->first;
	while (suiteStmt != nullptr) {
		switch (suiteStmt->stmtType)
		{
			case _RETURN:
				if(suiteStmt->list != nullptr) return "L__BASE__;";
				return "V";
		}

		suiteStmt = suiteStmt->next;
	}

	return "V";
}

int findElementIndexInVector(vector<string> vec, string element) {
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i] == element) {
			return i;
		}
	}

	return -1;
}

int defineMethodRefByExprNode(Class* clazz, Method* method, ExprNode* expr) {
	switch (expr->exprType)
	{
		case _FUNCTION_CALL:
			if (expr->left->identifier == "print") {
				if (expr->funcArgs != nullptr) {
					if (expr->funcArgs->exprList->first != expr->funcArgs->exprList->last) throw runtime_error("S: ERROR -> Wrong amount of args in function call with name: " + expr->left->identifier);
					return clazz->pushOrFindMethodRef("__BASE__", "print", "(L__BASE__;)V");
				}
				else return clazz->pushOrFindMethodRef("__BASE__", "print", "()V");
			}
			else if (expr->left->identifier == "input") {
				if (expr->funcArgs != nullptr) {
					if (expr->funcArgs->exprList->first != expr->funcArgs->exprList->last) throw runtime_error("S: ERROR -> Wrong amount of args in function call with name: " + expr->left->identifier);
					return clazz->pushOrFindMethodRef("__BASE__", "input", "(L__BASE__;)L__BASE__;");
				}
				else return clazz->pushOrFindMethodRef("__BASE__", "input", "()L__BASE__;");
			}
			else return clazz->pushOrFindMethodRef(clazz->name, expr->left->identifier, clazz->methods[expr->left->identifier]->descriptor);
	}

	throw runtime_error("UNSUPPORTED TYPE");
}
