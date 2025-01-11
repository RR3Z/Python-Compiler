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