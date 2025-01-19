#pragma once
#include "../nodes/nodes.h"
#include <string>
#include <vector>
#include <map>
#include "./Constant.h"
#include "../codeGen/AccessFlag.h"
using namespace std;

struct Field {
	int number = -1;	// Номер поля

	int nameNumber = -1; // Номер навзвания поля
	string name = "";	// Название поля
	ExprNode* nameNode = nullptr; // Узел с названием
	
	int descriptorNumber = -1; // Номер дескриптора
	string descriptor = ""; // Дескриптор (тип переменной)
	 
	AccessFlag accessModifier; // Модификатор доступа
};

struct Method {
	int number = -1; // Номер метода

	// Для имени метода
	string name = "";
	int nameNumber = -1;

	// Дескриптор нужен для корректной работы JVM
	string descriptor = "";
	int descriptorNumber = -1;

	// Для локальных переменных (TODO: почему строки, а не int?)
	vector<string> localVars = {};

	// Ссылка на узел дерева с элементами тела
	StmtsListNode* suite = nullptr;

	// Super класса Java (java/lang/Object)
	int selfMethodRef = -1;

	// Количество параметров (передаваемых в функцию)
	int paramsCount = -1;

	// Ссылка на super класс (в моем случае, __BASE__)
	int baseClassNumber = -1;
	// Ссылка на конструктор super класса
	int baseConstructorNumber = -1;

	// Модификатор доступа
	AccessFlag accessModifier;
};

class Class {
public:
	int number; // Номер класса

	// Родительский класс
	int parentNumber;
	Class* parent;

	string name; // Название класса

	// Таблицы
	map<Constant, int> constants;
	map<string, Method*> methods;
	map<string, Field*> fields;
	// TODO: добавить таблицу для модификаторов доступа (для AccessModifier)

	int pushOrFindConstant(const Constant& constant) {
		// Поиск константы в таблице констант
		auto iter = constants.find(constant);

		// Если не нашли, то добавляем в таблицу
		if (iter == constants.end()) {
			++_ID;
			constants[constant] = _ID;
			return _ID;
		}

		// Если нашли, то возвращаем номер найденной константы
		return iter->second;
	}

	int pushOrFindFieldRef(const string& className, const string& fieldName, const string& type) {
		int nameNumber = pushOrFindConstant(*Constant::UTF8(fieldName));
		int typeNumber = pushOrFindConstant(*Constant::UTF8(type));
		int nameAndTypeNumber = pushOrFindConstant(*Constant::NameAndType(nameNumber, typeNumber));
		int classNumber = pushOrFindConstant(*Constant::Class(pushOrFindConstant(*Constant::UTF8(className))));
		int fieldRefNumber = pushOrFindConstant(*Constant::FieldRef(classNumber, nameAndTypeNumber));
		return fieldRefNumber;
	}

	int pushOrFindFieldRef(const string& fieldName, const string& type) {
		return pushOrFindFieldRef(this->name, fieldName, type);
	}

	int pushOrFindMethodRef(const string& className, const string& methodName, const string& descriptor) {
		int nameNumber = pushOrFindConstant(*Constant::UTF8(methodName));
		int descriptorNumber = pushOrFindConstant(*Constant::UTF8(descriptor));
		int nameAndTypeNumber = pushOrFindConstant(*Constant::NameAndType(nameNumber, descriptorNumber));
		int classNumber = pushOrFindConstant(*Constant::Class(pushOrFindConstant(*Constant::UTF8(className))));
		int methodRefNumber = pushOrFindConstant(*Constant::MethodRef(classNumber, nameAndTypeNumber));
		return methodRefNumber;
	}

	int pushOrFindMethodRef(const string& methodName, const string& descriptor) {
		return pushOrFindMethodRef(this->name, methodName, descriptor);
	}

private:
	long long _ID = 0;
};

// Функции для преобразования дерева
void transformTree(FileNode* program);
void transform(FileElementNode* programElement);
void transform(FuncNode* funcDef);
void transform(FuncArgsListNode* funcArgsList);
void transform(FuncArgNode* funcArg);
void transform(ClassNode* classDef);
void transform(ClassElementsListNode* classElementsList);
void transform(ClassElementNode* classElement);
void transform(StmtsListNode* stmtsList);
void transform(StmtNode* stmt);
void transform(ExprListNode* exprList);
void transform(ExprNode* expr);

// Функции для определения модификатора доступа элементов внутри класса
void defineAccessModifier(FuncNode* funcDef);
void defineAccessModifier(StmtNode* stmt);

// Функции для определния ошибок
void checkCompoundAssignForErrors(StmtNode* stmt);
void checkReturnValue(Class* clazz, Method* method, ExprNode* expr);
void checkMethodNameForErrors(FuncNode* funcDef);
void checkFunctionCallParams(Class* clazz, Method* method, ExprNode* expr);

// Функции для заполнения таблиц
void fillTables(FileNode* program);
void fillTables(ClassNode* classDef);
// Таблица методов
void fillMethodTable(Class* clazz, FuncNode* funcDef);
void fillMethodTable(Class* clazz, Method* method, StmtsListNode* stmts);
void fillMethodTable(Class* clazz, Method* method, StmtNode* stmt);
void fillMethodTable(Class* clazz, Method* method, ExprNode* expr);
// Таблица полей
void fillFieldTable(Class* clazz, StmtsListNode* compoundAssign);
void fillFieldTable(Class* clazz, StmtNode* assignStmt);

// RTL
void addRTLToClass(Class* clazz);

// Вспомогательные функции для заполнения таблиц
string generateMethodDescriptor(int paramsNumber, string returnValueDescriptor);
string defineMethodReturnType(Method* method);
int findElementIndexInVector(vector<string> vec, string element);
int defineMethodRefByExprNode(Class* clazz, Method* method, ExprNode* expr);
