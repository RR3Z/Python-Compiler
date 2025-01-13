#pragma once

#include "./semantic.h"

void fillTable(FileNode* program) {
	/*
	// Класс как точка входа в программу
	Class* entryClass = new Class();
	entryClass->name = "__PROGRAM__";
	classesList[entryClass->name] = entryClass;
	entryClass->pushOrFindConstant(Constant::Utf8("Code"));
	entryClass->number = entryClass->pushOrFindConstant(Constant::Class(entryClass->pushOrFindConstant(Constant::Utf8(entryClass->name))));

	// Наследуемый класс
	// Замечание: мы тут не можем явно прописать класс для parent, т.к. это java класс, а не наш собственный.
	int parentClassName = entryClass->pushOrFindConstant(Constant::Utf8("Ljava/lang/Object;"));
	entryClass->parentNumber = entryClass->pushOrFindConstant(Constant::Class(parentClassName));

	// Точка входа в программу
	/* 
	Замечание: в Python нет явной точки входа в программу(в Ruby есть - метод main).
	Поэтому нам по хорошему реализовать что-то типа гибридного подхода:
	1) if __name__ == "__main__": *code*
	2) Просто код

	Иначе говоря:
	Нет явной точки входа: нет необходимости в методе main. Вместо этого можно будет обрабатывать любые операторы кода, который будет выполняться в области видимости.

	По факту мы должны создать метод __main__, как я понял. 
	Надо где-то про это почитать.
	*/

	// Обработка элементов программы (функции,классы,переменные и тд)


	// Инициализатор/конструктор этого класса
}