#pragma once
#include <string>
#include "NodeType.h"
using namespace std;

struct ExprListNode;
struct SlicingNode;
struct IdentifierListNode;

/* ========== EXPRESSION ========== */

struct ExprNode {
	// ������ ����
	int id = -1;
	// ��� ����
	NodeType exprType = _UNKNOWN;

	// ������������� - �������� ����������, �������, ������ � �.�.
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

	// ������������ ���: ������ ��������� list (��� ��� ��������)
	ExprListNode* list = nullptr;

	// ������������� ���: �������� �������� ��� slicing
	SlicingNode* slicing = nullptr;

	// ������������ ���: ������ ��������� list (��� ��� ��������)
	IdentifierListNode* listId = nullptr;
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

/* ========== SLICING ========== */

struct SlicingNode {
	// ������ ����
	int id = -1;

	// ������ �������, ������� ����������
	ExprNode* start = nullptr;
	// ������� ����� �������� �������������
	ExprNode* end = nullptr;
	// ���, � ������� ���������� ��������
	ExprNode* step = nullptr;
};

/* ========== IDENTIFIERS ========== */

struct IdentifierListNode {
	// ������ ����
	int id = -1;

	// ������ ������� ������
	ExprNode* firstId = nullptr;
	// ��������� ������� ������
	ExprNode* lastId = nullptr;
};
/* ========== TARGET ========== */

struct TargetNode {
	// ������ ����
	int id = -1;
	// ��� ����
	NodeType targetType = _UNKNOWN;

	// ������������� - �������� ����������, �������, ������, ���������, ������ � �.�.
	string identifier = "";

	// ��� ��������� target
	ExprNode* left = nullptr;
	ExprNode* right_expr = nullptr;
	TargetNode* right_target = nullptr;

	// ������������� ���: �������� �������� ��� slicing
	SlicingNode* slicing = nullptr;
};

