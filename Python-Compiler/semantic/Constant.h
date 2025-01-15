#pragma once
#include <string>
#include "./ConstantType.h"
using namespace std;

struct Constant {
	// ��� ���������
	ConstantType type;

	// �������� ���������
	int intValue;
	float floatValue;
	string strValue;

	// �����/-� ���������
	unsigned int utf8Number;
	unsigned int nameNumber;
	unsigned int typeNumber;
	unsigned int classNameNumber;
	unsigned int nameAndTypeNumber;
	unsigned int classNumber;

	static Constant* UTF8(const string& str) {
		Constant* constant = new Constant();
		constant->type = ConstantType::Utf8;
		constant->strValue = str;
		return constant;
	}
};