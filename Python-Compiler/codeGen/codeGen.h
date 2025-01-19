#pragma once
#include <iostream>
#include <algorithm>
#include "../nodes/nodes.h"
#include "../semantic/semantic.h"
#include "../semantic/ConstantType.h"
#include "./AccessFlag.h"
#include "./Command.h"
using namespace std;

/* ========= Функции генерации байт кода ========= */

void generate(FileNode* program, const map<string, Class*>& classesList);

void generateConstantCode(Constant constant);

void generateMainCode(Method* mainMethod, Class* clazz);
void generateMethodCode(Method* method, Class* clazz);
void generateAttributeCode(Method* method, Class* clazz);

void generateFieldCode(Field* field, Class* clazz);

vector<char> generateStatementCode(StmtNode* stmt, Class* clazz, Method* method);
vector<char> generateAssignStatementCode(StmtNode* assignStmt, Class* clazz, Method* method);
vector<char> generateWhileStatementCode(StmtNode* stmt, Class* clazz, Method* method);
vector<char> generateStatementListCode(StmtsListNode* stmts, Class* clazz, Method* method);
vector<char> generateIfStatementCode(StmtNode* stmt, Class* clazz, Method* method);

vector<char> generateExpressionCode(ExprNode* expr, Class* clazz, Method* method);

/* ========= Вспомогательные функции ========= */

// Перевод в байтовое представление
vector <char> intToFourBytes(int value);
vector <char> intToBytes(int value, int size);
vector<char> floatToBytes(float value);

// Для сортировки (по номерам)
bool compare(pair<Constant, int>& a, pair<Constant, int>& b);
