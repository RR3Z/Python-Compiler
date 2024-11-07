#include <iostream>

extern int yyparse();
extern FILE* yyin;

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
}
