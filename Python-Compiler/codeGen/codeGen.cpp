#define _CRT_SECURE_NO_WARNINGS
#pragma once
#include "./codeGen.h"

FILE* fileClass;

/* ========= Генерация байт кода ========= */

/*
	Документация JVM по содержимому файлов .class .
	https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.1
*/
void generate(FileNode* program, const map<string, Class*>& classesList) {
	for (auto clazz : classesList) {
		// количество элементов в constantPool + 1
		vector<char> constantPoolCount = intToFourBytes(clazz.second->constants.size() + 1);

		// Файл для класса
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

		// interfaces count (u2) - не реализуем
		fprintf(fileClass, "%c%c", 0x00, 0x00);

		// ========= FIELDS =========
		// fields count (u2)
		bytes = intToFourBytes(clazz.second->fields.size());
		fprintf(fileClass, "%c%c", bytes[2], bytes[3]);
		// fields pool TODO
		for (auto it = clazz.second->fields.begin(); it != clazz.second->fields.end(); it++) {
			generateFieldCode(it->second, clazz.second);
		}

		// ========= METHODS =========
		// methods count (u2)
		bytes = intToFourBytes(clazz.second->methods.size());
		fprintf(fileClass, "%c%c", bytes[2], bytes[3]);
		// methods pool
		for (auto it = clazz.second->methods.begin(); it != clazz.second->methods.end(); it++) {
			if (it->second->name == "main") generateMainCode(it->second, clazz.second);
			else generateMethodCode(it->second, clazz.second);
		}

		// attributes count (u2) - для класса не используется
		fprintf(fileClass, "%c%c", 0x00, 0x00);

		fclose(fileClass);
	}
}

void generateConstantCode(Constant constant) {
	// UTF8
	if (constant.type == ConstantType::Utf8) {
		// �������������� string � char*
		const char* c = constant.strValue.c_str();

		// ��� ��������� (u1)
		fprintf(fileClass, "%c", (char)ConstantType::Utf8);

		// ����� ������ � ������ (u2)
		vector<char> len = intToFourBytes(strlen(c));
		fprintf(fileClass, "%c%c", (char)len[2], (char)len[3]);

		// ������� ������ � ���� ������������������ ���� ��������� ����� (u1[])
		for (int i = 0; i < strlen(c); ++i) {
			fprintf(fileClass, "%c", c[i]);
		}
	}

	// Integer
	if (constant.type == ConstantType::Integer) {
		// ��� ��������� (u1)
		fprintf(fileClass, "%c", (char)ConstantType::Integer);

		// ����� �� ������ (s4)
		vector<char> len = intToFourBytes(constant.intValue);
		fprintf(fileClass, "%c%c%c%c", len[0], len[1], len[2], len[3]);
	}

	// Float
	if (constant.type == ConstantType::Float) {
		// ��� ��������� (u1)
		fprintf(fileClass, "%c", (char)ConstantType::Float);

		// ����� �� ������ (s4)
		vector<char> len = floatToBytes(constant.floatValue);
		fprintf(fileClass, "%c%c%c%c", len[0], len[1], len[2], len[3]);
	}

	// String
	if (constant.type == ConstantType::String) {
		// ��� ��������� (u1)
		fprintf(fileClass, "%c", (char)ConstantType::String);

		// ����� ��������� UTF8 - �������� �������� ��������� (u2)
		vector<char> len = intToFourBytes(constant.utf8Number);
		fprintf(fileClass, "%c%c", len[2], len[3]);
	}

	// NameAndType
	if (constant.type == ConstantType::NameAndType) {
		// ��� ��������� (u1)
		fprintf(fileClass, "%c", (char)ConstantType::NameAndType);

		// ����� ��������� UTF8 - ��� ����/������ (u2)
		vector<char> len = intToFourBytes(constant.nameNumber);
		fprintf(fileClass, "%c%c", len[2], len[3]);

		// ����� ��������� UTF8 - ���������� ����/������ (u2)
		len = intToFourBytes(constant.typeNumber);
		fprintf(fileClass, "%c%c", len[2], len[3]);
	}

	// Class
	if (constant.type == ConstantType::Class) {
		// ��� ��������� (u1)
		fprintf(fileClass, "%c", (char)ConstantType::Class);

		// ����� ��������� UTF8 - ������ ����������������� ��� ������ (u2)
		vector<char> len = intToFourBytes(constant.classNameNumber);
		fprintf(fileClass, "%c%c", len[2], len[3]);
	}

	// FieldRef
	if (constant.type == ConstantType::FieldRef) {
		// ��� ��������� (u1)
		fprintf(fileClass, "%c", (char)ConstantType::FieldRef);

		// ����� ��������� Class - �����, �������� ����������� ���� (u2)
		vector<char> len = intToFourBytes(constant.classNumber);
		fprintf(fileClass, "%c%c", len[2], len[3]);

		// ����� ��������� NameAndType - ��� � ���������� ���� (u2)
		len = intToFourBytes(constant.nameAndTypeNumber);
		fprintf(fileClass, "%c%c", len[2], len[3]);
	}

	// MethodRef
	if (constant.type == ConstantType::MethodRef) {
		// ��� ��������� (u1)
		fprintf(fileClass, "%c", (char)ConstantType::MethodRef);

		// ����� ��������� Class - �����, �������� ����������� ����� (u2)
		vector<char> len = intToFourBytes(constant.classNumber);
		fprintf(fileClass, "%c%c", len[2], len[3]);

		// ����� ��������� NameAndType - ��� � ���������� ������ (u2)
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
	if(clazz->name == "__PROGRAM__") fprintf(fileClass, "%c%c", 0x00, 0x09);
	else fprintf(fileClass, "%c%c", 0x00, (char)(method->accessModifier));
	
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

	// Подсчитываю кол-во байт в коде
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
			// return void для корректной работы JVM
			bytes.clear();
			bytes.push_back((char)Command::_return);
			codeBytes.insert(codeBytes.end(), bytes.begin(), bytes.end());
		}
	}
	else {
		// return void для корректной работы JVM
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
	for (int i = 0; i < codeBytes.size(); i++) {
		fprintf(fileClass, "%c", codeBytes[i]);
	}

	// exception_table_length (u2) - TODO: исключения (https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.3)
	fprintf(fileClass, "%c%c", 0x00, 0x00);

	// attributes_count (u2)
	fprintf(fileClass, "%c%c", 0x00, 0x00);
}

void generateFieldCode(Field* field, Class* clazz) {
	// access flags (u2)
	if (clazz->name == "__PROGRAM__") fprintf(fileClass, "%c%c", 0x00, (char)AccessFlag::STATIC); // ���� ������ __PROGRAM__ ������ ����� static
	else fprintf(fileClass, "%c%c", 0x00, (char)(field->accessModifier)); // ���� ������ ������� ��� ����� ��������������� ����������� �������

	// name index (u2)
	vector<char> bytes = intToBytes(field->nameNumber, 2);
	fprintf(fileClass, "%c%c", bytes[0], bytes[1]);

	// descriptor index (u2)
	bytes = intToBytes(field->descriptorNumber, 2);
	fprintf(fileClass, "%c%c", bytes[0], bytes[1]);

	// attributes count (u2) - ��� ���� �� ������ ���������
	fprintf(fileClass, "%c%c", 0x00, 0x00);
}

vector<char> generateStatementCode(StmtNode* stmt, Class* clazz, Method* method) {
	vector<char> result, bytes = {};

	switch (stmt->stmtType)
	{
		case _EXPR_STMT:
			bytes = generateExpressionCode(stmt->expr, clazz, method);
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
		case _WHILE:
			bytes = generateWhileStatementCode(stmt, clazz, method);
			result.insert(result.end(), bytes.begin(), bytes.end());
			break;
		case _IF:
			bytes = generateIfStatementCode(stmt, clazz, method);
			result.insert(result.end(), bytes.begin(), bytes.end());
			break;
		case _COMPOUND_IF:
			bytes = generateCompoundIfStatementCode(stmt, clazz, method);
			result.insert(result.end(), bytes.begin(), bytes.end());
			break;
		case _ELSE:
			break;
		case _ASSIGN_AND_ACCESS:
			bytes = generateExpressionCode(stmt->leftExpr, clazz, method);
			result.insert(result.end(), bytes.begin(), bytes.end());
			bytes = generateExpressionCode(stmt->expr, clazz, method);
			result.insert(result.end(), bytes.begin(), bytes.end());
			bytes = generateExpressionCode(stmt->rightExpr, clazz, method);
			result.insert(result.end(), bytes.begin(), bytes.end());
			result.push_back((char)Command::invokevirtual);
			bytes = intToBytes(stmt->number, 2);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);
			break;
	}

	return result;
}

vector<char> generateIfStatementCode(StmtNode* stmt, Class* clazz, Method* method) {
	vector<char> result, condition, ifSuite, bytes = {};

	// condition
	if (stmt->expr->exprType == _BRACKETS) condition = generateExpressionCode(stmt->expr->left, clazz, method);
	else condition = generateExpressionCode(stmt->expr, clazz, method);

	//suite
	ifSuite = generateStatementListCode(stmt->suite, clazz, method);

	// condition
	result.insert(result.end(), condition.begin(), condition.end());

	result.push_back((char)Command::getfield);
	bytes = intToBytes(stmt->boolFieldMethodRef, 2);
	result.insert(result.end(), bytes.begin(), bytes.end());

	result.push_back((char)Command::ifeq);
	bytes = intToBytes(ifSuite.size() + 3, 2); // ofset
	result.insert(result.end(), bytes.begin(), bytes.end());

	// suite
	result.insert(result.end(), ifSuite.begin(), ifSuite.end());

	return result;
}

vector<char> generateCompoundIfStatementCode(StmtNode* stmt, Class* clazz, Method* method) {
	vector<char> result, bytes = {};

	vector<char> condition, ifSuite, elseSuite = {};
	vector<vector<char>> elifConditions, elifSuites = {};

	if (stmt->leftNode->expr->exprType == _BRACKETS) condition = generateExpressionCode(stmt->leftNode->expr->left, clazz, method);
	else condition = generateExpressionCode(stmt->leftNode->expr, clazz, method);

	ifSuite = generateStatementListCode(stmt->leftNode->suite, clazz, method);

	elifConditions.push_back(condition);
	elifSuites.push_back(ifSuite);

	if (stmt->stmtsList != nullptr) {
		StmtNode* stmtElif = stmt->stmtsList->first;
		while (stmtElif != nullptr) {
			elifConditions.push_back(generateExpressionCode(stmtElif->expr, clazz, method));
			elifSuites.push_back(generateStatementListCode(stmtElif->suite, clazz, method));
			stmtElif = stmtElif->next;
		}
	}

	if (stmt->rightNode != nullptr) elseSuite = generateStatementListCode(stmt->rightNode->suite, clazz, method);
	result.insert(result.begin(), elseSuite.begin(), elseSuite.end());

	for (int i = elifConditions.size() - 1; i >= 0; --i) {
		if (result.size() != 0) {
			elifSuites[i].push_back((char)Command::goto_);
			bytes = intToBytes(result.size() + 3, 2);
			elifSuites[i].push_back(bytes[0]);
			elifSuites[i].push_back(bytes[1]);
		}

		result.insert(result.begin(), elifSuites[i].begin(), elifSuites[i].end());
		elifConditions[i].push_back((char)Command::getfield);
		bytes = intToBytes(stmt->boolFieldMethodRef, 2);
		elifConditions[i].push_back(bytes[0]);
		elifConditions[i].push_back(bytes[1]);
		elifConditions[i].push_back((char)Command::ifeq);
		bytes = intToBytes(elifSuites[i].size() + 3, 2);
		elifConditions[i].push_back(bytes[0]);
		elifConditions[i].push_back(bytes[1]);
		result.insert(result.begin(), elifConditions[i].begin(), elifConditions[i].end());
	}

	return result;
}

vector<char> generateStatementListCode(StmtsListNode* stmts, Class* clazz, Method* method) {
	vector<char> result, bytes = {};

	if (stmts != nullptr) {
		StmtNode* stmt = stmts->first;
		while (stmt != nullptr) {
			bytes = generateStatementCode(stmt, clazz, method);
			result.insert(result.end(),bytes.begin(), bytes.end());
			stmt = stmt->next;
		}
	}
	
	return result;
}

vector<char> generateWhileStatementCode(StmtNode* stmt, Class* clazz, Method* method) {
	vector<char> result, bytes, code = {};

	// condition
	if (stmt->expr->exprType == _BRACKETS) bytes = generateExpressionCode(stmt->expr->left, clazz, method);
	else bytes = generateExpressionCode(stmt->expr, clazz, method);
	result.insert(result.end(), bytes.begin(), bytes.end());

	code = generateStatementListCode(stmt->suite, clazz, method);

	result.push_back((char)Command::getfield);
	bytes = intToBytes(stmt->boolFieldMethodRef, 2);
	result.push_back(bytes[0]);
	result.push_back(bytes[1]);

	result.push_back((char)Command::ifeq);
	bytes = intToBytes(code.size() + 6, 2);
	result.push_back(bytes[0]);
	result.push_back(bytes[1]);

	// suite
	result.insert(result.end(), code.begin(), code.end());

	bytes = intToBytes(-1 * result.size(), 2);
	result.push_back((char)Command::goto_);
	result.push_back(bytes[0]);
	result.push_back(bytes[1]);

	return result;
}

vector<char> generateAssignStatementCode(StmtNode* assignStmt, Class* clazz, Method* method) {
	vector<char> result, bytes = {};

	bytes = generateExpressionCode(assignStmt->rightExpr, clazz, method);
	result.insert(result.end(), bytes.begin(), bytes.end());

	if (clazz->fields.find(assignStmt->leftExpr->identifier) != clazz->fields.end()) {
		if (clazz->name == "__PROGRAM__") {
			if (find(method->localVars.begin(), method->localVars.end(), assignStmt->leftExpr->identifier) != method->localVars.end()) {
				result.push_back((char)Command::astore);
				result.push_back(assignStmt->leftExpr->paramLocalVarNum);
				return result;
			}

			result.push_back((char)Command::putstatic);
			bytes = intToBytes(assignStmt->number, 2);
			result.insert(result.end(), bytes.begin(), bytes.end());
			return result;
		}

		// TODO: Добавить для обычных классов
	} 
	else if (find(method->localVars.begin(), method->localVars.end(), assignStmt->leftExpr->identifier) != method->localVars.end()) {
		result.push_back((char)Command::astore);
		result.push_back(assignStmt->leftExpr->paramLocalVarNum);
		return result;
	}

	throw runtime_error("CodeGen: ERROR -> Trying assign value to unknown identifier");
}

// TODO
vector<char> generateExpressionCode(ExprNode* expr, Class* clazz, Method* method) {
	vector<char> result, bytes = {};

	ExprNode* exprCounter = nullptr;

	int counter = 0;

	switch (expr->exprType)
	{
		case _INT_CONST:
			result.push_back((char)Command::_new);

			bytes = intToBytes(expr->classNumber, 2);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);

			result.push_back((char)Command::dup);

			bytes = intToBytes(expr->intVal, 2);
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
		case _TRUE:
		case _FALSE:
			result.push_back((char)Command::_new);
			bytes = intToBytes(expr->classNumber, 2);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);
			result.push_back((char)Command::dup);
			if (expr->boolVal == 1) {
				result.push_back((char)Command::iconst_1);
			}
			else {
				result.push_back((char)Command::iconst_0);
			}
			result.push_back((char)Command::invokespecial);
			bytes = intToBytes(expr->number, 2);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);
			break;
		case _IDENTIFIER:
			if (clazz->fields.find(expr->identifier) != clazz->fields.end()) {
				if (clazz->name == "__PROGRAM__") {
					if (find(method->localVars.begin(), method->localVars.end(), expr->identifier) != method->localVars.end()) {
						result.push_back((char)Command::aload);
						bytes = intToBytes(expr->paramLocalVarNum, 1);
						result.push_back(bytes[0]);
						break;
					}

					result.push_back((char)Command::getstatic);
					bytes = intToBytes(clazz->fields[expr->identifier]->number, 2);
					result.push_back(bytes[0]);
					result.push_back(bytes[1]);
					break;
				}

				// TODO: Сделать для обычных классов
			}
			if (find(method->localVars.begin(), method->localVars.end(), expr->identifier) != method->localVars.end()) {
				result.push_back((char)Command::aload);
				bytes = intToBytes(expr->paramLocalVarNum, 1);
				result.push_back(bytes[0]);
				break;
			}

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
			bytes = intToBytes(expr->number, 2);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);
			break;
		case _U_PLUS:
		case _U_MINUS:
			bytes = generateExpressionCode(expr->right, clazz, method);
			result.insert(result.end(), bytes.begin(), bytes.end());
			result.push_back((char)Command::invokevirtual);
			bytes = intToBytes(expr -> number, 2);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);
			break;
		case _LIST_CREATION:
			result.push_back((char)Command::_new);
			bytes = intToBytes(expr->classNumber,2);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);
			result.push_back((char)Command::dup);
			result.push_back((char)Command::_new);
			bytes = intToBytes(expr->arrayListClassNumber,2);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);
			result.push_back((char)Command::dup);
			bytes = intToBytes(countExprs(expr->list), 2);
			result.push_back((char)Command::sipush);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);
			result.push_back((char)Command::anewarray);
			bytes = intToBytes(expr->classNumber,2);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);
			result.push_back((char)Command::dup);

			// Add elements
			if (expr->list != nullptr) {
				exprCounter = expr->list->first;
				while (exprCounter != nullptr) {
					bytes = intToBytes(counter,2);
					result.push_back((char)Command::sipush);
					result.push_back(bytes[0]);
					result.push_back(bytes[1]);
					bytes = generateExpressionCode(exprCounter, clazz,method);
					result.insert(result.end(), bytes.begin(), bytes.end());
					result.push_back((char)Command::aastore);
					result.push_back((char)Command::dup);
					++counter;
					exprCounter = exprCounter->next;
				}
			}
			// because after last element we dup data.
			result.push_back((char)Command::pop);
			// call Arrays.asList
			result.push_back((char)Command::invokestatic);
			bytes = intToBytes(expr->listConstructorMethodRef,2);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);

			// create ArrayList
			result.push_back((char)Command::invokespecial);
			bytes = intToBytes(expr->arrayListConstructorMethodRef,2);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);
			result.push_back((char)Command::invokespecial);
			bytes = intToBytes(expr->number, 2);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);
			break;
		case _LIST_ACCESS:
			bytes = generateExpressionCode(expr->left, clazz, method);
			result.insert(result.end(), bytes.begin(), bytes.end());
			bytes = generateExpressionCode(expr->right, clazz, method);
			result.insert(result.end(), bytes.begin(), bytes.end());
			result.push_back((char)Command::invokevirtual);
			bytes = intToBytes(expr->number, 2);
			result.push_back(bytes[0]);
			result.push_back(bytes[1]);
			break;
		case _FUNCTION_CALL:
			if (expr->funcArgs != nullptr) {
				exprCounter = expr->funcArgs->exprList->first;
				while (exprCounter != nullptr) {
					// Проверка, что передаваемый аргумент существует (ЭТО ПЛОХО, ЭТО НАДО ДЕЛАТЬ НА УРОВНЕ ТАБЛИЦ)
					bytes = checkArgExistence(clazz, method, exprCounter);
					result.insert(result.end(), bytes.begin(), bytes.end());

					// Генерация кода вызова функции
					bytes = generateExpressionCode(exprCounter, clazz, method);
					result.insert(result.end(), bytes.begin(), bytes.end());
					exprCounter = exprCounter->next;
				}
			}
			if (clazz->name == "__PROGRAM__") result.push_back((char)Command::invokestatic);
			else result.push_back((char)Command::invokevirtual);

			bytes = intToBytes(expr->number, 2);
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

int countExprs(ExprListNode* expr) {
	if (expr->first == nullptr) return 0;
	int count = 0;
	ExprNode* countExpr = expr->first;
	while (countExpr != nullptr) {
		count++;
		countExpr = countExpr->next;
	}
	return count;
}

/* ========= Проверки на ошибки ========= */

vector<char> checkArgExistence(Class* clazz, Method* method, ExprNode* arg) {
	if (arg->exprType != _IDENTIFIER) return {};

	vector<char> result, tmp, bytes = {};

	// Загрузка локальной переменной (2 байта)
	result.push_back((char)Command::aload);
	bytes = intToBytes(arg->paramLocalVarNum, 1);
	result.push_back(bytes[0]);

	// Загрузка проверки на null + смещение (3 байта)
	result.push_back((char)Command::ifnonnull);

	// Код с исключением
	tmp.push_back((char)Command::_new);
	bytes = intToBytes(clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("java/lang/NullPointerException")))), 2);
	tmp.push_back(bytes[0]);
	tmp.push_back(bytes[1]);
	tmp.push_back((char)Command::dup);
	tmp.push_back((char)Command::ldc_w);
	bytes = intToBytes(clazz->pushOrFindConstant(*Constant::String(clazz->pushOrFindConstant(*Constant::UTF8("Code: ERROR -> identifier in FUNCTION CALL is not defined.")))), 2);
	tmp.push_back(bytes[0]);
	tmp.push_back(bytes[1]);
	tmp.push_back((char)Command::invokespecial);
	bytes = intToBytes(clazz->pushOrFindMethodRef("java/lang/NullPointerException", "<init>", "(Ljava/lang/String;)V"), 2);
	tmp.push_back(bytes[0]);
	tmp.push_back(bytes[1]);
	tmp.push_back((char)Command::athrow);

	// Добавляем offset для ifnonnull (на конец проверки)
	bytes = intToBytes(tmp.size() + 5, 2);
	result.push_back(bytes[0]);
	result.push_back(bytes[1]);

	result.insert(result.end(), tmp.begin(), tmp.end());

	return result;
}
