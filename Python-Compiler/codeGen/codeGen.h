#pragma once
#include <iostream>
#include <algorithm>
#include "../nodes/nodes.h"
#include "../semantic/semantic.h"
#include "../semantic/ConstantType.h"
#include "./AccessFlag.h"
using namespace std;

/* ========= Функции генерации байт кода ========= */

void generate(FileNode* program, const map<string, Class*>& classesList);

void generateConstantCode(Constant constant);

void generateDefaultInitMethod(Method* initMethod);

void generateMainMethod(Method* mainMethod, Class* clazz);

void generateMethodCode(Method* method, Class* clazz);

void generateAttributeCode(Method* method, Class* clazz);

/* ========= Вспомогательные функции ========= */

// Перевод в байтовое представление
vector <char> intToBytes(int value);
vector<char> floatToBytes(float value);

// Для сортировки (по номерам)
bool compare(pair<Constant, int>& a, pair<Constant, int>& b);
