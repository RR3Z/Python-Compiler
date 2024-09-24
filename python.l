%option never-interactive
%option noyywrap
%option c++

%{
	#include <iostream>
	extern yyFlexLexer *lexer = new yyFlexLexer();

	#define PRINT_HARD_KEYWORD() std::cout << "Found hard keyword: " << lexer->YYText();
	#define PRINT_SOFT_KEYWORD() std::cout << "Found soft keyword: " << lexer->YYText();
	#define PRINT_OPERATOR() std::cout << "Found operator: " << lexer->YYText();
	#define PRINT_DELIMITER() std::cout << "Found delimiter: " << lexer->YYText();
%}

%%

\+ { PRINT_OPERATOR(); }
\- { PRINT_OPERATOR(); }
\* { PRINT_OPERATOR(); }
\*\* { PRINT_OPERATOR(); }
\/ { PRINT_OPERATOR(); }
\/\/ { PRINT_OPERATOR(); }
\% { PRINT_OPERATOR(); }
\@ { PRINT_OPERATOR(); } // ERROR: Used as delimiter (@decorator) and as operator (a = matrix1 @ matrix2)
\<\< { PRINT_OPERATOR(); }
\< { PRINT_OPERATOR(); }
\>\> { PRINT_OPERATOR(); }
\> { PRINT_OPERATOR(); }
\>\= { PRINT_OPERATOR(); }
\<\= { PRINT_OPERATOR(); }
\& { PRINT_OPERATOR(); }
\| { PRINT_OPERATOR(); }
\^ { PRINT_OPERATOR(); }
\~ { PRINT_OPERATOR(); }
\:\= { PRINT_OPERATOR(); }
\=\= { PRINT_OPERATOR(); }
\!\= { PRINT_OPERATOR(); }

\( { PRINT_DELIMITER(); }
\) { PRINT_DELIMITER(); }
\[ { PRINT_DELIMITER(); }
\] { PRINT_DELIMITER(); }
\{ { PRINT_DELIMITER(); }
\} { PRINT_DELIMITER(); }
\, { PRINT_DELIMITER(); }
\: { PRINT_DELIMITER(); }
\! { PRINT_DELIMITER(); }
\. { PRINT_DELIMITER(); }
\; { PRINT_DELIMITER(); }
\@ { PRINT_DELIMITER(); } // ERROR: Used as delimiter (@decorator) and as operator (a = matrix1 @ matrix2)
\= { PRINT_DELIMITER(); }
\-\> { PRINT_DELIMITER(); }
\+\= { PRINT_DELIMITER(); }
\-\= { PRINT_DELIMITER(); }
\*\= { PRINT_DELIMITER(); }
\/\= { PRINT_DELIMITER(); }
\/\/\= { PRINT_DELIMITER(); }
\%\= { PRINT_DELIMITER(); }
\@\= { PRINT_DELIMITER(); }
\&\= { PRINT_DELIMITER(); }
\|\= { PRINT_DELIMITER(); }
\^\= { PRINT_DELIMITER(); }
\>\>\= { PRINT_DELIMITER(); }
\<\<\= { PRINT_DELIMITER(); }
\*\*\= { PRINT_DELIMITER(); }

False { PRINT_HARD_KEYWORD(); }     
None { PRINT_HARD_KEYWORD(); }      
True { PRINT_HARD_KEYWORD(); }        
and { PRINT_HARD_KEYWORD(); }           
as { PRINT_HARD_KEYWORD(); }           
assert { PRINT_HARD_KEYWORD(); }          
async { PRINT_HARD_KEYWORD(); }     
await { PRINT_HARD_KEYWORD(); }  
break { PRINT_HARD_KEYWORD(); }
class { PRINT_HARD_KEYWORD(); }  
continue { PRINT_HARD_KEYWORD(); }
def { PRINT_HARD_KEYWORD(); }
del { PRINT_HARD_KEYWORD(); }
elif { PRINT_HARD_KEYWORD(); }
else { PRINT_HARD_KEYWORD(); }
except { PRINT_HARD_KEYWORD(); }
finally { PRINT_HARD_KEYWORD(); }
for { PRINT_HARD_KEYWORD(); }
from { PRINT_HARD_KEYWORD(); }
global { PRINT_HARD_KEYWORD(); }
if { PRINT_HARD_KEYWORD(); }
import { PRINT_HARD_KEYWORD(); }
in { PRINT_HARD_KEYWORD(); }
is { PRINT_HARD_KEYWORD(); }
lambda { PRINT_HARD_KEYWORD(); }
nonlocal { PRINT_HARD_KEYWORD(); }
not { PRINT_HARD_KEYWORD(); }
or { PRINT_HARD_KEYWORD(); }
pass { PRINT_HARD_KEYWORD(); }
raise { PRINT_HARD_KEYWORD(); }
return { PRINT_HARD_KEYWORD(); }
try { PRINT_HARD_KEYWORD(); }
while { PRINT_HARD_KEYWORD(); }
with { PRINT_HARD_KEYWORD(); }
yield { PRINT_HARD_KEYWORD(); }

match { PRINT_SOFT_KEYWORD(); }
case { PRINT_SOFT_KEYWORD(); }
_ { PRINT_SOFT_KEYWORD(); }
type { PRINT_SOFT_KEYWORD(); }

. {}
%%

int main(int argc, char *argv[])
{
	lexer->yylex();
	return 0;
}