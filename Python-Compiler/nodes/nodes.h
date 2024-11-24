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

	// Идентификатор (переменная, функция, методы, аттрибуты, класс)
	string identifier = "";

	// Для хранения значений констант
	int boolVal = -1;
	float floatVal = -1.0f;
	int intVal = -1;
	string stringVal = "";

	// Для бинарного expr
	struct ExprNode* left = nullptr;
	struct ExprNode* right = nullptr;
};