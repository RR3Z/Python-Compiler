%option never-interactive
%option noyywrap
%option c++

%{
	#include <iostream>
	#include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <iostream>
    #include <string>
    #include <ctype.h>
    using namespace std;

	extern yyFlexLexer *lexer = new yyFlexLexer();

	#define PRINT_HARD_KEYWORD() std::cout << "Found hard keyword: " << lexer->YYText() << std::endl;
	#define PRINT_SOFT_KEYWORD() std::cout << "Found soft keyword: " << lexer->YYText() << std::endl;
	#define PRINT_OPERATOR() std::cout << "Found operator: " << lexer->YYText() << std::endl;
	#define PRINT_DELIMITER() std::cout << "Found delimiter: " << lexer->YYText() << std::endl;
	#define PRINT_ID() std::cout << "Found identifier: " << lexer->YYText() << std::endl;
%}


%x STRING_SINGLE_QUOTES
%x STRING_DOUBLE_QUOTES
%x STRING_TRIPLE_SINGLE_QUOTES
%x STRING_TRIPLE_DOUBLE_QUOTES


%%

%{

char buffer[1024];
string str;

%}



['"] {
if(yytext[0] == '\''){
BEGIN(STRING_SINGLE_QUOTES);
}else{
BEGIN(STRING_DOUBLE_QUOTES);
}
str.clear();
}
\'\'\' { BEGIN(STRING_TRIPLE_SINGLE_QUOTES); str.clear(); }
\"\"\" { BEGIN(STRING_TRIPLE_DOUBLE_QUOTES); str.clear(); }

<STRING_SINGLE_QUOTES>([^\\\"]|\\.)+ { str += yytext; }
<STRING_DOUBLE_QUOTES>([^\\\"]|\\.)++ { str += yytext; }
<STRING_TRIPLE_SINGLE_QUOTES>[^\\\']+ { str += yytext; }
<STRING_TRIPLE_DOUBLE_QUOTES>[^\\\"]++ { str += yytext; }

<STRING_SINGLE_QUOTES><STRING_DOUBLE_QUOTES><STRING_TRIPLE_SINGLE_QUOTES><STRING_TRIPLE_DOUBLE_QUOTES>\\\\ { str += '\\'; }
<STRING_SINGLE_QUOTES><STRING_DOUBLE_QUOTES><STRING_TRIPLE_SINGLE_QUOTES><STRING_TRIPLE_DOUBLE_QUOTES>\\\' { str += '\''; }
<STRING_SINGLE_QUOTES><STRING_DOUBLE_QUOTES><STRING_TRIPLE_SINGLE_QUOTES><STRING_TRIPLE_DOUBLE_QUOTES>\\\" { str += '\"'; }
<STRING_SINGLE_QUOTES><STRING_DOUBLE_QUOTES><STRING_TRIPLE_SINGLE_QUOTES><STRING_TRIPLE_DOUBLE_QUOTES>\\a { str += '\a'; }
<STRING_SINGLE_QUOTES><STRING_DOUBLE_QUOTES><STRING_TRIPLE_SINGLE_QUOTES><STRING_TRIPLE_DOUBLE_QUOTES>\\b { str += '\b'; }
<STRING_SINGLE_QUOTES><STRING_DOUBLE_QUOTES><STRING_TRIPLE_SINGLE_QUOTES><STRING_TRIPLE_DOUBLE_QUOTES>\\f { str += '\f'; }
<STRING_SINGLE_QUOTES><STRING_DOUBLE_QUOTES><STRING_TRIPLE_SINGLE_QUOTES><STRING_TRIPLE_DOUBLE_QUOTES>\\\\n { }
<STRING_SINGLE_QUOTES><STRING_DOUBLE_QUOTES><STRING_TRIPLE_SINGLE_QUOTES><STRING_TRIPLE_DOUBLE_QUOTES>\\n { str += '\n'; }
<STRING_SINGLE_QUOTES><STRING_DOUBLE_QUOTES><STRING_TRIPLE_SINGLE_QUOTES><STRING_TRIPLE_DOUBLE_QUOTES>\\r { str += '\r'; }
<STRING_SINGLE_QUOTES><STRING_DOUBLE_QUOTES><STRING_TRIPLE_SINGLE_QUOTES><STRING_TRIPLE_DOUBLE_QUOTES>\\t { str += '\t'; }
<STRING_SINGLE_QUOTES><STRING_DOUBLE_QUOTES><STRING_TRIPLE_SINGLE_QUOTES><STRING_TRIPLE_DOUBLE_QUOTES>\\v { str += '\v'; }

<STRING_SINGLE_QUOTES><STRING_DOUBLE_QUOTES><STRING_TRIPLE_SINGLE_QUOTES><STRING_TRIPLE_DOUBLE_QUOTES>\\[0-7]{1,3} {
    str += (char)strtol(yytext + 1, NULL, 8);  
}

<STRING_SINGLE_QUOTES><STRING_DOUBLE_QUOTES><STRING_TRIPLE_SINGLE_QUOTES><STRING_TRIPLE_DOUBLE_QUOTES>\\x[0-9a-fA-F]{2} {
    str += (char)strtol(yytext + 2, NULL, 16); 
}

<STRING_SINGLE_QUOTES><STRING_DOUBLE_QUOTES><STRING_TRIPLE_SINGLE_QUOTES><STRING_TRIPLE_DOUBLE_QUOTES>\\u[0-9A-Fa-f]{0,4} {
    if (strlen(yytext) < 4)
    {
        printf("ERROR: Illegal escape \'\\u\');
    }
    int value = strtol(yytext+2,NULL, 16);
    if (value > 127)
    {
        printf("ERROR: non-supported UNICODE character);
    }
    else
    {
        str += value;
    }
}

<STRING_SINGLE_QUOTES><STRING_DOUBLE_QUOTES><STRING_TRIPLE_SINGLE_QUOTES><STRING_TRIPLE_DOUBLE_QUOTES>\\U[0-9a-fA-F]{8} {
    unsigned int codepoint = strtol(yytext + 2, NULL, 16);
    str += (char)(codepoint & 0xFF); 
}

<STRING_SINGLE_QUOTES><STRING_DOUBLE_QUOTES><STRING_TRIPLE_SINGLE_QUOTES><STRING_TRIPLE_DOUBLE_QUOTES>\\N\{[^}]+\} {
   str += yytext;
}

<STRING_SINGLE_QUOTES>\' { 
    cout << "Found single-quoted string literal: " << str << endl; 
    BEGIN(INITIAL); 
}
<STRING_DOUBLE_QUOTES>\" { 
    cout << "Found double-quoted string literal: " << str << endl; 
    BEGIN(INITIAL); 
}
<STRING_TRIPLE_SINGLE_QUOTES>\'\'\' { 
    cout << "Found triple single-quoted string literal: " << str << endl; 
    BEGIN(INITIAL); 
}
<STRING_TRIPLE_DOUBLE_QUOTES>\"\"\" { 
    cout << "Found triple double-quoted string literal: " << str << endl; 
    BEGIN(INITIAL); 
}


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

[A-Za-z_][A-Za-z0-9_]*  { PRINT_ID(); }

. {}
%%

int main(int argc, char *argv[])
{
	lexer->yylex();
	return 0;
}