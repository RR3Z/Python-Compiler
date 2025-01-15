#pragma once
#include "../nodes/nodes.h"
#include <string>
#include <vector>
#include <map>
using namespace std;

// ��������������� ������� (��� ���� �������, � ��� ����� ����� ���������)
extern map<string, Class*> classesList;

struct Field {
	int number = -1;	// ����� ����
	string name = "";	// �������� ����
	
	// ���������� �� ����� (������������ ��������� - � ��� ����� ������ ���� � ��� �� super �����)
	// TODO: �������� ����������� ������� (��� AccessModifier)
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

	// ������ �� super �����
	int baseClassNumber = -1;
	// ������ �� ����������� super ������
	int baseConstructorNumber = -1;

	// TODO: �������� ����������� ������� (��� AccessModifier)
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