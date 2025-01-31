#pragma once
#include "./semantic.h"
#include <iostream>
using namespace std;

map<string, Class*> classesList;

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
					fillTables(programElement->classDef);
					break;
				case _FUNC_DEF:
					fillMethodTable(entryClass, programElement->funcDef);
					break;
				case _STMT:
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

	// Проверка различных вызовов (так как можно вызвать функцию до ее объявления)
	for (auto _class : classesList) {
		for (auto class_ : classesList) {
			_class.second->pushOrFindMethodRef(class_.second->name, "<init>", "()V");
		}
		castVariables(_class.second);
		checkAttributeRefsNodes(_class.second);
		checkFuncMethodCallsForErrors(_class.second);
	}
}

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
		if (classesList.find(classDef->base->identifier) != classesList.end()) {
			newClass->parent = classesList[classDef->base->identifier];
			newClass->parentNumber = newClass->pushOrFindConstant(*Constant::Class(newClass->pushOrFindConstant(*Constant::UTF8(classDef->base->identifier))));
		}
		else {
			throw runtime_error("S: ERROR -> parent class " + classDef->base->identifier + " for class " + classDef->identifier->identifier + " not found");
		}
	}
	// Родительский класс по умолчанию
	else newClass->parentNumber = newClass->pushOrFindConstant(*Constant::Class(newClass->pushOrFindConstant(*Constant::UTF8("__BASE__"))));

	// Конструктор по умолчанию
	Method* initMethod = new Method();
	initMethod->name = "<init>";
	initMethod->nameNumber = newClass->pushOrFindConstant(*Constant::UTF8(initMethod->name));
	initMethod->descriptorNumber = newClass->pushOrFindConstant(*Constant::UTF8("()V"));
	initMethod->localVars.push_back("this");
	initMethod->baseClassNumber = newClass->pushOrFindConstant(*Constant::Class(newClass->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
	initMethod->baseConstructorNumber = newClass->pushOrFindMethodRef("__BASE__", "<init>", "()V");
	initMethod->suite = nullptr;
	initMethod->number = newClass->pushOrFindMethodRef(newClass->name, initMethod->name, "()V");
	initMethod->selfMethodRef = newClass->pushOrFindMethodRef("__BASE__", initMethod->name, "()V");
	initMethod->isClassCreated = false;
	newClass->methods[initMethod->name] = initMethod;

	// Заполнение тела
	if (classDef->suite != nullptr && classDef->suite->first != nullptr) {
		ClassElementNode* classElement = classDef->suite->first;
		while (classElement != nullptr) {
			switch (classElement->elementType)
			{
				case _FUNCTION_DEF:
					fillMethodTable(newClass, classElement->funcDef);
					break;
				case _STMT_NODE:
					// В конструктор идут только поля класса
					if (classElement->stmt->stmtType == _COMPOUND_ASSIGN && classElement->stmt->stmtsList != nullptr) {
						fillFieldTable(newClass, classElement->stmt->stmtsList);

						if (initMethod->suite == nullptr) initMethod->suite = new StmtsListNode();
						if (initMethod->suite->first != nullptr) {
							initMethod->suite->last->next = classElement->stmt;
							initMethod->suite->last = classElement->stmt;
						}
						else {
							initMethod->suite->first = classElement->stmt;
							initMethod->suite->last = classElement->stmt;
						}

						fillMethodTable(newClass, initMethod, classElement->stmt->stmtsList);
					}
					// Все остальное идет в main функцию класса __PROGRAM__ (вызывается при запуске программы)
					else {
						Method* mainMethod = classesList["__PROGRAM__"]->methods["main"];
						if(mainMethod->suite == nullptr) mainMethod->suite = new StmtsListNode();

						if (mainMethod->suite->first != nullptr) {
							mainMethod->suite->last->next = classElement->stmt;
							mainMethod->suite->last = classElement->stmt;
						}
						else {
							mainMethod->suite->first = classElement->stmt;
							mainMethod->suite->last = classElement->stmt;
						}

						fillMethodTable(classesList["__PROGRAM__"], mainMethod, classElement->stmt);
					}
					break;
			}

			classElement = classElement->next;
		}
	}

	newClass->pushOrFindMethodRef(newClass->name, "<init>", "()V");

	// Дефолтный конструктор добавляется в constant pool нашей программы
	classesList["__PROGRAM__"]->pushOrFindMethodRef(newClass->name, "<init>", "()V");
}

// ========= Заполнение таблиц методов =========

// TODO: дескриптор
void fillMethodTable(Class* clazz, FuncNode* funcDef) {
	checkMethodNameForErrors(funcDef);

	if (clazz->name != "__PROGRAM__" && funcDef->args == nullptr) {
		throw runtime_error("S: ERROR -> Incorrect amount of args in function definition (" + funcDef->identifier->identifier + ") in class \"" + clazz->name + "\"");
	}

	Method* method = new Method();
	method->name = funcDef->identifier->identifier;
	method->nameNumber = clazz->pushOrFindConstant(*Constant::UTF8(method->name));
	// Модификатор доступа - AccessModifier -> AccessFlag
	switch (funcDef->accessModifier)
	{
		case _PRIVATE:
			method->accessModifier = PRIVATE;
			break;
		case _PROTECTED: // Protected работает аналогично Public (да, Python...)
		case _UNKNOWN: // Если нет модификатора доступа, то по дефолту public.
		case _PUBLIC:
			method->accessModifier = PUBLIC;
			break;
	}

	// Аргументы метода
	if (clazz->name != "__PROGRAM__") { method->localVars.push_back("this"); } // нулевой аргумент в любом методе собственного класса - this
	int paramsCounter = 0;
	if (funcDef->args != nullptr) {
		// Обычные аргументы (a,b,c,...)
		ExprNode* arg = funcDef->args->exprList->first;
		while (arg != nullptr) {
			paramsCounter++;
			method->localVars.push_back(arg->identifier);

			if (paramsCounter == 1 && clazz->name != "__PROGRAM__") {
				method->varType[arg->identifier] = clazz->name;
			}

			arg = arg->next;
		}

		// Здесь могли быть именованные аргументы...
	}
	method->paramsCount = paramsCounter;

	// Ссылки на super класс __BASE__
	method->baseClassNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
	method->baseConstructorNumber = clazz->pushOrFindMethodRef("__BASE__", "<init>", "()V");
	clazz->methods[method->name] = method;

	method->suite = funcDef->suite;

	// Составление дескриптора
	string methodReturnType = defineMethodReturnType(method);
	string methodDescriptor = generateMethodDescriptor(paramsCounter, methodReturnType);
	method->descriptor = methodDescriptor;
	method->descriptorNumber = clazz->pushOrFindConstant(*Constant::UTF8(methodDescriptor));
	method->number = clazz->pushOrFindMethodRef(clazz->name, method->name, methodDescriptor);
	funcDef->idSemantic = method->number;

	// Генерация таблиц для тела функции
	fillMethodTable(clazz, method, method->suite);

	// Составление дескриптора
	methodReturnType = defineMethodReturnType(method);
	methodDescriptor = generateMethodDescriptor(paramsCounter, methodReturnType);
	method->descriptor = methodDescriptor;
	method->descriptorNumber = clazz->pushOrFindConstant(*Constant::UTF8(methodDescriptor));
	method->number = clazz->pushOrFindMethodRef(clazz->name, method->name, methodDescriptor);
	funcDef->idSemantic = method->number;

	if (clazz->name != "__PROGRAM__") {
		classesList["__PROGRAM__"]->pushOrFindMethodRef(clazz->name, method->name, method->descriptor);
	}
}

void fillMethodTable(Class* clazz, Method* method, StmtsListNode* stmts) {
	if (stmts != nullptr) {
		StmtNode* stmt = stmts->first;
		while (stmt != nullptr) {

			if (stmt->stmtType == StmtType::_RETURN) {
				stmt->next = nullptr;
				if (method->suite == stmts) {
					method->suite->last = stmt;
					method->returnNode = stmt;
				}
			}
			
			stmt = stmt->next;
		}

		stmt = stmts->first;
		while (stmt != nullptr) {
			fillMethodTable(clazz, method, stmt);
			stmt = stmt->next;
		}
	}
}

// TODO
void fillMethodTable(Class* clazz, Method* method, StmtNode* stmt) {
	int iteratorNum = 0;
	string iteratorVar = "";
	
	switch (stmt->stmtType)
	{
		case _ASSIGN:
			clazz->assignCalls.push_back(make_pair(method->name, stmt));

			// Для обращения к полю класса в левой части выражения (a.b = 5)
			if (stmt->leftExpr->exprType == _ATTRIBUTE_REF) {
				string fieldName = stmt->leftExpr->right->identifier;
				string thisName = stmt->leftExpr->left->identifier;

				// Проверка, задан ли объект, к которому обращаются среди локалок метода
				if (method->name != "main" && find(method->localVars.begin(), method->localVars.end(), thisName) == method->localVars.end()) {
					throw runtime_error("S: ERROR -> Can't get REF \"" + thisName + "\" in class (" + clazz->name + ") method \"" + method->name + "\"");
				}
				else { stmt->leftExpr->left->paramLocalVarNum = findElementIndexInVector(method->localVars, thisName); }

				// Проверка на наличие поля в классе
				if (clazz->fields.find(fieldName) == clazz->fields.end()) {
					if (clazz->name != "__PROGRAM__") {
						fillFieldTable(clazz, stmt->leftExpr->right);
						stmt->number = clazz->fields[fieldName]->number;
					}
					else if (method->name != "main") throw runtime_error("S: ERROR -> Unknown field \"" + thisName + "\" in class \"" + clazz->name + "\"");
				} else if (clazz->name != "__PROGRAM__") { stmt->number = clazz->fields[fieldName]->number; }
			}

			// Для обычной переменной (a = 5)
			if (stmt->leftExpr->exprType == _IDENTIFIER) {

				// Проверка, что переменная, которую присваивают, существует
				if (stmt->rightExpr->exprType == _IDENTIFIER) {
					if (find(method->localVars.begin(), method->localVars.end(), stmt->rightExpr->identifier) == method->localVars.end() &&
						clazz->fields.find(stmt->rightExpr->identifier) == clazz->fields.end()) {
						throw runtime_error("S: ERROR -> Trying to assign unknown value \"" +
							stmt->rightExpr->identifier + "\" to variable \"" + stmt->leftExpr->identifier + "\" in method \"" + method->name + "\" of class \"" + clazz->name + "\"");
					}
				}

				if (find(method->localVars.begin(), method->localVars.end(), stmt->leftExpr->identifier) != method->localVars.end()) {
					stmt->leftExpr->paramLocalVarNum = findElementIndexInVector(method->localVars, stmt->leftExpr->identifier);
				}
				// Если переменная не является полем класса, добавляем ее в качестве локальной переменной текущей функции
				else if (clazz->fields.find(stmt->leftExpr->identifier) == clazz->fields.end()) {
					method->localVars.push_back(stmt->leftExpr->identifier);
					stmt->leftExpr->paramLocalVarNum = findElementIndexInVector(method->localVars, stmt->leftExpr->identifier);

					if (stmt->leftExpr->paramLocalVarNum == -1) {
						throw runtime_error("S: ERROR -> Trying to assign value to unknown identifier \"" +
							stmt->leftExpr->identifier + "\" in method \"" + method->name + "\" of class \"" + clazz->name + "\"");
					}
				}
				else {
					if(clazz->name != "__PROGRAM__") {
						if (method->name != "<init>") {
							method->localVars.push_back(stmt->leftExpr->identifier);
							stmt->leftExpr->paramLocalVarNum = findElementIndexInVector(method->localVars, stmt->leftExpr->identifier);
						}
					}
					else stmt->number = clazz->fields[stmt->leftExpr->identifier]->number;
				}
			}

			// var
			fillMethodTable(clazz, method, stmt->leftExpr);

			// value
			//castVariable(clazz, method, stmt);
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
			checkForValue(clazz, method, stmt->expr);
			// array
			if (stmt->expr != nullptr) fillMethodTable(clazz, method, stmt->expr);

			// var (РАБОТАЕТ ТОЛЬКО С ОДНОЙ ПЕРЕМЕННОЙ!)
			if (stmt->list != nullptr) {
				fillMethodTable(clazz, method, stmt->list->first);
				method->localVars.push_back(stmt->list->first->identifier);
				stmt->iterableVarNumber = findElementIndexInVector(method->localVars, stmt->list->first->identifier);
			}

			// suite
			if (stmt->suite != nullptr) fillMethodTable(clazz, method, stmt->suite);

			// Дополнительная переменная для корректной работы цикла
			iteratorNum = 0;
			iteratorVar = "iterator_reserve" + iteratorNum;
			while (find(method->localVars.begin(), method->localVars.end(), iteratorVar) != method->localVars.end()) {
				iteratorNum++;
				iteratorVar = "iterator_reserve" + iteratorNum;
			}
			method->localVars.push_back(iteratorVar);
			stmt->iteratorNumber = findElementIndexInVector(method->localVars, iteratorVar);
			stmt->baseClassNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
			// Дополнительные методы для реализации цикла
			stmt->getIteratorMethodRef = clazz->pushOrFindMethodRef("__BASE__", "__get_iterator__", "()Ljava/util/Iterator;");
			stmt->forHasNextMethodRef = clazz->pushOrFindInterfaceMethodRef("java/util/Iterator", "hasNext", "()Z");
			stmt->forNextMethodRef = clazz->pushOrFindInterfaceMethodRef("java/util/Iterator", "next", "()Ljava/lang/Object;");
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
			if (stmt->list != nullptr) {
				method->isContainReturn = true;
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
		case _ATTRIBUTE_REF:
			clazz->attributeRefs.push_back(make_pair(method->name, expr));

			// TODO: сделать проверку что слева и справа допустимый тип данных
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);

			// Получение номера в constant pool на объект, к которому обращаются
			if (expr->left->exprType == _IDENTIFIER) {
				string classObjectName = expr->left->identifier;
				string fieldName = expr->right->identifier;

				// Если объект, к которому обращаются, является локалкой, запоминаем индекс аргумента в функции
				if (find(method->localVars.begin(), method->localVars.end(), classObjectName) != method->localVars.end()) {
					expr->paramLocalVarNum = findElementIndexInVector(method->localVars, classObjectName);
				}
				// Если объект, к которому обращаются, является полем класса
				else if (clazz->fields.find(classObjectName) != clazz->fields.end()) {
					expr->number = clazz->fields[classObjectName]->number;
				}
			}
			break;
		case _FUNCTION_CALL:
			// Добавляю узел, для будущей проверки на правильность
			clazz->funcMethodCalls.push_back(make_pair(method->name, expr));

			expr->argsCount = 0;
			if (expr->funcArgs != nullptr) {
				ExprNode* arg = expr->funcArgs->exprList->first;
				while (arg != nullptr) {
					fillMethodTable(clazz, method, arg);
					expr->argsCount++;
					arg = arg->next;
				}
			}
			break;
		case _METHOD_CALL:
			// Добавляю узел, для будущей проверки на правильность
			clazz->funcMethodCalls.push_back(make_pair(method->name, expr));

			expr->left->paramLocalVarNum = findElementIndexInVector(method->localVars, expr->left->identifier);

			expr->argsCount = 0;
			if (expr->funcArgs != nullptr) {
				ExprNode* arg = expr->funcArgs->exprList->first;
				while (arg != nullptr) {
					fillMethodTable(clazz, method, arg);
					expr->argsCount++;
					arg = arg->next;
				}
			}
			break;
	}
}

void fillMethodTable(Class* clazz, Method* method, ExprListNode* exprList) {
	if (exprList != nullptr) {
		ExprNode* expr = exprList->first;
		while (expr != nullptr) {
			fillMethodTable(clazz, method, expr);
			expr = expr->next;
		}
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
	case _UNKNOWN: // Если нет модификатора доступа, то по умолчанию public.
	case _PROTECTED: // Protected работает аналогично Public (да, Python...)
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

	classesList["__PROGRAM__"]->pushOrFindFieldRef(clazz->name, field->name, field->descriptor);
}

void fillFieldTable(Class* clazz, ExprNode* identifierExpr) {
	Field* field = new Field();

	// Имя
	field->name = identifierExpr->identifier;
	field->nameNumber = clazz->pushOrFindConstant(*Constant::UTF8(field->name));
	field->nameNode = identifierExpr;

	// Модификатор доступа - AccessModifier -> AccessFlag
	switch (identifierExpr->accessModifier) {
		case _PRIVATE:
			field->accessModifier = PRIVATE;
			break;
		case _UNKNOWN: // Если нет модификатора доступа, то по умолчанию public.
		case _PROTECTED: // Protected работает аналогично Public (да, Python...)
		case _PUBLIC:
			field->accessModifier = PUBLIC;
			break;
	}

	// Дескриптор
	field->descriptor = "L__BASE__;";
	field->descriptorNumber = clazz->pushOrFindConstant(*Constant::UTF8(field->descriptor));

	// FieldRef
	field->number = clazz->pushOrFindFieldRef(clazz->name, field->name, field->descriptor);
	identifierExpr->number = field->number;

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

	// range
	clazz->pushOrFindMethodRef("__BASE__", "range", "(L__BASE__;L__BASE__;)L__BASE__;");
	clazz->pushOrFindMethodRef("__BASE__", "range", "(L__BASE__;)L__BASE__;");

	// upper
	clazz->pushOrFindMethodRef("__BASE__", "upper", "()L__BASE__;");
	// lower
	clazz->pushOrFindMethodRef("__BASE__", "lower", "()L__BASE__;");
	// find
	clazz->pushOrFindMethodRef("__BASE__", "find", "(L__BASE__;)L__BASE__;");
}

// ========= Функции проверок =========

void checkFunctionCallParams(Class* clazz, Method* method, ExprNode* expr) {
	if (expr != nullptr) {
		if (expr->exprType == _FUNCTION_CALL) {
			// 1) Проверка на существование передаваемых аргументов
			if (expr->funcArgs != nullptr) {
				ExprNode* arg = expr->funcArgs->exprList->first;
				while (arg != nullptr) {
					if (find(method->localVars.begin(), method->localVars.end(), arg->identifier) == method->localVars.end() && arg->exprType == _IDENTIFIER) {
						throw runtime_error("S: ERROR -> variable \"" + arg->identifier + "\" is not defined. Function call \"" + expr->left->identifier + "\" in method \"" + method->name + "\" class \"" + clazz->name + "\"");
					}

					if (arg->exprType == _ATTRIBUTE_REF) {
						if (find(method->localVars.begin(), method->localVars.end(), arg->left->identifier) == method->localVars.end()) {
							throw runtime_error("S: ERROR -> variable \"" + arg->left->identifier + "\" is not defined. Function call \"" + expr->left->identifier + "\" in method \"" + method->name + "\"");
						}
						else {
							Class* classRef = classesList[method->varType[arg->left->identifier]];
							if (classRef == nullptr)
								throw runtime_error("S: ERROR -> Trying get field of __BASE__ class. \"" + expr->left->identifier + "." +
									expr->right->identifier + "\" in class \"" + clazz->name + "\" method \"" + method->name + "\"");
							Field* fieldRef = classRef->findField(arg->right->identifier);
							if (fieldRef == nullptr) {
								throw runtime_error("S: ERROR -> field \"" + arg->right->identifier + "\" is not defined for object \"" + arg->left->identifier + "\". Function call \"" + expr->left->identifier + "\" in method \"" + method->name + "\"");
							}
						}
					}

					arg = arg->next;
				}
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

		if (expr->exprType == _METHOD_CALL) {
			/*
			// 1) Проверка на существование передаваемых аргументов
			if (expr->funcArgs != nullptr) {
				ExprNode* arg = expr->funcArgs->exprList->first;
				while (arg != nullptr) {
					if (find(method->localVars.begin(), method->localVars.end(), arg->identifier) == method->localVars.end() && arg->exprType == _IDENTIFIER) {
						throw runtime_error("S: ERROR -> variable \"" + arg->identifier + "\" is not defined. Method call \"" + expr->right->identifier
							+ "\" for object \"" + expr->left->identifier + "\" in method \"" + method->name + "\"");
					}

					if (arg->exprType == _ATTRIBUTE_REF) {
						if (find(method->localVars.begin(), method->localVars.end(), arg->left->identifier) == method->localVars.end()) {
							throw runtime_error("S: ERROR -> variable \"" + arg->left->identifier + "\" is not defined. Function call \"" + expr->left->identifier + "\" in method \"" + method->name + "\"");
						}
						else {
							Class* classRef = classesList[method->varType[arg->left->identifier]];
							Field* fieldRef = classRef->findField(arg->right->identifier);
							if (fieldRef == nullptr) {
								throw runtime_error("S: ERROR -> field \"" + arg->right->identifier + "\" is not defined for object \"" + arg->left->identifier
									+ "\". Function call \"" + expr->left->identifier + "\" in method \"" + method->name + "\"");
							}
						}
					}

					arg = arg->next;
				}
			}

			// 2) Сравнение количества передаваемых аргументов с количеством требуемых
			// Для RTL функций
			if (checkRTLFunctionCallParams(expr)) return;

			if (expr->left->exprType == _FUNCTION_CALL || expr->left->exprType == _METHOD_CALL) {
				checkFunctionCallParams(clazz, method, expr->left);
			}
			else {
				// Для метода собственного класса
				string className = method->varType[expr->left->identifier];
				if (classesList[className]->methods.find(expr->right->identifier) == classesList[className]->methods.end()) {
					throw runtime_error("S: ERROR -> Unknown method \"" + expr->right->identifier + "\" for object \"" + expr->left->identifier + "\". Class \"" +
						clazz->name + "\" method \"" + method->name + "\"");
				}
				int paramsCount = classesList[className]->methods[expr->right->identifier]->paramsCount - 1;
				int argsCount = expr->argsCount;

				if (paramsCount - argsCount != 0) {
					throw runtime_error("S: ERROR -> method \"" + expr->right->identifier + "\" takes " + to_string(paramsCount) + " arguments but " + to_string(argsCount) + " was given");
				}
			}
			*/

			if (isRTLMethodExists(clazz, expr)) {
				checkRTLFunctionArgsCount(expr);

				if (expr->left->exprType == _STRING_CONST) {
					method->localVars.push_back("rtl_str");
					expr->left->paramLocalVarNum = findElementIndexInVector(method->localVars, "rtl_str");
					clazz->pushOrFindMethodRef("__BASE__", "<init>", "(Ljava/lang/String;)V");
					expr->objectFieldRef = clazz->pushOrFindConstant(*Constant::String(clazz->pushOrFindConstant(*Constant::UTF8(expr->left->stringVal))));
				}
				else if (expr->left->exprType != _IDENTIFIER) {
					throw runtime_error("S: ERROR -> UNSUPPORTED OPERAND IN LEFT PART OF METHOD CALL \"" + expr->right->identifier + "\"");
				}
				else {
					if (find(method->localVars.begin(), method->localVars.end(), expr->left->identifier) == method->localVars.end()) {
						throw runtime_error("S: ERROR -> UNKNOWN OPERAND IN LEFT PART OF METHOD CALL \"" + expr->right->identifier + "\"");
					}
				}
			}
			else {
				throw runtime_error("S: ERROR -> UNKNOWN METHOD CALL \"" + expr->right->identifier + "\"");
			}
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

	if (expr->left->identifier == "range") {
		if (expr->argsCount > 2 || expr->argsCount <= 0) {
			throw runtime_error("S: ERROR -> function \"" + expr->left->identifier + "\" takes 1 or 2 arguments but " + to_string(expr->argsCount) + " was given");
		}
		return true;
	}

	return false;
}

void isMethodExists(Class* clazz, Method* method, ExprNode* functionCall) {
	if (functionCall->isConstructor) return;

	if (functionCall != nullptr) {
		if (functionCall->exprType == _FUNCTION_CALL) {
			if (clazz->methods.find(functionCall->left->identifier) == clazz->methods.end()) {
				if (isRTLMethodExists(clazz, functionCall) || method->name == functionCall->left->identifier) return;
				throw runtime_error("S: ERROR -> trying call unknown function \"" + functionCall->left->identifier + "\"");
			}
		}

		if (functionCall->exprType == _METHOD_CALL) {
			if (isRTLMethodExists(clazz, functionCall)) return;
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

	if (functionCall->left->identifier == "range") {
		int rangeFirstMethodRefNumber = clazz->findMethodRef("__BASE__", "range", "(L__BASE__;)L__BASE__;");
		int rangeSecondMethodRefNumber = clazz->findMethodRef("__BASE__", "range", "(L__BASE__;L__BASE__;)L__BASE__;");
		if (rangeFirstMethodRefNumber == -1 && rangeSecondMethodRefNumber == -1) {
			throw runtime_error("S: ERROR -> trying call unknown function " + functionCall->left->identifier);
		}
		return true;
	}

	if (functionCall->exprType == _METHOD_CALL) {
		if (functionCall->right->identifier == "find") {
			int methodRefNumber = clazz->findMethodRef("__BASE__", "find", "(L__BASE__;)L__BASE__;");
			if (methodRefNumber == -1) {
				throw runtime_error("S: ERROR -> trying call unknown method " + functionCall->right->identifier);
			}
			return true;
		}

		if (functionCall->right->identifier == "upper") {
			int methodRefNumber = clazz->findMethodRef("__BASE__", "upper", "()L__BASE__;");
			if (methodRefNumber == -1) {
				throw runtime_error("S: ERROR -> trying call unknown method " + functionCall->right->identifier);
			}
			return true;
		}

		if (functionCall->right->identifier == "lower") {
			int methodRefNumber = clazz->findMethodRef("__BASE__", "lower", "()L__BASE__;");
			if (methodRefNumber == -1) {
				throw runtime_error("S: ERROR -> trying call unknown method " + functionCall->right->identifier);
			}
			return true;
		}
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

		if (funcDef->identifier->identifier == "range") {
			throw runtime_error("S: ERROR -> Changes to the signature of the \"range\" function!");
		}
	}
}

void checkConditionForErrors(Class* clazz, Method* method, ExprNode* condition, string stmtType) {
	if (condition == nullptr) throw runtime_error("S: ERROR -> No condition for IF.");

	if (condition->exprType == _BRACKETS) condition = condition->left;

	switch (condition->exprType)
	{
		case _IDENTIFIER:
			if (find(method->localVars.begin(), method->localVars.end(), condition->identifier) == method->localVars.end()&&
				!condition->identifier.empty()) {
				throw runtime_error("S: ERROR -> Unknown variable \"" + condition->identifier + "\" in " + stmtType + " condition. Class \"" + clazz->name + "\" in method \"" + method->name + "\"");
			}
			break;
		case _EQUAL:
		case _GREAT_EQUAL:
		case _GREAT:
		case _LESS:
		case _LESS_EQUAL:
		case _NOT_EQUAL:
		case _AND_LOGIC:
		case _OR_LOGIC:
			checkConditionForErrors(clazz, method, condition->left, stmtType);
			checkConditionForErrors(clazz, method, condition->right, stmtType);
			break;
		case _NOT:
			checkConditionForErrors(clazz, method, condition->left, stmtType);
			break;
	}
}

bool isConstructorCall(Class* clazz, ExprNode* functionCall) {
	if (functionCall->exprType == _FUNCTION_CALL) {
		if (functionCall->funcArgs != nullptr) return false;
		if (clazz->findMethodRef(functionCall->left->identifier, "<init>", "()V") != -1) return true;
		return false;
	}
	else if (functionCall->exprType == _METHOD_CALL) {
		return false;
	}
}

void checkFuncMethodCallsForErrors(Class* clazz) {
	ExprNode* callNode = nullptr;
	string methodName = "";

	for (const auto& call : clazz->funcMethodCalls) {
		methodName = call.first;
		callNode = call.second;

		if (callNode == nullptr || methodName.empty()) throw runtime_error("CRITICAL ERROR!");

		switch (callNode->exprType)
		{
		case _METHOD_CALL:
		case _FUNCTION_CALL:
			if (isConstructorCall(clazz, callNode)) callNode->isConstructor = true;

			isMethodExists(clazz, clazz->methods[methodName], callNode);
			checkFunctionCallParams(clazz, clazz->methods[methodName], callNode);
			callNode->number = defineMethodRefByExprNode(clazz, clazz->methods[methodName], callNode);
			break;
		}
	}
}

void checkAttributeRefsNodes(Class* clazz) {
	ExprNode* attributeRefNode = nullptr;
	string methodName = "";

	Class* classRef = nullptr;
	Field* fieldRef = nullptr;

	for (const auto& ref : clazz->attributeRefs) {
		methodName = ref.first;
		attributeRefNode = ref.second;

		if (attributeRefNode == nullptr || methodName.empty() || attributeRefNode->exprType != _ATTRIBUTE_REF) throw runtime_error("CRITICAL ERROR!");

		if (attributeRefNode->objectFieldRef == -1) {
			classRef = classesList[clazz->methods[methodName]->varType[attributeRefNode->left->identifier]];
			if (classRef == nullptr)
				throw runtime_error("S: ERROR -> Trying get field of __BASE__ class. \"" + attributeRefNode->left->identifier + "." +
				attributeRefNode->right->identifier + "\" in class \"" + clazz->name + "\" method \"" + methodName + "\"");
			fieldRef = classRef->findField(attributeRefNode->right->identifier);
			if (fieldRef == nullptr)
				throw runtime_error("S: ERROR -> Trying get field \"" + attributeRefNode->left->identifier + "." +
					attributeRefNode->right->identifier + "\" of class \"" + classRef->name + "\". Class \"" + clazz->name + "\" method \"" + methodName + "\"");
			attributeRefNode->objectFieldRef = clazz->pushOrFindFieldRef(classRef->name, attributeRefNode->right->identifier, fieldRef->descriptor);
		}
	}
}

void checkForValue(Class* clazz, Method* method, ExprNode* value) {
	if (value == nullptr) {
		return;
	}

	switch (value->exprType)
	{
		case _FUNCTION_CALL:
			if (isRTLMethodExists(clazz, value)) return;
			if (clazz->methods.find(value->left->identifier) != clazz->methods.end()) {
				StmtNode* returnStmt = clazz->methods[value->left->identifier]->returnNode;
				if (returnStmt == nullptr || returnStmt->list == nullptr) {
					throw runtime_error("S: ERROR -> for takes void arg. Class \"" + clazz->name + "\" method \"" + method->name + "\"");
				}
			}
			break;
		case _IDENTIFIER:
			if (find(method->localVars.begin(), method->localVars.end(), value->identifier) == method->localVars.end()) {
				throw runtime_error("S: ERROR -> Unknown variable \"" + value->identifier + "\" Class \"" + clazz->name + "\" method \"" + method->name + "\"");
			}
			break;
		case _METHOD_CALL:
		case _ATTRIBUTE_REF:
			if (find(method->localVars.begin(), method->localVars.end(), value->left->identifier) == method->localVars.end()) {
				throw runtime_error("S: ERROR -> Unknown variable \"" + value->left->identifier + "\" Class \"" + clazz->name + "\" method \"" + method->name + "\"");
			}
			break;
	}
}

void checkRTLFunctionArgsCount(ExprNode* methodCall) {
	if (methodCall == nullptr) return;

	if(methodCall->right->identifier == "upper" && methodCall->argsCount != 0) {
		throw runtime_error("S: ERROR -> Incorrect amount of args. Method call \"upper\".");
	}

	if (methodCall->right->identifier == "lower" && methodCall->argsCount != 0) {
		throw runtime_error("S: ERROR -> Incorrect amount of args. Method call \"lower\".");
	}

	if (methodCall->right->identifier == "find" && methodCall->argsCount != 1) {
		throw runtime_error("S: ERROR -> Incorrect amount of args. Method call \"find\".");
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

string defineMethodReturnType(Method* method) {
	if (method->isContainReturn) return "L__BASE__;";

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
			else if (expr->left->identifier == "range")
			{
				if(expr->funcArgs->exprList->first != expr->funcArgs->exprList->last) return clazz->pushOrFindMethodRef("__BASE__", "range", "(L__BASE__;L__BASE__;)L__BASE__;");
				return clazz->pushOrFindMethodRef("__BASE__", "range", "(L__BASE__;)L__BASE__;");
			}
			else if (expr->isConstructor && expr->funcArgs == nullptr) return clazz->pushOrFindMethodRef(expr->left->identifier, "<init>", "()V");
			else {
				if (clazz->methods.find(expr->left->identifier) != clazz->methods.end()) {
					return clazz->pushOrFindMethodRef(clazz->name, expr->left->identifier, clazz->methods[expr->left->identifier]->descriptor);
				}
				else {
					throw runtime_error("S: ERROR -> Trying call unknown function \"" + expr->left->identifier + "\". Class \"" + clazz->name + "\" method \"" + method->name + "\"");
				}
			}
			break;
		case _METHOD_CALL:
			if (expr->left->exprType == _FUNCTION_CALL || expr->left->exprType == _METHOD_CALL) {
				defineMethodRefByExprNode(clazz, method, expr->left);
			}
			else {
				/*
				string className = method->varType[expr->left->identifier];
				string methodName = expr->right->identifier;
				string methodDescriptor = classesList[className]->methods[methodName]->descriptor;
				int methodRefNumber = clazz->pushOrFindMethodRef(className, methodName, methodDescriptor);
				return methodRefNumber;
				*/
				if (expr->right->identifier == "upper") {
					return clazz->pushOrFindMethodRef("__BASE__", "upper", "()L__BASE__;");
				}

				if (expr->right->identifier == "lower") {
					return clazz->pushOrFindMethodRef("__BASE__", "lower", "()L__BASE__;");
				}

				if (expr->right->identifier == "find") {
					return clazz->pushOrFindMethodRef("__BASE__", "find", "(L__BASE__;)L__BASE__;");
				}
				break;
			}
	} 
}

void castVariable(Class* clazz, Method* method, StmtNode* assignStmt) {
	if (assignStmt->rightExpr->exprType == _FUNCTION_CALL) {
		if (clazz->findMethodRef(assignStmt->rightExpr->left->identifier, "<init>", "()V") != -1) {
			method->varType[assignStmt->leftExpr->identifier] = assignStmt->rightExpr->left->identifier;
		}
	}
	else {
		method->varType[assignStmt->leftExpr->identifier] = "__BASE__";
	}
}

void castVariables(Class* clazz) {
	StmtNode* assignNode = nullptr;
	string methodName = "";

	for (const auto& call : clazz->assignCalls) {
		methodName = call.first;
		assignNode = call.second;

		if (assignNode == nullptr || methodName.empty() || assignNode->stmtType != _ASSIGN) throw runtime_error("CRITICAL ERROR!");

		castVariable(clazz, clazz->methods[methodName], assignNode);
	}
}
