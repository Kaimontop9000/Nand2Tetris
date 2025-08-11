/*Created August 11th - Last edited August 11th*/

/*Token Creation Functions
-------------------------------------------------
int hasMoreTokens(FILE *input)
Reads FILE *input, while not EOF,
skips whitespaces until it finds a valid character		
to return

-------------------------------------------------
int advance(FILE *in, char *string, int *flag)	
Only called if hasMoreTokens returns true. Builds 
a token. Skips over comments. Does not keep track 
of the type of token it creates but it will create 
valid token types.There is a flag that is used to 
represent if "string constants" are found. """ are
not stored in the char *string. If there is """ 
then flag is true. 
-------------------------------------------------
int tokenType(char *token, int flag)
Determines the token type. Returns one of five 
constants: 
SYMBOL, KEYWORD, IDENTIFIER,
INT_CONST, STRING_CONST
-------------------------------------------------												
int keyword(char *string)
Returns the type of KEYWORD: 
CLASS, METHOD,FUNCTION,
CONSTRUCTOR, INTE ,BOOLEA, 
CHARA, VOIDS, VAR, STATIC, 
FIELD, LET, DO, IF, ELSE, 
WHILE, RETURN, TRU, FALS,
NUL,THIS
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
/*Symbol Tables
name:(name of identifier)
type:(int, char, boolean or class type)
kind:(field,static,arg or var(local))
#:(index of kind)
category:(class or subroutine)
usage:(declared or used)
proposed XML output
</identifier> name:%s, type:%s, kind:%s, #:%d, category:%s, usage:%s </identifier>*/

/*hasMoreTokens() skips over whitespace and returns if a valid character is found. I think it only skips over the first initial whitespace
but because we assume that the source code is error free I hope that there is never double space characters, note, I think this issue was fixed*/

int hasMoreTokens(FILE *input){
	int c; //Must be int (not char) so it can store all possible character 
		   //values plus the EOF constant, which is typically -1.
	while((c=fgetc(input))!=EOF){ //fgetc returns an int
		if(!isspace(c)){
			ungetc(c, input); //hasMoreTokens is a peek function.
			return 1;
		}				
	}
	return 0;
}

/* token generation/creation : advance() only called if hasMoreTokens() 
returns true. Takes a FILE pointer to read in the next token, 
a char string[] to hold the token, 
maxLen for buffer overflow and 
a global flag that is used to for string constants(this helps differentiate 
between string constants and identifiers because we eliminate the "" from string constants 
Only creates/builds the token but doenst know what type of token has been made. tokenType() can 
read a built token and returns the tokens type*/

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

/*tokenType() returns a constant of either 
KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST 
as set by define*/
int tokenType(char *token, int flag){

	if (token == NULL || *token == '\0') {
    	return -1; // invalid token
	}
	else if(strcmp(token,"{") == 0 || strcmp(token,"}") == 0 || 
			strcmp(token,"(") == 0 || strcmp(token,")") == 0 || 
			strcmp(token, "[") == 0|| strcmp(token, "]") == 0|| 
			strcmp(token, ".") == 0 || strcmp(token, ",") == 0||
			strcmp(token, ";") == 0 || strcmp(token, "+") == 0 || 
			strcmp(token,"-") == 0 || strcmp(token, "*") == 0 ||
			strcmp(token, "/") == 0 || strcmp(token, "&") == 0 ||
			strcmp(token, "|") == 0 || strcmp(token, "<") == 0 || 
			strcmp(token,">") == 0 || strcmp(token,"=") == 0 || 
			strcmp(token,"~") == 0){
		return SYMBOL;	
	} 
	else if(strcmp(token, "class") == 0 || strcmp(token, "constructor") == 0||
			strcmp(token,"function") == 0 ||strcmp(token, "method") == 0 || 
			strcmp(token, "field") == 0 || strcmp(token, "static") == 0 || 
			strcmp(token,"var") == 0 || strcmp(token, "int")== 0 || 
			strcmp(token,"char")== 0 || strcmp(token, "boolean") == 0||
			strcmp(token, "void") == 0|| strcmp(token , "true") == 0 || 
			strcmp(token, "false") == 0 || strcmp(token, "null") == 0 ||
			strcmp(token,"this") == 0|| strcmp(token, "let") == 0 || 
			strcmp(token, "do") == 0 || strcmp(token, "if") == 0 || 
			strcmp(token, "else") == 0 || strcmp(token, "while") == 0 || 
			strcmp(token, "return") == 0){
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
	else{
    	return IDENTIFIER;
	}
}