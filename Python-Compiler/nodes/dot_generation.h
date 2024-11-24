#pragma once
#include "nodes.h"

string generateDotFromExprNode(ExprNode* node);

string generateDotFromExprListNode(ExprListNode* node);

string generateDotFromSlicingNode(SlicingNode* node);

string generateDotFromTargetNode(TargetNode* node);