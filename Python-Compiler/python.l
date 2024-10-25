%option never-interactive
%option noyywrap
%option yylineno

%{
    #include <iostream>
    #include <string>
    #include <algorithm>
    #include <deque>
    using namespace std;

    #include "python.tab.h"

    string removeUnderline(string text);
    void calculateIndentiation(int* indentStack, int* indentStackTopIndex, int indentLength);
    void checkIndentiationForKeywords(deque<int>& keywordsLineNumber, int* indentStack, int indentStackTopIndex, int* emptyLinesCounter, int indentLength);
%}

%x SHORT_SINGLE_STRING
%x SHORT_DOUBLE_STRING
%x LONG_SINGLE_STRING
%x LONG_DOUBLE_STRING
%x INDENTIATION
%x COMMENT
%x FIRSTLINE

%%

%{
    string str;
    deque<int> keywordsLineNumber = {};
    int emptyLinesCounter = 0;
    bool isString = false;
    bool isFirstLine = true;

    int indentStackTopIndex = 0;
    int indentStack[1000];
    indentStack[0] = 0;

    int parenthesesBracketsCounter = 0;
    int squareBracketsCounter = 0;
    int curlyBracketsCounter = 0;
%}

\s*# { BEGIN(COMMENT); }
<COMMENT>[^\n]* { cout << "Line: " << yylineno << ". Found comment: \"" << yytext << "\"" << endl; }
<COMMENT>\n { BEGIN(INITIAL); }

\n {
        if(parenthesesBracketsCounter == 0 && squareBracketsCounter == 0 && curlyBracketsCounter == 0) {
            isString = false;
            BEGIN(INDENTIATION);
        }
        cout << "Line: " << yylineno << ". Found \\n (NEWLINE)" << endl;
   }

^[ ]+ {
            if(!(isString || parenthesesBracketsCounter || squareBracketsCounter || curlyBracketsCounter)) {
                calculateIndentiation(indentStack, &indentStackTopIndex, yyleng);
            }
      }

^\t+ {
            if(!(isString || parenthesesBracketsCounter || squareBracketsCounter || curlyBracketsCounter)) {
                calculateIndentiation(indentStack, &indentStackTopIndex, yyleng*4);
            }
      }

<INDENTIATION>^[ \t]*\n { 
                            emptyLinesCounter++; 
                            calculateIndentiation(indentStack, &indentStackTopIndex, 0); 
                        }

<INDENTIATION>^[\t]+ {
                            if(!(isString || parenthesesBracketsCounter || squareBracketsCounter || curlyBracketsCounter)) {
                                checkIndentiationForKeywords(keywordsLineNumber, indentStack, indentStackTopIndex, &emptyLinesCounter, yyleng * 4);
                                calculateIndentiation(indentStack, &indentStackTopIndex, yyleng * 4);
                                BEGIN(INITIAL);
                            }
                     }

<INDENTIATION>^[ ]+ {
                        if(!(isString || parenthesesBracketsCounter || squareBracketsCounter || curlyBracketsCounter)) {
                            checkIndentiationForKeywords(keywordsLineNumber, indentStack, indentStackTopIndex, &emptyLinesCounter, yyleng);
                            calculateIndentiation(indentStack, &indentStackTopIndex, yyleng);
                            BEGIN(INITIAL);
                        }
                    }

<INDENTIATION>^[ \t]{2,} {
                                if(!(isString || parenthesesBracketsCounter || squareBracketsCounter || curlyBracketsCounter)) {
                                    cout << "Line: " << yylineno << ". ERROR: mixed tabs and spaces (dentiation)." << endl;
                                    BEGIN(INITIAL);
                                }
                         }

<INDENTIATION>""/. {
                        calculateIndentiation(indentStack, &indentStackTopIndex, 0);
                        BEGIN(INITIAL);
                   }

\+ { cout << "Line: " << yylineno << ". Found arithmetical operator: " << yytext << endl; }
\- { cout << "Line: " << yylineno << ". Found arithmetical operator: " << yytext << endl; }
\* { cout << "Line: " << yylineno << ". Found arithmetical operator: " << yytext << endl; }
\*\* { cout << "Line: " << yylineno << ". Found arithmetical operator: " << yytext << endl; }
\/ { cout << "Line: " << yylineno << ". Found arithmetical operator: " << yytext << endl; }
\/\/ { cout << "Line: " << yylineno << ". Found arithmetical operator: " << yytext << endl; }
\% { cout << "Line: " << yylineno << ". Found arithmetical operator: " << yytext << endl; }

\<\< { cout << "Line: " << yylineno << ". Found bitwise operator: " << yytext << endl; }
\>\> { cout << "Line: " << yylineno << ". Found bitwise operator: " << yytext << endl; }
\& { cout << "Line: " << yylineno << ". Found bitwise operator: " << yytext << endl; }
\| { cout << "Line: " << yylineno << ". Found bitwise operator: " << yytext << endl; }
\^ { cout << "Line: " << yylineno << ". Found bitwise operator: " << yytext << endl; }
\~ { cout << "Line: " << yylineno << ". Found bitwise operator: " << yytext << endl; }

\< { cout << "Line: " << yylineno << ". Found comparison operator: " << yytext << endl; }
\> { cout << "Line: " << yylineno << ". Found comparison operator: " << yytext << endl; }
\>\= { cout << "Line: " << yylineno << ". Found comparison operator: " << yytext << endl; }
\<\= { cout << "Line: " << yylineno << ". Found comparison operator: " << yytext << endl; }
\=\= { cout << "Line: " << yylineno << ". Found comparison operator: " << yytext << endl; }
\!\= { cout << "Line: " << yylineno << ". Found comparison operator: " << yytext << endl; }

\:\= { cout << "Line: " << yylineno << ". Found assignment operator: " << yytext << endl; }
\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }
\+\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }
\-\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }
\*\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }
\/\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }
\/\/\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }
\%\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }
\&\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }
\|\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }
\^\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }
\>\>\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }
\<\<\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }
\*\*\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }

\, { cout << "Line: " << yylineno << ". Found delimiter: " << yytext << endl; }
\: { cout << "Line: " << yylineno << ". Found delimiter: " << yytext << endl; }
\! { cout << "Line: " << yylineno << ". Found delimiter: " << yytext << endl; }
\. { cout << "Line: " << yylineno << ". Found delimiter: " << yytext << endl; }
\; { cout << "Line: " << yylineno << ". Found delimiter: " << yytext << endl; }
\-\> { cout << "Line: " << yylineno << ". Found delimiter: " << yytext << endl; }

\( { 
        cout << "Line: " << yylineno << ". Found open parentheses bracket: " << yytext << endl;
        parenthesesBracketsCounter++; 
   }
\) { 
        cout << "Line: " << yylineno << ". Found close parentheses bracket: " << yytext << endl;
        parenthesesBracketsCounter--;
   }
\[ { 
        cout << "Line: " << yylineno << ". Found open square bracket: " << yytext << endl;
        squareBracketsCounter++;
   }
\] {
        cout << "Line: " << yylineno << ". Found close square bracket: " << yytext << endl;
        squareBracketsCounter--;
   }
\{ { 
        cout << "Line: " << yylineno << ". Found open curly bracket: " << yytext << endl;
        curlyBracketsCounter++;
   }
\} {
        cout << "Line: " << yylineno << ". Found close curly bracket: " << yytext << endl;
        curlyBracketsCounter--;
   }

int { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; }
float { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; }
complex { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; }
bool { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; }
list { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; }
tuple { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; }
range { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; }
str { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; }
bytes { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; }
bytearray { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; }
memoryview { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; }
set { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; }
frozenset { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; }
dict { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; }
NoneType { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; }

if {
        cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
        keywordsLineNumber.push_back(yylineno);
   }
elif {
        cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
        keywordsLineNumber.push_back(yylineno);
     }
else {
        cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
        keywordsLineNumber.push_back(yylineno);
     }
class {
        cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
        keywordsLineNumber.push_back(yylineno);
      }
def {
        cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
        keywordsLineNumber.push_back(yylineno);
    }
try {
        cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
        keywordsLineNumber.push_back(yylineno);
    }
except {
            cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
            keywordsLineNumber.push_back(yylineno);
       }
finally {
            cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
            keywordsLineNumber.push_back(yylineno);
        }
for {
        cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
        keywordsLineNumber.push_back(yylineno);
    }
while {
        cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
        keywordsLineNumber.push_back(yylineno);
      }
with {
        cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
        keywordsLineNumber.push_back(yylineno);
     }
False { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }     
None { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }      
True { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }        
and { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }           
as { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }           
assert { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }          
async { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }     
await { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }  
break { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
continue { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
del { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
from { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
global { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
import { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
in { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
is { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
lambda { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
nonlocal { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
not { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
or { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
pass { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
raise { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
return { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
yield { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }

0[bB](\_?[0-1])* {
    if(yyleng == 2) {
        cout << "Line: " << yylineno << ". ERROR: found empty binary literal." << endl;
    } else {
        string text = yytext;
        text = removeUnderline(text);
        cout << "Line: " << yylineno << ". Found binary literal: " << stoll(text.substr(2), NULL, 2) << endl;
    }
}
0[bB](\_?[A-Za-z0-9])* { cout << "Line: " << yylineno << ". ERROR: invalid binary literal (illegal use of letter)." << endl; }
0[bB][01]*(\_+[01]*)* { cout << "Line: " << yylineno << ". ERROR: invalid binary literal (illegal use of underscore)." << endl; }

0[xX](\_?[0-9A-Fa-f])* {
    if(yyleng == 2) {
        cout << "Line: " << yylineno << ". ERROR: found empty hexadecimal literal." << endl;
    } else {
        string text = yytext;
        text = removeUnderline(text);
        cout << "Line: " << yylineno << ". Found hexadecimal literal: " << stoll(text.substr(2), NULL, 16) << endl;
    }
}
0[xX](\_?[A-Za-z0-9])* { cout << "Line: " << yylineno << ". ERROR: invalid hexadecimal literal (illegal use of letter)." << endl; }
0[xX][0-9A-Fa-f]*(\_+[0-9A-Fa-f]*)* { cout << "Line: " << yylineno << ". ERROR: invalid hexadecimal literal (illegal use of underscore)." << endl; }

0[oO](\_?[0-7])* {
    if(yyleng == 2) {
        cout << "Line: " << yylineno << ". ERROR: found empty octal literal." << endl;
    } else {
        string text = yytext;
        text = removeUnderline(text);
        cout << "Line: " << yylineno << ". Found octal literal: " << stoll("0" + text.substr(2), NULL, 8) << endl;
    }
}
0[oO](\_?[A-Za-z0-9])* { cout << "Line: " << yylineno << ". ERROR: invalid octal literal (unexpected character)." << endl; }
0[oO][0-7]*(\_+[0-7]*)* { cout << "Line: " << yylineno << ". ERROR: invalid octal literal (illegal use of underscore)." << endl; }

([0-9]\_?)*([0-9]\.|\.[0-9])(\_?[0-9])*([eE][+-]?[0-9](\_?[0-9])*)?|([0-9]\_?)*([eE][+-]?[0-9](\_?[0-9])*) {
    cout << "Line: " << yylineno << ". Found float literal: " << removeUnderline(yytext) << endl;
}
[0-9](\_?[0-9])* {
    string text = yytext;

    if(yyleng > 1 && text[0] == '0') {
        cout << "Line: " << yylineno << ". ERROR: invalid decimal literal (illegal use of 0)." << endl;
    } else {
        text = removeUnderline(text);
        cout << "Line: " << yylineno << ". Found decimal literal: " << text << endl;
    }
}
[0-9](\_?[A-Za-z0-9])* { cout << "Line: " << yylineno << ". ERROR: invalid decimal literal (unexpected character)." << endl; }
[0-9]([0-9]*\_+[0-9]*)+ { cout << "Line: " << yylineno << ". ERROR: invalid decimal literal (illegal use of underscore)." << endl; }

\' { 
    BEGIN(SHORT_SINGLE_STRING);
    str.clear();
    isString = true;
}
\" { 
    BEGIN(SHORT_DOUBLE_STRING);  
    str.clear();
    isString = true;
}
\'{3} { 
    BEGIN(LONG_SINGLE_STRING); 
    str.clear();
    isString = true;
}
\"{3} { 
    BEGIN(LONG_DOUBLE_STRING); 
    str.clear();
    isString = true;
}

<SHORT_SINGLE_STRING>[^\\\'\n]+ { str += yytext; }
<SHORT_DOUBLE_STRING>[^\\\"\n]+ { str += yytext; }
<LONG_SINGLE_STRING>[^\\\']+ { str += yytext; }
<LONG_DOUBLE_STRING>[^\\\"]+ { str += yytext; }

<SHORT_SINGLE_STRING,SHORT_DOUBLE_STRING,LONG_SINGLE_STRING,LONG_DOUBLE_STRING>\\n { str += "\n"; }
<SHORT_SINGLE_STRING,SHORT_DOUBLE_STRING,LONG_SINGLE_STRING,LONG_DOUBLE_STRING>\\\\ { str += "\\"; }
<SHORT_SINGLE_STRING,SHORT_DOUBLE_STRING,LONG_SINGLE_STRING,LONG_DOUBLE_STRING>\\\' { str += "\'"; }
<SHORT_SINGLE_STRING,SHORT_DOUBLE_STRING,LONG_SINGLE_STRING,LONG_DOUBLE_STRING>\\\" { str += "\""; }
<SHORT_SINGLE_STRING,SHORT_DOUBLE_STRING,LONG_SINGLE_STRING,LONG_DOUBLE_STRING>\\a { str += "\a"; }
<SHORT_SINGLE_STRING,SHORT_DOUBLE_STRING,LONG_SINGLE_STRING,LONG_DOUBLE_STRING>\\b { str += "\b"; }
<SHORT_SINGLE_STRING,SHORT_DOUBLE_STRING,LONG_SINGLE_STRING,LONG_DOUBLE_STRING>\\f { str += "\f"; }
<SHORT_SINGLE_STRING,SHORT_DOUBLE_STRING,LONG_SINGLE_STRING,LONG_DOUBLE_STRING>\\r { str += "\r"; }
<SHORT_SINGLE_STRING,SHORT_DOUBLE_STRING,LONG_SINGLE_STRING,LONG_DOUBLE_STRING>\\t { str += "\t"; }
<SHORT_SINGLE_STRING,SHORT_DOUBLE_STRING,LONG_SINGLE_STRING,LONG_DOUBLE_STRING>\\v { str += "\v"; }
<SHORT_SINGLE_STRING,SHORT_DOUBLE_STRING,LONG_SINGLE_STRING,LONG_DOUBLE_STRING>\\[0-7]{1,3} { str += (char)stoi(yytext + 1, NULL, 8); }
<SHORT_SINGLE_STRING,SHORT_DOUBLE_STRING,LONG_SINGLE_STRING,LONG_DOUBLE_STRING>\\x[0-9a-fA-F]{2} { str += (char)stoi(yytext + 2, NULL, 16); }
<SHORT_SINGLE_STRING,SHORT_DOUBLE_STRING,LONG_SINGLE_STRING,LONG_DOUBLE_STRING>\\u[0-9A-Fa-f]{4} { str += (char)stoi(yytext + 2, NULL, 16); }
<SHORT_SINGLE_STRING,SHORT_DOUBLE_STRING,LONG_SINGLE_STRING,LONG_DOUBLE_STRING>\\U[0-9a-fA-F]{8} { str += (char)stoi(yytext + 2, NULL, 16); }
<LONG_SINGLE_STRING>\' { str += "\'"; }
<LONG_DOUBLE_STRING>\" { str += "\""; }

<SHORT_SINGLE_STRING>\' {
    cout << "Line: " << yylineno << ". Found inline single-quoted string literal: \'" + str + "\'" << endl;
    isString = false;
    BEGIN(INITIAL); 
}
<SHORT_DOUBLE_STRING>\" {
    cout << "Line: " << yylineno << ". Found inline double-quoted string literal: \"" + str + "\"" << endl;
    isString = false;
    BEGIN(INITIAL); 
}
<LONG_SINGLE_STRING>\'{3} {
    cout << "Line: " << yylineno << ". Found multiline single-quoted string literal:\n\'" + str + "\'" << endl;
    isString = false;
    BEGIN(INITIAL); 
}
<LONG_DOUBLE_STRING>\"{3} {
    cout << "Line: " << yylineno << ". Found multiline double-quoted string literal:\n\"" + str + "\"" << endl;
    isString = false;
    BEGIN(INITIAL); 
}

<SHORT_SINGLE_STRING,SHORT_DOUBLE_STRING>[\n\r] {
    int strLength = static_cast<int>(str.length());

    if(strLength > 1 && str[strLength - 1] == '\\') {
        str.erase(strLength - 1, 1);
    } else {
        cout << "Line: " << yylineno << ". ERROR: inline string literal is not closed." << endl;
        BEGIN(INITIAL);
    }
}
<SHORT_SINGLE_STRING,SHORT_DOUBLE_STRING><<EOF>> {
    cout << "Line: " << yylineno << ". ERROR: inline string literal is not closed." << endl;
    BEGIN(INITIAL);
}
<LONG_SINGLE_STRING,LONG_DOUBLE_STRING><<EOF>> {
    cout << "Line: " << yylineno << ". ERROR: multiline string literal is not closed." << endl;
    BEGIN(INITIAL);
}

[A-Za-z_][A-Za-z0-9_]*  { cout << "Line: " << yylineno << ". Found identifier: " << yytext << endl; }

<<EOF>> {
            checkIndentiationForKeywords(keywordsLineNumber, indentStack, indentStackTopIndex, &emptyLinesCounter, 0);
            return 0;
        }

. {}

[[:space:]] {}

%%

string removeUnderline(string input)
{
    string str = input;
    str.erase(remove(str.begin(), str.end(), '_'), str.end());
    return str;
}

void calculateIndentiation(int* indentStack, int* indentStackTopIndex, int indentLength) {
    if(indentLength > indentStack[*indentStackTopIndex]) {
        (*indentStackTopIndex)++;
        indentStack[*indentStackTopIndex] = indentLength;
        cout << "Line: " << yylineno << ". Found indent: " << to_string(indentLength) << endl;
    }

    if(indentLength < indentStack[*indentStackTopIndex]) {
        int tmpIndex = *indentStackTopIndex;

        while(indentLength < indentStack[tmpIndex]) { tmpIndex--; }

        if(indentLength != indentStack[tmpIndex]) { cout << "Line: " << yylineno << ". ERROR: incosistent dedent: " << to_string(indentLength) << endl; }
        else {
            while(*indentStackTopIndex > tmpIndex) { 
                (*indentStackTopIndex)--;
                cout << "Line: " << yylineno << ". Found dedent. Indent count: " << indentStack[*indentStackTopIndex] << endl; 
            }
        }  
    }
}

void checkIndentiationForKeywords(deque<int>& keywordsLineNumber, int* indentStack, int indentStackTopIndex, int* emptyLinesCounter, int indentLength) {
    if(keywordsLineNumber.empty()) {
        *emptyLinesCounter = 0;
        return;
    }

    if(yylineno - *emptyLinesCounter - keywordsLineNumber.front() != 1 || indentLength - indentStack[indentStackTopIndex] <= 0) {
        cout << "Line: " << keywordsLineNumber.front() << ". ERROR: not found indent for keyword." << endl;
    }

    *emptyLinesCounter = 0;
    keywordsLineNumber.pop_front();
}