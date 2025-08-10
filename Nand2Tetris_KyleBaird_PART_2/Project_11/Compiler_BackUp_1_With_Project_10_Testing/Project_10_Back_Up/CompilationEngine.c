/*Created June 01 2025- Last edited July 21th 2025
Jack Tokenizer
=================================================
Usage - ./Tokenizer_01 <filename.jack or folderName>

Usage for TextComparer - bash TextComparer.sh Folder/file.xml Folder_test/file.xml 
--------------------------------------------------------------------------------------------------------------------------------------
Token Creation Functions:						|Description
--------------------------------------------------------------------------------------------------------------------------------------
int hasMoreTokens(FILE *input)					|Reads FILE *input, while not EOF, skips whitespaces until it finds a valid character
												|to return
--------------------------------------------------------------------------------------------------------------------------------------												
int advance(FILE *in, char *string, int *flag)	|Only called if hasMoreTokens returns true. Builds a token. Skips over comments. Does
												|not keep track of the type of token it creates but it will create valid token types.
												|There is a flag that is used to represent if "string constants" are found. """ are
												|not stored in the char *string. If there is """ then flag is true. 
--------------------------------------------------------------------------------------------------------------------------------------
int tokenType(char *token, int flag)			|Determines the token type. Returns one of five constants: SYMBOL, KEYWORD, IDENTIFIER,
												|													INT_CONST, STRING_CONST
--------------------------------------------------------------------------------------------------------------------------------------												
int keyword(char *string)						|Returns the type of KEYWORD : CLASS, METHOD,FUNCTION, CONSTRUCTOR, INTE ,BOOLEA
												|							CHARA, VOIDS, VAR, STATIC, FIELD, LET, DO, IF, ELSE, 
												|							WHILE, RETURN, TRU, FALS ,NUL,THIS
--------------------------------------------------------------------------------------------------------------------------------------
char symbol(char *string)						|Returns string[0] which is one of the symbols
--------------------------------------------------------------------------------------------------------------------------------------
char *identifer(char *string)					|Returns char *string: 
--------------------------------------------------------------------------------------------------------------------------------------
char *intVal(char *string)						|Returns char *string: integerConstant
---------------------------------------------------------------------------------------------------------------------------------------
char *stringVal(char *string)					|Returns char *string: stringConstant
---------------------------------------------------------------------------------------------------------------------------------------
_______________________________________________________________________________________________________________________________________
LEXICAL ELEMENTS: 5 types of terminal elements(tokens)
_______________________________________________________________________________________________________________________________________
KEYWORD: 'class'|'constructor'|'function'|'method'|'field'|'static'|'var'|'int'|'char'|'boolean'|'void'|'true'|'false'|'null'|
		 'this'	|'let'		  |'do'		 |'if'	  |'else' |'while' |'return'
_______________________________________________________________________________________________________________________________________
SYMBOL: '{' | '}' | '(' | ')' | '[' |
		']' | '.' | ',' | ';' | '+' |
	    '-' | '*' | '/' | '&' | '|' |	
	    '<' | '>' | '=' | '~'
_______________________________________________________________________________________________________________________________________
INTEGERCONSTANT: decimal integer range 0...32767
_______________________________________________________________________________________________________________________________________
STRINGCONSTANT: "sequence of characters not including double quotes or newline"
_______________________________________________________________________________________________________________________________________
IDENTIFIER:		 sequence of letters, digits and underscore not starting with a digit.
_______________________________________________________________________________________________________________________________________

PROGRAM STRUCTURE FUNCTIONS(SENTENCE BUILDING)
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
------------------------------------------------------
FILE PROCESSING FUNCTIONS
------------------------------------------------------
void make_output_filename(const char *input_filename, char *output_filename, size_t size)

file processing handled in main. No functions yet, should probably make functions for 
file processing.

----------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <stdlib.h>

//TokenType
#define SYMBOL 0
#define KEYWORD 1
#define IDENTIFIER 2
#define INT_CONST 3
#define STRING_CONST 4

//KeywordType
#define CLASS 5
#define METHOD 6
#define FUNCTION 7
#define CONSTRUCTOR 8
#define INTE 9
#define BOOLEA 10
#define CHARA 11
#define VOIDS 12
#define VAR 13
#define STATIC 14
#define FIELD 15
#define LET 16
#define DO 17
#define IF 18
#define ELSE 19
#define WHILE 20
#define RETURN 21
#define TRU 22
#define FALS 23
#define NUL 24
#define THIS 25 


char token[250];			//used to hold the token 
int stringFlag;				//used for string constants, because we remove the "" from the string, this flag helps us to differentiate
							//between identifiers. Flag is set in advance() and is tested in the tokenType(), i could take the flag and make it a local variable for the keyword type function. by copying the strinf with """ and then ky removjng them qhen needed.
int len = 250;

//function declarations
void compileExpression(FILE *in, FILE *out);
void compileStatements(FILE *in,FILE *out);

void skipBlockComment(FILE *in) {
    int c, prev = 0;
    int count = 0;

    while ((c = fgetc(in)) != EOF) {
        if (prev == '*' && c == '/') {
            break;
        }
        prev = c;

        count++;
        if (count % 10000 == 0) {
            fprintf(stderr, "Skipping block comment... chars read: %d\n", count);
            fflush(stderr);
        }
        if (count > 100000) {
            fprintf(stderr, "Error: Possibly unterminated block comment\n");
            exit(1);
        }
    }
}


/*hasMoreTokens() skips over whitespace and returns if a valid character is found. I think it only skips over the first initial whitespace
but because we assume that the source code is error free I hope that there is never double space characters*/

int hasMoreTokens(FILE *input){
	int c;
	while((c=fgetc(input))!=EOF){
		if(!isspace(c)){
			ungetc(c, input);
			return 1;
		}				
	}
	return 0;
}

/* token generation/creation : advance() only called if hasMoreTokens() returns true. Takes a FILE pointer to read in the next token, 
	a char string[] to hold the token, maxLen for buffer overflow and a global flag that is used to for string constants(this helps
	differentiate between string constants and identifiers because we eliminat the "" from string constants */

int advance(FILE *in, char *string, int *flag){
	*flag = 0;  //reset flag everytime advance is called, gets ready to find next token

	int x = 1; //used for the while loop, posisbly another more elegant solution

	int c;		//holds the next char from FILE pointer
	int index=0;	//lets us know how far along we are into the token. How many chars we have read in 
	
	while(x){
		c=fgetc(in);

		if (c == '/') {
   			int next = fgetc(in);
    		if (next == '/') {
        		// Line comment: skip until end of line
        		while ((c = fgetc(in)) != EOF && c != '\n');
       				continue;  // start over to get next character
    		} 
    		else if (next == '*') {
       			//fprintf(stderr, "Found block comment start\n");
        		skipBlockComment(in);
       			//fprintf(stderr, "Finished skipping block comment\n");
        		continue;
   			}   // start over to get next character 
   			else {
   				// Not a comment: treat '/' as symbol
    			ungetc(next, in);
    			if(index > 0){
        			string[index] = '\0';
        			x = 0;
       		 	return 1;
    			} else {
        			string[index++] = '/';
        			string[index] = '\0';
       				 x = 0;
        		return 1;
    			}
			}	
		}	
		else if (c == EOF) {
 		   if (index > 0) {
   				string[index] = '\0';  // return last token
        		x = 0;
        		return 1;
    			} 
    			else {
        		x = 0; // stop loop
        		string[0] = '\0'; // no more tokens
    			return 0;
    			}
		}
		else if (isspace(c)) {
			if (index == 0) {
				continue;  // skip leading whitespace
			}else {
			string[index] = '\0';  // finish current token
			x = 0;
			return 1;
			}
		}
		else if(c == '{' || c == '}' || c == '(' || c == ')'|| c == '[' || c == ']' || c == '.' || c == ',' || c == ';' || c == '+' 
			|| c == '-' || c == '*' || c == '/' || c == '&' || c == '|' || c == '<'|| c == '>' || c == '=' || c == '~'){
			//already building a token
			if(index>0){
				ungetc(c, in);
				string[index] = '\0';
				x = 0;
				return 1;
			}
			//token is a symbol
			else{
				string[index++]=c;
				string[index] = '\0';
				x = 0;
				return 1;
			}
		}	//carry on building token
		else if (c == '"') {
    		if (index == 0) {
        	// start reading a string constant token
        	index = 0;
       		while ((c = fgetc(in)) != EOF && c != '"') {
            	if (index < len - 1) {
                string[index++] = c;
            	}
        	}
        	string[index] = '\0';
        	*flag = 1;
        	x = 0;  // done with this token
        	return 1;
    		} else {
        		// already building a token, so this " is a delimiter
        		ungetc(c, in);  // put back the quote for next token read
        		string[index] = '\0';
       			x = 0;  // end current token here
    			return 1;
    			}
			}			
		else if (isalpha(c) || c == '_') {
    		// Start of an identifier
    		string[index++] = c;
    		while ((c = fgetc(in)) != EOF && (isalnum(c) || c == '_')) {
        		if (index < len - 1) {
            		string[index++] = c;
        		}
    		}
    		if (c != EOF) {
       	 		ungetc(c, in);  // put back non-identifier character
   			}
    		string[index] = '\0';
    		return 1;
		}
		else if (isdigit(c)) {
   			string[index++] = c;
    		while ((c = fgetc(in)) != EOF && isdigit(c)) {
        		if (index < len - 1) {
            		string[index++] = c;
        		}
    		}
    		if (c != EOF) {
        		ungetc(c, in);
    		}
    		string[index] = '\0';
    		return 1;
		}		
	}
}

/*tokenType() returns a constant of either KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST as set by define*/

int tokenType(char *token, int flag){

	if (token == NULL || *token == '\0') {
    	return -1; // invalid token
	}
	else if(strcmp(token,"{") == 0 || strcmp(token,"}") == 0 || strcmp(token,"(") == 0 || strcmp(token,")") == 0 || strcmp(token, "[") == 0
		|| strcmp(token, "]") == 0 || strcmp(token, ".") == 0 || strcmp(token, ",") == 0 ||
		strcmp(token, ";") == 0 || strcmp(token, "+") == 0 || strcmp(token,"-") == 0 || strcmp(token, "*") == 0 ||
		strcmp(token, "/") == 0 || strcmp(token, "&") == 0 || strcmp(token, "|") == 0 || strcmp(token, "<") == 0 || 
		strcmp(token,">") == 0 || strcmp(token,"=") == 0 || strcmp(token,"~") == 0){

		return SYMBOL;	
	} 
	else if(strcmp(token, "class") == 0 || strcmp(token, "constructor") == 0 || strcmp(token,"function") == 0 ||
	 	strcmp(token, "method") == 0 || strcmp(token, "field") == 0 || strcmp(token, "static") == 0 || strcmp(token,"var") == 0 
	 	|| strcmp(token, "int")== 0 || strcmp(token,"char")== 0 || strcmp(token, "boolean") == 0 || strcmp(token, "void") == 0
	 	|| strcmp(token , "true") == 0 || strcmp(token, "false") == 0 || strcmp(token, "null") == 0 || strcmp(token,"this") == 0
		|| strcmp(token, "let") == 0 || strcmp(token, "do") == 0 || strcmp(token, "if") == 0 || strcmp(token, "else") == 0 
		|| strcmp(token, "while") == 0 || strcmp(token, "return") == 0){

		return KEYWORD;
	}
	else if(flag == 1){
    	return STRING_CONST;
	}
	else if (isdigit(token[0])) {
    	// Check if all characters are digits
    	const char *str = token;
   	 	while (*str) {
        	if (!isdigit((unsigned char)*str)) {
            	return IDENTIFIER;  // Not a pure integer => must be identifier
        	}
        	str++;
    	}
    	return INT_CONST; // All digits
	}
	else {
    	return IDENTIFIER;
	}
}
	
int keyword(char *string){
	if(strcmp(string, "class")==0){
		return CLASS;
	}
	else if(strcmp(string, "method")==0){
		return METHOD;
	}
	else if(strcmp(string, "function")==0){
		return FUNCTION;
	}
	else if(strcmp(string, "constructor")==0){
		return CONSTRUCTOR;
	}
	else if(strcmp(string, "int")==0){
		return INTE;
	}
	else if(strcmp(string, "boolean")==0){
		return BOOLEA;
	}
	else if(strcmp(string, "char")==0){
		return CHARA;
	}
	else if(strcmp(string, "void")==0){
		return VOIDS;
	}
	else if(strcmp(string, "var")==0){
		return VAR;
	}
	else if(strcmp(string, "static")==0){
		return STATIC;
	} 
	else if(strcmp(string, "field")==0){
		return FIELD;
	}
	else if(strcmp(string, "let")==0){
		return LET;
	}
	else if(strcmp(string, "do")==0){
		return DO;
	}
	else if(strcmp(string, "if")==0){
		return IF;
	}
	else if(strcmp(string, "else")==0){
		return ELSE;
	}
	else if(strcmp(string, "while")==0){
		return WHILE;
	}
	else if(strcmp(string, "return")==0){
		return RETURN;
	}
	else if(strcmp(string, "true")==0){
		return TRU;
	}
	else if(strcmp(string, "false")==0){
		return FALS;
	}
	else if(strcmp(string, "null")==0){
		return NUL;
	}
	else if(strcmp(string, "this")==0){
		return THIS;
	}else{
		return -1;
	}
}

char symbol(char *string){
	return string[0];
}

char *identifer(char *string){
	return string;
}

char *intVal(char *string){
	return string;
}

char *stringVal(char *string){
	return string;
}

void make_output_filename(const char *input_filename, char *output_filename, size_t size) {
    const char *dot = strrchr(input_filename, '.');
    size_t base_len = dot ? (size_t)(dot - input_filename) : strlen(input_filename);

    // Make sure there's room for base name + "T.xml" + null terminator
    if (base_len + 4 >= size) base_len = size - 5;

    strncpy(output_filename, input_filename, base_len);
    output_filename[base_len] = '\0';
    strncat(output_filename, ".xml", size - base_len - 1);
}

void printXmlToken(int tokenDefinedbyX, FILE *outXML){
	
	if(tokenDefinedbyX == SYMBOL){
		char c = symbol(token);
		if(c == '<'){
			fprintf(outXML, "<symbol> &lt; </symbol>\n");
		}else if(c == '>'){
			fprintf(outXML, "<symbol> &gt; </symbol>\n");
		}else if(c == '&'){
			fprintf(outXML, "<symbol> &amp; </symbol>\n");
		}else{
			fprintf(outXML, "<symbol> %c </symbol>\n",c);
		}
	}
	else if(tokenDefinedbyX == KEYWORD){				
		int y = keyword(token);
		if( y == CLASS){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}else if(y == METHOD ){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == FUNCTION){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == CONSTRUCTOR){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == INTE){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);	
		}
		else if(y == BOOLEA){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == CHARA){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == VOIDS){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == VAR){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == STATIC){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == FIELD){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == LET){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == DO){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == IF){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == ELSE){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == WHILE){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == FALS){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == RETURN){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == NUL){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
		else if(y == THIS){
			fprintf(outXML, "<keyword> %s </keyword>\n",token);
		}
	}
	else if(tokenDefinedbyX == STRING_CONST){
		fprintf(outXML, "<stringConstant> %s </stringConstant>\n",token);
	}else if(tokenDefinedbyX == INT_CONST){
		fprintf(outXML, "<intConstant> %s </intConstant>\n",token);
	}else if(tokenDefinedbyX == IDENTIFIER){
		fprintf(outXML, "<identifier> %s </identifier>\n",token);
	}
}	

void process(char *process,  FILE *in,  FILE *out) {
	
	if(strcmp(token,process)==0) {

		int x = tokenType(token, stringFlag);
		printXmlToken(x, out);

		printf("Before advance: token = '%s'\n", token);
		if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
			printf("After advance: token = '%s'\n", token);
		}
		else{
        // Debug info: print error message showing expected vs actual token
        printf("Syntax error: expected token '%s' but could not advance\n", process);
		}
	}else{
		fprintf(out,"Syntax error: expected token '%s' but found '%s'\n", process,token);
	}
}

void compileExpressionList(FILE *in,FILE *out){
	printf("<expressionList>\n");
	fprintf(out, "<expressionList>\n");
	
	if(strcmp(token,")")==0){
		fprintf(out, "</expressionList>\n");
		printf("</expressionList>\n");
		return;
	}
	compileExpression(in,out);
	while(1){
		if(strcmp(token,",")==0){
			process(",",in,out);
			//if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
		   //printf("token read: %s\n",token );
		//}
			compileExpression(in,out);
		}else{
		break;
		}
	}
	fprintf(out, "</expressionList>\n");
	printf("</expressionList>\n");
}


//term: integerConstant |stringConstant |keywordConstant |varName |varName '[' expression ']'| 
//	'(' expression ')' |(unaryOp term) |subroutineCall
void compileTerm(FILE *in, FILE *out){
	fprintf(out, "<term>\n");
	printf("<term>\n");
	

	int x = tokenType(token, stringFlag);

	if(x == INT_CONST){
		fprintf(out, "<integerConstant> %s </integerConstant>\n",token);
		//if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
		//    printf("token read: %s\n",token );
		//}
	}else if(x == STRING_CONST){
		fprintf(out, "<stringConstant> %s </stringConstant>\n",token);
		//if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
		//    printf("token read: %s\n",token );
	}else if(strcmp(token, "true")==0||strcmp(token,"false")==0||
	strcmp(token,"null")==0||strcmp(token,"this")==0){
		fprintf(out, "<keyword> %s </keyword>\n",token);
		//if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
		//    printf("token read: %s\n",token );
		//}
	}else if(x == IDENTIFIER){

		char identifier[250];
		strcpy(identifier, token); // save it

		int flagL1 = 0;
		char tokenL1[250];

		long savedPos = ftell(in);  // save current file position
		if(hasMoreTokens(in) && advance(in, tokenL1, &flagL1)){	// lookahead	
			fseek(in, savedPos, SEEK_SET); // restore position
		}

		//these two are the irregular lookahead. We must lookahead to determine wether we are
		//dealing with an array[] or a subroutineCall. We lookead only when token is a varName
		//or subroutine name. In other words, when we have an identifier as our term.



		if(strcmp(tokenL1,"[")==0){
			fprintf(out, "<identifier> %s </identifier>\n",identifier);
			if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			printf("token read: %s\n",token );
			}
			process("[", in, out);
			compileExpression(in,out);
			process("]", in, out);
			fprintf(out, "</term>\n");
			return;
		}else if(strcmp(tokenL1,"(")==0){
			fprintf(out, "<identifier> %s </identifier>\n",identifier);
			if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			printf("token read: %s\n",token );
			}
			process("(", in,out);
			compileExpressionList(in, out);
			process(")", in, out);
			fprintf(out, "</term>\n");
			return;

		}else if(strcmp(tokenL1,".")==0){
			
			fprintf(out, "<identifier> %s </identifier>\n",identifier);
			if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			printf("token read: %s\n",token );
			}
			process(".", in, out);
			fprintf(out, "<identifier> %s </identifier>\n",token);
			if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			printf("token read: %s\n",token );
			}
			process("(", in,out);
			compileExpressionList(in,out);
			process(")", in,out);
			fprintf(out, "</term>\n");
			return;
		}else {
			fprintf(out, "<identifier> %s </identifier>\n", identifier);
			strcpy(token, tokenL1);  // restore token for parent context
			stringFlag = flagL1;
		}

	}else if(strcmp(token,"~")==0 || strcmp(token,"-")==0){
		fprintf(out, "<symbol> %s </symbol>\n",token);
		if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    		printf("token read: %s\n",token );
		}
		compileTerm(in,out);
		fprintf(out, "</term>\n");
		return;
	}
	else if(strcmp(token,"(")==0){
		process("(",in,out);
		compileExpression(in, out);
		process(")", in, out);
		fprintf(out, "</term>\n");
		return;
	}else {
    	fprintf(stderr, "Syntax error in term: unexpected token '%s'\n", token);
	}
	if (hasMoreTokens(in)) {
		advance(in, token, &stringFlag);
	}
	fprintf(out, "</term>\n");
	printf("</term>\n");
}

//expression: term(op term)*
void compileExpression(FILE *in, FILE *out){
	fprintf(out, "<expression>\n");
	printf("<expression>\n");

	compileTerm(in,out);
	
	while(strcmp(token,"+")==0 || strcmp(token,"-")==0 || strcmp(token,"*")==0 ||
		  strcmp(token,"/")==0 || strcmp(token,"&")==0 || strcmp(token,"|")==0 ||
		  strcmp(token,"<")==0 || strcmp(token,">")==0 || strcmp(token,"=")==0) {
		
		if(strcmp(token,"<")==0){
			fprintf(out, "<symbol> &lt; </symbol>\n");
		}
		else if(strcmp(token,">")==0){
			fprintf(out, "<symbol> &gt; </symbol>\n");
		}else if(strcmp(token,"&")==0){
			fprintf(out, "<symbol> &amp; </symbol>\n");
		}else{
			fprintf(out, "<symbol> %s </symbol>\n", token);  
		}
		if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			//fprintf(out,"token read after symbol: %s\n",token );
			}

		compileTerm(in, out);
	}

	fprintf(out, "</expression>\n");
	printf("</expression>\n");
}



/*  letStatement: 'let' varName ('[' expression ']')? '=' expression ';'  */
void compileLet(FILE *in,FILE *out){

	fprintf(out, "<letStatement>\n");
	printf("<letStatement>\n");
	//let
	process("let",in,out);

	//varName
	int x = tokenType(token, stringFlag);
	if(x == IDENTIFIER){
		fprintf(out,"<identifier> %s </identifier>\n",token);
	}else{
		printf("ERROR\n");
	}

	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    	printf("token read: %s\n",token );
	}


	if(strcmp(token,"[")==0){
		process("[",in,out);
		compileExpression(in, out);
		process("]",in,out);
	}

			
	//equal sign
	if(strcmp(token,"=")==0){
		process("=", in, out);
	
		compileExpression(in,out);
		//;
		if(strcmp(token,";")==0){
		process(";",in,out);
		//if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
		//	printf("After advance: token = '%s'\n", token);
		//}
		//fprintf(out,"process ; complete. token now =%s\n",token );
		}
	}
	else if(strcmp(token,";")==0){
		process(";", in, out);
	}

	fprintf(out, "</letStatement>\n");
	printf("</letStatement>\n");
}

void compileIf(FILE *in,FILE *out){
	fprintf(out, "<ifStatement>\n");
	printf("<ifStatement>\n");
	process("if",in,out);
	process("(",in,out);
	compileExpression(in,out);
	process(")",in,out);
	process("{",in,out);
	compileStatements(in,out);
	process("}",in,out);
	if(strcmp(token,"else")==0){
		process("else",in,out);
		process("{",in,out);
		compileStatements(in,out);
		process("}",in,out);
	}
	fprintf(out, "</ifStatement>\n");
	printf("</ifstatement>\n");
}

void compileWhile(FILE * in,FILE *out){
	fprintf(out, "<whileStatement>\n");
	printf("<whileStatement>\n");

	process("while",in,out);
	process("(",in,out);
	compileExpression(in,out);
	process(")",in,out);
	process("{",in,out);
	compileStatements(in,out);
	process("}",in,out);
	fprintf(out, "</whileStatement>\n");
	printf("</whileStatement>\n");
}

void compileDo(FILE *in,FILE *out){
	fprintf(out, "<doStatement>\n");
	printf("<doStatement>\n");
	process("do",in,out);
	fprintf(out, "<identifier> %s </identifier>\n",token);
	
	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    	printf("token read: %s\n",token );
	}

	if(strcmp(token,"(")==0){
		process("(", in,out);
		compileExpressionList(in, out);
		process(")", in, out);

	}else if(strcmp(token,".")==0){

		process(".", in, out);
		fprintf(out, "<identifier> %s </identifier>\n",token);
		if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
   			printf("token read: %s\n",token );
		}
		process("(", in,out);
		compileExpressionList(in,out);
		process(")", in,out);
		//if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			//fprintf(out,"token read after symbol: %s\n",token );
		//	}
	}
	process(";",in,out);
	fprintf(out, "</doStatement>\n");
	printf("</doStatement>\n");
}

void compileReturn(FILE * in, FILE *out){
	fprintf(out, "<returnStatement>\n");
	printf("<returnStatement>\n");
	process("return", in,out);
	
	int x = tokenType(token, stringFlag);

	if(x == INT_CONST||x == STRING_CONST|| x == IDENTIFIER ||
	strcmp(token, "true")==0||strcmp(token,"false")==0||
	strcmp(token,"null")==0||strcmp(token,"this")==0){
		
		compileExpression(in,out);
	}else if(strcmp(token,";")==0){
		process(";",in,out);
	}

	if(strcmp(token,";")==0){
		process(";",in,out);
	}
	fprintf(out, "</returnStatement>\n");
	printf("</returnStatement>\n");
}

void compileStatements(FILE *in,FILE *out){
	printf("<statements>\n");
	fprintf(out, "<statements>\n");

	//while (strcmp(token, "let") == 0 || strcmp(token, "if") == 0 ||
   // strcmp(token, "while") == 0 || strcmp(token, "do") == 0 ||
    //strcmp(token, "return") == 0) {
		
		while(1){
			if(strcmp(token, "let")==0){
				compileLet(in,out);
			}else if(strcmp(token, "if")==0){
				compileIf(in, out);
			}else if(strcmp(token, "while")==0){
				compileWhile(in, out);
			}else if(strcmp(token, "do")==0){
				compileDo(in, out);
			}else if(strcmp(token, "return")==0){
				compileReturn(in, out);
			}else{
				break;
			}
		}
	//}
	fprintf(out, "</statements>\n");
	printf("</statements\n");
}

void compileParamaterList(FILE *in, FILE *out){
	fprintf(out, "<parameterList>\n");
	printf("<parameterList>\n");
	int x = tokenType(token, stringFlag);
	//type
	if(strcmp(token, "int")==0 || strcmp(token, "char")==0
	|| strcmp(token, "boolean")==0){
		fprintf(out, "<keyword> %s </keyword>\n", token);
	}else if(x == IDENTIFIER){
		fprintf(out, "<identifier> %s </identifier>\n", token);
	}else if(strcmp(token,")")==0){
		fprintf(out, "</parameterList>\n");
		return;
	}	
	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
	    printf("token read: %s\n",token );
	}
	//varName
	x = tokenType(token, stringFlag);
	if(x == IDENTIFIER){
		fprintf(out, "<identifier> %s </identifier>\n", token);
	}
	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    	printf("token read: %s\n",token );
	}
	while(1){
		if(strcmp(token,",")==0){
			process(",",in,out);
			x = tokenType(token, stringFlag);
			if(strcmp(token, "int")==0 || strcmp(token, "char")==0
			|| strcmp(token, "boolean")==0){
				fprintf(out, "<keyword> %s </keyword>\n", token);
			}else if(x == IDENTIFIER){
				fprintf(out, "<identifier> %s </identifier>\n", token);
			}
			if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			printf("token read: %s\n",token );
			}
			//varName
			x = tokenType(token, stringFlag);
			if(x == IDENTIFIER){
				fprintf(out, "<identifier> %s </identifier>\n", token);
			}
			if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    			printf("token read: %s\n",token );
			}
		}else if(strcmp(token,")")==0){
			fprintf(out, "</parameterList>\n");
			return;
		}
	}

	fprintf(out, "</parameterList>\n");
	printf("</parameterList>\n");
}
void compileVarDec(FILE *in, FILE *out){

	fprintf(out, "<varDec>\n");
	printf("<varDec>\n");
	//var
	process("var", in, out);
	
	//type

	int x = tokenType(token, stringFlag);
	if(x == IDENTIFIER){
		fprintf(out, "<identifier> %s </identifier>\n", token);
		

	}else if(strcmp(token, "int")==0||strcmp(token,"char")==0||
		strcmp(token, "boolean")==0){
		fprintf(out, "<keyword> %s </keyword>\n", token);
	}
	//varName
	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    	printf("token read: %s\n",token );
	}
	x = tokenType(token, stringFlag);
	if(x == IDENTIFIER){
		fprintf(out, "<identifier> %s </identifier>\n", token);
		if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
    		printf("token read: %s\n",token );
		}
	}
	
	while(1){
		if(strcmp(token,",")==0){
			process(",", in, out);

			x = tokenType(token, stringFlag);
			if(x == IDENTIFIER){
				fprintf(out, "<identifier> %s </identifier>\n", token);
			}
			if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
			    printf("token read: %s\n",token );
			}
		}else if(strcmp(token,";")==0){
			process(";", in, out);
			break;
		}
	}
	fprintf(out, "</varDec>\n");
	printf("</varDec>\n");
}
void compileClassVarDec(FILE *in, FILE *out, char *token){
	fprintf(out, "<classVarDec>\n");
	printf("<classVarDec>\n");

	int x = tokenType(token, stringFlag);

	if(strcmp(token, "static")==0){
		process("static", in, out);
	}else if(strcmp(token, "field")==0){
		process("field", in, out);
	}

		//type is either keyword(int, char or boolean) or identifier(className)
		x = tokenType(token, stringFlag);
	
		if (x == KEYWORD) {
			fprintf(out, "<keyword> %s </keyword>\n", token);
		}
		else if(x ==  IDENTIFIER){
			fprintf(out, "<identifier> %s </identifier>\n", token);
		}
	
		if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
		    printf("token read: %s\n",token );
		}
		
		while(1){
			x = tokenType(token, stringFlag);
			if(x == IDENTIFIER){
				fprintf(out, "<identifier> %s </identifier>\n", token);
			}

			if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
			    printf("token read: %s\n",token );
			}

			if(token[0] == ';'){
				process(";",in, out);
				break;
			}
			else if(token[0] == ','){
				process(",",in, out);
				continue;
			}
		}	
	fprintf(out, "</classVarDec>\n");
	printf("</classVarDec>\n");
}

//subroutineBody: '{' varDec* statements '}'
void compileSubroutineBody(FILE *in,FILE *out){
	fprintf(out, "<subroutineBody>\n");
	printf("<subroutineBody>\n");
	process("{", in, out);

	while(1){
		if(strcmp(token, "var")==0){
			compileVarDec(in, out);
		}else if(strcmp(token, "let")==0){
			compileStatements(in,out);
		}else if(strcmp(token, "if")==0){
			compileStatements(in, out);
		}else if(strcmp(token, "while")==0){
			compileStatements(in, out);
		}else if(strcmp(token, "do")==0){
			compileStatements(in, out);
		}else if(strcmp(token, "return")==0){
			compileStatements(in, out);
		}else{
			break;
		}
	}
	process("}",in, out);
	fprintf(out, "</subroutineBody>\n");
	printf("</subroutineBody>\n");
}


void compileSubroutineDec(FILE *in, FILE *out){
	fprintf(out, "<subroutineDec>\n");
	printf("<subroutineDec>\n");
	//printf("Calling subroutineDec\n");
	fprintf(out, "<keyword> %s </keyword>\n", token);

	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
	    printf("token read: %s\n",token );
	}
	int x = tokenType(token, stringFlag);
	if(strcmp(token, "void")==0 || strcmp(token, "int")==0 ||
		strcmp(token, "char")==0 || strcmp(token, "boolean")==0){

		fprintf(out, "<keyword> %s </keyword>\n", token);
	}else if(x == IDENTIFIER){
		fprintf(out, "<identifier> %s </identifier>\n", token);
	}

	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
	    printf("token read: %s\n",token );
	}
	x = tokenType(token, stringFlag);
	if(x == IDENTIFIER){
		fprintf(out, "<identifier> %s </identifier>\n", token);
	}
	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
	    printf("token read: %s\n",token );
	}
	
	process( "(" , in, out);
	compileParamaterList(in, out);
	process( ")" , in, out);
	
	compileSubroutineBody(in, out);
		
	fprintf(out, "</subroutineDec>\n");
	printf("</subroutineDec>\n");
}

void compileClass(FILE *in, FILE *out){
	fprintf(out, "<class>\n");
	printf("<class>\n");
	//printf("Calling class()\n");
	while(hasMoreTokens(in)) {
    	advance(in, token, &stringFlag); // prime the first token
    	if (strlen(token) > 0) break;  	
	}
	
	process("class", in, out);
	int x = tokenType(token, stringFlag);
	if(x == IDENTIFIER){
		fprintf(out, "<identifier> %s </identifier>\n", token);
		printf("token read: %s\n",token );
	}
	if (hasMoreTokens(in) && advance(in, token, &stringFlag)) {
	    printf("token read: %s\n",token );
	}
	process("{", in, out);

	while(1){
		if(strcmp(token, "static")==0 || strcmp(token, "field")==0){
			compileClassVarDec(in,out,token);
		}else{
			break;
		}		
	}
	while(1){
		//printf("Calling subroutineDec\n");
		if(strcmp(token, "constructor") == 0 || 
    	strcmp(token,"function") == 0 || 
    	strcmp(token,"method") == 0) {
    		compileSubroutineDec(in, out);
		}else{
			break;
		}
	}	
	process("}", in, out);
	fprintf(out, "</class>\n");
	printf("</class>\n");
}
//============================================================================================================
int main(int argc, char const *argv[])
{
//=============================================================================================================
	//shows how to use the program if argument line is not correctly used
	if(argc != 2){
		printf("Usage: %s filename.jack|<folder_Path>\n", argv[0]);
		return 1;
	}
	
	//if argv[1] is a single .jack file we open the file and output it 
	if((strstr(argv[1],".jack")!= NULL)){

		
		//open input file so it is ready to be read in
		FILE *inputFile = fopen(argv[1], "r");
		if(inputFile == NULL) {
			printf("Error opening file %s\n", argv[1]);
			return 1;
		}

		//create the output file name
		char single_Output_File[280];
		make_output_filename(argv[1], single_Output_File, sizeof(single_Output_File));
		
		//open the output file, name is based off of input name. Example : prog.jack becomes progT.xml
		FILE *outputFile = fopen(single_Output_File, "w");
		if(outputFile == NULL){
			printf("Error opening output file\n");
			return 1;
		} 
	/*Up next is for single file tokenization*/
//=============================================================================================================
		compileClass(inputFile, outputFile);
//=====================================================================================================================
		fclose(inputFile);
		fclose(outputFile);
	}
	else{
		//Step 1: open the folder 
		DIR *folder = opendir(argv[1]);
		if(folder == NULL) {
			perror("Error opening folder\n");
			return 1;
		}

		struct dirent *entry;
		//Step 2: Read files from inside the folder
		while((entry = readdir(folder))!= NULL){
			
			//Skip current and parent folder
			if(strcmp(entry->d_name,".")==0||strcmp(entry->d_name, "..")==0)
				continue;

			//optional: open files only ending in .jack
			const char *ext = strrchr(entry->d_name, '.');
			if (ext && strcmp(ext, ".jack") == 0) {

				//Step 4: Build file path
				char filepath[512];
				snprintf(filepath, sizeof(filepath),"%s/%s", argv[1], entry->d_name);

				//open input file so it is ready to be read in
				FILE *folderFile = fopen(filepath, "r");
				if(folderFile == NULL){
					perror("Error opening folder's file\n");
					continue;
				}

				// create the output file name in the same folder
				char folder_Output_File[512];              // full path
				char output_filename_only[280];            // just the name like FooT.xml
				make_output_filename(entry->d_name, output_filename_only, sizeof(output_filename_only));
				snprintf(folder_Output_File, sizeof(folder_Output_File), "%s/%s", argv[1], output_filename_only);

				//open the output file, name is based off of input name. Example : prog.jack becomes progT.xml
				FILE *F_OutputFile = fopen(folder_Output_File, "w");
				if(F_OutputFile == NULL){
					printf("Error opening output file\n");
					return 1;
				}
	/*Up next is for folder with one or more files tokenization*/
//=======================================================================================================================
				compileClass(folderFile, F_OutputFile);
//================================================================================================================================
				fclose(folderFile);
				fclose(F_OutputFile);
			}
		}
		closedir(folder);
	}

	return 0;
}


