#pragma once

#include "../nodes/nodes.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

struct Field {
	bool isClassVar = false;
	string name = "";

	int number = -1; // Номер поля/переменной в таблице
};

struct Method {
	bool isInstanceMethod = false;
	string name = "";
	vector<string> localVars = {};
	StmtsListNode* suite = nullptr;

	int number = -1; // Номер метода/функции в таблице
	int nameNumber = -1; // Номер имени метода/функции в таблице
	int descriptorNumber = -1; // Номер дескриптора метода/функции в таблице
	int selfMethodRef = -1; // Номер ссылки на этот же метод/функцию в таблице
};

class Class {
	/*
public:
	int number = -1; // Номер класса в таблице
	int parentNumber = -1; // Номер родительского класса в таблице

	string name = "";
	Class* parent = nullptr; // Ссылка на родительский класс (одиночное наследование)

	// Таблицы
	map<Constant, int> constants = {};
	map<string, Method*> methods = {};
	map<string, Field*> fields = {};

	// Функции для работы с таблицами

	int pushOrFindConstant(const Constant& constant) {
		auto iter = constants.find(constant);

		if (iter == constants.end()) {
			id++;
			constants[constant] = id;
			return id;
		}

		return iter->second; // second - значение, first - ключ
	}

	void addField(const string& fieldName, const string& type) {
		// Поиск/добавление в таблицу констант
		int id = pushOrFindFieldRef(fieldName, type);

		// Поиск переменной в таблице полей
		if (fields.find(fieldName) == fields.end()) {
			Field* field = new Field();
			field->isClassVar = true;
			field->name = fieldName;
			field->number = id;
			// Добавление в таблицу полей
			fields[fieldName] = field;
		}
	}

	int pushOrFindFieldRef(const string& className, const string& fieldName, const string& type) {
		// Создание/поиск переменной в таблице констант
		int nameId = pushOrFindConstant(Constant::Utf8(fieldName));
		int typeId = pushOrFindConstant(Constant::Utf8(type));
		int nameAndTypeId = pushOrFindConstant(Constant::NameAndType(nameId, typeId));

		// Создание/поиск класса в таблице констант
		int classId = pushOrFindConstant(Constant::Class(pushOrFindConstant(Constant::Utf8(className))));

		// Связывание поля с классом (которому он принадлежит)
		int fileldRefId = pushOrFindConstant(Constant::FieldRef(classId, nameAndTypeId));
		return fileldRefId;
	}

	int pushOrFindFieldRef(const string& fieldName, const string& type) {
		return pushOrFindFieldRef(this->name, fieldName, type);
	}

	int pushOrFindMethodRef(const string& className, const string& methodName, const string& descriptor) {
		// Создание/поиск метода в таблице констант
		int nameId = pushOrFindConstant(Constant::Utf8(methodName));
		int descriptorId = pushOrFindConstant(Constant::Utf8(descriptor));
		int nameAndTypeId = pushOrFindConstant(Constant::NameAndType(nameId, descriptorId));

		// Создание/поиск класса в таблице констант
		int classId = pushOrFindConstant(Constant::Class(pushOrFindConstant(Constant::Utf8(className))));

		// Связывание метода с классом (которому он принадлежит)
		int methodRefId = pushOrFindConstant(Constant::MethodRef(classId, nameAndTypeId));
		return methodRefId;
	}
	
	int pushOrFindMethodRef(const string& methodName, const string& descriptor) {
		return pushOrFindMethodRef(this->name, methodName, descriptor);
	}
	
	*/
private:
	long long id = 0;
};

extern map<string, Class*> classesList = {};

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

// Функции для заполнения таблиц
void fillTable(FileNode* program);