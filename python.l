%option never-interactive
%option noyywrap
%option yylineno
%option c++

%{
    #include <iostream>
    #include <string>
    #include <algorithm>
    #include <deque>
    using namespace std;

    extern yyFlexLexer *lexer = new yyFlexLexer();

    string removeUnderline(string text);
    void calculateIndentiation(int* indentStack, int* lastIndentIndex, int dentLen);
    void checkIndentiationForKeywords(deque<int>& keywordLinesStack, int* indentStack, int lastIndentIndex, int dentLen);
%}

%x INLINE_STRING_SINGLE_QUOTES
%x INLINE_STRING_DOUBLE_QUOTES
%x MULTILINE_STRING_SINGLE_QUOTES
%x MULTILINE_STRING_DOUBLE_QUOTES
%x NEW_LINE

%%

%{
    string str;

    int lastIndentIndex = 0;
    int indentStack[1000];
    indentStack[0] = 0;

    deque<int> keywordLinesStack = {};

    int parenthesesBracketsCounter = 0;
    int squareBracketsCounter = 0;
    int curlyBracketsCounter = 0;
    bool isString = false;
%}

#.* {
    cout << "Line: " << lexer->lineno() << ". Found comment: \"" << lexer->YYText()+1 << "\"" << endl;
}

\n[ ]* {
    if(!(isString || parenthesesBracketsCounter || squareBracketsCounter || curlyBracketsCounter)) {
        checkIndentiationForKeywords(keywordLinesStack, indentStack, lastIndentIndex, lexer->YYLeng() - 1);
        calculateIndentiation(indentStack, &lastIndentIndex, lexer->YYLeng() - 1);
    }
}
^[ ]+ {
    if(!(isString || parenthesesBracketsCounter || squareBracketsCounter || curlyBracketsCounter)) {
        checkIndentiationForKeywords(keywordLinesStack, indentStack, lastIndentIndex, lexer->YYLeng() - 1);
        calculateIndentiation(indentStack, &lastIndentIndex, lexer->YYLeng());
    }
}
\n\t* {
    if(!(isString || parenthesesBracketsCounter || squareBracketsCounter || curlyBracketsCounter)) {
        checkIndentiationForKeywords(keywordLinesStack, indentStack, lastIndentIndex, (lexer->YYLeng() - 1) * 4);
        calculateIndentiation(indentStack, &lastIndentIndex, (lexer->YYLeng() - 1) * 4);
    }
}
^\t+ {
    if(!(isString || parenthesesBracketsCounter || squareBracketsCounter || curlyBracketsCounter)) {
        checkIndentiationForKeywords(keywordLinesStack, indentStack, lastIndentIndex, (lexer->YYLeng() - 1) * 4);
        calculateIndentiation(indentStack, &lastIndentIndex, lexer->YYLeng() * 4);
    }
}
^[ \t]{2,} {
    if(!(isString || parenthesesBracketsCounter || squareBracketsCounter || curlyBracketsCounter)) {
        cout << "Line: " << lexer->lineno() << ". ERROR: mixed tabs and spaces (dentiation)." << endl;
    }
}

\+ { cout << "Line: " << lexer->lineno() << ". Found arithmetical operator: " << lexer->YYText() << endl; }
\- { cout << "Line: " << lexer->lineno() << ". Found arithmetical operator: " << lexer->YYText() << endl; }
\* { cout << "Line: " << lexer->lineno() << ". Found arithmetical operator: " << lexer->YYText() << endl; }
\*\* { cout << "Line: " << lexer->lineno() << ". Found arithmetical operator: " << lexer->YYText() << endl; }
\/ { cout << "Line: " << lexer->lineno() << ". Found arithmetical operator: " << lexer->YYText() << endl; }
\/\/ { cout << "Line: " << lexer->lineno() << ". Found arithmetical operator: " << lexer->YYText() << endl; }
\% { cout << "Line: " << lexer->lineno() << ". Found arithmetical operator: " << lexer->YYText() << endl; }

\<\< { cout << "Line: " << lexer->lineno() << ". Found bitwise operator: " << lexer->YYText() << endl; }
\>\> { cout << "Line: " << lexer->lineno() << ". Found bitwise operator: " << lexer->YYText() << endl; }
\& { cout << "Line: " << lexer->lineno() << ". Found bitwise operator: " << lexer->YYText() << endl; }
\| { cout << "Line: " << lexer->lineno() << ". Found bitwise operator: " << lexer->YYText() << endl; }
\^ { cout << "Line: " << lexer->lineno() << ". Found bitwise operator: " << lexer->YYText() << endl; }
\~ { cout << "Line: " << lexer->lineno() << ". Found bitwise operator: " << lexer->YYText() << endl; }

\< { cout << "Line: " << lexer->lineno() << ". Found comparison operator: " << lexer->YYText() << endl; }
\> { cout << "Line: " << lexer->lineno() << ". Found comparison operator: " << lexer->YYText() << endl; }
\>\= { cout << "Line: " << lexer->lineno() << ". Found comparison operator: " << lexer->YYText() << endl; }
\<\= { cout << "Line: " << lexer->lineno() << ". Found comparison operator: " << lexer->YYText() << endl; }
\=\= { cout << "Line: " << lexer->lineno() << ". Found comparison operator: " << lexer->YYText() << endl; }
\!\= { cout << "Line: " << lexer->lineno() << ". Found comparison operator: " << lexer->YYText() << endl; }

\= { cout << "Line: " << lexer->lineno() << ". Found assignment operator: " << lexer->YYText() << endl; }
\+\= { cout << "Line: " << lexer->lineno() << ". Found assignment operator: " << lexer->YYText() << endl; }
\-\= { cout << "Line: " << lexer->lineno() << ". Found assignment operator: " << lexer->YYText() << endl; }
\*\= { cout << "Line: " << lexer->lineno() << ". Found assignment operator: " << lexer->YYText() << endl; }
\/\= { cout << "Line: " << lexer->lineno() << ". Found assignment operator: " << lexer->YYText() << endl; }
\/\/\= { cout << "Line: " << lexer->lineno() << ". Found assignment operator: " << lexer->YYText() << endl; }
\%\= { cout << "Line: " << lexer->lineno() << ". Found assignment operator: " << lexer->YYText() << endl; }
\&\= { cout << "Line: " << lexer->lineno() << ". Found assignment operator: " << lexer->YYText() << endl; }
\|\= { cout << "Line: " << lexer->lineno() << ". Found assignment operator: " << lexer->YYText() << endl; }
\^\= { cout << "Line: " << lexer->lineno() << ". Found assignment operator: " << lexer->YYText() << endl; }
\>\>\= { cout << "Line: " << lexer->lineno() << ". Found assignment operator: " << lexer->YYText() << endl; }
\<\<\= { cout << "Line: " << lexer->lineno() << ". Found assignment operator: " << lexer->YYText() << endl; }
\*\*\= { cout << "Line: " << lexer->lineno() << ". Found assignment operator: " << lexer->YYText() << endl; }

\( { 
        cout << "Line: " << lexer->lineno() << ". Found open parentheses bracket: " << lexer->YYText() << endl;
        parenthesesBracketsCounter++; 
   }
\) { 
        cout << "Line: " << lexer->lineno() << ". Found close parentheses bracket: " << lexer->YYText() << endl;
        parenthesesBracketsCounter--;
   }
\[ { 
        cout << "Line: " << lexer->lineno() << ". Found open square bracket: " << lexer->YYText() << endl;
        squareBracketsCounter++;
   }
\] {
        cout << "Line: " << lexer->lineno() << ". Found close square bracket: " << lexer->YYText() << endl;
        squareBracketsCounter--;
   }
\{ { 
        cout << "Line: " << lexer->lineno() << ". Found open curly bracket: " << lexer->YYText() << endl;
        curlyBracketsCounter++;
   }
\} {
        cout << "Line: " << lexer->lineno() << ". Found close curly bracket: " << lexer->YYText() << endl;
        curlyBracketsCounter--;
   }

\, { cout << "Line: " << lexer->lineno() << ". Found delimiter: " << lexer->YYText() << endl; }
\: { cout << "Line: " << lexer->lineno() << ". Found delimiter: " << lexer->YYText() << endl; }
\! { cout << "Line: " << lexer->lineno() << ". Found delimiter: " << lexer->YYText() << endl; }
\. { cout << "Line: " << lexer->lineno() << ". Found delimiter: " << lexer->YYText() << endl; }
\; { cout << "Line: " << lexer->lineno() << ". Found delimiter: " << lexer->YYText() << endl; }
\-\> { cout << "Line: " << lexer->lineno() << ". Found delimiter: " << lexer->YYText() << endl; }

if {
        cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl;
        keywordLinesStack.push_back(lexer->lineno());
   }
elif {
        cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl;
        keywordLinesStack.push_back(lexer->lineno());
     }
else {
        cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl;
        keywordLinesStack.push_back(lexer->lineno());
     }
class {
        cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl;
        keywordLinesStack.push_back(lexer->lineno());
      }
def {
        cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl;
        keywordLinesStack.push_back(lexer->lineno());
    }
try {
        cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl;
        keywordLinesStack.push_back(lexer->lineno());
    }
except {
            cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl;
            keywordLinesStack.push_back(lexer->lineno());
       }
finally {
            cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl;
            keywordLinesStack.push_back(lexer->lineno());
        }
for {
        cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl;
        keywordLinesStack.push_back(lexer->lineno());
    }
while {
        cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl;
        keywordLinesStack.push_back(lexer->lineno());
      }
with {
        cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl;
        keywordLinesStack.push_back(lexer->lineno());
     }
False { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }     
None { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }      
True { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }        
and { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }           
as { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }           
assert { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }          
async { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }     
await { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }  
break { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }
continue { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }
del { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }
from { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }
global { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }
import { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }
in { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }
is { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }
lambda { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }
nonlocal { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }
not { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }
or { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }
pass { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }
raise { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }
return { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }
yield { cout << "Line: " << lexer->lineno() << ". Found hard keyword: " << lexer->YYText() << endl; }

int { cout << "Line: " << lexer->lineno() << ". Found data type: " << lexer->YYText() << endl; }
float { cout << "Line: " << lexer->lineno() << ". Found data type: " << lexer->YYText() << endl; }
complex { cout << "Line: " << lexer->lineno() << ". Found data type: " << lexer->YYText() << endl; }
bool { cout << "Line: " << lexer->lineno() << ". Found data type: " << lexer->YYText() << endl; }
list { cout << "Line: " << lexer->lineno() << ". Found data type: " << lexer->YYText() << endl; }
tuple { cout << "Line: " << lexer->lineno() << ". Found data type: " << lexer->YYText() << endl; }
range { cout << "Line: " << lexer->lineno() << ". Found data type: " << lexer->YYText() << endl; }
str { cout << "Line: " << lexer->lineno() << ". Found data type: " << lexer->YYText() << endl; }
bytes { cout << "Line: " << lexer->lineno() << ". Found data type: " << lexer->YYText() << endl; }
bytearray { cout << "Line: " << lexer->lineno() << ". Found data type: " << lexer->YYText() << endl; }
memoryview { cout << "Line: " << lexer->lineno() << ". Found data type: " << lexer->YYText() << endl; }
set { cout << "Line: " << lexer->lineno() << ". Found data type: " << lexer->YYText() << endl; }
frozenset { cout << "Line: " << lexer->lineno() << ". Found data type: " << lexer->YYText() << endl; }
dict { cout << "Line: " << lexer->lineno() << ". Found data type: " << lexer->YYText() << endl; }
NoneType { cout << "Line: " << lexer->lineno() << ". Found data type: " << lexer->YYText() << endl; }

0[bB](\_?[0-1])* {
    if(lexer->YYLeng() == 2) {
        cout << "Line: " << lexer->lineno() << ". ERROR: found empty binary literal." << endl;
    } else {
        string text = lexer->YYText();
        text = removeUnderline(text);
        cout << "Line: " << lexer->lineno() << ". Found binary literal: " << to_string(stoll(text.substr(2), NULL, 2)) << endl;
    }
}
0[bB](\_?[A-Za-z0-9])* { cout << "Line: " << lexer->lineno() << ". ERROR: invalid binary literal (illegal use of letter)." << endl; }
0[bB][01]*(\_+[01]*)* { cout << "Line: " << lexer->lineno() << ". ERROR: invalid binary literal (illegal use of underscore)." << endl; }

0[xX](\_?[0-9A-Fa-f])* {
    if(lexer->YYLeng() == 2) {
        cout << "Line: " << lexer->lineno() << ". ERROR: found empty hexadecimal literal." << endl;
    } else {
        string text = lexer->YYText();
        text = removeUnderline(text);
        cout << "Line: " << lexer->lineno() << ". Found hexadecimal literal: " << to_string(stoll(text.substr(2), NULL, 16)) << endl;
    }
}
0[xX](\_?[A-Za-z0-9])* { cout << "Line: " << lexer->lineno() << ". ERROR: invalid hexadecimal literal (illegal use of letter)." << endl; }
0[xX][0-9A-Fa-f]*(\_+[0-9A-Fa-f]*)* { cout << "Line: " << lexer->lineno() << ". ERROR: invalid hexadecimal literal (illegal use of underscore)." << endl; }

0[oO](\_?[0-7])* {
    if(lexer->YYLeng() == 2) {
        cout << "Line: " << lexer->lineno() << ". ERROR: found empty octal literal." << endl;
    } else {
        string text = lexer->YYText();
        text = removeUnderline(text);
        cout << "Line: " << lexer->lineno() << ". Found octal literal: " << stoll("0" + text.substr(2), NULL, 8) << endl;
    }
}
0[oO](\_?[A-Za-z0-9])* { cout << "Line: " << lexer->lineno() << ". ERROR: invalid octal literal (unexpected character)." << endl; }
0[oO][0-7]*(\_+[0-7]*)* { cout << "Line: " << lexer->lineno() << ". ERROR: invalid octal literal (illegal use of underscore)." << endl; }

[0-9](\_?[0-9])* {
    string text = lexer->YYText();

    if(lexer->YYLeng() > 1 && text[0] == '0') {
        cout << "Line: " << lexer->lineno() << ". ERROR: invalid decimal literal (illegal use of 0)." << endl;
    } else {
        text = removeUnderline(text);
        cout << "Line: " << lexer->lineno() << ". Found decimal literal: " << text << endl;
    }
}
[0-9](\_?[A-Za-z0-9])* { cout << "Line: " << lexer->lineno() << ". ERROR: invalid decimal literal (unexpected character)." << endl; }
[0-9]([0-9]*\_+[0-9]*)+ { cout << "Line: " << lexer->lineno() << ". ERROR: invalid decimal literal (illegal use of underscore)." << endl; }

\' { 
    BEGIN(INLINE_STRING_SINGLE_QUOTES);
    str.clear();
    isString = true;
}
\" { 
    BEGIN(INLINE_STRING_DOUBLE_QUOTES);  
    str.clear();
    isString = true;
}
\'\'\' { 
    BEGIN(MULTILINE_STRING_SINGLE_QUOTES); 
    str.clear();
    isString = true;
}
\"\"\" { 
    BEGIN(MULTILINE_STRING_DOUBLE_QUOTES); 
    str.clear();
    isString = true;
}

[A-Za-z_][A-Za-z0-9_]*  { cout << "Line: " << lexer->lineno() << ". Found identifier: " << lexer->YYText() << endl; }

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
<MULTILINE_STRING_SINGLE_QUOTES>\' { str += "\'"; }
<MULTILINE_STRING_DOUBLE_QUOTES>\" { str += "\""; }

<INLINE_STRING_SINGLE_QUOTES>\' {
    cout << "Line: " << lexer->lineno() << ". Found inline single-quoted string literal: \'" + str + "\'" << endl;
    isString = false;
    BEGIN(INITIAL); 
}
<INLINE_STRING_DOUBLE_QUOTES>\" {
    cout << "Line: " << lexer->lineno() << ". Found inline double-quoted string literal: \"" + str + "\"" << endl;
    isString = false;
    BEGIN(INITIAL); 
}
<MULTILINE_STRING_SINGLE_QUOTES>\'{3} {
    cout << "Line: " << lexer->lineno() << ". Found multiline single-quoted string literal:\n\'" + str + "\'" << endl;
    isString = false;
    BEGIN(INITIAL); 
}
<MULTILINE_STRING_DOUBLE_QUOTES>\"{3} {
    cout << "Line: " << lexer->lineno() << ". Found multiline double-quoted string literal:\n\"" + str + "\"" << endl;
    isString = false;
    BEGIN(INITIAL); 
}

<INLINE_STRING_SINGLE_QUOTES,INLINE_STRING_DOUBLE_QUOTES>[\n\r] {
    int strLength = static_cast<int>(str.length());

    if(strLength > 1 && str[strLength - 1] == '\\') {
        str.erase(strLength - 1, 1);
    } else {
        cout << "Line: " << lexer->lineno() << ". ERROR: inline string literal is not closed." << endl;
        BEGIN(INITIAL);
    }
}
<INLINE_STRING_SINGLE_QUOTES,INLINE_STRING_DOUBLE_QUOTES><<EOF>> {
    cout << "Line: " << lexer->lineno() << ". ERROR: inline string literal is not closed." << endl;
    BEGIN(INITIAL);
}
<MULTILINE_STRING_SINGLE_QUOTES,MULTILINE_STRING_DOUBLE_QUOTES><<EOF>> {
    cout << "Line: " << lexer->lineno() << ". ERROR: multiline string literal is not closed." << endl;
    BEGIN(INITIAL);
}

<<EOF>> { checkIndentiationForKeywords(keywordLinesStack, indentStack, lastIndentIndex, 0); return 0; }

^[ \t]*\n$ {
    cout << "ZAD" << endl;
}

[[:space:]] {}
%%

void calculateIndentiation(int* indentStack, int* lastIndentIndex, int dentLen) {
    /* Indent */
    if(dentLen > indentStack[*lastIndentIndex]) {
        (*lastIndentIndex)++;
        indentStack[*lastIndentIndex] = dentLen;
        cout << "Line: " << lexer->lineno() << ". Found indent: " << to_string(dentLen) << endl;
    }

    /* Dedent */
    if(dentLen < indentStack[*lastIndentIndex]) {
        int tmpIndex = *lastIndentIndex;

        while(dentLen < indentStack[tmpIndex]) { tmpIndex--; }

        if(dentLen != indentStack[tmpIndex]) { cout << "Line: " << lexer->lineno() << ". ERROR: incosistent dedent: " << to_string(dentLen) << endl; }
        else {
            while(*lastIndentIndex > tmpIndex) { 
                cout << "Line: " << lexer->lineno() << ". Found dedent" << endl; 
                (*lastIndentIndex)--;
            }
        }  
    }
}

void checkIndentiationForKeywords(deque<int>& keywordLinesStack, int* indentStack, int lastIndentIndex, int dentLen) {
    if(keywordLinesStack.empty()) {
        return;
    }

    if(lexer->lineno() - keywordLinesStack.front() != 1 || dentLen - indentStack[lastIndentIndex] <= 0) {
        cout << "Line: " << keywordLinesStack.front() << ". ERROR: not found indent for keyword." << endl;
    }

    keywordLinesStack.pop_front();
}

string removeUnderline(string input)
{
    string str = input;
    str.erase(remove(str.begin(), str.end(), '_'), str.end());
    return str;
}

int main(int argc, char *argv[])
{
	lexer->yylex();
	return 0;
}