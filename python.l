%option never-interactive
%option noyywrap
%option yylineno
%option c++

%{
    #include <iostream>
    #include <string>
    using namespace std;

    extern yyFlexLexer *lexer = new yyFlexLexer();

    #define PRINT_MESSAGE(message) cout << message << endl;
    #define PRINT_HARD_KEYWORD() cout << "Found hard keyword: " << lexer->YYText() << endl;
    #define PRINT_OPERATOR() cout << "Found operator: " << lexer->YYText() << endl;
    #define PRINT_DELIMITER() cout << "Found delimiter: " << lexer->YYText() << endl;
    #define PRINT_ID() cout << "Found identifier: " << lexer->YYText() << endl;
    #define PRINT_ERROR(desc) cout << "ERROR: " << desc << endl;  
%}

%x INLINE_STRING_SINGLE_QUOTES
%x INLINE_STRING_DOUBLE_QUOTES
%x MULTILINE_STRING_SINGLE_QUOTES
%x MULTILINE_STRING_DOUBLE_QUOTES

%%

%{
    string str;
    int multilineBegin;
    int lineBegin;
%}

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

[A-Za-z_][A-Za-z0-9_]*  { PRINT_ID(); }

\' { BEGIN(INLINE_STRING_SINGLE_QUOTES);  str.clear(); lineBegin = lexer->lineno(); }
\" { BEGIN(INLINE_STRING_DOUBLE_QUOTES);  str.clear(); lineBegin = lexer->lineno(); }
\'\'\' { BEGIN(MULTILINE_STRING_SINGLE_QUOTES); str.clear(); multilineBegin = lexer->lineno(); }
\"\"\" { BEGIN(MULTILINE_STRING_DOUBLE_QUOTES); str.clear(); multilineBegin = lexer->lineno(); }

<INLINE_STRING_SINGLE_QUOTES>[^\\\'\n]+ { str += lexer->YYText(); }
<INLINE_STRING_DOUBLE_QUOTES>[^\\\"\n]+ { str += lexer->YYText(); }
<MULTILINE_STRING_SINGLE_QUOTES>[^\\\']+ { str += lexer->YYText(); }
<MULTILINE_STRING_DOUBLE_QUOTES>[^\\\"]+ { str += lexer->YYText(); }

<INLINE_STRING_SINGLE_QUOTES,INLINE_STRING_DOUBLE_QUOTES,MULTILINE_STRING_SINGLE_QUOTES,MULTILINE_STRING_DOUBLE_QUOTES>\\n { str += "\n"; }
<INLINE_STRING_SINGLE_QUOTES,INLINE_STRING_DOUBLE_QUOTES,MULTILINE_STRING_SINGLE_QUOTES,MULTILINE_STRING_DOUBLE_QUOTES>\\\\ { str += "\\"; }
<INLINE_STRING_SINGLE_QUOTES,INLINE_STRING_DOUBLE_QUOTES,MULTILINE_STRING_SINGLE_QUOTES,MULTILINE_STRING_DOUBLE_QUOTES>\\\' { str += "\'"; }
<INLINE_STRING_SINGLE_QUOTES,INLINE_STRING_DOUBLE_QUOTES,MULTILINE_STRING_SINGLE_QUOTES,MULTILINE_STRING_DOUBLE_QUOTES>\\\" { str += "\""; }
<INLINE_STRING_SINGLE_QUOTES,INLINE_STRING_DOUBLE_QUOTES,MULTILINE_STRING_SINGLE_QUOTES,MULTILINE_STRING_DOUBLE_QUOTES>\\a { str += "\a"; }
<INLINE_STRING_SINGLE_QUOTES,INLINE_STRING_DOUBLE_QUOTES,MULTILINE_STRING_SINGLE_QUOTES,MULTILINE_STRING_DOUBLE_QUOTES>\\b { str += "\b"; }
<INLINE_STRING_SINGLE_QUOTES,INLINE_STRING_DOUBLE_QUOTES,MULTILINE_STRING_SINGLE_QUOTES,MULTILINE_STRING_DOUBLE_QUOTES>\\f { str += "\f"; }
<INLINE_STRING_SINGLE_QUOTES,INLINE_STRING_DOUBLE_QUOTES,MULTILINE_STRING_SINGLE_QUOTES,MULTILINE_STRING_DOUBLE_QUOTES>\\r { str += "\r"; }
<INLINE_STRING_SINGLE_QUOTES,INLINE_STRING_DOUBLE_QUOTES,MULTILINE_STRING_SINGLE_QUOTES,MULTILINE_STRING_DOUBLE_QUOTES>\\t { str += "\t"; }
<INLINE_STRING_SINGLE_QUOTES,INLINE_STRING_DOUBLE_QUOTES,MULTILINE_STRING_SINGLE_QUOTES,MULTILINE_STRING_DOUBLE_QUOTES>\\v { str += "\v"; }
<INLINE_STRING_SINGLE_QUOTES,INLINE_STRING_DOUBLE_QUOTES,MULTILINE_STRING_SINGLE_QUOTES,MULTILINE_STRING_DOUBLE_QUOTES>\\[0-7]{1,3} { str += (char)stoi(yytext + 1, NULL, 8); }
<INLINE_STRING_SINGLE_QUOTES,INLINE_STRING_DOUBLE_QUOTES,MULTILINE_STRING_SINGLE_QUOTES,MULTILINE_STRING_DOUBLE_QUOTES>\\x[0-9a-fA-F]{2} { str += (char)stoi(yytext + 2, NULL, 16); }
<INLINE_STRING_SINGLE_QUOTES,INLINE_STRING_DOUBLE_QUOTES,MULTILINE_STRING_SINGLE_QUOTES,MULTILINE_STRING_DOUBLE_QUOTES>\\u[0-9A-Fa-f]{4} { str += (char)stoi(yytext + 2, NULL, 16); }
<INLINE_STRING_SINGLE_QUOTES,INLINE_STRING_DOUBLE_QUOTES,MULTILINE_STRING_SINGLE_QUOTES,MULTILINE_STRING_DOUBLE_QUOTES>\\U[0-9a-fA-F]{8} { str += (char)stoi(yytext + 2, NULL, 16); }

<INLINE_STRING_SINGLE_QUOTES>\' {
    PRINT_MESSAGE("Found inline single-quoted string literal: " + str)
    BEGIN(INITIAL); 
}
<INLINE_STRING_DOUBLE_QUOTES>\" {
    PRINT_MESSAGE("Found inline double-quoted string literal: " + str)
    BEGIN(INITIAL); 
}
<MULTILINE_STRING_SINGLE_QUOTES>\'{3} {
    PRINT_MESSAGE("Found multiline single-quoted string literal:\n" + str)
    BEGIN(INITIAL); 
}
<MULTILINE_STRING_DOUBLE_QUOTES>\"{3} {
    PRINT_MESSAGE("Found multiline double-quoted string literal:\n" + str)
    BEGIN(INITIAL); 
}

<INLINE_STRING_SINGLE_QUOTES,INLINE_STRING_DOUBLE_QUOTES>[\n\r] {
    PRINT_ERROR("inline string literal not closed in line: " + to_string(lineBegin));
    BEGIN(INITIAL);
}
<INLINE_STRING_SINGLE_QUOTES,INLINE_STRING_DOUBLE_QUOTES><<EOF>> { 
    PRINT_ERROR("inline string literal not closed in line: " + to_string(lineBegin));
    BEGIN(INITIAL);
}
<MULTILINE_STRING_SINGLE_QUOTES,MULTILINE_STRING_DOUBLE_QUOTES><<EOF>> {
    PRINT_ERROR("multiline string literal not closed for line: " + to_string(multilineBegin));
    BEGIN(INITIAL);
}

. {}
%%

int main(int argc, char *argv[])
{
	lexer->yylex();
	return 0;
}