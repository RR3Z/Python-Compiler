#pragma once
#include "./semantic.h"
#include <iostream>
using namespace std;

// Самая верхоуровневая таблица классов
std::map<std::string, Class*> classesList;

// Заполнение таблиц
void fillTable(FileNode* program) {
	// Создание класса, как точки входа в программу
	Class* entryClass = new Class();
	entryClass->name = "__PROGRAM__";

	// Добавление класса в глобальную таблицу
	classesList[entryClass->name] = entryClass;

	// Добавление констант класса
	entryClass->pushOrFindConstant(*Constant::UTF8("Code")); // По идее добавляется, так как у нас будет конструктор по умолчанию
	entryClass->number = entryClass->pushOrFindConstant(*Constant::Class(entryClass->pushOrFindConstant(*Constant::UTF8(entryClass->name))));

	// Добавление родительского класса
	int parentClassName = entryClass->pushOrFindConstant(*Constant::UTF8("java/lang/Object"));
	entryClass->parentNumber = entryClass->pushOrFindConstant(*Constant::Class(parentClassName));

	// Создание функции, как точки входа в программу - ПО ИДЕЕ НЕ НУЖНА ЗДЕСЬ
	/*Method* mainMethod = new Method();
	mainMethod->name = "main";
	// Добавление констант метода
	mainMethod->nameNumber = entryClass->pushOrFindConstant(*Constant::UTF8(mainMethod->name));
	mainMethod->descriptorNumber = entryClass->pushOrFindConstant(*Constant::UTF8("()V"));
	mainMethod->number = entryClass->pushOrFindMethodRef(entryClass->name, mainMethod->name, "()V");
	// Тело метода (изначально пустое)
	mainMethod->suite = nullptr;
	// Добавление main в таблицу методов entryClass
	entryClass->methods[mainMethod->name] = mainMethod;*/
	
	// Разбор кода программы
	if (program != nullptr && program->elementsList != nullptr) {
		FileElementNode* programElement = program->elementsList->first;

		while (programElement != nullptr) {
			switch (programElement->elementType)
			{
				case _CLASS_DEF:
					// TODO
					break;
				case _FUNC_DEF:
					fillTable(entryClass, programElement->funcDef);
					break;
				case _STMT:
					// TODO
					break;
			}

			// Переходим к следующему элементу
			programElement = programElement->next;
		}
	}

	// Создание конструктора по умолчанию для __PROGRAM__ - ПО ИДЕЕ ТОЖЕ НАХЕР НЕ НУЖЕН
	/*Method* constructor = new Method();
	constructor->name = "<init>";
	constructor->accessModifier = PUBLIC;
	constructor->nameNumber = entryClass->pushOrFindConstant(*Constant::UTF8(constructor->name));
	constructor->descriptorNumber = entryClass->pushOrFindConstant(*Constant::UTF8("()V"));
	constructor->localVars.push_back("this");
	constructor->suite = nullptr;
	constructor->number = entryClass->pushOrFindMethodRef(entryClass->name, constructor->name, "()V");
	constructor->selfMethodRef = entryClass->pushOrFindMethodRef("java/lang/Object", constructor->name, "()V"); // Как я понял, нам надо создать объект нашего языка на основе Java объекта
	// Добавление __init__ в таблицу методов entryClass
	entryClass->methods[constructor->name] = constructor;*/
}

// TODO
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
			// ERROR
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
					// TODO
					break;
			}

			classElement = classElement->next;
		}
	}

	// TODO: Конструктор по умолчанию
	//newClass->pushOrFindConstant(*Constant::UTF8("<init>"));
}

// TODO: именованные аргументы
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

	string methodDescriptor = "";
	if (method->name == "main") methodDescriptor = generateMethodDescriptor(paramsCounter, "V");
	else methodDescriptor = generateMethodDescriptor(paramsCounter, "L__BASE__");

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
	}
}

// Вспомогательные функции

/*
TODO
	Егор:
	Разве всегда __BASE__ должен возвращать?
	Например, если я делаю return самописного класса (myClass) и внутри него будут определены какие-то собственные методы, то
	если у нас будет __BASE__ без явного каста в этот класс обратиться к методам класса (myClass) не получится.

	Короче, при нахождении returnStmt надо изменять дескриптор.
*/
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
