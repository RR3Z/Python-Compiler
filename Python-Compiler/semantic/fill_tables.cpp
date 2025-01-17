#pragma once
#include "./semantic.h"
#include <iostream>
using namespace std;

// Самая верхоуровневая таблица классов
std::map<std::string, Class*> classesList;

// Заполнение таблиц (стартовая функция)
void fillTable(FileNode* program) {
	// Создание класса, как точки входа в программу
	Class* entryClass = new Class();
	entryClass->name = "__PROGRAM__";

	// Добавление констант класса
	entryClass->pushOrFindConstant(*Constant::UTF8("Code")); // По идее добавляется, так как у нас будет конструктор по умолчанию
	entryClass->number = entryClass->pushOrFindConstant(*Constant::Class(entryClass->pushOrFindConstant(*Constant::UTF8(entryClass->name))));

	// Добавление родительского класса
	int parentClassName = entryClass->pushOrFindConstant(*Constant::UTF8("java/lang/Object"));
	entryClass->parentNumber = entryClass->pushOrFindConstant(*Constant::Class(parentClassName));

	// Создание функции, как точки входа в программу
	Method* mainMethod = new Method();
	mainMethod->name = "main";
	// Добавление констант метода
	mainMethod->nameNumber = entryClass->pushOrFindConstant(*Constant::UTF8(mainMethod->name));
	mainMethod->descriptorNumber = entryClass->pushOrFindConstant(*Constant::UTF8("([Ljava/lang/String;)V"));
	mainMethod->number = entryClass->pushOrFindMethodRef(entryClass->name, mainMethod->name, "([Ljava/lang/String;)V");
	// Добавление локальной переменной (в собственную таблицу локальных переменных)
	mainMethod->localVars.push_back("args");
	// Тело метода (изначально пустое)
	mainMethod->suite = nullptr;

	// Разбор кода программы
	if (program != nullptr && program->elementsList != nullptr) {
		FileElementNode* programElement = program->elementsList->first;

		while (programElement != nullptr) {
			switch (programElement->elementType)
			{
				case _CLASS_DEF:
					//fillTable(programElement->classDef);
					break;
				case _FUNC_DEF:
					//fillTable(entryClass, programElement->funcDef);
					break;
				case _STMT:
					if (mainMethod->suite == nullptr) mainMethod->suite = new StmtsListNode();

					if (mainMethod->suite->first != nullptr) {
						mainMethod->suite->last = programElement->stmt; 
						mainMethod->suite->first->next = programElement->stmt;
					}
					else { 
						mainMethod->suite->first = programElement->stmt; 
						mainMethod->suite->last = programElement->stmt; 
					}

					fillTable(entryClass, mainMethod, programElement->stmt);

					break;
			}

			// Переходим к следующему элементу
			programElement = programElement->next;
		}
	}

	// Добавление класса в глобальную таблицу
	classesList[entryClass->name] = entryClass;
	// Добавление main в таблицу методов entryClass
	entryClass->methods[mainMethod->name] = mainMethod;
}

// TODO: классы
void fillTable(ClassNode* classDef) {
	// Создание нового класса
	Class* newClass = new Class();
	// Добавление класса в глобальную таблицу
	newClass->name = classDef->identifier->stringVal;
	classesList[newClass->name] = newClass;
	// Добавление констант в класс
	newClass->pushOrFindConstant(*Constant::UTF8("Code"));
	newClass->pushOrFindConstant(*Constant::Class(newClass->pushOrFindConstant(*Constant::UTF8(newClass->name))));

	// Родительский класс (одиночное наследование)
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
					fillTable(newClass, classElement->funcDef);
					break;
				case _STMT_NODE:
					// TODO: если это явное присвоение значения -> добавляем в таблицу полей
					//if (classElement->stmt->stmtType == _ASSIGN || classElement->stmt->stmtType == _COMPOUND_ASSIGN) fillFieldTable(newClass, classElement->stmt);
					//		 если же какой-то код рандомный (цикл for или что бы то ни было) -> идет в main функцию
					//else fillTable(classesList.find("__PROGRAM__")->second, classesList.find("__PROGRAM__")->second->methods.find("main")->second, classElement->stmt);
					break;
			}

			classElement = classElement->next;
		}
	}

	// TODO: Конструктор по умолчанию (от java/lang/Object)
	//newClass->pushOrFindConstant(*Constant::UTF8("<init>"));
}

// TODO: именованные аргументы + дескриптор
void fillTable(Class* clazz, FuncNode* funcDef) {
	Method* method = new Method();
	method->name = funcDef->identifier->identifier;
	method->nameNumber = clazz->pushOrFindConstant(*Constant::UTF8(method->name));
	// Модификатор доступа - AccessModifier -> AccessFlag 
	switch (funcDef->accessModifier)
	{
		case _UNKNOWN:
			// Ничего не делать
			break;
		case _PRIVATE:
			method->accessModifier = PRIVATE;
			break;
		case _PROTECTED:
			method->accessModifier = PROTECTED;
			break;
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
			arg->paramLocalVarNum = method->localVars.size() - 1;

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

	string methodDescriptor = generateMethodDescriptor(paramsCounter, "L__BASE__"); // TODO: не всегда должен возвращать __BASE__, а надо смотреть что за объект возвращается
	method->descriptorNumber = clazz->pushOrFindConstant(*Constant::UTF8(methodDescriptor));
	method->number = clazz->pushOrFindMethodRef(clazz->name, method->name, methodDescriptor);
	funcDef->idSemantic = method->number;
	method->suite = funcDef->suite;
	// Ссылки на super класс __BASE__
	method->baseClassNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
	method->baseConstructorNumber = clazz->pushOrFindMethodRef("__BASE__", "<init>", "()V");
	clazz->methods[method->name] = method;

	fillTable(clazz, method, method->suite);
}

void fillTable(Class* clazz, Method* method, StmtsListNode* suite) {
	if (suite != nullptr) {
		StmtNode* stmt = suite->first;
		while (stmt != nullptr) {
			fillTable(clazz, method, stmt);
			stmt = stmt->next;
		}
	}
}

// TODO
void fillTable(Class* clazz, Method* method, StmtNode* stmt) {
	switch (stmt->stmtType)
	{
		case _ASSIGN:
			if (!stmt->leftExpr->identifier.empty()) method->localVars.push_back(stmt->leftExpr->identifier);

			fillTable(clazz, method, stmt->leftExpr);
			fillTable(clazz, method, stmt->rightExpr);
			break;
		case _COMPOUND_ASSIGN:
			if (stmt->stmtsList != nullptr) {
				StmtNode* assignStmt = stmt->stmtsList->first;
				while (assignStmt != nullptr) {
					fillTable(clazz, method, assignStmt);
					assignStmt = assignStmt->next;
				}
			}
			break;
		case _IF:
			break;
		case _COMPOUND_IF:
			break;
		case _FOR:
			break;
		case _WHILE:
			break;
		case _RETURN:
			break;
		case _EXPR_STMT:
			break;
		case _EXPR_LIST_STMT:
			break;
	}
}

// TODO
void fillTable(Class* clazz, Method* method, ExprNode* expr) {
	switch (expr->exprType)
	{
		case _INT_CONST:
			expr->valueNumber = clazz->pushOrFindConstant(*Constant::Integer(expr->intVal));
			expr->classNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "<init>", "(I)V");
			break;
		case _STRING_CONST:
			expr->valueNumber = clazz->pushOrFindConstant(*Constant::String(clazz->pushOrFindConstant(*Constant::UTF8(expr->stringVal))));
			expr->classNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "<init>", "([Ljava/lang/String;)V");
			break;
		case _FLOAT_CONST:
			expr->valueNumber = clazz->pushOrFindConstant(*Constant::Float(expr->floatVal));
			expr->classNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "<init>", "(F)V");
			break;
		case _IDENTIFIER:
			expr->valueNumber = clazz->pushOrFindConstant(*Constant::String(clazz->pushOrFindConstant(*Constant::UTF8(expr->identifier))));
			expr->classNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "<init>", "([Ljava/lang/String;)V");
			expr->paramLocalVarNum = method->localVars.size() - 1;
			break;
	}
}

// Вспомогательные функции

string generateMethodDescriptor(int paramsNumber, string returnValueDescriptor) {
	string descriptor = "(";
	
	// Заполнение параметров
	for (int i = 0; i < paramsNumber; i++) {
		descriptor += "L__BASE__;";
	}
	// Возвращаемое значение
	descriptor += ")" + returnValueDescriptor + ";";

	return descriptor;
}
