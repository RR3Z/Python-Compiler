#pragma once
#include "../nodes/nodes.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

// Верхнеуровненая таблица (для всех классов, в том числе самой программы)
extern map<string, Class*> classesList;

struct Field {
	int number = -1;	// Номер поля
	string name = "";	// Название поля
	
	// Дескриптор не нужен (динамическая типизация - у нас будет всегда один и тот же super класс)
	// TODO: добавить модификатор доступа (для AccessModifier)
};

struct Method {
	int number = -1; // Номер метода

	// Для имени метода
	string name = "";
	int nameNumber = -1;

	// Дескриптор нужен для корректной работы JVM
	int descriptorNumber = -1;

	// Для локальных переменных (TODO: почему строки, а не int?)
	vector<string> localVars = {};

	// Ссылка на узел дерева с элементами тела
	StmtsListNode* suite = nullptr;

	// Ссылка на super класс
	int baseClassNumber = -1;
	// Ссылка на конструктор super класса
	int baseConstructorNumber = -1;

	// TODO: добавить модификатор доступа (для AccessModifier)
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

	void pushField(const string& fieldName, const string& type) {
		// Ищем/добавляем номер поля в таблице констант
		int number = pushOrFindFieldRef(fieldName, type);

		// Если поле отсутствует в таблице полей, то добавляем в таблицу
		if (fields.find(fieldName) == fields.end()) {
			Field* field = new Field();
			field->name = fieldName;
			field->number = number;
			fields[fieldName] = field;
		}
	}

	int pushOrFindFieldRef(const string& className, const string& fieldName, const string& type) {
		int nameNumber = pushOrFindConstant(Constant::Utf8(fieldName));
		int typeNumber = pushOrFindConstant(Constant::Utf8(type));
		int nameAndTypeNumber = pushOrFindConstant(Constant::NameAndType(nameNumber, typeNumber));
		int classNumber = pushOrFindConstant(Constant::Class(pushOrFindConstant(Constant::Utf8(className))));
		int fieldRefNumber = pushOrFindConstant(Constant::FieldRef(classNumber, nameAndTypeNumber));
		return fieldRefNumber;
	}

	int pushOrFindFieldRef(const string& fieldName, const string& type) {
		return pushOrFindFieldRef(this->name, fieldName, type);
	}

	int pushOrFindMethodRef(const string& className, const string& methodName, const string& descriptor) {
		int nameNumber = pushOrFindConstant(Constant::Utf8(methodName));
		int descriptorNumber = pushOrFindConstant(Constant::Utf8(descriptor));
		int nameAndTypeNumber = pushOrFindConstant(Constant::NameAndType(nameNumber, descriptorNumber));
		int classNumber = pushOrFindConstant(Constant::Class(pushOrFindConstant(Constant::Utf8(className))));
		int fieldRefNumber = pushOrFindConstant(Constant::FieldRef(classNumber, nameAndTypeNumber));
		return fieldRefNumber;
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

// Функции для определения модификатора доступа элементов внутри класса
void defineAccessModifier(FuncNode* funcDef);
void defineAccessModifier(StmtNode* stmt);

// Функции для определния ошибок
void checkCompoundAssignForErrors(StmtNode* stmt);

// Функции для заполнения таблиц
void fillTable(FileNode* program);