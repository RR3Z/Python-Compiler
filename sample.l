%{
	#include <iostream>
	extern yyFlexLexer *flexLexer = new yyFlexLexer();
%}

%option never-interactive
%option noyywrap
%option c++
%%
[0-9]* {std::cout << flexLexer->YYText() << std::endl;}
False {std::cout << "Found keyword: False" << std::endl;}     
None {std::cout << "Found keyword: None" << std::endl;}   
True {std::cout << "Found keyword: True" << std::endl;}     
and {std::cout << "Found keyword: and" << std::endl;}       
as {std::cout << "Found keyword: as" << std::endl;}       
assert {std::cout << "Found keyword: assert" << std::endl;}       
async {std::cout << "Found keyword: async" << std::endl;}

await {std::cout << "Found keyword: await" << std::endl;}   
break {std::cout << "Found keyword: break" << std::endl;}   
class {std::cout << "Found keyword: class" << std::endl;}   
continue {std::cout << "Found keyword: continue" << std::endl;}   
def {std::cout << "Found keyword: def" << std::endl;}   
del {std::cout << "Found keyword: del" << std::endl;}   
elif {std::cout << "Found keyword: elif" << std::endl;}

else {std::cout << "Found keyword: else" << std::endl;}
except {std::cout << "Found keyword: except" << std::endl;}
finally {std::cout << "Found keyword: finally" << std::endl;}
for {std::cout << "Found keyword: for" << std::endl;}
from {std::cout << "Found keyword: from" << std::endl;}
global {std::cout << "Found keyword: global" << std::endl;}
if {std::cout << "Found keyword: if" << std::endl;}

import {std::cout << "Found keyword: import" << std::endl;}
in {std::cout << "Found keyword: in" << std::endl;}
is {std::cout << "Found keyword: is" << std::endl;}
lambda {std::cout << "Found keyword: lambda" << std::endl;}
nonlocal {std::cout << "Found keyword: nonlocal" << std::endl;}
not {std::cout << "Found keyword: not" << std::endl;}
or {std::cout << "Found keyword: or" << std::endl;}

pass {std::cout << "Found keyword: pass" << std::endl;}
raise {std::cout << "Found keyword: raise" << std::endl;}
return {std::cout << "Found keyword: return" << std::endl;}
try {std::cout << "Found keyword: try" << std::endl;}
while {std::cout << "Found keyword: while" << std::endl;}
with {std::cout << "Found keyword: with" << std::endl;}
yield {std::cout << "Found keyword: yield" << std::endl;}

match {std::cout << "Found keyword: match" << std::endl;}
case {std::cout << "Found keyword: case" << std::endl;}
type {std::cout << "Found keyword: type" << std::endl;}
_ {std::cout << "Found keyword: _" << std::endl;}

. {}
%%
int main(int argc, char *argv[])
{
	flexLexer->yylex();
	return 0;
}