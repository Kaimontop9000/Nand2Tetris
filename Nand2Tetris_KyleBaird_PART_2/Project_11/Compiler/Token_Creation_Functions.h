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
void skipBlockComment(FILE *in)
used inside of the advance() function. 
Skips comments
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
NUL, THIS

-------------------------------------------------
char symbol(char *string)
Returns string[0] which is one of the symbols
-------------------------------------------------
char *identifer(char *string)
Returns char *string
-------------------------------------------------
char *intVal(char *string)
Returns char *string(integerConstant)
-------------------------------------------------
char *stringVal(char *string)
Returns char *string(stringConstant)
--------------------------------------------------
LEXICAL ELEMENTS: 5 types of terminal elements(tokens)

KEYWORD: 'class'|'constructor'|'function'|'method'|
		 'field'|'static'|'var'|'int'|'char'|
		 'boolean'|'void'|'true'|'false'|'null'|
		 'this'	|'let'	 |'do'	|'if'	|'else' |
		 'while' |'return'
SYMBOL: '{' | '}' | '(' | ')' | '[' |
		']' | '.' | ',' | ';' | '+' |
	    '-' | '*' | '/' | '&' | '|' |	
	    '<' | '>' | '=' | '~'
INTEGERCONSTANT: decimal integer range 0...32767
STRINGCONSTANT: "sequence of characters not including double quotes or newline"
IDENTIFIER: sequence of letters, digits and underscore not starting with a digit.
*/
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

int stringFlag;				//used for string constants, because we remove the 
							//"" from the string, this flag helps us to differentiate
							//between identifiers. Flag is set in advance() and is 
							//tested in the tokenType(), i could take the flag and
							//make it a local variable for the keyword type function. 
							//by copying the string with """ and then by removing them when needed.

#define LEN 250				//used in advance function


/*hasMoreTokens() skips over whitespace and returns if a valid character is found. 
I think it only skips over the first initial whitespace but because we 
assume that the source code is error free I hope that there is never double space characters,
note, I think this issue was fixed*/

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
            	if (index < LEN - 1) {
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
        		if (index < LEN - 1) {
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
        		if (index < LEN - 1) {
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
/*if tokenType function returns KEYWORD then
can use this function to take the token and returns the specific keyword type*/
int keyword(char *string){
	if(strcmp(string, "class")==0){
		return CLASS;
	}else if(strcmp(string, "method")==0){
		return METHOD;
	}else if(strcmp(string, "function")==0){
		return FUNCTION;
	}else if(strcmp(string, "constructor")==0){
		return CONSTRUCTOR;
	}else if(strcmp(string, "int")==0){
		return INTE;
	}else if(strcmp(string, "boolean")==0){
		return BOOLEA;
	}else if(strcmp(string, "char")==0){
		return CHARA;
	}else if(strcmp(string, "void")==0){
		return VOIDS;
	}else if(strcmp(string, "var")==0){
		return VAR;
	}else if(strcmp(string, "static")==0){
		return STATIC;
	}else if(strcmp(string, "field")==0){
		return FIELD;
	}else if(strcmp(string, "let")==0){
		return LET;
	}else if(strcmp(string, "do")==0){
		return DO;
	}else if(strcmp(string, "if")==0){
		return IF;
	}else if(strcmp(string, "else")==0){
		return ELSE;
	}else if(strcmp(string, "while")==0){
		return WHILE;
	}else if(strcmp(string, "return")==0){
		return RETURN;
	}else if(strcmp(string, "true")==0){
		return TRU;
	}else if(strcmp(string, "false")==0){
		return FALS;
	}else if(strcmp(string, "null")==0){
		return NUL;
	}else if(strcmp(string, "this")==0){
		return THIS;
	}else{
		return -1;
	}
}
/*If tokenType function returns SYMBOL then
can use this function to take the token and returns the symbol*/
char symbol(char *string){
	return string[0];
}

/*If tokenType function returns IDENTIFIER then
can use this function to take the token and returns the identifier
as its string*/
char *identifer(char *string){
	return string;
}

/*If tokenType function returns INT_CONST then
can use this function to take the token and returns the integer
value as its string*/
char *intVal(char *string){
	return string;
}
/*If tokenType function returns STRING_CONST then
can use this function to take the token and returns the integer
value as its string*/
char *stringVal(char *string){
	return string;
}
