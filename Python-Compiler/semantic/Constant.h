#pragma once
#include <string>
#include "./ConstantType.h"
using namespace std;

struct Constant {
	// Тип константы
	ConstantType type;

	// Значение константы
	int intValue;
	float floatValue;
	string strValue;

	// Номер/-а константы
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
	static Constant* Integer(int value) {
		Constant* constant = new Constant();
		constant->type = ConstantType::Integer;
		constant->intValue = value;
		return constant;
	}

	static Constant* Float(float value) {
		Constant* constant = new Constant();
		constant->type = ConstantType::Float;
		constant->floatValue = value;
		return constant;
	}

	static Constant* String(int utf8Number) {
		Constant* constant = new Constant();
		constant->type = ConstantType::String;
		constant->utf8Number = utf8Number;
		return constant;
	}

	static Constant* Class(int utf8Number) {
		Constant* constant = new Constant();
		constant->type = ConstantType::Class;
		constant -> classNameNumber = utf8Number;
		return constant;
	}

	static Constant* FieldRef(int classNumber, int nameAndTypeNumber) {
		Constant* constant = new Constant();
		constant->type = ConstantType::Fieldref;
		constant->classNumber = classNumber;
		constant->nameAndTypeNumber = nameAndTypeNumber;
		return constant;
	}

	static Constant* MethodRef(int classNumber, int nameAndTypeNumber) {
		Constant* constant = new Constant();
		constant->type = ConstantType::Methodref;
		constant->classNumber = classNumber;
		constant->nameAndTypeNumber = nameAndTypeNumber;
		return constant;
	}

	static Constant* NameAndType(int nameUtf8Number, int descriptorUtf8Number) {
		Constant* constant = new Constant();
		constant->type = ConstantType::NameAndType;
		constant->nameNumber = nameUtf8Number;
		constant->typeNumber = descriptorUtf8Number;
		return constant;
	}

	friend bool operator==(const Constant& l, const Constant& r) {
		if (l.type != r.type) return false;
		switch (l.type)
		{
		case ConstantType::Utf8:
			return l.strValue == r.strValue;
		case  ConstantType::Integer:
			return l.intValue == r.intValue;
		case  ConstantType::Float:
			return l.floatValue == r.floatValue;
		case  ConstantType::String:
		case  ConstantType::Class:
			return l.utf8Number == r.utf8Number;
		case  ConstantType::NameAndType:
			return (l.nameNumber == r.nameNumber) && (l.typeNumber == r.typeNumber);
		case  ConstantType::Methodref:
		case  ConstantType::Fieldref:
			return (l.nameAndTypeNumber == r.nameAndTypeNumber) && (l.classNameNumber == r.classNameNumber);
		}
		return false;
	}

	friend bool operator<(const Constant l, const Constant r) {
		if (l.type < r.type) {
			return true;
		}
		else if (l.type == r.type) {
			switch (l.type) {
			case ConstantType::Utf8:
				return l.strValue < r.strValue;
			case ConstantType::Float:
				return l.floatValue < r.floatValue;
			case ConstantType::Integer:
				return l.intValue < r.intValue;
			case ConstantType::String:
				return l.utf8Number < r.utf8Number;
			case ConstantType::Class:
				return l.classNameNumber < r.classNameNumber;
			case ConstantType::NameAndType:
				return l.nameNumber < r.nameNumber || ((l.nameNumber == r.nameNumber) && (l.typeNumber < r.typeNumber));
			case ConstantType::Fieldref:
			case ConstantType::Methodref:
				return l.classNumber < r.classNumber || ((l.classNumber == r.classNumber) && (l.nameAndTypeNumber < r.nameAndTypeNumber));
			}
		}

		return false;
	}
};