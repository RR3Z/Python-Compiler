#pragma once
#include <string>
#include "../ExprType.h"
using namespace std;

struct ExprListNode;
struct SlicingNode;
struct IdentifierListNode;

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

	// Используется для: списка элементов list (при его создании)
	ExprListNode* list = nullptr;

	// Используяется для: указания значений для arraySlice
	SlicingNode* slicing = nullptr;

	// Используется для: списка элементов list (при его создании)
	IdentifierListNode* listId = nullptr;
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

/* ========== IDENTIFIERS ========== */

struct IdentifierListNode {
	// Индекс узла
	int id = -1;

	// Первый элемент списка
	ExprNode* firstId = nullptr;
	// Последний элемент списка
	ExprNode* lastId = nullptr;
};