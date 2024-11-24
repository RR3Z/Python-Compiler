#pragma once
#include <string>
#include "../ExprType.h"

using namespace std;

/* ========== EXPRESSION ========== */

struct ExprNode {
	// Индекс узла
	int id = -1;
	// Тип узла
	ExprType exprType = _UNKNOWN;

	// Идентификатор - название переменной, функции, метода, аттрибута, класса и т.д.
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

/* ========== ARRAY SLICE ========== */

struct ArraySliceNode {
	// Индекс узла
	int id = -1;

	// Первый элемент, который включается
	ExprNode* start = nullptr;
	// Элемент после которого заканчивается
	ExprNode* end = nullptr;
	// Шаг, с которым выбираются элементы
	ExprNode* step = nullptr;
};