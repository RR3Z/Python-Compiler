#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "./codeGen.h"

FILE* fileClass;

/*
	main - ��� ������������� ����������.
	main - ��� ����, ���� ������ ��������� ����.
*/

/* ========= ������� ��������� ���� ���� ========= */

/*
	��� ���������, ��� �� ����� ��. class file format � ������������ JVM ��� � ���������.
	https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.1
*/
void generate(FileNode* program, const map<string, Class*>& classesList) {
	for (auto clazz : classesList) {
		// ��� constant pool count � ���������� � 1, � �� � ���� (������� +1)
		vector<char> constantPoolCount = intToBytes(clazz.second->constants.size() + 1);

		// ���� ��� ����������
		string fileName = clazz.second->name + ".class";
		fopen_s(&fileClass, fileName.c_str(), "wb");

		// magic (u4) - CAFEBABE
		fprintf(fileClass, "%c%c%c%c", 0xCA, 0xFE, 0xBA, 0xBE);

		// version - minor (u2) + major (u2) - JAVA 8 (version 52.0 (0x34))
		fprintf(fileClass, "%c%c%c%c", 0x00, 0x00, 0x00, 0x34);

		// constant pool count (u2)
		fprintf(fileClass, "%c%c", constantPoolCount[2], constantPoolCount[3]);

		// constant pool (u2)
		vector<pair<Constant, int>> constantsPool;
		for (auto& it : clazz.second->constants) {
			constantsPool.push_back(it);
		}
		sort(constantsPool.begin(), constantsPool.end(), compare);
		for (auto i : constantsPool) {
			generateConstantCode(i.first);
		}

		// access flags (u2) - ACC_SUPER (0x20) + ACC_PUBLIC (0x01)
		fprintf(fileClass, "%c%c", 0x00, 0x21);

		// this class (u2)
		vector<char> bytes = intToBytes(clazz.second->number);
		fprintf(fileClass, "%c%c", bytes[2], bytes[3]);

		// super class (u2) - parent
		bytes = intToBytes(clazz.second->parentNumber);
		fprintf(fileClass, "%c%c", bytes[2], bytes[3]);

		// interfaces count (u2) - �� ��������
		fprintf(fileClass, "%c%c", 0x00, 0x00);

		// ========= FIELDS ========= TODO
		// fields count (u2)
		//bytes = intToBytes(clazz.second->fields.size());
		//fprintf(fileClass, "%c%c", bytes[2], bytes[3]);
		fprintf(fileClass, "%c%c", 0x00, 0x00);
		// fields pool TODO
		fprintf(fileClass, "%c%c", 0x00, 0x00);

		// ========= METHODS ========= TODO
		// methods count (u2)
		//bytes = intToBytes(clazz.second->methods.size());
		//fprintf(fileClass, "%c%c", bytes[2], bytes[3]);
		fprintf(fileClass, "%c%c", 0x00, 0x00);
		// methods pool
		fprintf(fileClass, "%c%c", 0x00, 0x00);
		// ����������� ������
		for (auto it = clazz.second->methods.begin(); it != clazz.second->methods.end(); it++) {
			//generateMethodCode(it->second, clazz.second);
		}
		// Main �����

		// attributes count (u2) - �� ����� �� ������ ������
		fprintf(fileClass, "%c%c", 0x00, 0x00);

		// ������� ����
		fclose(fileClass);
	}
}

void generateConstantCode(Constant constant) {
	// UTF8
	if (constant.type == ConstantType::Utf8) {
		// �������������� string � char*
		const char* c = constant.strValue.c_str();

		// ��� ��������� (u1)
		fprintf(fileClass, "%c", (char)ConstantType::Utf8);

		// ����� ������ � ������ (u2)
		vector<char> len = intToBytes(strlen(c));
		fprintf(fileClass, "%c%c", (char)len[2], (char)len[3]);

		// ������� ������ � ���� ������������������ ���� ��������� ����� (u1[])
		for (int i = 0; i < strlen(c); ++i) {
			fprintf(fileClass, "%c", c[i]);
		}
	}

	// Integer
	if (constant.type == ConstantType::Integer) {
		// ��� ��������� (u1)
		fprintf(fileClass, "%c", (char)ConstantType::Integer);

		// ����� �� ������ (s4)
		vector<char> len = intToBytes(constant.intValue);
		fprintf(fileClass, "%c%c%c%c", len[0], len[1], len[2], len[3]);
	}

	// Float
	if (constant.type == ConstantType::Float) {
		// ��� ��������� (u1)
		fprintf(fileClass, "%c", (char)ConstantType::Float);

		// ����� �� ������ (s4)
		vector<char> len = floatToBytes(constant.floatValue);
		fprintf(fileClass, "%c%c%c%c", len[0], len[1], len[2], len[3]);
	}

	// String
	if (constant.type == ConstantType::String) {
		// ��� ��������� (u1)
		fprintf(fileClass, "%c", (char)ConstantType::String);

		// ����� ��������� UTF8 - �������� �������� ��������� (u2)
		vector<char> len = intToBytes(constant.utf8Number);
		fprintf(fileClass, "%c%c", len[2], len[3]);
	}

	// NameAndType
	if (constant.type == ConstantType::NameAndType) {
		// ��� ��������� (u1)
		fprintf(fileClass, "%c", (char)ConstantType::NameAndType);

		// ����� ��������� UTF8 - ��� ����/������ (u2)
		vector<char> len = intToBytes(constant.nameNumber);
		fprintf(fileClass, "%c%c", len[2], len[3]);

		// ����� ��������� UTF8 - ���������� ����/������ (u2)
		len = intToBytes(constant.typeNumber);
		fprintf(fileClass, "%c%c", len[2], len[3]);
	}

	// Class
	if (constant.type == ConstantType::Class) {
		// ��� ��������� (u1)
		fprintf(fileClass, "%c", (char)ConstantType::Class);

		// ����� ��������� UTF8 - ������ ����������������� ��� ������ (u2)
		vector<char> len = intToBytes(constant.classNameNumber);
		fprintf(fileClass, "%c%c", len[2], len[3]);
	}

	// FieldRef
	if (constant.type == ConstantType::FieldRef) {
		// ��� ��������� (u1)
		fprintf(fileClass, "%c", (char)ConstantType::FieldRef);

		// ����� ��������� Class - �����, �������� ����������� ���� (u2)
		vector<char> len = intToBytes(constant.classNumber);
		fprintf(fileClass, "%c%c", len[2], len[3]);

		// ����� ��������� NameAndType - ��� � ���������� ���� (u2)
		len = intToBytes(constant.nameAndTypeNumber);
		fprintf(fileClass, "%c%c", len[2], len[3]);
	}

	// MethodRef
	if (constant.type == ConstantType::MethodRef) {
		// ��� ��������� (u1)
		fprintf(fileClass, "%c", (char)ConstantType::MethodRef);

		// ����� ��������� Class - �����, �������� ����������� ����� (u2)
		vector<char> len = intToBytes(constant.classNumber);
		fprintf(fileClass, "%c%c", len[2], len[3]);

		// ����� ��������� NameAndType - ��� � ���������� ������ (u2)
		len = intToBytes(constant.nameAndTypeNumber);
		fprintf(fileClass, "%c%c", len[2], len[3]);
	}
}

// TODO
void generateDefaultInitMethod(Method* initMethod) {

}

void generateMethodCode(Method* method, Class* clazz) {
	// access flags (u2)
	fprintf(fileClass, "%c%c", 0x00, method->accessModifier);

	// name index (u2)
	vector<char> bytes = intToBytes(method->nameNumber);
	fprintf(fileClass, "%c%c", bytes[2], bytes[3]);

	// descriptor index (u2)
	bytes = intToBytes(method->descriptorNumber);
	fprintf(fileClass, "%c%c", bytes[2], bytes[3]);

	// attributes count (u2)
	fprintf(fileClass, "%c%c", 0x00, 0x01);

	// attributes
	generateAttributeCode(method, clazz);
}

// TODO
void generateAttributeCode(Method* method, Class* clazz) {

}

/* ========= ��������������� ������� ========= */

vector <char> intToBytes(int value) {
	// int = 4 bytes
	vector<char> arrayOfByte(4);

	for (int i = 0; i < 4; ++i) {
		arrayOfByte[3 - i] = (value >> (i * 8));
	}

	return arrayOfByte;
}

vector<char> floatToBytes(float value)
{
	// float = 4 bytes
	vector<char> arrayOfByte(4);

	for (int i = 0; i < sizeof(float); ++i)
		arrayOfByte[3 - i] = ((char*)&value)[i];

	return arrayOfByte;
}

bool compare(pair<Constant, int>& a, pair<Constant, int>& b) {
	return a.second < b.second;
}


