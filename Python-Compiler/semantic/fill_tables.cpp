#pragma once
#include "./semantic.h"

std::map<std::string, Class*> classesList;

void fillTable(FileNode* program) {
	// Создание класса, как точки входа в программу
	Class* entryClass = new Class();
	entryClass->name = "__PROGRAM__";
	// Добавление класса в глобальную таблицу
	classesList[entryClass->name] = entryClass;

	// Добавление констант класса
	entryClass->pushOrFindConstant(*Constant::UTF8("Code")); // По идее добавляется, так как у нас будет конструктор по умолчанию
	entryClass->number = entryClass->pushOrFindConstant(*Constant::Class(entryClass->pushOrFindConstant(*Constant::UTF8(entryClass->name))));

	// Создание функции, как точки входа в программу
	Method* mainMethod = new Method();
	mainMethod->name = "main";
	// Добавление констант метода
	mainMethod->nameNumber = entryClass->pushOrFindConstant(*Constant::UTF8(mainMethod->name));
	// Нужен для корректной работы программы/JVM
	mainMethod->descriptorNumber = entryClass->pushOrFindConstant(*Constant::UTF8("([Ljava/lang/String;)V"));
	mainMethod->number = entryClass->pushOrFindMethodRef(entryClass->name, mainMethod->name, "([Ljava/lang/String;)V");
	mainMethod->localVars.push_back("args");
	// Добавление main в таблицу методов entryClass
	entryClass->methods[mainMethod->name] = mainMethod;
	// Тело метода (изначально пустое)
	mainMethod->suite = nullptr;

	// Разбор кода программы
	if (program != nullptr && program->elementsList != nullptr) {
		FileElementNode* programElement = program->elementsList->first;

		while (programElement != nullptr) {
			// TODO
			switch (programElement->elementType)
			{
				case _CLASS_DEF:
					break;
				case _FUNC_DEF:
					break;
				case _STMT:
					break;
			}

			// Переходим к следующему элементу
			programElement = programElement->next;
		}
	}

	// Создание конструктора класса __PROGRAM__
	Method* constructor = new Method();
	constructor->name = "__init__"; // в Python конструктор задается с помощью __init__
	constructor->nameNumber = entryClass->pushOrFindConstant(*Constant::UTF8(constructor->name));
	constructor->descriptorNumber = entryClass->pushOrFindConstant(*Constant::UTF8("()V"));
	constructor->localVars.push_back("self"); // в Python экземляр класса хранится в self
	constructor->suite = nullptr;
	constructor->number = entryClass->pushOrFindMethodRef(entryClass->name, constructor->name, "()V");
	constructor->selfNumber = entryClass->pushOrFindMethodRef("java/lang/Object", constructor->name, "()V"); // Как я понял, нам надо создать объект нашего языка, на основе Java объекта
	// Добавление __init__ в таблицу методов entryClass
	entryClass->methods[constructor->name] = constructor;
}


