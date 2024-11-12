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

    struct DentStruct {
        int count;
        int DentType;
    };

    int indentStack[1000];
    int indentStackTopIndex = 0;
    struct DentStruct dents{0,0};

    int parenthesesBracketsCounter = 0;
    int squareBracketsCounter = 0;
    int curlyBracketsCounter = 0;

    int emptyLinesCounter = 0;

    bool isString = false;
    bool isFirstLine = true;

    bool isEofWithIndent = false;
    int dentsCountEOF = 0;

    string removeUnderline(string text);
    void calculateIndentiation(int* indentStack, int* indentStackTopIndex, int indentLength, struct DentStruct* dents);
    void checkIndentiationForKeywords(deque<int>& keywordsLineNumber, int* indentStack, int indentStackTopIndex, int* emptyLinesCounter, int indentLength);
%}

%x SHORT_SINGLE_STRING
%x SHORT_DOUBLE_STRING
%x LONG_SINGLE_STRING
%x LONG_DOUBLE_STRING
%x INDENTATION
%x INDENT_OUTPUT
%x COMMENT
%x FIRSTLINE
%x END_OF_FILE

%%

%{
    string str;
    deque<int> keywordsLineNumber = {};
    indentStack[0] = 0;
%}

<INDENT_OUTPUT>""/. {
    if(dents.count > 0) {
        cout << "Line: " << yylineno << ". Return DentType (265-indent, 266-dedent): " << dents.DentType << endl;
        dents.count--;
        return dents.DentType;
    } else {
        BEGIN(INITIAL);
    }
}

^\s*\n {}  

# { BEGIN(COMMENT); }
<COMMENT>[^\n]+ { 
                    cout << "Line: " << yylineno << ". Found comment: \"" << yytext << "\"" << endl;
                    BEGIN(INDENTATION); 
                }

\n {
        if(parenthesesBracketsCounter == 0 && squareBracketsCounter == 0 && curlyBracketsCounter == 0) {
            isString = false;
            BEGIN(INDENTATION);
            cout << "Line: " << yylineno << ". Found \\n (NEWLINE)" << endl;
            return NEWLINE;
        }
   }


<INDENTATION>^[ \t]*# { BEGIN(COMMENT); }

<INDENTATION>^[ \t]*\n { emptyLinesCounter++; }

<INDENTATION>^\t+ {
                        if(!(isString || parenthesesBracketsCounter || squareBracketsCounter || curlyBracketsCounter)) {
                            checkIndentiationForKeywords(keywordsLineNumber, indentStack, indentStackTopIndex, &emptyLinesCounter, yyleng * 4);
                            calculateIndentiation(indentStack, &indentStackTopIndex, yyleng * 4, &dents);
                            BEGIN(INDENT_OUTPUT);
                        }
                  }

<INDENTATION>^[ ]+ {
                        if(!(isString || parenthesesBracketsCounter || squareBracketsCounter || curlyBracketsCounter)) {
                            checkIndentiationForKeywords(keywordsLineNumber, indentStack, indentStackTopIndex, &emptyLinesCounter, yyleng);
                            calculateIndentiation(indentStack, &indentStackTopIndex, yyleng, &dents);
                            BEGIN(INDENT_OUTPUT);
                        }
                   }

<INDENTATION>^[ \t]{2,} {
                            if(!(isString || parenthesesBracketsCounter || squareBracketsCounter || curlyBracketsCounter)) {
                                cout << "Line: " << yylineno << ". ERROR: mixed tabs and spaces (dentiation)." << endl;
                                BEGIN(INITIAL);
                            }
                        }

<INDENTATION>""/. {
                        calculateIndentiation(indentStack, &indentStackTopIndex, 0, &dents);
                        BEGIN(INDENT_OUTPUT);
                  }

<INDENTATION>\n {}

\+ { cout << "Line: " << yylineno << ". Found arithmetical operator: " << yytext << endl; return '+'; return '+'; }
\- { cout << "Line: " << yylineno << ". Found arithmetical operator: " << yytext << endl; return '-'; return '-'; }
\* { cout << "Line: " << yylineno << ". Found arithmetical operator: " << yytext << endl; return '*'; return '*'; }
\*\* { cout << "Line: " << yylineno << ". Found arithmetical operator: " << yytext << endl; }
\/ { cout << "Line: " << yylineno << ". Found arithmetical operator: " << yytext << endl; return '/'; }
\/\/ { cout << "Line: " << yylineno << ". Found arithmetical operator: " << yytext << endl; }
\% { cout << "Line: " << yylineno << ". Found arithmetical operator: " << yytext << endl; }

\<\< { cout << "Line: " << yylineno << ". Found bitwise operator: " << yytext << endl; }
\>\> { cout << "Line: " << yylineno << ". Found bitwise operator: " << yytext << endl; }
\& { cout << "Line: " << yylineno << ". Found bitwise operator: " << yytext << endl; return '&'; }
\| { cout << "Line: " << yylineno << ". Found bitwise operator: " << yytext << endl; return '|'; }
\^ { cout << "Line: " << yylineno << ". Found bitwise operator: " << yytext << endl; }
\~ { cout << "Line: " << yylineno << ". Found bitwise operator: " << yytext << endl; }

\< { cout << "Line: " << yylineno << ". Found comparison operator: " << yytext << endl; return LT; }
\> { cout << "Line: " << yylineno << ". Found comparison operator: " << yytext << endl; return GT; }
\>\= { cout << "Line: " << yylineno << ". Found comparison operator: " << yytext << endl; return GE; }
\<\= { cout << "Line: " << yylineno << ". Found comparison operator: " << yytext << endl; return LE; }
\=\= { cout << "Line: " << yylineno << ". Found comparison operator: " << yytext << endl; return EQ; }
\!\= { cout << "Line: " << yylineno << ". Found comparison operator: " << yytext << endl; return NE; }

\:\= { cout << "Line: " << yylineno << ". Found assignment operator: " << yytext << endl; }
\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; return '='; }
\+\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; return PLUS_ASSIGN; }
\-\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; return MINUS_ASSIGN; }
\*\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; return MUL_ASSIGN; }
\/\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; return DIV_ASSIGN; }
\/\/\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }
\%\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }
\&\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }
\|\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }
\^\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }
\>\>\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }
\<\<\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }
\*\*\= { cout << "Line: " << yylineno << ". Found symbol of assignment statement: " << yytext << endl; }

\, { cout << "Line: " << yylineno << ". Found delimiter: " << yytext << endl; return ','; }
\: { cout << "Line: " << yylineno << ". Found delimiter: " << yytext << endl; return ':'; }
\! { cout << "Line: " << yylineno << ". Found delimiter: " << yytext << endl; return '!'; }
\. { cout << "Line: " << yylineno << ". Found delimiter: " << yytext << endl; return '.'; }
\; { cout << "Line: " << yylineno << ". Found delimiter: " << yytext << endl; return ';'; }
\-\> { cout << "Line: " << yylineno << ". Found delimiter: " << yytext << endl; }

\( { 
        cout << "Line: " << yylineno << ". Found open parentheses bracket: " << yytext << endl;
        parenthesesBracketsCounter++; 
        return '(';
   }
\) { 
        cout << "Line: " << yylineno << ". Found close parentheses bracket: " << yytext << endl;
        parenthesesBracketsCounter--;
        return ')';
   }
\[ { 
        cout << "Line: " << yylineno << ". Found open square bracket: " << yytext << endl;
        squareBracketsCounter++;
        return '[';
   }
\] {
        cout << "Line: " << yylineno << ". Found close square bracket: " << yytext << endl;
        squareBracketsCounter--;
        return ']';
   }
\{ { 
        cout << "Line: " << yylineno << ". Found open curly bracket: " << yytext << endl;
        curlyBracketsCounter++;
   }
\} {
        cout << "Line: " << yylineno << ". Found close curly bracket: " << yytext << endl;
        curlyBracketsCounter--;
   }

int { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; return INT_TYPE; }
float { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; return FLOAT_TYPE; }
complex { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; }
bool { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; return BOOL_TYPE; }
list { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; return LIST_TYPE; }
tuple { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; }
range { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; return RANGE_TYPE; }
str { cout << "Line: " << yylineno << ". Found data type: " << yytext << endl; return STR_TYPE; }
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
        return IF;
   }
elif {
        cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
        keywordsLineNumber.push_back(yylineno);
        return ELIF;
     }
else {
        cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
        keywordsLineNumber.push_back(yylineno);
        return ELSE;
     }
class {
        cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
        keywordsLineNumber.push_back(yylineno);
        return CLASS;
      }
def {
        cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
        keywordsLineNumber.push_back(yylineno);
        return DEF;
    }
try {
        cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
        keywordsLineNumber.push_back(yylineno);
        return TRY;
    }
except {
            cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
            keywordsLineNumber.push_back(yylineno);
            return EXCEPT;
       }
finally {
            cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
            keywordsLineNumber.push_back(yylineno);
            return FINALLY;
        }
for {
        cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
        keywordsLineNumber.push_back(yylineno);
        return FOR;
    }
while {
        cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
        keywordsLineNumber.push_back(yylineno);
        return WHILE;
      }
with {
        cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl;
        keywordsLineNumber.push_back(yylineno);
     }
False { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; return FALSE; }     
None { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }      
True { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; return TRUE;}        
and { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; return AND; }           
as { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; return AS; }           
assert { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }          
async { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }     
await { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }  
break { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
continue { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
del { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
from { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
global { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
import { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
in { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; return IN; }
is { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
lambda { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; return LAMBDA; }
nonlocal { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
not { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; return NOT; }
or { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; return OR; }
pass { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
raise { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
return { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; return RETURN; }
yield { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; }
self { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; return SELF; }
super { cout << "Line: " << yylineno << ". Found keyword: " << yytext << endl; return SUPER; }

0[bB](\_?[0-1])* {
    if(yyleng == 2) {
        cout << "Line: " << yylineno << ". ERROR: found empty binary literal." << endl;
    } else {
        int intNum = stoll(removeUnderline(yytext).substr(2), NULL, 2);
        cout << "Line: " << yylineno << ". Found binary literal: " << intNum << endl;
        yylval.intVal = intNum;
        return INT_C;
    }
}
0[bB](\_?[A-Za-z0-9])* { cout << "Line: " << yylineno << ". ERROR: invalid binary literal (illegal use of letter)." << endl; }
0[bB][01]*(\_+[01]*)* { cout << "Line: " << yylineno << ". ERROR: invalid binary literal (illegal use of underscore)." << endl; }

0[xX](\_?[0-9A-Fa-f])* {
    if(yyleng == 2) {
        cout << "Line: " << yylineno << ". ERROR: found empty hexadecimal literal." << endl;
    } else {
        int intNum = stoll(removeUnderline(yytext).substr(2), NULL, 16);
        cout << "Line: " << yylineno << ". Found hexadecimal literal: " << intNum << endl;
        yylval.intVal = intNum;
        return INT_C;
    }
}
0[xX](\_?[A-Za-z0-9])* { cout << "Line: " << yylineno << ". ERROR: invalid hexadecimal literal (illegal use of letter)." << endl; }
0[xX][0-9A-Fa-f]*(\_+[0-9A-Fa-f]*)* { cout << "Line: " << yylineno << ". ERROR: invalid hexadecimal literal (illegal use of underscore)." << endl; }

0[oO](\_?[0-7])* {
    if(yyleng == 2) {
        cout << "Line: " << yylineno << ". ERROR: found empty octal literal." << endl;
    } else {
        int intNum = stoll(removeUnderline(yytext).substr(2), NULL, 8);
        cout << "Line: " << yylineno << ". Found octal literal: " << intNum << endl;
        yylval.intVal = intNum;
        return INT_C;
    }
}
0[oO](\_?[A-Za-z0-9])* { cout << "Line: " << yylineno << ". ERROR: invalid octal literal (unexpected character)." << endl; }
0[oO][0-7]*(\_+[0-7]*)* { cout << "Line: " << yylineno << ". ERROR: invalid octal literal (illegal use of underscore)." << endl; }

([0-9]\_?)*([0-9]\.|\.[0-9])(\_?[0-9])*([eE][+-]?[0-9](\_?[0-9])*)?|[0-9](\_?[0-9])*([eE][+-]?[0-9](\_?[0-9])*) {
    float floatNum = stof(removeUnderline(yytext));
    cout << "Line: " << yylineno << ". Found float literal: " << floatNum << endl;
    yylval.floatVal = floatNum;
    return FLOAT_C;
}
([0-9]\_?)*([0-9]\.|\.[0-9])(\_+[0-9]*)*([eE][+-]?[0-9](\_+[0-9]*)*)?|([0-9]\_?)+([eE][+-]?[0-9](\_+[0-9]*)*) {
     cout << "Line: " << yylineno << ". ERROR: invalid float literal (illegal use of underscore)." << endl;
}

[0-9](\_?[0-9])* {
    int intNum = stoi(removeUnderline(yytext));
    
    if(yyleng > 1 && yytext[0] == '0' && intNum != 0) {
        cout << "Line: " << yylineno << ". ERROR: invalid decimal literal (illegal use of 0)." << endl;
    } else {
        cout << "Line: " << yylineno << ". Found decimal literal: " << intNum << endl;
        yylval.intVal = intNum;
        return INT_C;
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
    
    yylval.stringVal = &str;
    return STRING_C; 
}
<SHORT_DOUBLE_STRING>\" {
    cout << "Line: " << yylineno << ". Found inline double-quoted string literal: \"" + str + "\"" << endl;
    isString = false;
    BEGIN(INITIAL); 

    yylval.stringVal = &str;
    return STRING_C; 
}
<LONG_SINGLE_STRING>\'{3} {
    cout << "Line: " << yylineno << ". Found multiline single-quoted string literal:\n\'" + str + "\'" << endl;
    isString = false;
    BEGIN(INITIAL); 

    yylval.stringVal = &str;
    return STRING_C; 
}
<LONG_DOUBLE_STRING>\"{3} {
    cout << "Line: " << yylineno << ". Found multiline double-quoted string literal:\n\"" + str + "\"" << endl;
    isString = false;
    BEGIN(INITIAL); 

    yylval.stringVal = &str;
    return STRING_C; 
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

[A-Za-z_][A-Za-z0-9_]*  { 
    cout << "Line: " << yylineno << ". Found identifier: " << yytext << endl;

    string text = yytext; 
    yylval.identifier = &text;
    return ID; 
}

<INITIAL,INDENTATION,INDENT_OUTPUT><<EOF>> {
    cout << "======== LEXER: FIRST EOF ========" << endl;

    // Used to correctly count indent/dedent in line with EOF
    if(dents.count > 0 && dents.DentType == INDENT) { isEofWithIndent = true; }
    else if(dents.count > 0 && dents.DentType == DEDENT) { dentsCountEOF += dents.count; }

    checkIndentiationForKeywords(keywordsLineNumber, indentStack, indentStackTopIndex, &emptyLinesCounter, 0);
    calculateIndentiation(indentStack, &indentStackTopIndex, 0, &dents);

    // Used to correctly count indent/dedent in line with EOF
    if(isEofWithIndent) { dentsCountEOF += dents.count - 1; }
    else { dentsCountEOF += dents.count; }

    BEGIN(END_OF_FILE);
    cout << "Return NEWLINE" << endl;
    return NEWLINE;
}

<END_OF_FILE><<EOF>> {
    cout << "======== LEXER: SECOND EOF ========" << endl;

    if(dentsCountEOF > 0) {
        dentsCountEOF--;
        cout << "Return dents.DentType (265-indent, 266-dedent): " << dents.DentType << endl;
        return dents.DentType;
    } else {
        cout << "LEXER: yyterminate()" << endl;
        yyterminate();
    }
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

void calculateIndentiation(int* indentStack, int* indentStackTopIndex, int indentLength, struct DentStruct* dents) {
    // INDENT
    if(indentLength > indentStack[*indentStackTopIndex]) {
        (*indentStackTopIndex)++;
        indentStack[*indentStackTopIndex] = indentLength;
        cout << "Line: " << yylineno << ". Found indent: " << to_string(indentLength) << endl;

        (*dents).count = 1;
        (*dents).DentType = INDENT;
    }
    
    // DEDENT
    if(indentLength < indentStack[*indentStackTopIndex]) {
        int tmpIndex = *indentStackTopIndex;

        while(indentLength < indentStack[tmpIndex]) { tmpIndex--; }

        if(indentLength != indentStack[tmpIndex]) { cout << "Line: " << yylineno << ". ERROR: incosistent dedent: " << to_string(indentLength) << endl; }
        else {
            (*dents).count = 0;
            (*dents).DentType = DEDENT;

            while(*indentStackTopIndex > tmpIndex) { 
                (*indentStackTopIndex)--;
                (*dents).count++;
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