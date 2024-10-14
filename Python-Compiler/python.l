%option never-interactive
%option noyywrap
%option yylineno

%{
    #include <iostream>
    #include <string>
    #include <algorithm>
    #include <deque>
    using namespace std;

    //extern int yylineno;

    string removeUnderline(string text);
    void calculateIndentiation(int* indentStack, int* lastIndentIndex, int dentLen);
    void checkIndentiationForKeywords(deque<int>& keywordLines, int* indentStack, int lastIndentIndex, int* emptyLinesCounter, int dentLen);
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

    int emptyLinesCounter = 0;

    deque<int> keywordLines = {};

    int parenthesesBracketsCounter = 0;
    int squareBracketsCounter = 0;
    int curlyBracketsCounter = 0;
    bool isString = false;
%}

#.* {
    cout << "Line: " << yylineno << ". Found comment: \"" << yytext+1 << "\"" << endl;
}

\n[ \t]*\n+$ {
    string text = yytext;
    emptyLinesCounter = count(text.begin(), text.end(), '\n');
}

\n[ ]* {
    if(!(isString || parenthesesBracketsCounter || squareBracketsCounter || curlyBracketsCounter)) {
        checkIndentiationForKeywords(keywordLines, indentStack, lastIndentIndex, &emptyLinesCounter, yyleng - 1);
        calculateIndentiation(indentStack, &lastIndentIndex, yyleng - 1);
    }
}
^[ ]+ {
    if(!(isString || parenthesesBracketsCounter || squareBracketsCounter || curlyBracketsCounter)) {
        checkIndentiationForKeywords(keywordLines, indentStack, lastIndentIndex, &emptyLinesCounter, yyleng - 1);
        calculateIndentiation(indentStack, &lastIndentIndex, yyleng);
    }
}
\n\t* {
    if(!(isString || parenthesesBracketsCounter || squareBracketsCounter || curlyBracketsCounter)) {
        checkIndentiationForKeywords(keywordLines, indentStack, lastIndentIndex, &emptyLinesCounter, (yyleng - 1) * 4);
        calculateIndentiation(indentStack, &lastIndentIndex, (yyleng - 1) * 4);
    }
}
^\t+ {
    if(!(isString || parenthesesBracketsCounter || squareBracketsCounter || curlyBracketsCounter)) {
        checkIndentiationForKeywords(keywordLines, indentStack, lastIndentIndex, &emptyLinesCounter, (yyleng - 1) * 4);
        calculateIndentiation(indentStack, &lastIndentIndex, yyleng * 4);
    }
}
^[ \t]{2,} {
    if(!(isString || parenthesesBracketsCounter || squareBracketsCounter || curlyBracketsCounter)) {
        cout << "Line: " << yylineno << ". ERROR: mixed tabs and spaces (dentiation)." << endl;
    }
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

\= { cout << "Line: " << yylineno << ". Found assignment operator: " << yytext << endl; }
\+\= { cout << "Line: " << yylineno << ". Found assignment operator: " << yytext << endl; }
\-\= { cout << "Line: " << yylineno << ". Found assignment operator: " << yytext << endl; }
\*\= { cout << "Line: " << yylineno << ". Found assignment operator: " << yytext << endl; }
\/\= { cout << "Line: " << yylineno << ". Found assignment operator: " << yytext << endl; }
\/\/\= { cout << "Line: " << yylineno << ". Found assignment operator: " << yytext << endl; }
\%\= { cout << "Line: " << yylineno << ". Found assignment operator: " << yytext << endl; }
\&\= { cout << "Line: " << yylineno << ". Found assignment operator: " << yytext << endl; }
\|\= { cout << "Line: " << yylineno << ". Found assignment operator: " << yytext << endl; }
\^\= { cout << "Line: " << yylineno << ". Found assignment operator: " << yytext << endl; }
\>\>\= { cout << "Line: " << yylineno << ". Found assignment operator: " << yytext << endl; }
\<\<\= { cout << "Line: " << yylineno << ". Found assignment operator: " << yytext << endl; }
\*\*\= { cout << "Line: " << yylineno << ". Found assignment operator: " << yytext << endl; }

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

\, { cout << "Line: " << yylineno << ". Found delimiter: " << yytext << endl; }
\: { cout << "Line: " << yylineno << ". Found delimiter: " << yytext << endl; }
\! { cout << "Line: " << yylineno << ". Found delimiter: " << yytext << endl; }
\. { cout << "Line: " << yylineno << ". Found delimiter: " << yytext << endl; }
\; { cout << "Line: " << yylineno << ". Found delimiter: " << yytext << endl; }
\-\> { cout << "Line: " << yylineno << ". Found delimiter: " << yytext << endl; }

if {
        cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl;
        keywordLines.push_back(yylineno);
   }
elif {
        cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl;
        keywordLines.push_back(yylineno);
     }
else {
        cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl;
        keywordLines.push_back(yylineno);
     }
class {
        cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl;
        keywordLines.push_back(yylineno);
      }
def {
        cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl;
        keywordLines.push_back(yylineno);
    }
try {
        cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl;
        keywordLines.push_back(yylineno);
    }
except {
            cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl;
            keywordLines.push_back(yylineno);
       }
finally {
            cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl;
            keywordLines.push_back(yylineno);
        }
for {
        cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl;
        keywordLines.push_back(yylineno);
    }
while {
        cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl;
        keywordLines.push_back(yylineno);
      }
with {
        cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl;
        keywordLines.push_back(yylineno);
     }
False { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }     
None { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }      
True { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }        
and { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }           
as { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }           
assert { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }          
async { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }     
await { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }  
break { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }
continue { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }
del { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }
from { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }
global { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }
import { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }
in { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }
is { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }
lambda { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }
nonlocal { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }
not { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }
or { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }
pass { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }
raise { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }
return { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }
yield { cout << "Line: " << yylineno << ". Found hard keyword: " << yytext << endl; }

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

0[bB](\_?[0-1])* {
    if(yyleng == 2) {
        cout << "Line: " << yylineno << ". ERROR: found empty binary literal." << endl;
    } else {
        string text = yytext;
        text = removeUnderline(text);
        cout << "Line: " << yylineno << ". Found binary literal: " << to_string(stoll(text.substr(2), NULL, 2)) << endl;
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
        cout << "Line: " << yylineno << ". Found hexadecimal literal: " << to_string(stoll(text.substr(2), NULL, 16)) << endl;
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

[A-Za-z_][A-Za-z0-9_]*  { cout << "Line: " << yylineno << ". Found identifier: " << yytext << endl; }

<INLINE_STRING_SINGLE_QUOTES>[^\\\'\n]+ { str += yytext; }
<INLINE_STRING_DOUBLE_QUOTES>[^\\\"\n]+ { str += yytext; }
<MULTILINE_STRING_SINGLE_QUOTES>[^\\\']+ { str += yytext; }
<MULTILINE_STRING_DOUBLE_QUOTES>[^\\\"]+ { str += yytext; }

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
    cout << "Line: " << yylineno << ". Found inline single-quoted string literal: \'" + str + "\'" << endl;
    isString = false;
    BEGIN(INITIAL); 
}
<INLINE_STRING_DOUBLE_QUOTES>\" {
    cout << "Line: " << yylineno << ". Found inline double-quoted string literal: \"" + str + "\"" << endl;
    isString = false;
    BEGIN(INITIAL); 
}
<MULTILINE_STRING_SINGLE_QUOTES>\'{3} {
    cout << "Line: " << yylineno << ". Found multiline single-quoted string literal:\n\'" + str + "\'" << endl;
    isString = false;
    BEGIN(INITIAL); 
}
<MULTILINE_STRING_DOUBLE_QUOTES>\"{3} {
    cout << "Line: " << yylineno << ". Found multiline double-quoted string literal:\n\"" + str + "\"" << endl;
    isString = false;
    BEGIN(INITIAL); 
}

<INLINE_STRING_SINGLE_QUOTES,INLINE_STRING_DOUBLE_QUOTES>[\n\r] {
    int strLength = static_cast<int>(str.length());

    if(strLength > 1 && str[strLength - 1] == '\\') {
        str.erase(strLength - 1, 1);
    } else {
        cout << "Line: " << yylineno << ". ERROR: inline string literal is not closed." << endl;
        BEGIN(INITIAL);
    }
}
<INLINE_STRING_SINGLE_QUOTES,INLINE_STRING_DOUBLE_QUOTES><<EOF>> {
    cout << "Line: " << yylineno << ". ERROR: inline string literal is not closed." << endl;
    BEGIN(INITIAL);
}
<MULTILINE_STRING_SINGLE_QUOTES,MULTILINE_STRING_DOUBLE_QUOTES><<EOF>> {
    cout << "Line: " << yylineno << ". ERROR: multiline string literal is not closed." << endl;
    BEGIN(INITIAL);
}

<<EOF>> { checkIndentiationForKeywords(keywordLines, indentStack, lastIndentIndex, &emptyLinesCounter, 0); return 0; }

. {}

[[:space:]] {}
%%

void calculateIndentiation(int* indentStack, int* lastIndentIndex, int dentLen) {
    /* Indent */
    if(dentLen > indentStack[*lastIndentIndex]) {
        (*lastIndentIndex)++;
        indentStack[*lastIndentIndex] = dentLen;
        cout << "Line: " << yylineno << ". Found indent: " << to_string(dentLen) << endl;
    }

    /* Dedent */
    if(dentLen < indentStack[*lastIndentIndex]) {
        int tmpIndex = *lastIndentIndex;

        while(dentLen < indentStack[tmpIndex]) { tmpIndex--; }

        if(dentLen != indentStack[tmpIndex]) { cout << "Line: " << yylineno << ". ERROR: incosistent dedent: " << to_string(dentLen) << endl; }
        else {
            while(*lastIndentIndex > tmpIndex) { 
                (*lastIndentIndex)--;
                cout << "Line: " << yylineno << ". Found dedent. Indent count: " << indentStack[*lastIndentIndex] << endl; 
            }
        }  
    }
}

void checkIndentiationForKeywords(deque<int>& keywordLines, int* indentStack, int lastIndentIndex, int* emptyLinesCounter, int dentLen) {  
    if(keywordLines.empty()) {
        *emptyLinesCounter = 0;
        return;
    }

    if(yylineno - *emptyLinesCounter - keywordLines.front() != 1 || dentLen - indentStack[lastIndentIndex] <= 0) {
        cout << "Line: " << keywordLines.front() << ". ERROR: not found indent for keyword." << endl;
    }

    *emptyLinesCounter = 0;
    keywordLines.pop_front();
}

string removeUnderline(string input)
{
    string str = input;
    str.erase(remove(str.begin(), str.end(), '_'), str.end());
    return str;
}