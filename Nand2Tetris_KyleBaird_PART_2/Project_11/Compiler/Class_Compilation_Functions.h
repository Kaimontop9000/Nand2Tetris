/*PROGRAM STRUCTURE FUNCTIONS(SENTENCE BUILDING)
=======================================================================================================================================
void printXmlToken(int tokenDefinedbyX, FILE *outXML)			|Tests X to see if it matches either one of the 5 token types(as well as
																|the sub KEYWORD types) X can be found with tokenType. Will print the
																|token in xml tags along with the token in string format.
---------------------------------------------------------------------------------------------------------------------------------------
void process(char *process,  FILE *in,  FILE *out) 				|Tests char *process with the existing token. If they match then 
																|printXmlToken is called. Afterwards we invoke hasMoreTokens and then
																|we advance() to get the next token.
---------------------------------------------------------------------------------------------------------------------------------------
=======================================================================================================================================
PROGRAM STRUCTURE: A Jack program is a collection of classes, each appearing in a seperate file. The compilation unit is a class. A class
					is a sequence of tokens, as follows
_______________________________________________________________________________________________________________________________________ 
class: 					|	'class' className '{' classVarDec* subroutineDec* '}'
--------------------------------------------------------------------------------------------------------------------------------------
classVarDec:			|	('static'|'field') type varName (';'varName)*
--------------------------------------------------------------------------------------------------------------------------------------
type:					|	'int'|'char'|'boolean'| className
--------------------------------------------------------------------------------------------------------------------------------------						
subroutineDec:			|	('constructor'|'function'|'method') ('void'|type) subroutineName '(' parameterList ')' subroutineBody
--------------------------------------------------------------------------------------------------------------------------------------
parameterList:			|	(type varName) (',' type varName)*)?
--------------------------------------------------------------------------------------------------------------------------------------
subroutineBody:			|	'{' varDec* statements '}'
--------------------------------------------------------------------------------------------------------------------------------------
varDec:					|	'var' type varName (',' varName)* ';'
--------------------------------------------------------------------------------------------------------------------------------------
className:				|	identifier
--------------------------------------------------------------------------------------------------------------------------------------
subroutineName:			|	identifier
--------------------------------------------------------------------------------------------------------------------------------------
varName:				|	identifier
--------------------------------------------------------------------------------------------------------------------------------------
______________________________________________________________________________________________________________________________________
======================================================================================================================================
STATEMENTS:
______________________________________________________________________________________________________________________________________
statements:				|	statement*
--------------------------------------------------------------------------------------------------------------------------------------
statement:				|	letStatement|ifStatement|whileStatement|doStatement|returnStatement
--------------------------------------------------------------------------------------------------------------------------------------
letStatement:			|	'let' varName ('[' expression ']')? '=' expression ';'
--------------------------------------------------------------------------------------------------------------------------------------
ifStatement				|	'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?
--------------------------------------------------------------------------------------------------------------------------------------
whileStatement			|	'while' '(' expression ')' '{' statements '}'
--------------------------------------------------------------------------------------------------------------------------------------
doStatement				|	'do' subroutineCall ';'
--------------------------------------------------------------------------------------------------------------------------------------
returnStatement:		|	'return' expression? ';'
--------------------------------------------------------------------------------------------------------------------------------------	
======================================================================================================================================
EXPRESSIONS:
______________________________________________________________________________________________________________________________________
expression:		term(op term)*

term:			integerConstant |stringConstant |keywordConstant |varName |varName '[' expression ']'| 
				'(' expression ')' |(unaryOp term) |subroutineCall

subroutineCall: subroutineName '(' expressionList ')' | (className|varName)'.'subroutineName '(' expressionList ')'

expressionList:	(expression(',' expression)*)?

op:				'+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '=' 

unaryOp:		'-' | '~'

keywordConstant:'true' | 'false' | 'null' | 'this'
----------------------------------------------------------------------------------------------------------------------------------------
'xxx':	Represents language tokens that appear verbatim
xxx:	Represents names of terminal and nonterminal elements
():		used for grouping
x|y:	either x or y
x?:		x appears 0 or 1 times
x*:		x appears 0 or more times

void compileClass(FILE *in, FILE *out)							|Compiles all the lexical elements of the program structure of
																|a class and outputs this. Recursive
---------------------------------------------------------------------------------------------------------------------------------------
void compileClassVarDec(FILE *out)
void compileSubroutineDec(FILE *out)
void compileParamaterList(FILE *out)
void compileSubroutineBody(FILE *out)
void compileStatements(FILE *out)
void compileLet(FILE *out)
void compileIf(FILE *out)
void compileWhile(FILE *out)
void compileDo(FILE *out)
void compileReturn(FILE *out)
void compileExpression(FILE *out)
void compileTerm(FILE *out)
void compileExpressionList(FILE *out)

------------------------------------------------------------------------------------------------------------
HANDLED BY THE ROUTINES THAT REFER TO THEM
------------------------------------------------------------------------------------------------------------
className
subroutineName
varName
Statement
subroutineCall
All these have no compilexxx routines.
*/