#pragma once
#include <string>
#include "NodeType.h"
using namespace std;

struct ExprListNode;
struct SlicingNode;
struct IdentifierListNode;

/* ========== EXPRESSION ========== */

struct ExprNode {
	// Индекс узла
	int id = -1;
	// Тип узла
	NodeType exprType = _UNKNOWN;

	// Название переменной
	string identifier = "";

	// Значения
	int boolVal = -1;
	float floatVal = -1.0f;
	int intVal = -1;
	string stringVal = "";

	// Для бинарного expr
	ExprNode* left = nullptr;
	ExprNode* right = nullptr;

	// ТОЛЬКО ДЛЯ EXPRLIST
	ExprNode* next = nullptr;

	// ТОЛЬКО ДЛЯ ЗАДАНИЯ LIST - [1,2,3]
	ExprListNode* list = nullptr;

	// ТОЛЬКО ДЛЯ ЗАДАНИЯ LIST СО SLICING
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

	// С какого значения (включительно)
	ExprNode* start = nullptr;
	// До какого значения (не включительно)
	ExprNode* end = nullptr;
	// Шаг
	ExprNode* step = nullptr;
};

/* ========== IDENTIFIERS ========== */

struct IdentifierListNode {
	// Индекс узла
	int id = -1;

	// Первый элемент списка
	ExprNode* first = nullptr;
	// Последний элемент списка
	ExprNode* last = nullptr;
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

	// Следующий expr в списке targetList (используется только при работе с targetList)
	TargetNode* next = nullptr;

	// Используется для: списка элементов list (при его создании)
	TargetListNode* list = nullptr;
};

/* ========== TARGET LIST ========== */

struct TargetListNode {
	// Индекс узла
	int id = -1;

	// Первый элемент списка
	TargetNode* first = nullptr;
	// Последний элемент списка
	TargetNode* last = nullptr;
};
