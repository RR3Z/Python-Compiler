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

    #define PRINT_EOF_ERROR(desc) cout << "ERROR: unexpected end of file" << desc << endl;
	#define PRINT_HARD_KEYWORD() cout << "Found hard keyword: " << lexer->YYText() << endl;
	#define PRINT_SOFT_KEYWORD() cout << "Found soft keyword: " << lexer->YYText() << endl;
	#define PRINT_OPERATOR() cout << "Found operator: " << lexer->YYText() << endl;
	#define PRINT_DELIMITER() cout << "Found delimiter: " << lexer->YYText() << endl;
	#define PRINT_ID() cout << "Found identifier: " << lexer->YYText() << endl;
%}


%x INLINE_STRING_SINGLE_QUOTES
%x INLINE_STRING_DOUBLE_QUOTES
%x MULTILINE_STRING_SINGLE_QUOTES
%x MULTILINE_STRING_DOUBLE_QUOTES


%%

%{
char buffer[10240];
string str;
%}

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

\+ { PRINT_OPERATOR(); }
\- { PRINT_OPERATOR(); }
\* { PRINT_OPERATOR(); }
\*\* { PRINT_OPERATOR(); }
\/ { PRINT_OPERATOR(); }
\/\/ { PRINT_OPERATOR(); }
\% { PRINT_OPERATOR(); }
\@ { PRINT_OPERATOR(); }
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

[A-Za-z_][A-Za-z0-9_]*  { PRINT_ID(); }

\' { BEGIN(INLINE_STRING_SINGLE_QUOTES);  str.clear(); cout << lexer->YYText() << endl;}
\" { BEGIN(INLINE_STRING_DOUBLE_QUOTES);  str.clear(); }
\'\'\' { BEGIN(MULTILINE_STRING_SINGLE_QUOTES); str.clear(); }
\"\"\" { BEGIN(MULTILINE_STRING_DOUBLE_QUOTES); str.clear(); }

<INLINE_STRING_SINGLE_QUOTES>[^\'\n]+ { str += lexer->YYText(); }
<INLINE_STRING_DOUBLE_QUOTES>[^\"\n]+ { str += lexer->YYText(); }
<MULTILINE_STRING_SINGLE_QUOTES>[^\']+ { str += lexer->YYText(); }
<MULTILINE_STRING_DOUBLE_QUOTES>[^\"]+ { str += lexer->YYText(); }

<INLINE_STRING_SINGLE_QUOTES><INLINE_STRING_DOUBLE_QUOTES><MULTILINE_STRING_SINGLE_QUOTES><MULTILINE_STRING_DOUBLE_QUOTES>\\[0-7]{1,3} {
    cout << yytext + 1 << endl;
    str += (char)stoi(yytext + 1, NULL, 8);
}

<INLINE_STRING_SINGLE_QUOTES><INLINE_STRING_DOUBLE_QUOTES><MULTILINE_STRING_SINGLE_QUOTES><MULTILINE_STRING_DOUBLE_QUOTES>\\x[0-9a-fA-F]{2} {
    str += (char)stoi(yytext + 2, NULL, 16);
}

<INLINE_STRING_SINGLE_QUOTES><INLINE_STRING_DOUBLE_QUOTES><MULTILINE_STRING_SINGLE_QUOTES><MULTILINE_STRING_DOUBLE_QUOTES>\\u[0-9A-Fa-f]{4} {
    str += (char)stoi(yytext + 2, NULL, 16);
}

<INLINE_STRING_SINGLE_QUOTES><INLINE_STRING_DOUBLE_QUOTES><MULTILINE_STRING_SINGLE_QUOTES><MULTILINE_STRING_DOUBLE_QUOTES>\\U[0-9a-fA-F]{8} {
    str += (char)stoi(yytext + 2, NULL, 16);
}

<INLINE_STRING_SINGLE_QUOTES><INLINE_STRING_DOUBLE_QUOTES><MULTILINE_STRING_SINGLE_QUOTES><MULTILINE_STRING_DOUBLE_QUOTES>\\\\ { str += "\\"; }
<INLINE_STRING_SINGLE_QUOTES><INLINE_STRING_DOUBLE_QUOTES><MULTILINE_STRING_SINGLE_QUOTES><MULTILINE_STRING_DOUBLE_QUOTES>\\\' { str += "\'"; }
<INLINE_STRING_SINGLE_QUOTES><INLINE_STRING_DOUBLE_QUOTES><MULTILINE_STRING_SINGLE_QUOTES><MULTILINE_STRING_DOUBLE_QUOTES>\\\" { str += "\""; }
<INLINE_STRING_SINGLE_QUOTES><INLINE_STRING_DOUBLE_QUOTES><MULTILINE_STRING_SINGLE_QUOTES><MULTILINE_STRING_DOUBLE_QUOTES>\\a { str += "\a"; }
<INLINE_STRING_SINGLE_QUOTES><INLINE_STRING_DOUBLE_QUOTES><MULTILINE_STRING_SINGLE_QUOTES><MULTILINE_STRING_DOUBLE_QUOTES>\\b { str += "\b"; }
<INLINE_STRING_SINGLE_QUOTES><INLINE_STRING_DOUBLE_QUOTES><MULTILINE_STRING_SINGLE_QUOTES><MULTILINE_STRING_DOUBLE_QUOTES>\\f { str += "\f"; }
<INLINE_STRING_SINGLE_QUOTES><INLINE_STRING_DOUBLE_QUOTES><MULTILINE_STRING_SINGLE_QUOTES><MULTILINE_STRING_DOUBLE_QUOTES>\\n { str += "\n"; }
<INLINE_STRING_SINGLE_QUOTES><INLINE_STRING_DOUBLE_QUOTES><MULTILINE_STRING_SINGLE_QUOTES><MULTILINE_STRING_DOUBLE_QUOTES>\\r { str += "\r"; }
<INLINE_STRING_SINGLE_QUOTES><INLINE_STRING_DOUBLE_QUOTES><MULTILINE_STRING_SINGLE_QUOTES><MULTILINE_STRING_DOUBLE_QUOTES>\\t { str += "\t"; }
<INLINE_STRING_SINGLE_QUOTES><INLINE_STRING_DOUBLE_QUOTES><MULTILINE_STRING_SINGLE_QUOTES><MULTILINE_STRING_DOUBLE_QUOTES>\\v { str += "\v"; }

<INLINE_STRING_SINGLE_QUOTES>\' { 
    cout << "Found single-quoted string literal: " << str << endl; 
    BEGIN(INITIAL); 
}
<INLINE_STRING_DOUBLE_QUOTES>\" { 
    cout << "Found double-quoted string literal: " << str << endl; 
    BEGIN(INITIAL); 
}
<MULTILINE_STRING_SINGLE_QUOTES>\'\'\' { 
    cout << "Found triple single-quoted string literal: " << str << endl; 
    BEGIN(INITIAL); 
}
<MULTILINE_STRING_DOUBLE_QUOTES>\"\"\" { 
    cout << "Found triple double-quoted string literal: " << str << endl; 
    BEGIN(INITIAL); 
}

<MULTILINE_STRING_DOUBLE_QUOTES><<EOF>> { 
    PRINT_EOF_ERROR("(string literal not closed)");
    BEGIN(INITIAL);
}
<MULTILINE_STRING_DOUBLE_QUOTES><<EOF>> { 
    PRINT_EOF_ERROR("(string literal not closed)");
    BEGIN(INITIAL);
}
<MULTILINE_STRING_SINGLE_QUOTES><<EOF>> { 
    PRINT_EOF_ERROR("(string literal not closed)");
    BEGIN(INITIAL);
}
<MULTILINE_STRING_DOUBLE_QUOTES><<EOF>> { 
    PRINT_EOF_ERROR("(string literal not closed)");
    BEGIN(INITIAL);
}

. {}
%%

int main(int argc, char *argv[])
{
	lexer->yylex();
	return 0;
}