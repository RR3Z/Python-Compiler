#pragma once
#include <string>
#include "NodeType.h"
using namespace std;

struct ExprListNode;
struct SlicingNode;

/* ========== EXPRESSION ========== */

struct ExprNode {
	// Индекс узла
	int id = -1;
	// Тип узла
	NodeType exprType = _UNKNOWN;

	// Идентификатор - название переменной, функции, метода и т.д.
	string identifier = "";

	// Для хранения значений констант
	int boolVal = -1;
	float floatVal = -1.0f;
	int intVal = -1;
	string stringVal = "";

	// Для бинарного expr
	ExprNode* left = nullptr;
	ExprNode* right = nullptr;

	// Следующий expr в списке exprList (используется только при работе с exprList)
	ExprNode* next = nullptr;

	// Используется для: списка элементов list (при его создании)
	ExprListNode* list = nullptr;

	// Используяется для: указания значений для slicing
	SlicingNode* slicing = nullptr;
};

/* ========== EXPRESSION LIST ========== */

struct ExprListNode {
	// Индекс узла
	int id = -1;

	// Первый элемент списка
	ExprNode* first = nullptr;
	// Последний элемент списка
	ExprNode* last = nullptr;
};

/* ========== SLICING ========== */

struct SlicingNode {
	// Индекс узла
	int id = -1;

	// Первый элемент, который включается
	ExprNode* start = nullptr;
	// Элемент после которого заканчивается
	ExprNode* end = nullptr;
	// Шаг, с которым выбираются элементы
	ExprNode* step = nullptr;
};

/* ========== TARGET ========== */

struct TargetNode {
	// Индекс узла
	int id = -1;
	// Тип узла
	NodeType targetType = _UNKNOWN;

	// Идентификатор - название переменной, функции, метода, аттрибута, класса и т.д.
	string identifier = "";

	// Для бинарного target
	ExprNode* left = nullptr;
	ExprNode* right_expr = nullptr;
	TargetNode* right_target = nullptr;

	// Используяется для: указания значений для slicing
	SlicingNode* slicing = nullptr;
};

