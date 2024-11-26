#pragma once
#include "nodes.h"

string generateDotFromRoot(FileNode* root);

string generateDotFromFileElementNode(int parentId, FileElementNode* node);

string generateDotFromClassNode(ClassNode* node);

string generateDotFromFuncNode(FuncNode* node);

string generateDotFromStmtNode(StmtNode* node);

string generateDotFromStmtsListNode(StmtsListNode* node);

string generateDotFromFuncArgsListNode(FuncArgsListNode* node);

string generateDotFromFuncArgNode(int parentId, FuncArgNode* node);

string generateDotFromClassElementsListNode(ClassElementsListNode* node);

string generateDotFromClassElementNode(int parentId, ClassElementNode* node);

string generateDotFromExprNode(ExprNode* node);

string generateDotFromExprListNode(ExprListNode* node);

string generateDotFromSlicingNode(SlicingNode* node);
