#ifndef CLASS_COMPILATION_H   
#define CLASS_COMPILATION_H   

/*Created August 11th - Last edited August 11th*/
/*PROGRAM STRUCTURE FUNCTIONS(SENTENCE BUILDING)

void printXmlToken(int tokenDefinedbyX, FILE *outXML)

Tests X to see if it matches either one of the 5 token types(as well as
the sub KEYWORD types) X can be found with tokenType. Will print the
token in xml tags along with the token in string format.

void process(char *process,  FILE *in,  FILE *out) 				
Tests char *process with the existing token. If they match then 
printXmlToken is called. Afterwards we invoke hasMoreTokens and then
we advance() to get the next token.


PROGRAM STRUCTURE: A Jack program is a collection of classes, each appearing in a seperate file. 
					The compilation unit is a class. A class is a sequence of tokens, as follows
 
class: 	'class' className '{' classVarDec* subroutineDec* '}'
classVarDec: ('static'|'field') type varName (';'varName)*

type:'int'|'char'|'boolean'| className
						
subroutineDec:	('constructor'|'function'|'method') ('void'|type)
				subroutineName '(' parameterList ')' subroutineBody

parameterList:	(type varName) (',' type varName)*)?

subroutineBody:	'{' varDec* statements '}'

varDec:	'var' type varName (',' varName)* ';'

className:	identifier

subroutineName:	identifier

varName:	identifier


STATEMENTS:

statements:	statement*

statement:letStatement|ifStatement|whileStatement|doStatement|returnStatement

letStatement:'let' varName ('[' expression ']')? '=' expression ';'

ifStatement:if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?

whileStatement:	'while' '(' expression ')' '{' statements '}'

doStatement: 'do' subroutineCall ';'

returnStatement:'return' expression? ';'
	

EXPRESSIONS:

expression:	term(op term)*

term:
integerConstant |stringConstant |keywordConstant |varName|
varName '[' expression ']'| '(' expression ')' |(unaryOp term) |
subroutineCall

subroutineCall:subroutineName '(' expressionList ')' |
(className|varName)'.'subroutineName '(' expressionList ')'

expressionList:(expression(',' expression)*)?

op:	'+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '=' 

unaryOp:'-' | '~'

keywordConstant:'true' | 'false' | 'null' | 'this'

'xxx':	Represents language tokens that appear verbatim
xxx:	Represents names of terminal and nonterminal elements
():		used for grouping
x|y:	either x or y
x?:		x appears 0 or 1 times
x*:		x appears 0 or more times

void compileClass(FILE *in, FILE *out)
Compiles all the lexical elements of the program 
structure of a class and outputs this.Recursive.

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


HANDLED BY THE ROUTINES THAT REFER TO THEM
className
subroutineName
varName
Statement
subroutineCall
All these have no compilexxx routines.
*/

#include "Token_Creation_Functions.h"

//function declarations

void printXmlToken(int tokenDefinedbyX, FILE *outXML);

void process(char *process,  FILE *in,  FILE *out);

void compileExpressionList(FILE *in,FILE *out);


//term: integerConstant |stringConstant |keywordConstant |varName |varName '[' expression ']'| 
//	'(' expression ')' |(unaryOp term) |subroutineCall
void compileTerm(FILE *in, FILE *out);

//expression: term(op term)*
void compileExpression(FILE *in, FILE *out);


/*  letStatement: 'let' varName ('[' expression ']')? '=' expression ';'  */
void compileLet(FILE *in,FILE *out);

void compileIf(FILE *in,FILE *out);

void compileWhile(FILE * in,FILE *out);

void compileDo(FILE *in,FILE *out);

void compileReturn(FILE * in, FILE *out);

void compileStatements(FILE *in,FILE *out);

void compileParamaterList(FILE *in, FILE *out);

void compileVarDec(FILE *in, FILE *out);
//should I pass the symbolTable variables as arguments into compileClassVarDec?
//	char classSymbolTableName[250];
	//char classSymbolTableType[250];
	//char classSymbolTableKind[250];
	//int classSymbolTable#Static;
	//int classSymbolTable#Field;

	//char subroutineSymbolTableName[250];
	//char subroutineSymbolTableType[250];
	//char subroutineSymbolTableKind[250];
	//int subroutineSymbolTable#Static;
	//int subroutineSymbolTable#Field;

	//possibly have two flags to show if inside a class or inside a subroutine
	//int flagSymbolTableClass;
	//int flagSymbolTableSubroutine;

	//flag to show usage is declared or used
	//int symbolTableUsedorDeclared;

//classVarDec:('static'|'field') type varName (';'varName)*
//type: 'int'|'char'|'boolean'| className
//className:identifier
//varName:identifier

void compileClassVarDec(FILE *in, FILE *out, char *token);

//subroutineBody: '{' varDec* statements '}'
void compileSubroutineBody(FILE *in,FILE *out);

void compileSubroutineDec(FILE *in, FILE *out);
//<identifier> name:%s, type:%s, kind:%s, #:%d, category:%s, usage:%s </identifier>
//<identifier> name:%s, type:%s(class), kind(none):%s, #:%d(none), category:%s, usage:%s </identifier>

/*according to Compiling Classes(project 11, when starting to compile a class, compiler creates a class-level
symbol table and adds to it the field and static variables declared in the class declaration. Compiler also
creates an empty subroutine-level symbol table. No code is generated*/
void compileClass(FILE *in, FILE *out);

#endif