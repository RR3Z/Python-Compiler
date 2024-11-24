#pragma once
#include <string>
#include "NodeType.h"
#include "StmtType.h"
using namespace std;

struct ExprListNode;
struct SlicingNode;
struct StmtsListNode;

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

/* ========== TARGET LIST ========== */

struct TargetListNode {
	// Индекс узла
	int id = -1;

	// Первый элемент списка
	ExprNode* first = nullptr;
	// Последний элемент списка
	ExprNode* last = nullptr;
};

/* ========== STATEMENT ========== */

struct StmtNode {
	// Индекс узла
	int id = -1;
	// Тип узла
	StmtType stmtType = _UNDEFINED;

	// Используется для компоновки нескольких StmtNode в один (например, ifStmt + elseStmt => compoundIf)
	StmtNode* leftNode = nullptr;
	StmtNode* rightNode = nullptr;

	// Используется для: задания самого условия (if, else)
	ExprNode* expr = nullptr;

	// Используется для задания тела stmt
	StmtsListNode* suite = nullptr;

	// Используется, если для одного stmt можно задать несколько условий (например, elifStmtsList в ifStmt)
	StmtsListNode* stmtsList = nullptr;

	// ТОЛЬКО ДЛЯ STMTSLIST
	StmtNode* next = nullptr;
};

/* ========== STATEMENTS LIST ========== */

struct StmtsListNode {
	// Индекс узла
	int id = -1;

	// Первый элемент списка
	StmtNode* first = nullptr;
	// Последний элемент списка
	StmtNode* last = nullptr;
};
