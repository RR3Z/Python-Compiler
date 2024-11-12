#pragma once
#include "nodes.h"

/* ========== EXPRESSION ========== */

struct ExprNode* createIdExprNode(string* id);

struct ExprNode* createIntConstExprNode(int value);

struct ExprNode* createFloatConstExprNode(float value);

struct ExprNode* createStringConstExprNode(string text);

struct ExprNode* createTrueConstExprNode();

struct ExprNode* createFalseConstExprNode();

struct ExprNode* createSelfExprNode();

struct ExprNode* createSuperExprNode();

struct ExprNode* createPlusExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand);

struct ExprNode* createUnaryPlusExprNode(struct ExprNode* value);

struct ExprNode* createMinusExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand);

struct ExprNode* createUnaryMinusExprNode(struct ExprNode* value);

struct ExprNode* createMulExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand);

struct ExprNode* createDivExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand);

struct ExprNode* createGreatExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand);

struct ExprNode* createGreateEqualExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand);

struct ExprNode* createLessExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand);

struct ExprNode* createLessEqualExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand);

struct ExprNode* createEqualExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand);

struct ExprNode* createNotEqualExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand);

// my part
struct ExprNode* createAndLogicExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand);

struct ExprNode* createAndBitwiseExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand);

struct ExprNode* createOrLogicExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand);

struct ExprNode* createOrBitwiseExprNode(struct ExprNode* leftOperand, struct ExprNode* rightOperand);
