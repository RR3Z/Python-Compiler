#include <iostream>
#include <string>
#include "nodes/dot_generation.h"
using namespace std;

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
		cout << "Incorrect amount of args! The only argument is the file name" << endl;
		return 1;
	}

	fopen_s(&yyin, argv[1], "r");
#endif //  _DEBUG

	
	if (!yyin)
	{
		cout << "Couldn't open file! Check the path!" << endl;
		return 1;
	}

	yyparse();
	fclose(yyin);

	string parseRes = "digraph G {\n" + generateDotFromExprNode(exprTest) + "}";
	//cout << parseRes << endl;
	FILE* dotFile;
	fopen_s(&dotFile, "diagram.dot", "w");
	fprintf(dotFile, parseRes.c_str());
	fclose(dotFile);

	// Auto generate image from Dot Code
	system("cd");
	system("Graphviz\\bin\\dot.exe -Tpng -O diagram.dot");
	system("diagram.dot.png");
}
