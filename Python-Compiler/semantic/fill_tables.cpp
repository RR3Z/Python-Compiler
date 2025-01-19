#pragma once
#include "./semantic.h"
#include <iostream>
using namespace std;

std::map<std::string, Class*> classesList;

// ========= ���������� ������ =========

// ���������� ������ (��������� �������)
void fillTables(FileNode* program) {
	// �������� ������, ��� ����� ����� � ���������
	Class* entryClass = new Class();
	entryClass->name = "__PROGRAM__";
	// ���������� ������ � ���������� �������
	classesList[entryClass->name] = entryClass;

	// ���������� �������� ������
	entryClass->pushOrFindConstant(*Constant::UTF8("Code")); // �� ���� �����������, ��� ��� � ��� ����� ����������� �� ���������
	entryClass->number = entryClass->pushOrFindConstant(*Constant::Class(entryClass->pushOrFindConstant(*Constant::UTF8(entryClass->name))));

	// ���������� RTL � �����
	addRTLToClass(entryClass);

	// ���������� ������������� ������
	int parentClassName = entryClass->pushOrFindConstant(*Constant::UTF8("java/lang/Object"));
	entryClass->parentNumber = entryClass->pushOrFindConstant(*Constant::Class(parentClassName));

	// �������� �������, ��� ����� ����� � ���������
	Method* mainMethod = new Method();
	mainMethod->name = "main";
	mainMethod->descriptor = "([Ljava/lang/String;)V";
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
					fillMethodTable(entryClass, programElement->funcDef);
					break;
				case _STMT:
					/*
						� Python ��� ����� ����� ����� � ���������. � ����� ������, � ��� ��� ����������� ������ ����.
						��� ������, ��� ���� � ��� ����������� ���-�� � ���������� ������� ��������� ����� funcDef, classDef 
						� assignStmt(������ � ���� ������) ���� ����� � main �����.
					*/

					// ���� Assign Stmt, �� ���������� ��������� � ������� �����
					if (programElement->stmt->stmtType == _COMPOUND_ASSIGN && programElement->stmt->stmtsList != nullptr) {
						fillFieldTable(entryClass, programElement->stmt->stmtsList);
					}

					// ���������� ���� main ������
					if (mainMethod->suite == nullptr) mainMethod->suite = new StmtsListNode();

					if (mainMethod->suite->first != nullptr) {
						mainMethod->suite->last->next = programElement->stmt;
						mainMethod->suite->last = programElement->stmt;
					}
					else {
						mainMethod->suite->first = programElement->stmt;
						mainMethod->suite->last = programElement->stmt;
					}

					fillMethodTable(entryClass, mainMethod, programElement->stmt);
					
					break;
			}

			// ��������� � ���������� ��������
			programElement = programElement->next;
		}
	}
}

// TODO: ������
void fillTables(ClassNode* classDef) {
	// �������� ������ ������
	Class* newClass = new Class();
	// ���������� ������ � ���������� �������
	newClass->name = classDef->identifier->stringVal;
	classesList[newClass->name] = newClass;

	// ���������� �������� � �����
	newClass->pushOrFindConstant(*Constant::UTF8("Code"));
	newClass->number = newClass->pushOrFindConstant(*Constant::Class(newClass->pushOrFindConstant(*Constant::UTF8(newClass->name))));

	// ���������� RTL � �����
	addRTLToClass(newClass);

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
					//fillMethodTable(newClass, classElement->funcDef);
					break;
				case _STMT_NODE:
					break;
			}

			classElement = classElement->next;
		}
	}

	// TODO: ����������� �� ��������� (�� java/lang/Object)
	//newClass->pushOrFindConstant(*Constant::UTF8("<init>"));
}

// ========= ���������� ������ ������� =========

// TODO: ����������� ��������� + ����������
void fillMethodTable(Class* clazz, FuncNode* funcDef) {
	checkMethodForErrors(funcDef);

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
	//method->localVars.push_back(clazz->name); // ������ ���� ����� �������, ������ ���������� ������ ���� this (������ �� �����, ������ �������� ����������� �������), ���� ��� �������������
	if (funcDef->args != nullptr) {
		// ������� ��������� (a,b,c,...)
		ExprNode* arg = funcDef->args->exprList->first;
		while (arg != nullptr) {
			paramsCounter++;
			method->localVars.push_back(arg->identifier);

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
	fillMethodTable(clazz, method, method->suite);

	// ����������� �����������
	string methodReturnType = defineMethodReturnType(method);
	string methodDescriptor = generateMethodDescriptor(paramsCounter, methodReturnType);
	method->descriptor = methodDescriptor;
	method->descriptorNumber = clazz->pushOrFindConstant(*Constant::UTF8(methodDescriptor));

	method->number = clazz->pushOrFindMethodRef(clazz->name, method->name, methodDescriptor);
	funcDef->idSemantic = method->number;
	// ������ �� super ����� __BASE__
	method->baseClassNumber = clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));
	method->baseConstructorNumber = clazz->pushOrFindMethodRef("__BASE__", "<init>", "()V");
	clazz->methods[method->name] = method;
}

void fillMethodTable(Class* clazz, Method* method, StmtsListNode* stmts) {
	if (stmts != nullptr) {
		StmtNode* stmt = stmts->first;
		while (stmt != nullptr) {
			fillMethodTable(clazz, method, stmt);

			if (stmt->stmtType == StmtType::_RETURN) {
				stmt->next = nullptr;
				if(method->suite == stmts) method->suite->last = stmt;
			}
			
			stmt = stmt->next;
		}
	}
}

// TODO
void fillMethodTable(Class* clazz, Method* method, StmtNode* stmt) {
	switch (stmt->stmtType)
	{
		case _ASSIGN:
			if (!stmt->leftExpr->identifier.empty()) {
				if (find(method->localVars.begin(), method->localVars.end(), stmt->leftExpr->identifier) != method->localVars.end()) {
					stmt->leftExpr->paramLocalVarNum = findElementIndexInVector(method->localVars, stmt->leftExpr->identifier);
				}

				// ���� ���������� �� �������� ����� ������, ��������� �� � �������� ��������� ���������� � ��������� �� ��� � constant pool ������
				if (clazz->fields.find(stmt->leftExpr->identifier) == clazz->fields.end()) {
					method->localVars.push_back(stmt->leftExpr->identifier);
					stmt->leftExpr->paramLocalVarNum = findElementIndexInVector(method->localVars, stmt->leftExpr->identifier);
					fillMethodTable(clazz, method, stmt->leftExpr);
				}
				else {
					stmt->number = clazz->fields[stmt->leftExpr->identifier]->number;
				}
			}

			fillMethodTable(clazz, method, stmt->rightExpr);
			break;
		case _COMPOUND_ASSIGN:
			if (stmt->stmtsList != nullptr) {
				StmtNode* assignStmt = stmt->stmtsList->first;
				while (assignStmt != nullptr) {
					fillMethodTable(clazz, method, assignStmt);
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
			fillMethodTable(clazz, method, stmt->expr->left);
			fillMethodTable(clazz, method, stmt->suite);
			stmt->boolFieldMethodRef = clazz->pushOrFindFieldRef("__BASE__", "__bVal", "Z");
			break;
		case _RETURN:
			// TODO: ������������� �����������
			if (stmt->list != nullptr) {
				ExprNode* expr = stmt->list->first;

				checkReturnValue(clazz, method, expr);
				fillMethodTable(clazz, method, expr);
			}
			break;
		case _EXPR_STMT:
			fillMethodTable(clazz, method, stmt->expr);
			break;
		case _EXPR_LIST_STMT:
			break;
	}
}

// TODO
void fillMethodTable(Class* clazz, Method* method, ExprNode* expr) {
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
			expr->paramLocalVarNum = findElementIndexInVector(method->localVars, expr->identifier);
			break;
		case _PLUS:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__add__", "(L__BASE__;)L__BASE__;");
			break;
		case _MINUS:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__sub__", "(L__BASE__;)L__BASE__;");
			break;
		case _MUL:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__mul__", "(L__BASE__;)L__BASE__;");
			break;
		case _DIV:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__div__", "(L__BASE__;)L__BASE__;");
			break;
		case _EQUAL:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__eql__", "(L__BASE__;)L__BASE__;");
			break;
		case _NOT_EQUAL:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__not_eql__", "(L__BASE__;)L__BASE__;");
			break;
		case _LESS:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__les__", "(L__BASE__;)L__BASE__;");
			break;
		case _LESS_EQUAL:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__les_or_eql__", "(L__BASE__;)L__BASE__;");
			break;
		case _GREAT:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__greater__", "(L__BASE__;)L__BASE__;");
			break;
		case _GREAT_EQUAL:
			fillMethodTable(clazz, method, expr->left);
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__greater_or_eql__", "(L__BASE__;)L__BASE__;");
			break;
		case _U_PLUS:
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__unary_plus__", "()L__BASE__;");
			break;
		case _U_MINUS:
			fillMethodTable(clazz, method, expr->right);
			expr->number = clazz->pushOrFindMethodRef("__BASE__", "__unary_minus__", "()L__BASE__;");
			break;
		case _FUNCTION_CALL:
			if (expr->funcArgs != nullptr) {
				ExprNode* arg = expr->funcArgs->exprList->first;
				while (arg != nullptr) {
					fillMethodTable(clazz, method, arg);
					arg = arg->next;
				}
			}

			// ���������� ������� (RTL ��� �������)
			if (expr->left->identifier == "print") {
				if (expr->funcArgs != nullptr) {
					if (expr->funcArgs->exprList->first != expr->funcArgs->exprList->last) {
						throw runtime_error("S: ERROR -> Wrong amount of args in function call with name: " + expr->left->identifier);
					}
					expr->number = clazz->pushOrFindMethodRef("__BASE__", "print", "(L__BASE__;)V");
				} else expr->number = clazz->pushOrFindMethodRef("__BASE__", "print", "()V");
			}
			else if (expr->left->identifier == "input") {
				if (expr->funcArgs != nullptr) {
					if (expr->funcArgs->exprList->first != expr->funcArgs->exprList->last) {
						throw runtime_error("S: ERROR -> Wrong amount of args in function call with name: " + expr->left->identifier);
					}
					expr->number = clazz->pushOrFindMethodRef("__BASE__", "input", "(L__BASE__;)L__BASE__;");
				}
				else expr->number = clazz->pushOrFindMethodRef("__BASE__", "input", "()L__BASE__;");
			}
			else expr->number = clazz->pushOrFindMethodRef(clazz->name, expr->left->identifier, clazz->methods[expr->left->identifier]->descriptor);
			
			break;
	}
}

// ========= ���������� ������ ����� =========

void fillFieldTable(Class* clazz, StmtsListNode* compoundAssign) {
	if (compoundAssign->first != nullptr) {
		StmtNode* assignStmt = compoundAssign->first;
		while (assignStmt != nullptr) {
			fillFieldTable(clazz, assignStmt);
			assignStmt = assignStmt->next;
		}
	}
}

void fillFieldTable(Class* clazz, StmtNode* assignStmt) {
	Field* field = new Field();

	// ���
	field->name = assignStmt->leftExpr->identifier;
	field->nameNumber = clazz->pushOrFindConstant(*Constant::UTF8(field->name));
	field->nameNode = assignStmt->leftExpr;

	// ����������� ������� - AccessModifier -> AccessFlag
	switch (assignStmt->accessModifier) {
	case _PRIVATE:
		field->accessModifier = PRIVATE;
		break;
	case _PROTECTED:
		field->accessModifier = PROTECTED;
		break;
	case _UNKNOWN: // ���� ��� ������������ �������, �� �� ��������� public.
	case _PUBLIC:
		field->accessModifier = PUBLIC;
		break;
	}

	// ����������
	field->descriptor = "L__BASE__;";
	field->descriptorNumber = clazz->pushOrFindConstant(*Constant::UTF8(field->descriptor));

	// FieldRef
	field->number = clazz->pushOrFindFieldRef(clazz->name, field->name, field->descriptor);
	assignStmt->number = field->number;

	clazz->fields[field->name] = field;
}

// ========= RTL =========

void addRTLToClass(Class* clazz) {
	// �������� �������� ������
	clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"));
	clazz->pushOrFindConstant(*Constant::Class(clazz->pushOrFindConstant(*Constant::UTF8("__BASE__"))));

	// ������ ��� ������ ������ � �������
	clazz->pushOrFindMethodRef("__BASE__", "print", "(L__BASE__;)V");
	clazz->pushOrFindMethodRef("__BASE__", "print", "()V");

	// ������ ��� ��������� ������ �� �������
	clazz->pushOrFindMethodRef("__BASE__", "input", "(L__BASE__;)L__BASE__;");
	clazz->pushOrFindMethodRef("__BASE__", "input", "()L__BASE__;");
}

// ========= ������� �������� =========

void checkReturnValue(Class* clazz, Method* method, ExprNode* expr) {
	if (expr != nullptr) {
		switch (expr->exprType)
		{
			case _IDENTIFIER:
				// Main Method local vars - TODO: �� ���� �� ���� ��������� (���������)
				//vector<string> mainMethodLocalVars = classesList["__PROGRAM__"]->methods["main"]->localVars;
				//if (find(mainMethodLocalVars.begin(), mainMethodLocalVars.end(), expr->identifier) != mainMethodLocalVars.end()) return;

				// Method local vars
				if (find(method->localVars.begin(), method->localVars.end(), expr->identifier) != method->localVars.end()) return;
				// Fields
				if (clazz->fields.find(expr->identifier) != clazz->fields.end()) return;

				throw runtime_error("S: ERROR -> local variable " + expr->identifier + " is not defined");
		}
	}
}

void checkMethodForErrors(FuncNode* funcDef) {
	if (funcDef != nullptr) {
		if (funcDef->identifier->identifier == "print") {
			throw runtime_error("S: ERROR -> Changes to the signature of the \"print\" function!");
		}

		if (funcDef->identifier->identifier == "input") {
			throw runtime_error("S: ERROR -> Changes to the signature of the \"input\" function!");
		}

		if (funcDef->identifier->identifier == "main") {
			throw runtime_error("S: ERROR -> Changes to the signature of the \"main\" function!");
		}
	}
}

// ========= ��������������� ������� =========

// TODO: � �������� ���������� ����� ������������ � ������ ������, ��� ������� ����� ������ ����������
string generateMethodDescriptor(int paramsNumber, string returnValueDescriptor) {
	string descriptor = "(";
	
	// ���������� ����������
	for (int i = 0; i < paramsNumber; i++) {
		descriptor += "L__BASE__;";
	}
	// ������������ ��������
	descriptor += ")" + returnValueDescriptor;

	return descriptor;
}

// TODO: ����������� ������������� ����������� (������ ����� ������� ������ ���� �������)
string defineMethodReturnType(Method* method) {
	vector<ExprNode*> returnValues = {};

	if (method->suite == nullptr) return "V";

	StmtNode* suiteStmt = method->suite->first;
	while (suiteStmt != nullptr) {
		switch (suiteStmt->stmtType)
		{
			case _RETURN:
				return "L__BASE__;";
		}

		suiteStmt = suiteStmt->next;
	}

	return "V";
}

int findElementIndexInVector(vector<string> vec, string element) {
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i] == element) {
			return i;
		}
	}

	return -1;
}
