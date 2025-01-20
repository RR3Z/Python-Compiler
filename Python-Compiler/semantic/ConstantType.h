/*
	Почему такие значения?
	https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.4
*/
#ifndef CONSTANTTYPE_H
#define CONSTANTTYPE_H

enum class ConstantType {
	Utf8 = 1,
	Integer = 3,
	Float = 4,
	String = 8,
	Boolean = 0, // нет значения
	Class = 7,
	FieldRef = 9,
	MethodRef = 10,
	InterfaceMethodRef = 11,
	NameAndType = 12
};

#endif // CONSTANTTYPE_H
