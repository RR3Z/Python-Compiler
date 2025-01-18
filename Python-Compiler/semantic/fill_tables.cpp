#pragma once
#include "./semantic.h"
#include <iostream>
using namespace std;

std::map<std::string, Class*> classesList;

// ========= Заполнение таблиц =========

// Заполнение таблиц (стартовая функция)
void fillTables(FileNode* program) {
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
	// Добавление main в таблицу методов entryClass
	entryClass->methods[mainMethod->name] = mainMethod;

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
					fillMethodTable(entryClass, programElement->funcDef);
					break;
				case _STMT:
					/*
						В Python нет явной точки входа в программу. В таком случае, у нас код выполняется сверху вниз.
						Это значит, что если у нас встречается что-то в глобальной области видимости кроме funcDef, classDef 
						и assignStmt(пойдет в поля класса) идет сразу в main метод.
					*/

					// Если Assign Stmt, то переменная заносится в таблицу полей
					if (programElement->stmt->stmtType == _COMPOUND_ASSIGN && programElement->stmt->stmtsList != nullptr) {
						fillFieldTable(entryClass, programElement->stmt->stmtsList);
					}

					// Обновление тела main метода
					if (mainMethod->suite == nullptr) mainMethod->suite = new StmtsListNode();
					if (mainMethod->suite->first != nullptr) {
						mainMethod->suite->last = programElement->stmt;
						mainMethod->suite->first->next = programElement->stmt;
					}
					else {
						mainMethod->suite->first = programElement->stmt;
						mainMethod->suite->last = programElement->stmt;
					}

					fillMethodTable(entryClass, mainMethod, programElement->stmt);
					
					break;
			}

			// Переходим к следующему элементу
			programElement = programElement->next;
		}
	}
}

// TODO: классы
void fillTables(ClassNode* classDef) {
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
					fillMethodTable(newClass, classElement->funcDef);
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

// ========= Заполнение таблиц методов =========

// TODO: именованные аргументы + дескриптор
void fillMethodTable(Class* clazz, FuncNode* funcDef) {
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

	method->suite = funcDef->suite;
	fillMethodTable(clazz, method, method->suite);

	// Составление дескриптора
	//string methodReturnType = defineMethodReturnType(funcDef);
	string methodDescriptor = generateMethodDescriptor(paramsCounter, "L__BASE__"); // TODO: параметры не всегда должны быть только __BASE__
	method->descriptorNumber = clazz->pushOrFindConstant(*Constant::UTF8(methodDescriptor));

	method->number = clazz->pushOrFindMethodRef(clazz->name, method->name, methodDescriptor);
	funcDef->idSemantic = method->number;
	// Ссылки на super класс __BASE__
	method->baseClassNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
	method->baseConstructorNumber = clazz->pushOrFindMethodRef("__BASE__", "<init>", "()V");
	clazz->methods[method->name] = method;
}

void fillMethodTable(Class* clazz, Method* method, StmtsListNode* stmts) {
	if (stmts != nullptr) {
		StmtNode* stmt = stmts->first;
		while (stmt != nullptr) {
			fillMethodTable(clazz, method, stmt);
			stmt = stmt->next;
		}
	}
}

// TODO
void fillMethodTable(Class* clazz, Method* method, StmtNode* stmt) {
	switch (stmt->stmtType)
	{
		case _ASSIGN:
			// Если переменная не является полем класса, добавляем ее в качестве локальной переменной и загружаем ее имя в constant pool класса
			if (!stmt->leftExpr->identifier.empty() && clazz->fields.find(stmt->leftExpr->identifier) == clazz->fields.end()) {
				method->localVars.push_back(stmt->leftExpr->identifier);
				fillMethodTable(clazz, method, stmt->leftExpr);
			}

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
			break;
		case _COMPOUND_IF:
			break;
		case _FOR:
			break;
		case _WHILE:
			break;
		case _RETURN:
			// TODO: множественное возвращение
			if (stmt->list != nullptr) {
				ExprNode* expr = stmt->list->first;
				//while (expr != nullptr) {
				//	fillTable(clazz, method, expr);
				//	expr = expr->next;
				//}
				checkReturnValue(clazz, method, expr);
				fillMethodTable(clazz, method, expr);
			}
			break;
		case _EXPR_STMT:
			break;
		case _EXPR_LIST_STMT:
			break;
	}
}

// TODO
void fillMethodTable(Class* clazz, Method* method, ExprNode* expr) {
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
			expr->paramLocalVarNum = method->localVars.size() - 1;
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

// ========= Функции проверки =========

void checkReturnValue(Class* clazz, Method* method, ExprNode* expr) {
	if (expr != nullptr) {
		switch (expr->exprType)
		{
			case _IDENTIFIER:
				// Main Method local vars - TODO: по идее не надо проверять (логически)
				//vector<string> mainMethodLocalVars = classesList["__PROGRAM__"]->methods["main"]->localVars;
				//if (find(mainMethodLocalVars.begin(), mainMethodLocalVars.end(), expr->identifier) != mainMethodLocalVars.end()) return;

				// Method local vars
				if (find(method->localVars.begin(), method->localVars.end(), expr->identifier) != method->localVars.end()) return;
				// Fields
				if (clazz->fields.find(expr->identifier) != clazz->fields.end()) return;

				throw runtime_error("S: ERROR -> local variable " + expr->identifier + " is not defined");
		}
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
	descriptor += ")" + returnValueDescriptor + ";";

	return descriptor;
}

// TODO: реализовать множественное возвращение (сейчас можно вернуть только один элемент)
string defineMethodReturnType(Method* method) {
	vector<ExprNode*> returnValues = {};

	StmtNode* lastStmt = method->suite->last;
	if (lastStmt->stmtType == _RETURN) {
		if (lastStmt->list != nullptr) {
			ExprNode* expr = lastStmt->list->first;

			switch (expr->exprType)
			{
				case _STRING_CONST:
				case _INT_CONST:
				case _FLOAT_CONST:
					return "L__BASE__";
				case _IDENTIFIER:
					if (!expr->identifier.empty()) {
						if (find(method->localVars.begin(), method->localVars.end(), expr->identifier) != method->localVars.end()) {
							if (classesList.find(expr->identifier) != classesList.end()) return "L" + expr->identifier;
						}
						// TODO: проверка локальных переменных
						//else throw runtime_error("S: ERROR -> Something went wrong...");
					}
					return "";
			}
		}
	}

	//throw runtime_error("S: ERROR -> Unsupported type in Return Stmt");
	return "";
}
