#include "nodes/dot_generation.h"
#include "semantic/semantic.h"
#include "codeGen/codeGen.h"

extern int yyparse();
extern FILE* yyin;
extern struct FileNode* fileRoot;
extern map<string, Class*> classesList;

int main(int argc, const char* argv[])
{
	try
	{
		// INPUT DATA
#ifdef  _DEBUG
		fopen_s(&yyin, "input.txt", "r");
#else
		if (argc != 3)
		{
			cout << "Incorrect amount of args!" << endl;
			return 1;
		}

		fopen_s(&yyin, argv[1], "r");
#endif
		// GRAMMAR
		if (!yyin)
		{
			cout << "Couldn't open file! Check the path!" << endl;
			return 1;
		}
		yyparse();
		fclose(yyin);

		// SEMANTIC
		cout << "\n\n========= SEMANTIC =========" << endl;
		transformTree(fileRoot);
		fillTables(fileRoot);

		// CODE GENERATION
		cout << "\n\n========= CODE GENERATION =========" << endl;
		cout << "Compiling __BASE__.java file..." << endl;
		system("javac __BASE__.java");
		cout << "Completed!" << endl;

		generate(fileRoot, classesList);
		cout << "\n\n========= __PROGRAM__ FILE CONTENT =========" << endl;
		system("javap -verbose __PROGRAM__.class");

		cout << "\n\n========= __PROGRAM__ CALL =========" << endl;
		system("java -noverify -cp . __PROGRAM__");
		//system("java -cp . __PROGRAM__");
		cout << "\n\n" << endl;

		// DOT GENERATION
	/*
	#ifdef  _DEBUG
		string parseRes = "digraph G {\n" + generateDotFromRoot(fileRoot) + "}";
		FILE* dotFile;
		fopen_s(&dotFile, "diagram.dot", "w");
		fprintf(dotFile, parseRes.c_str());
		fclose(dotFile);

		// Image generation
		system("cd");
		system("Graphviz\\bin\\dot.exe -Tpng -O diagram.dot");
		system("diagram.dot.png");
	#else
		string parseRes = "digraph G {\n" + generateDotFromRoot(fileRoot) + "}";
		FILE* dotFile;
		char fileName[256];
		snprintf(fileName, sizeof(fileName), "%s.dot", argv[2]);
		fopen_s(&dotFile, fileName, "w");
		fprintf(dotFile, parseRes.c_str());
		fclose(dotFile);
	#endif
	*/
	}
	catch (const runtime_error& error)
	{
		cout << error.what() << endl;
		return 1;
	}

	return 0;
}
