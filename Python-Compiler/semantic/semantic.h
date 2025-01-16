#pragma once
#include "../nodes/nodes.h"
#include <string>
#include <vector>
#include <map>
#include "./Constant.h"
#include "../codeGen/AccessFlag.h"
using namespace std;

struct Field {
	int number = -1;	// ����� ����
	string name = "";	// �������� ����
	
	// ���������� �� ����� (������������ ��������� - � ��� ����� ������ ���� � ��� �� super �����)
	 
	// ����������� �������
	AccessFlag accessModifier;
};

struct Method {
	int number = -1; // ����� ������

	// ��� ����� ������
	string name = "";
	int nameNumber = -1;

	// ���������� ����� ��� ���������� ������ JVM
	int descriptorNumber = -1;

	// ��� ��������� ���������� (TODO: ������ ������, � �� int?)
	vector<string> localVars = {};

	// ������ �� ���� ������ � ���������� ����
	StmtsListNode* suite = nullptr;

	// Super ������ Java (java/lang/Object)
	int selfMethodRef = -1;

	/*
	����� �����?
	����: � ��� �����, ��� ������ �� �����������, ���� � ��� ����� ����� �������� �������������.
	����: �������� �� ��, ��� ���������� "()V", ����������� ���� �������������� ��������, � �� ���������� ������ ��� ������ �� ������.
	*/
	// ������ �� super ����� (� ���� ������, __BASE__)
	int baseClassNumber = -1;
	// ������ �� ����������� super ������
	int baseConstructorNumber = -1;

	// ����������� �������
	AccessFlag accessModifier;
};

class Class {
public:
	int number; // ����� ������

	// ������������ �����
	int parentNumber;
	Class* parent;

	string name; // �������� ������

	// �������
	map<Constant, int> constants;
	map<string, Method*> methods;
	map<string, Field*> fields;
	// TODO: �������� ������� ��� ������������� ������� (��� AccessModifier)

	int pushOrFindConstant(const Constant& constant) {
		// ����� ��������� � ������� ��������
		auto iter = constants.find(constant);

		// ���� �� �����, �� ��������� � �������
		if (iter == constants.end()) {
			++_ID;
			constants[constant] = _ID;
			return _ID;
		}

		// ���� �����, �� ���������� ����� ��������� ���������
		return iter->second;
	}

	void pushField(const string& fieldName, const string& type) {
		// ����/��������� ����� ���� � ������� ��������
		int number = pushOrFindFieldRef(fieldName, type);

		// ���� ���� ����������� � ������� �����, �� ��������� � �������
		if (fields.find(fieldName) == fields.end()) {
			Field* field = new Field();
			field->name = fieldName;
			field->number = number;
			fields[fieldName] = field;
		}
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

// ������� ��� �������������� ������
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

// ������� ��� ����������� ������������ ������� ��������� ������ ������
void defineAccessModifier(FuncNode* funcDef);
void defineAccessModifier(StmtNode* stmt);

// ������� ��� ���������� ������
void checkCompoundAssignForErrors(StmtNode* stmt);

// ������� ��� ���������� ������
void fillTable(FileNode* program);
void fillTable(ClassNode* classDef);
void fillTable(Class* clazz, FuncNode* funcDef);
void fillTable(Class* clazz, Method* method, StmtsListNode* suite);
void fillTable(Class* clazz, Method* method, StmtNode* stmt);
void fillTable(Class* clazz, Method* method, ExprNode* expr);

// ��������������� ������� ��� ���������� ������
string generateMethodDescriptor(int paramsNumber, string returnValueDescriptor);