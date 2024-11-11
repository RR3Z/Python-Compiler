#pragma once
#include <string>

using namespace std;

/* ========== EXPRESSION ========== */

enum ExprType {
	_PLUS,
	_MINUS,
	_MUL,
	_DIV,
	_AND_LOGIC,
	_AND_BITWISE,
	_OR_LOGIC,
	_OR_BITWISE,
	_GREAT,
	_GREAT_EQUAL,
	_LESS,
	_LESS_EQUAL,
	_EQUAL,
	_NOT_EQUAL,
	_U_PLUS,
	_U_MINUS,
	_LAMBDA,
	_ASSIGN,
	_SUPER,
	_SELF,
	_TYPE,
	_FALSE,
	_TRUE,
	_STRING_CONST,
	_FLOAT_CONST,
	_INT_CONST,
	_ATTRIBUTE_REF,
	_METHOD_CALL,
	_FUNCTION_CALL,
	_SLICING,
	_ARRAY_ACCESS,
	_LIST_COMPREHENSION,
	_ARRAY_CREATION,
	_BRACKETS, // '(' expr ')'
	_IDENTIFIER,
};

struct ExprNode {
	// Индекс узла
	int id;
	// Тип узла
	enum ExprType type;

	// Идентификатор (переменная, функция, методы, аттрибуты, класс)
	string identifier;

	// Для хранения значений констант
	int boolVal;
	float floatVal;
	int intVal;
	string stringVal;

	// Для бинарного expr
	struct ExprNode* left;
	struct ExprNode* right;
};