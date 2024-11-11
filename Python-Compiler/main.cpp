#include <iostream>
#include <string>
#include "nodes/dot_generation.h"

extern int yyparse();
extern FILE* yyin;
extern struct ExprNode* exprTest;

int main(int argc, const char* argv[])
{
#ifdef  _DEBUG
	fopen_s(&yyin, "input.txt", "r");
#else
	if (argc != 2)
	{
		std::cout << "Incorrect amount of args! The only argument is the file name" << std::endl;
		return 1;
	}

	fopen_s(&yyin, argv[1], "r");
#endif //  _DEBUG

	
	if (!yyin)
	{
		std::cout << "Couldn't open file! Check the path!" << std::endl;
		return 1;
	}

	yyparse();
	fclose(yyin);

	std::string parseRes = "digraph G {\n" + generateDotFromExprNode(exprTest) + "}";
	std::cout << parseRes << std::endl;
	FILE* dotFile;
	fopen_s(&dotFile, "diagram.dot", "w");
	fprintf(dotFile, parseRes.c_str());
	fclose(dotFile);
}
