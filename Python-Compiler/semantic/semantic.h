#pragma once

#include "../nodes/nodes.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

struct Field {
	bool isClassVar = false;
	string name = "";

	int number = -1; // ����� ����/���������� � �������
};

struct Method {
	bool isInstanceMethod = false;
	string name = "";
	vector<string> localVars = {};
	StmtsListNode* suite = nullptr;

	int number = -1; // ����� ������/������� � �������
	int nameNumber = -1; // ����� ����� ������/������� � �������
	int descriptorNumber = -1; // ����� ����������� ������/������� � �������
	int selfMethodRef = -1; // ����� ������ �� ���� �� �����/������� � �������
};

class Class {
	/*
public:
	int number = -1; // ����� ������ � �������
	int parentNumber = -1; // ����� ������������� ������ � �������

	string name = "";
	Class* parent = nullptr; // ������ �� ������������ ����� (��������� ������������)

	// �������
	map<Constant, int> constants = {};
	map<string, Method*> methods = {};
	map<string, Field*> fields = {};

	// ������� ��� ������ � ���������

	int pushOrFindConstant(const Constant& constant) {
		auto iter = constants.find(constant);

		if (iter == constants.end()) {
			id++;
			constants[constant] = id;
			return id;
		}

		return iter->second; // second - ��������, first - ����
	}

	void addField(const string& fieldName, const string& type) {
		// �����/���������� � ������� ��������
		int id = pushOrFindFieldRef(fieldName, type);

		// ����� ���������� � ������� �����
		if (fields.find(fieldName) == fields.end()) {
			Field* field = new Field();
			field->isClassVar = true;
			field->name = fieldName;
			field->number = id;
			// ���������� � ������� �����
			fields[fieldName] = field;
		}
	}

	int pushOrFindFieldRef(const string& className, const string& fieldName, const string& type) {
		// ��������/����� ���������� � ������� ��������
		int nameId = pushOrFindConstant(Constant::Utf8(fieldName));
		int typeId = pushOrFindConstant(Constant::Utf8(type));
		int nameAndTypeId = pushOrFindConstant(Constant::NameAndType(nameId, typeId));

		// ��������/����� ������ � ������� ��������
		int classId = pushOrFindConstant(Constant::Class(pushOrFindConstant(Constant::Utf8(className))));

		// ���������� ���� � ������� (�������� �� �����������)
		int fileldRefId = pushOrFindConstant(Constant::FieldRef(classId, nameAndTypeId));
		return fileldRefId;
	}

	int pushOrFindFieldRef(const string& fieldName, const string& type) {
		return pushOrFindFieldRef(this->name, fieldName, type);
	}

	int pushOrFindMethodRef(const string& className, const string& methodName, const string& descriptor) {
		// ��������/����� ������ � ������� ��������
		int nameId = pushOrFindConstant(Constant::Utf8(methodName));
		int descriptorId = pushOrFindConstant(Constant::Utf8(descriptor));
		int nameAndTypeId = pushOrFindConstant(Constant::NameAndType(nameId, descriptorId));

		// ��������/����� ������ � ������� ��������
		int classId = pushOrFindConstant(Constant::Class(pushOrFindConstant(Constant::Utf8(className))));

		// ���������� ������ � ������� (�������� �� �����������)
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

// ������� ��� ���������� ������
void fillTable(FileNode* program);