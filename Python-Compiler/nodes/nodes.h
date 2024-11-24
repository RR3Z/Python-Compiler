#pragma once
#include <string>
#include "../ExprType.h"

using namespace std;

/* ========== EXPRESSION ========== */

struct ExprNode {
	// ������ ����
	int id = -1;
	// ��� ����
	ExprType exprType = _UNKNOWN;

	// ������������� - �������� ����������, �������, ������, ���������, ������ � �.�.
	string identifier = "";

	// ��� �������� �������� ��������
	int boolVal = -1;
	float floatVal = -1.0f;
	int intVal = -1;
	string stringVal = "";

	// ��� ��������� expr
	ExprNode* left = nullptr;
	ExprNode* right = nullptr;

	// ��������� expr � ������ exprList (������������ ������ ��� ������ � exprList)
	ExprNode* next = nullptr;
};

/* ========== EXPRESSION LIST ========== */

struct ExprListNode {
	// ������ ����
	int id = -1;

	// ������ ������� ������
	ExprNode* first = nullptr;
	// ��������� ������� ������
	ExprNode* last = nullptr;
};

/* ========== ARRAY SLICE ========== */

struct ArraySliceNode {
	// ������ ����
	int id = -1;

	// ������ �������, ������� ����������
	ExprNode* start = nullptr;
	// ������� ����� �������� �������������
	ExprNode* end = nullptr;
	// ���, � ������� ���������� ��������
	ExprNode* step = nullptr;
};