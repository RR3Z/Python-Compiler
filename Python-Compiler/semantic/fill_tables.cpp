#pragma once

#include "./semantic.h"

void fillTable(FileNode* program) {
	/*
	// ����� ��� ����� ����� � ���������
	Class* entryClass = new Class();
	entryClass->name = "__PROGRAM__";
	classesList[entryClass->name] = entryClass;
	entryClass->pushOrFindConstant(Constant::Utf8("Code"));
	entryClass->number = entryClass->pushOrFindConstant(Constant::Class(entryClass->pushOrFindConstant(Constant::Utf8(entryClass->name))));

	// ����������� �����
	// ���������: �� ��� �� ����� ���� ��������� ����� ��� parent, �.�. ��� java �����, � �� ��� �����������.
	int parentClassName = entryClass->pushOrFindConstant(Constant::Utf8("Ljava/lang/Object;"));
	entryClass->parentNumber = entryClass->pushOrFindConstant(Constant::Class(parentClassName));

	// ����� ����� � ���������
	/* 
	���������: � Python ��� ����� ����� ����� � ���������(� Ruby ���� - ����� main).
	������� ��� �� �������� ����������� ���-�� ���� ���������� �������:
	1) if __name__ == "__main__": *code*
	2) ������ ���

	����� ������:
	��� ����� ����� �����: ��� ������������� � ������ main. ������ ����� ����� ����� ������������ ����� ��������� ����, ������� ����� ����������� � ������� ���������.

	�� ����� �� ������ ������� ����� __main__, ��� � �����. 
	���� ���-�� ��� ��� ��������.
	*/

	// ��������� ��������� ��������� (�������,������,���������� � ��)


	// �������������/����������� ����� ������
}