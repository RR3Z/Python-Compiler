#pragma once
#include <string>
#include "ExprType.h"
#include "StmtType.h"
#include "FuncArgType.h"
#include "FileElementType.h"
#include "ClassElementType.h"
using namespace std;

struct ExprListNode;
struct SlicingNode;
struct ClassNode;
struct FuncNode;
struct StmtNode;
struct StmtsListNode;
struct FuncArgsListNode;

/* ========== EXPRESSION ========== */

struct ExprNode {
	// Индекс узла
	int id = -1;
	// Тип узла
	ExprType exprType;

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

	// ТОЛЬКО ДЛЯ ХРАНЕНИЯ EXPRLIST (при задании LIST; если надо сохранить exprList)
	ExprListNode* list = nullptr;

	// ТОЛЬКО ДЛЯ СОЗДАНИЯ LIST С ПОМОЩЬЮ COMPREHENSION
	ExprListNode* forHeaderList = nullptr;
	ExprListNode* ifHeaderList = nullptr;

	// ТОЛЬКО ДЛЯ ЗАДАНИЯ LIST СО SLICING
	SlicingNode* slicing = nullptr;

	// ТОЛЬКО ДЛЯ LAMBDA
	FuncArgsListNode* funcArgs = nullptr;
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

/* ========== STATEMENT ========== */

struct StmtNode {
	// Индекс узла
	int id = -1;
	// Тип узла
	StmtType stmtType;

	// Используется для компоновки нескольких StmtNode в один (например, ifStmt + elseStmt => compoundIf)
	StmtNode* leftNode = nullptr;
	StmtNode* rightNode = nullptr;

	// Используется для: assignStmt
	ExprNode* leftExpr = nullptr;
	ExprNode* rightExpr = nullptr;

	// Используется для: задания условия (condition для if, while и т.д.)
	ExprNode* expr = nullptr;

	// Используется для задания списков (stmtsList, exprList)
	ExprListNode* list = nullptr;
	StmtsListNode* stmtsList = nullptr;

	// ТОЛЬКО ДЛЯ TRY STMT
	StmtNode* tryStmt = nullptr;

	// ТОЛЬКО ДЛЯ EXCEPT as IDENTIFIER
	ExprNode* identifier = nullptr;

	// ТОЛЬКО ДЛЯ SUITE
	StmtsListNode* suite = nullptr;

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

/* ========== FUNC ARG ========== */

struct FuncArgNode {
	// Индекс узла
	int id = -1;
	// Тип узла
	FuncArgType funcArgType;

	// Для именованного аргумента
	StmtNode* assignStmt = nullptr;

	// Имя (id) аргумента
	string identifier = "";

	// Следующий узел в FuncArgsListNode
	FuncArgNode* next = nullptr;
};

/* ========== FUNC ARGS LIST ========== */

struct FuncArgsListNode {
	// Индекс узла
	int id = -1;

	// Первый элемент списка
	FuncArgNode* first = nullptr;
	// Последний элемент списка
	FuncArgNode* last = nullptr;

	// ИСПОЛЬЗУЕТСЯ ТОЛЬКО ДЛЯ ЗАДАНИЯ FUNCARGS
	FuncArgsListNode* namedArgsList = nullptr;
	ExprListNode* exprList = nullptr;
};

/* ========== CLASS ========== */

struct ClassNode {
	// Индекс узла
	int id = -1;

	// Наименование класса
	string identifier = "";

	// Тело класса
	StmtsListNode* suite = nullptr;

	// Родительский класс
	ExprNode* base = nullptr;
};

/* ========== CLASS ELEMENT ========== */

struct ClassElementNode {
	// Индекс узла
	int id = -1;
	// Тип узла
	ClassElementType type;

	// ДЛЯ FUNCDEF
	FuncNode* funcDef = nullptr;

	// ДЛЯ STMT
	StmtNode* stmt = nullptr;

	// Следующий элемент в списке
	ClassElementNode* next = nullptr;
};

/* ========== CLASS ELEMENTS LIST ========== */

struct ClassElementsListNode {
	// Индекс узла
	int id = -1;

	// Первый элемент списка
	ClassElementNode* first = nullptr;
	// Последний элемент списка
	ClassElementNode* last = nullptr;
};

/* ========== FUNC ========== */

struct FuncNode {
	// Индекс узла
	int id = -1;

	// Наименование функции
	string identifier = "";

	// Аргументы функции
	FuncArgsListNode* args = nullptr;

	// Тело функции
	StmtsListNode* suite = nullptr;
};

/* ========== FILE ELEMENT ========== */

struct FileElementNode {
	// Индекс узла
	int id = -1;
	// Тип узла
	FileElementType elementType;

	// Следующий элемент в FileElementsListNode
	FileElementNode* next = nullptr; 
	
	// ТОЛЬКО ДЛЯ TOPLEVELSTMT
	FileElementNode* element = nullptr;

	// ТОЛЬКО ДЛЯ STMT
	StmtNode* stmt = nullptr;

	// ТОЛЬКО ДЛЯ FUNCDEF
	FuncNode* funcDef = nullptr;

	// ТОЛЬКО ДЛЯ CLASSDEF
	ClassNode* classDef = nullptr;
};

/* ========== FILE ELEMENTS LIST ========== */

struct FileElementsListNode {
	// Индекс узла
	int id = -1;

	// Первый элемент списка
	FileElementNode* first = nullptr;
	// Последний элемент списка
	FileElementNode* last = nullptr;
};

/* ========== FILE ========== */

struct FileNode {
	// Индекс узла
	int id = -1;

	// Для хранения элементов, из которых состои файл
	FileElementsListNode* elementsList = nullptr;
};
