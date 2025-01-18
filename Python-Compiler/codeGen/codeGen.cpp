#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "./codeGen.h"

FILE* fileClass;

/* ========= Функции генерации байт кода ========= */

/*
	Для понимания, что за байты см. class file format в документации JVM или в методичке.
	https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.1
*/
void generate(FileNode* program, const map<string, Class*>& classesList) {
	for (auto clazz : classesList) {
		// Для constant pool count и начинается с 1, а не с нуля (поэтому +1)
		vector<char> constantPoolCount = intToFourBytes(clazz.second->constants.size() + 1);

		// Файл для сохранения
		string fileName = clazz.second->name + ".class";
		fopen_s(&fileClass, fileName.c_str(), "wb");

		// magic (u4) - CAFEBABE
		fprintf(fileClass, "%c%c%c%c", 0xCA, 0xFE, 0xBA, 0xBE);

		// version - minor (u2) + major (u2) - JAVA 8 (version 52.0 (0x34))
		fprintf(fileClass, "%c%c%c%c", 0x00, 0x00, 0x00, 0x34);

		// constant pool count (u2)
		fprintf(fileClass, "%c%c", constantPoolCount[2], constantPoolCount[3]);

		// constant pool (u2)
		vector<pair<Constant, int>> constantsPool;
		for (auto& it : clazz.second->constants) {
			constantsPool.push_back(it);
		}
		sort(constantsPool.begin(), constantsPool.end(), compare);
		for (auto i : constantsPool) {
			generateConstantCode(i.first);
		}

		// access flags (u2) - ACC_SUPER (0x20) + ACC_PUBLIC (0x01)
		fprintf(fileClass, "%c%c", 0x00, 0x21);

		// this class (u2)
		vector<char> bytes = intToFourBytes(clazz.second->number);
		fprintf(fileClass, "%c%c", bytes[2], bytes[3]);

		// super class (u2) - parent
		bytes = intToFourBytes(clazz.second->parentNumber);
		fprintf(fileClass, "%c%c", bytes[2], bytes[3]);

		// interfaces count (u2) - не реализую
		fprintf(fileClass, "%c%c", 0x00, 0x00);

		// ========= FIELDS ========= TODO
		// fields count (u2)
		//bytes = intToFourBytes(clazz.second->fields.size());
		//fprintf(fileClass, "%c%c", bytes[2], bytes[3]);
		fprintf(fileClass, "%c%c", 0x00, 0x00);
		// fields pool TODO
		//fprintf(fileClass, "%c%c", 0x00, 0x00);

		// ========= METHODS ========= TODO
		// methods count (u2)
		bytes = intToFourBytes(clazz.second->methods.size());
		fprintf(fileClass, "%c%c", bytes[2], bytes[3]);
		// methods pool
		for (auto it = clazz.second->methods.begin(); it != clazz.second->methods.end(); it++) {
			if (it->second->name != "main") generateMethodCode(it->second, clazz.second);
			else  generateMainCode(it->second, clazz.second);
		}

		// attributes count (u2) - не нужны на уровне класса
		fprintf(fileClass, "%c%c", 0x00, 0x00);

		// Закрыть файл
		fclose(fileClass);
	}
}

void generateConstantCode(Constant constant) {
	// UTF8
	if (constant.type == ConstantType::Utf8) {
		// Преобразование string в char*
		const char* c = constant.strValue.c_str();

		// тип константы (u1)
		fprintf(fileClass, "%c", (char)ConstantType::Utf8);

		// длина строки в байтах (u2)
		vector<char> len = intToFourBytes(strlen(c));
		fprintf(fileClass, "%c%c", (char)len[2], (char)len[3]);

		// символы строки в виде последовательности байт указанной длины (u1[])
		for (int i = 0; i < strlen(c); ++i) {
			fprintf(fileClass, "%c", c[i]);
		}
	}

	// Integer
	if (constant.type == ConstantType::Integer) {
		// тип константы (u1)
		fprintf(fileClass, "%c", (char)ConstantType::Integer);

		// число со знаком (s4)
		vector<char> len = intToFourBytes(constant.intValue);
		fprintf(fileClass, "%c%c%c%c", len[0], len[1], len[2], len[3]);
	}

	// Float
	if (constant.type == ConstantType::Float) {
		// тип константы (u1)
		fprintf(fileClass, "%c", (char)ConstantType::Float);

		// число со знаком (s4)
		vector<char> len = floatToBytes(constant.floatValue);
		fprintf(fileClass, "%c%c%c%c", len[0], len[1], len[2], len[3]);
	}

	// String
	if (constant.type == ConstantType::String) {
		// тип константы (u1)
		fprintf(fileClass, "%c", (char)ConstantType::String);

		// номер константы UTF8 - значение строковй константы (u2)
		vector<char> len = intToFourBytes(constant.utf8Number);
		fprintf(fileClass, "%c%c", len[2], len[3]);
	}

	// NameAndType
	if (constant.type == ConstantType::NameAndType) {
		// тип константы (u1)
		fprintf(fileClass, "%c", (char)ConstantType::NameAndType);

		// номер константы UTF8 - имя поля/метода (u2)
		vector<char> len = intToFourBytes(constant.nameNumber);
		fprintf(fileClass, "%c%c", len[2], len[3]);

		// номер константы UTF8 - дескриптор поля/метода (u2)
		len = intToFourBytes(constant.typeNumber);
		fprintf(fileClass, "%c%c", len[2], len[3]);
	}

	// Class
	if (constant.type == ConstantType::Class) {
		// тип константы (u1)
		fprintf(fileClass, "%c", (char)ConstantType::Class);

		// номер константы UTF8 - полное квалифицированные имя класса (u2)
		vector<char> len = intToFourBytes(constant.classNameNumber);
		fprintf(fileClass, "%c%c", len[2], len[3]);
	}

	// FieldRef
	if (constant.type == ConstantType::FieldRef) {
		// тип константы (u1)
		fprintf(fileClass, "%c", (char)ConstantType::FieldRef);

		// номер константы Class - класс, которому принадлежит поле (u2)
		vector<char> len = intToFourBytes(constant.classNumber);
		fprintf(fileClass, "%c%c", len[2], len[3]);

		// номер константы NameAndType - имя и дескриптор поля (u2)
		len = intToFourBytes(constant.nameAndTypeNumber);
		fprintf(fileClass, "%c%c", len[2], len[3]);
	}

	// MethodRef
	if (constant.type == ConstantType::MethodRef) {
		// тип константы (u1)
		fprintf(fileClass, "%c", (char)ConstantType::MethodRef);

		// номер константы Class - класс, которому принадлежит метод (u2)
		vector<char> len = intToFourBytes(constant.classNumber);
		fprintf(fileClass, "%c%c", len[2], len[3]);

		// номер константы NameAndType - имя и дескриптор метода (u2)
		len = intToFourBytes(constant.nameAndTypeNumber);
		fprintf(fileClass, "%c%c", len[2], len[3]);
	}
}

void generateMainCode(Method* mainMethod, Class* clazz) {
	// access flags (u2) - PUBLIC + STATIC
	fprintf(fileClass, "%c%c", 0x00, 0x09);

	// name index (u2)
	vector<char> bytes = intToFourBytes(mainMethod->nameNumber);
	fprintf(fileClass, "%c%c", bytes[2], bytes[3]);

	// descriptor index (u2)
	bytes = intToFourBytes(mainMethod->descriptorNumber);
	fprintf(fileClass, "%c%c", bytes[2], bytes[3]);

	// attributes count (u2)
	fprintf(fileClass, "%c%c", 0x00, 0x01);

	// attributes
	generateAttributeCode(mainMethod, clazz);
}

void generateMethodCode(Method* method, Class* clazz) {
	// access flags (u2)
	fprintf(fileClass, "%c%c", 0x00, method->accessModifier);

	// name index (u2)
	vector<char> bytes = intToFourBytes(method->nameNumber);
	fprintf(fileClass, "%c%c", bytes[2], bytes[3]);

	// descriptor index (u2)
	bytes = intToFourBytes(method->descriptorNumber);
	fprintf(fileClass, "%c%c", bytes[2], bytes[3]);

	// attributes count (u2)
	fprintf(fileClass, "%c%c", 0x00, 0x01);

	// attributes
	generateAttributeCode(method, clazz);
}

void generateAttributeCode(Method* method, Class* clazz) {
	// attribute_name_index (u2)
	vector<char> bytes = intToFourBytes(clazz->pushOrFindConstant(*Constant::UTF8("Code")));
	fprintf(fileClass, "%c%c", bytes[2], bytes[3]);

	// Вычисление длины атрибута
	vector<char> codeBytes, stmtBytes = {};
	if (method->suite != nullptr) {
		if (method->suite->first != nullptr) {
			StmtNode* stmt = method->suite->first;
			while (stmt != nullptr) {
				stmtBytes = generateStatementCode(stmt, clazz, method);
				codeBytes.insert(codeBytes.end(), stmtBytes.begin(), stmtBytes.end());
				stmt = stmt->next;
			}
		}

		if (method->suite->last->stmtType != _RETURN) {
			// Генерирую return void (для корректной работы JVM)
			bytes.clear();
			bytes.push_back((char)Command::_return);
			codeBytes.insert(codeBytes.end(), bytes.begin(), bytes.end());
		}
	}
	else {
		// Генерирую return void (для корректной работы JVM)
		bytes.clear();
		bytes.push_back((char)Command::_return);
		codeBytes.insert(codeBytes.end(), bytes.begin(), bytes.end());
	}
	
	// attribute_length (u4)
	bytes = intToFourBytes(12 + codeBytes.size());
	fprintf(fileClass, "%c%c%c%c", bytes[0], bytes[1], bytes[2], bytes[3]);

	// max_stack (u2)
	bytes = intToFourBytes(1000);
	fprintf(fileClass, "%c%c", bytes[2], bytes[3]);

	// max_locals (u2)
	bytes = intToFourBytes(method->localVars.size());
	fprintf(fileClass, "%c%c", bytes[2], bytes[3]);

	// code_length (u4)
	bytes = intToFourBytes(codeBytes.size());
	fprintf(fileClass, "%c%c%c%c", bytes[0], bytes[1], bytes[2], bytes[3]);

	// code (code_length)
	int size = codeBytes.size();
	for (int i = 0; i < codeBytes.size(); i++) {
		fprintf(fileClass, "%c", codeBytes[i]);
	}

	// exception_table_length (u2) - TODO: сделать исключения (https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.3)
	fprintf(fileClass, "%c%c", 0x00, 0x00);

	// attributes_count (u2)
	fprintf(fileClass, "%c%c", 0x00, 0x00);
}

vector<char> generateStatementCode(StmtNode* stmt, Class* clazz, Method* method) {
	vector<char> result, bytes = {};

	switch (stmt->stmtType)
	{
		case _EXPR_STMT:
			generateExpressionCode(stmt->expr, clazz, method);
			result.insert(result.end(), bytes.begin(), bytes.end());
			break;
		case _ASSIGN:
			bytes = generateAssignStatementCode(stmt, clazz, method);
			result.insert(result.end(), bytes.begin(), bytes.end());
			break;
		case _COMPOUND_ASSIGN:
			if (stmt->stmtsList != nullptr) {
				StmtNode* assignStmt = stmt->stmtsList->first;

				while (assignStmt != nullptr) {
					bytes = generateStatementCode(assignStmt, clazz, method);
					result.insert(result.end(), bytes.begin(), bytes.end());
					assignStmt = assignStmt->next;
				}
			}
			break;
		case _RETURN:
			if (stmt->list != nullptr) {
				bytes = generateExpressionCode(stmt->list->first, clazz, method);
				result.insert(result.end(), bytes.begin(), bytes.end());
				result.push_back((char)Command::areturn);
			}
			else {
				result.push_back((char)Command::_return);
			}
			break;
	}

	return result;
}

vector<char> generateAssignStatementCode(StmtNode* assignStmt, Class* clazz, Method* method) {
	vector<char> result, bytes = {};

	// Byte код для значения (value)
	bytes = generateExpressionCode(assignStmt->rightExpr, clazz, method);
	result.insert(result.end(), bytes.begin(), bytes.end());

	// Byte код для источника (identifier)
	result.push_back((char)Command::astore);
	result.push_back(assignStmt->leftExpr->paramLocalVarNum);

	return result;
}

// TODO
vector<char> generateExpressionCode(ExprNode* expr, Class* clazz, Method* method) {
	vector<char> result, bytes = {};

	ExprNode* exprCounter = nullptr;

	switch (expr->exprType)
	{
		case _INT_CONST:
			result.push_back((char)Command::_new); // Создаем ссылку

			bytes = intToBytes(expr->classNumber, 2); // Загружаем базовый класс
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);

			result.push_back((char)Command::dup); // Копируем ссылку на базовый класс

			bytes = intToBytes(expr->intVal, 2); // Загружаем число
			if (expr->intVal >= -32768 && expr->intVal <= 32767) {
				result.push_back((char)Command::sipush);
				result.push_back(bytes[0]);
				result.push_back(bytes[1]);
			}
			else {
				result.push_back((char)Command::ldc_w);
				bytes = intToBytes(expr->valueNumber, 2);
				result.push_back(bytes[0]);
				result.push_back(bytes[1]);
			}

			result.push_back((char)Command::invokespecial);
			bytes = intToBytes(expr->number, 2);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);
			break;

		case _STRING_CONST:
		case _FLOAT_CONST:

			result.push_back((char)Command::_new);

			bytes = intToBytes(expr->classNumber, 2);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);

			result.push_back((char)Command::dup);

			result.push_back((char)Command::ldc_w);
			bytes = intToBytes(expr->valueNumber, 2);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);

			result.push_back((char)Command::invokespecial);
			bytes = intToBytes(expr->number, 2);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);
			break;
		case _IDENTIFIER:
			result.push_back((char)Command::aload);
			bytes = intToBytes(expr->paramLocalVarNum, 1);
			result.push_back(bytes[0]);
			break;
		case _METHOD_CALL:
			if (expr->list != nullptr) {
				exprCounter = expr->list->first;
				while (exprCounter != nullptr) {
					bytes = generateExpressionCode(exprCounter, clazz, method);
					result.insert(result.end(), bytes.begin(), bytes.end());
					exprCounter = exprCounter->next;
				}
			}
			result.push_back((char)Command::invokespecial);
			bytes = intToBytes(expr->number, 2);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);
			break;
		case _PLUS:
		case _MINUS:
		case _MUL:
		case _DIV:
		case _EQUAL:
		case _NOT_EQUAL:
		case _LESS:
		case _LESS_EQUAL:
		case _GREAT:
		case _GREAT_EQUAL:
			bytes = generateExpressionCode(expr->left, clazz, method);
			result.insert(result.end(), bytes.begin(), bytes.end());
			bytes = generateExpressionCode(expr->right, clazz, method);
			result.insert(result.end(), bytes.begin(), bytes.end());
			result.push_back((char)Command::invokevirtual);
			bytes = intToBytes(expr->id, 2);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);
			break;
		case _U_PLUS:
		case _U_MINUS:
			bytes = generateExpressionCode(expr->right, clazz, method);
			result.insert(result.end(), bytes.begin(), bytes.end());
			result.push_back((char)Command::invokevirtual);
			bytes = intToBytes(expr -> id, 2);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);
			break;
	}

	return result;
}

/* ========= Вспомогательные функции ========= */

vector <char> intToFourBytes(int value) {
	// int = 4 bytes
	vector<char> arrayOfByte(4);

	for (int i = 0; i < 4; ++i) {
		arrayOfByte[3 - i] = (value >> (i * 8));
	}

	return arrayOfByte;
}

vector <char> intToBytes(int value, int size) {
	// int = *size* bytes
	vector<char> arrayOfByte(size);

	for (int i = 0; i < size; ++i) {
		arrayOfByte[(size - 1) - i] = (value >> (i * 8));
	}

	return arrayOfByte;
}

vector<char> floatToBytes(float value)
{
	// float = 4 bytes
	vector<char> arrayOfByte(4);

	for (int i = 0; i < sizeof(float); ++i)
		arrayOfByte[3 - i] = ((char*)&value)[i];

	return arrayOfByte;
}

bool compare(pair<Constant, int>& a, pair<Constant, int>& b) {
	return a.second < b.second;
}
