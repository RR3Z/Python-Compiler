%{
	#include <iostream>
	extern yyFlexLexer *flexLexer = new yyFlexLexer();
%}

%option never-interactive
%option noyywrap
%option c++
%%
[0-9]* {std::cout << flexLexer->YYText() << std::endl;}

%%
int main(int argc, char *argv[])
{
	flexLexer->yylex();
	return 0;
}