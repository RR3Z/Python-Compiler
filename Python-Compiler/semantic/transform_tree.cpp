#pragma once

#include "./semantic.h"

void transformTree(FileNode* program) {
	if (program->elementsList != nullptr) {
		FileElementNode* node = program->elementsList->first;
		while (node != nullptr) {
			// TODO: transform(node);
			node = node->next;
		}
	}
}

void transform(StmtsListNode* stmt_list) {
	if (stmt_list != 0) {
		StmtNode* node = stmt_list->first;
		while (node != nullptr) {
			transform(node); // -> для этого ниже 
			node = node->next;
		}
	}
}

// TODO: ALL TRANSFORM
void transform(StmtNode* stmt) {
	switch (stmt->stmtType) {
	case _IF:
		//TODO:  ДЛЯ ЭТОГО ОТДЕЛЬНЫЙ transform (где просто if, а также все с прочим elif,else...)   
		// Т.к нет отдельной структуры с IF, назвал transformIF
		transformIf(stmt);
		break;
	case _ASSIGN:
		//transform(stmt->if_stmt_f);
		// TODO:  по идее, все что связано с ASSIGN проверять в expr transform
		break;
	case _WHILE:
		// АНАЛОГИЧНО IF, спасибо Python за возможность else))))))
		transformWhile(stmt);
		break;
	case _RETURN:
		// TODO: сюда обычный transform expr
		break;
	case _TRY:
		// TODO: по идее аналогично с IF, expt exptID finally и тд спонсоры показа
		break;
	case _FOR:
		// АНАЛОГИЧНО IF, спасибо Python за возможность else))))))
		transformFor(stmt);
		break;
	case _EXPR_STMT:
		// Просто transform expr, а в нем уже разбор листа
		break;
	default:
		break;
	}
}

//TODO: Реализация для func/class/slicing


//Реализация для IF
void transformIf(StmtNode* stmt) {
	//transform(stmt->expr); // Проверить - тут надо condition
	transform(stmt->suite);

	if (stmt->stmtsList != nullptr) {
		StmtNode* node = stmt->stmtsList->first;
		while (node != 0) {
			//transform(node->expr); TODO: EXPR
			transform(node->suite);
			node = node->next;
		}
	}

	// По идее, тут ветка else, у нас можно проверять right, надо проверять
	if (stmt->rightNode != nullptr) {
		transform(stmt->rightNode);
	}
}


//Реализация для for
// Т.к не выделяем отдельную структуру назвал так,
void transformFor(StmtNode* stmt) {
	//transform(stmt->expr); // Проверить - тут надо condition
	transform(stmt->suite);

	// По идее, тут ветка else, у нас можно проверять right, надо проверять
	if (stmt->rightNode != nullptr) {
		transform(stmt->rightNode);
	}
}
                                                                                       // Объединение for/while - ??????????????????????????
//Реализация для While
// Т.к не выделяем отдельную структуру назвал так,
void transformWhile(StmtNode* stmt) {
	//transform(stmt->expr); // Проверить - тут надо condition
	transform(stmt->suite);

	// По идее, тут ветка else, у нас можно проверять right, надо проверять
	if (stmt->rightNode != nullptr) {
		transform(stmt->rightNode);
	}
}

//Реализация для TRY
// Т.к не выделяем отдельную структуру назвал так,
void transformTRY(StmtNode* stmt) {
	//transform(stmt->expr); // Проверить - тут надо condition
	transform(stmt->suite);

	//TODO:  прописать разные моменты( с exept, finally и тд)


	// По идее, тут ветка else, у нас можно проверять right, надо проверять
	if (stmt->rightNode != nullptr) {
		transform(stmt->rightNode);
	}
}
