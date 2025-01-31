enum ExprType {
	_PLUS = 1,						
	_MINUS = 2,						
	_MUL = 3,						
	_DIV = 4,						
	_AND_LOGIC = 5,					
	_AND_BITWISE = 6,				
	_OR_LOGIC = 7,					
	_OR_BITWISE = 8,				
	_GREAT = 9,						
	_GREAT_EQUAL = 10,				
	_LESS = 11,						
	_LESS_EQUAL = 12,				
	_EQUAL = 13,					
	_NOT_EQUAL = 14,				
	_U_PLUS = 15,					
	_U_MINUS = 16,					
	_LAMBDA = 17,					
	_ASSIGN_OP = 18,				
	_SUPER = 19,					
	_SELF = 20,						
	_FALSE = 21,					
	_TRUE = 22,						
	_STRING_CONST = 23,				
	_FLOAT_CONST = 24,				
	_INT_CONST = 25,				
	_ATTRIBUTE_REF = 26, // Оно же field ref (ссылка на поле класса)		
	_METHOD_CALL = 27,				
	_FUNCTION_CALL = 28,			
	_SLICING_LIST_ACCESS = 29,		
	_LIST_ACCESS = 30,				
	_LIST_COMPREHENSION = 31,		
	_LIST_CREATION = 32,			
	_BRACKETS = 33, // '(' expr ')'	
	_IDENTIFIER = 34,				
	_IF_HEADER = 35,				
	_FOR_HEADER = 36,
	_FUNC_HEADER = 37,
	_NOT = 38
};
