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

	// ������������� (����������, �������, ������, ���������, �����)
	string identifier = "";

	// ��� �������� �������� ��������
	int boolVal = -1;
	float floatVal = -1.0f;
	int intVal = -1;
	string stringVal = "";

	// ��� ��������� expr
	struct ExprNode* left = nullptr;
	struct ExprNode* right = nullptr;
};