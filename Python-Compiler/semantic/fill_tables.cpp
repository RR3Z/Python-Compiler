#pragma once
#include "./semantic.h"
#include <iostream>
using namespace std;

// ����� �������������� ������� �������
std::map<std::string, Class*> classesList;

// ���������� ������ (��������� �������)
void fillTable(FileNode* program) {
	// �������� ������, ��� ����� ����� � ���������
	Class* entryClass = new Class();
	entryClass->name = "__PROGRAM__";
	// ���������� ������ � ���������� �������
	classesList[entryClass->name] = entryClass;

	// ���������� �������� ������
	entryClass->pushOrFindConstant(*Constant::UTF8("Code")); // �� ���� �����������, ��� ��� � ��� ����� ����������� �� ���������
	entryClass->number = entryClass->pushOrFindConstant(*Constant::Class(entryClass->pushOrFindConstant(*Constant::UTF8(entryClass->name))));

	// ���������� ������������� ������
	int parentClassName = entryClass->pushOrFindConstant(*Constant::UTF8("java/lang/Object"));
	entryClass->parentNumber = entryClass->pushOrFindConstant(*Constant::Class(parentClassName));

	// �������� �������, ��� ����� ����� � ���������
	Method* mainMethod = new Method();
	mainMethod->name = "main";
	// ���������� �������� ������
	mainMethod->nameNumber = entryClass->pushOrFindConstant(*Constant::UTF8(mainMethod->name));
	mainMethod->descriptorNumber = entryClass->pushOrFindConstant(*Constant::UTF8("([Ljava/lang/String;)V"));
	mainMethod->number = entryClass->pushOrFindMethodRef(entryClass->name, mainMethod->name, "([Ljava/lang/String;)V");
	// ���������� ��������� ���������� (� ����������� ������� ��������� ����������)
	mainMethod->localVars.push_back("args");
	// ���� ������ (���������� ������)
	mainMethod->suite = nullptr;
	// ���������� main � ������� ������� entryClass
	entryClass->methods[mainMethod->name] = mainMethod;

	// ������ ���� ���������
	if (program != nullptr && program->elementsList != nullptr) {
		FileElementNode* programElement = program->elementsList->first;

		while (programElement != nullptr) {
			switch (programElement->elementType)
			{
				case _CLASS_DEF:
					//fillTable(programElement->classDef);
					break;
				case _FUNC_DEF:
					fillTable(entryClass, programElement->funcDef);
					break;
				case _STMT:
					/*
					� Python ��� ����� ����� ����� � ���������. � ����� ������, � ��� ��� ����������� ������ ����.
					��� ������, ��� ���� � ��� ����������� ���-�� � ���������� ������� ��������� ����� funcDef � classDef ���� ����� � main �����.
					*/
					if (mainMethod->suite == nullptr) mainMethod->suite = new StmtsListNode();

					if (mainMethod->suite->first != nullptr) {
						mainMethod->suite->last = programElement->stmt; 
						mainMethod->suite->first->next = programElement->stmt;
					}
					else { 
						mainMethod->suite->first = programElement->stmt; 
						mainMethod->suite->last = programElement->stmt; 
					}

					fillTable(entryClass, mainMethod, programElement->stmt);

					break;
			}

			// ��������� � ���������� ��������
			programElement = programElement->next;
		}
	}
}

// TODO: ������
void fillTable(ClassNode* classDef) {
	// �������� ������ ������
	Class* newClass = new Class();
	// ���������� ������ � ���������� �������
	newClass->name = classDef->identifier->stringVal;
	classesList[newClass->name] = newClass;
	// ���������� �������� � �����
	newClass->pushOrFindConstant(*Constant::UTF8("Code"));
	newClass->pushOrFindConstant(*Constant::Class(newClass->pushOrFindConstant(*Constant::UTF8(newClass->name))));

	// ������������ ����� (��������� ������������)
	if (classDef->base != nullptr) {
		if (classesList.find(classDef->base->stringVal) != classesList.end()) {
			newClass->parent = classesList[classDef->base->stringVal];
		}
		else {
			throw runtime_error("S: ERROR -> parent class " + classDef->base->stringVal + " for class " + classDef->identifier->stringVal + " not found");
		}
	}

	// �� ���� �� ����� ���� ������ (������ ������������� �� ������ �������)
	if (classDef->suite != nullptr && classDef->suite->first != nullptr) {
		ClassElementNode* classElement = classDef->suite->first;
		while (classElement != nullptr) {
			switch (classElement->elementType)
			{
				case _FUNCTION_DEF:
					fillTable(newClass, classElement->funcDef);
					break;
				case _STMT_NODE:
					// TODO: ���� ��� ����� ���������� �������� -> ��������� � ������� �����
					//if (classElement->stmt->stmtType == _ASSIGN || classElement->stmt->stmtType == _COMPOUND_ASSIGN) fillFieldTable(newClass, classElement->stmt);
					//		 ���� �� �����-�� ��� ��������� (���� for ��� ��� �� �� �� ����) -> ���� � main �������
					//else fillTable(classesList.find("__PROGRAM__")->second, classesList.find("__PROGRAM__")->second->methods.find("main")->second, classElement->stmt);
					break;
			}

			classElement = classElement->next;
		}
	}

	// TODO: ����������� �� ��������� (�� java/lang/Object)
	//newClass->pushOrFindConstant(*Constant::UTF8("<init>"));
}

// TODO: ����������� ��������� + ����������
void fillTable(Class* clazz, FuncNode* funcDef) {
	Method* method = new Method();
	method->name = funcDef->identifier->identifier;
	method->nameNumber = clazz->pushOrFindConstant(*Constant::UTF8(method->name));
	// ����������� ������� - AccessModifier -> AccessFlag
	switch (funcDef->accessModifier)
	{
		case _PRIVATE:
			method->accessModifier = PRIVATE;
			break;
		case _PROTECTED:
			method->accessModifier = PROTECTED;
			break;
		case _UNKNOWN: // ���� ��� ������������ �������, �� �� ������� public.
		case _PUBLIC:
			method->accessModifier = PUBLIC;
			break;
	}

	// ��������� ������
	int paramsCounter = 0;
	if (funcDef->args != nullptr) {
		// ������� ��������� (a,b,c,...)
		ExprNode* arg = funcDef->args->exprList->first;
		while (arg != nullptr) {
			paramsCounter++;

			method->localVars.push_back(arg->identifier);
			arg->paramLocalVarNum = method->localVars.size() - 1;

			arg = arg->next;
		}

		// TODO: ����������� ��������� (d=1,e=2,...)
		/*
		FuncArgNode* namedArg = funcDef->args->namedArgsList->first;
		while (namedArg != nullptr) {
			paramsCounter++;

			//TODO: �������� �������� � ��������� � �������
			method->localVars.push_back(namedArg->assignStmt->leftExpr->identifier);
			// TODO: �������� � expr node ������ ��������� � ������ ???

			namedArg = namedArg->next;
		}
		*/
	}

	method->suite = funcDef->suite;
	fillTable(clazz, method, method->suite);

	// ����������� �����������
	//string methodReturnType = defineMethodReturnType(funcDef);
	string methodDescriptor = generateMethodDescriptor(paramsCounter, "L__BASE__"); // TODO: ��������� �� ������ ������ ���� ������ __BASE__
	method->descriptorNumber = clazz->pushOrFindConstant(*Constant::UTF8(methodDescriptor));

	method->number = clazz->pushOrFindMethodRef(clazz->name, method->name, methodDescriptor);
	funcDef->idSemantic = method->number;
	// ������ �� super ����� __BASE__
	method->baseClassNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
	method->baseConstructorNumber = clazz->pushOrFindMethodRef("__BASE__", "<init>", "()V");
	clazz->methods[method->name] = method;
}

void fillTable(Class* clazz, Method* method, StmtsListNode* stmts) {
	if (stmts != nullptr) {
		StmtNode* stmt = stmts->first;
		while (stmt != nullptr) {
			fillTable(clazz, method, stmt);
			stmt = stmt->next;
		}
	}
}

// TODO
void fillTable(Class* clazz, Method* method, StmtNode* stmt) {
	switch (stmt->stmtType)
	{
		case _ASSIGN:
			if (!stmt->leftExpr->identifier.empty()) method->localVars.push_back(stmt->leftExpr->identifier);

			fillTable(clazz, method, stmt->leftExpr);
			fillTable(clazz, method, stmt->rightExpr);
			break;
		case _COMPOUND_ASSIGN:
			if (stmt->stmtsList != nullptr) {
				StmtNode* assignStmt = stmt->stmtsList->first;
				while (assignStmt != nullptr) {
					fillTable(clazz, method, assignStmt);
					assignStmt = assignStmt->next;
				}
			}
			break;
		case _IF:
			break;
		case _COMPOUND_IF:
			break;
		case _FOR:
			break;
		case _WHILE:
			// condition
			fillTable(clazz, method, stmt->expr->left);
			fillTable(clazz, method, stmt->suite);
			stmt->boolFieldMethodRef = clazz->pushOrFindFieldRef("__BASE__", "__bVal", "Z");
			break;
		case _RETURN:
			// TODO: ������������� �����������
			if (stmt->list != nullptr) {
				ExprNode* expr = stmt->list->first;
				//while (expr != nullptr) {
				//	fillTable(clazz, method, expr);
				//	expr = expr->next;
				//}
				checkReturnValue(clazz, method, expr);
				fillTable(clazz, method, expr);
			}
			break;
		case _EXPR_STMT:
			fillTable(clazz, method, stmt->expr);
			break;
		case _EXPR_LIST_STMT:
			break;
	}
}

// TODO
void fillTable(Class* clazz, Method* method, ExprNode* expr) {
	switch (expr->exprType)
	{
		case _INT_CONST:
			expr->valueNumber = clazz->pushOrFindConstant(*Constant::Integer(expr->intVal));
			expr->classNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "<init>", "(I)V");
			break;
		case _TRUE:
		case _FALSE:
			expr->valueNumber = clazz->pushOrFindConstant(*Constant::Integer(expr->boolVal));
			expr->classNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "<init>", "(Z)V");
			break;
		case _STRING_CONST:
			expr->valueNumber = clazz->pushOrFindConstant(*Constant::String(clazz->pushOrFindConstant(*Constant::UTF8(expr->stringVal))));
			expr->classNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "<init>", "(Ljava/lang/String;)V");
			break;
		case _FLOAT_CONST:
			expr->valueNumber = clazz->pushOrFindConstant(*Constant::Float(expr->floatVal));
			expr->classNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "<init>", "(F)V");
			break;
		case _IDENTIFIER:
			expr->valueNumber = clazz->pushOrFindConstant(*Constant::String(clazz->pushOrFindConstant(*Constant::UTF8(expr->identifier))));
			expr->classNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "<init>", "(Ljava/lang/String;)V");
			expr->paramLocalVarNum = method->localVars.size() - 1;
			break;
		case _PLUS: //TODO ID �������� �� number
			fillTable(clazz, method, expr->left);
			fillTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__add__", "(L__BASE__;)L__BASE__;");
			break;
		case _MINUS:
			fillTable(clazz, method, expr->left);
			fillTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__sub__", "(L__BASE__;)L__BASE__;");
			break;
		case _MUL:
			fillTable(clazz, method, expr->left);
			fillTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__mul__", "(L__BASE__;)L__BASE__;");
			break;
		case _DIV:
			fillTable(clazz, method, expr->left);
			fillTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__div__", "(L__BASE__;)L__BASE__;");
			break;
		case _EQUAL:
			fillTable(clazz, method, expr->left);
			fillTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__eql__", "(L__BASE__;)L__BASE__;");
			break;
		case _NOT_EQUAL:
			fillTable(clazz, method, expr->left);
			fillTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__not_eql__", "(L__BASE__;)L__BASE__;");
			break;
		case _LESS:
			fillTable(clazz, method, expr->left);
			fillTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__les__", "(L__BASE__;)L__BASE__;");
			break;
		case _LESS_EQUAL:
			fillTable(clazz, method, expr->left);
			fillTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__les_or_eql__", "(L__BASE__;)L__BASE__;");
			break;
		case _GREAT:
			fillTable(clazz, method, expr->left);
			fillTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__greater__", "(L__BASE__;)L__BASE__;");
			break;
		case _GREAT_EQUAL:
			fillTable(clazz, method, expr->left);
			fillTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__greater_or_eql__", "(L__BASE__;)L__BASE__;");
			break;
		case _U_PLUS:
			fillTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__unary_plus__", "()L__BASE__;");
			break;
		case _U_MINUS:
			fillTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__unary_minus__", "()L__BASE__;");
			break;
	}
}

// ������� ��������

void checkReturnValue(Class* clazz, Method* method, ExprNode* expr) {
	if (expr != nullptr) {
		switch (expr->exprType)
		{
			case _IDENTIFIER:
				// Main Method local vars
				vector<string> mainMethodLocalVars = classesList["__PROGRAM__"]->methods["main"]->localVars;
				if (find(mainMethodLocalVars.begin(), mainMethodLocalVars.end(), expr->identifier) != mainMethodLocalVars.end()) return;
				// Method local vars
				if (find(method->localVars.begin(), method->localVars.end(), expr->identifier) != method->localVars.end()) return;
				// Fields
				if (clazz->fields.find(expr->identifier) != clazz->fields.end()) return;

				throw runtime_error("S: ERROR -> local variable " + expr->identifier + " is not defined");
		}
	}
}

// ��������������� �������

// TODO: � �������� ���������� ����� ������������ � ������ ������, ��� ������� ����� ������ ����������
string generateMethodDescriptor(int paramsNumber, string returnValueDescriptor) {
	string descriptor = "(";
	
	// ���������� ����������
	for (int i = 0; i < paramsNumber; i++) {
		descriptor += "L__BASE__;";
	}
	// ������������ ��������
	descriptor += ")" + returnValueDescriptor + ";";

	return descriptor;
}

// TODO: ����������� ������������� ����������� (������ ����� ������� ������ ���� �������)
string defineMethodReturnType(Method* method) {
	vector<ExprNode*> returnValues = {};

	StmtNode* lastStmt = method->suite->last;
	if (lastStmt->stmtType == _RETURN) {
		if (lastStmt->list != nullptr) {
			ExprNode* expr = lastStmt->list->first;

			switch (expr->exprType)
			{
				case _STRING_CONST:
				case _INT_CONST:
				case _FLOAT_CONST:
					return "L__BASE__";
				case _IDENTIFIER:
					if (!expr->identifier.empty()) {
						if (find(method->localVars.begin(), method->localVars.end(), expr->identifier) != method->localVars.end()) {
							if (classesList.find(expr->identifier) != classesList.end()) return "L" + expr->identifier;
						}
						// TODO: �������� ��������� ����������
						//else throw runtime_error("S: ERROR -> Something went wrong...");
					}
					return "";
			}
		}
	}

	//throw runtime_error("S: ERROR -> Unsupported type in Return Stmt");
	return "";
}
