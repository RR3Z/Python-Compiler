#pragma once
#include "nodes.h"

string generateDotFromExprNode(ExprNode* node);

string generateDotFromExprListNode(ExprListNode* node);

string generateDotFromIdentifierListNode(IdentifierListNode* node);

string generateDotFromTargetListNode(TargetListNode* node);

string generateDotFromSlicingNode(SlicingNode* node);
