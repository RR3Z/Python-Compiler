#pragma once
#include "./semantic.h"

std::map<std::string, Class*> classesList;

void fillTable(FileNode* program) {
	// �������� ������, ��� ����� ����� � ���������
	Class* entryClass = new Class();
	entryClass->name = "__PROGRAM__";
	// ���������� ������ � ���������� �������
	classesList[entryClass->name] = entryClass;

	// ���������� �������� ������
	entryClass->pushOrFindConstant(*Constant::UTF8("Code")); // �� ���� �����������, ��� ��� � ��� ����� ����������� �� ���������
	entryClass->number = entryClass->pushOrFindConstant(*Constant::Class(entryClass->pushOrFindConstant(*Constant::UTF8(entryClass->name))));

	// �������� �������, ��� ����� ����� � ���������
	Method* mainMethod = new Method();
	mainMethod->name = "main";
	// ���������� �������� ������
	mainMethod->nameNumber = entryClass->pushOrFindConstant(*Constant::UTF8(mainMethod->name));
	// ����� ��� ���������� ������ ���������/JVM
	mainMethod->descriptorNumber = entryClass->pushOrFindConstant(*Constant::UTF8("([Ljava/lang/String;)V"));
	mainMethod->number = entryClass->pushOrFindMethodRef(entryClass->name, mainMethod->name, "([Ljava/lang/String;)V");
	mainMethod->localVars.push_back("args");
	// ���������� main � ������� ������� entryClass
	entryClass->methods[mainMethod->name] = mainMethod;
	// ���� ������ (���������� ������)
	mainMethod->suite = nullptr;

	// ������ ���� ���������
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

			// ��������� � ���������� ��������
			programElement = programElement->next;
		}
	}

	// �������� ������������ ������ __PROGRAM__
	Method* constructor = new Method();
	constructor->name = "__init__"; // � Python ����������� �������� � ������� __init__
	constructor->nameNumber = entryClass->pushOrFindConstant(*Constant::UTF8(constructor->name));
	constructor->descriptorNumber = entryClass->pushOrFindConstant(*Constant::UTF8("()V"));
	constructor->localVars.push_back("self"); // � Python �������� ������ �������� � self
	constructor->suite = nullptr;
	constructor->number = entryClass->pushOrFindMethodRef(entryClass->name, constructor->name, "()V");
	constructor->selfNumber = entryClass->pushOrFindMethodRef("java/lang/Object", constructor->name, "()V"); // ��� � �����, ��� ���� ������� ������ ������ �����, �� ������ Java �������
	// ���������� __init__ � ������� ������� entryClass
	entryClass->methods[constructor->name] = constructor;
}


