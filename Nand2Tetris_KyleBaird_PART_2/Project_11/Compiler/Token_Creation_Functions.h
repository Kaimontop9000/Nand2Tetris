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


/*hasMoreTokens() skips over whitespace and returns if a valid character is found. 
I think it only skips over the first initial whitespace but because we 
assume that the source code is error free I hope that there is never double space characters,
note, I think this issue was fixed*/
int hasMoreTokens(FILE *input);

void skipBlockComment(FILE *in);

/* token generation/creation : advance() only called if hasMoreTokens() 
returns true. Takes a FILE pointer to read in the next token, 
a char string[] to hold the token, 
maxLen for buffer overflow and 
a global flag that is used to for string constants(this helps differentiate 
between string constants and identifiers because we eliminate the "" from string constants 
Only creates/builds the token but doenst know what type of token has been made. tokenType() can 
read a built token and returns the tokens type*/
int advance(FILE *in, char *string, int *flag);

/*tokenType() returns a constant of either 
KEYWORD, SYMBOL, IDENTIFIER, INT_CONST, STRING_CONST 
as set by define*/
int tokenType(char *token, int flag);

/*if tokenType function returns KEYWORD then
can use this function to take the token and returns the specific keyword type*/
int keyword(char *string);

/*If tokenType function returns SYMBOL then
can use this function to take the token and returns the symbol*/

char symbol(char *string);
/*If tokenType function returns IDENTIFIER then
can use this function to take the token and returns the identifier
as its string*/
char *identifer(char *string);

/*If tokenType function returns INT_CONST then
can use this function to take the token and returns the integer
value as its string*/
char *intVal(char *string);

/*If tokenType function returns STRING_CONST then
can use this function to take the token and returns the integer
value as its string*/
char *stringVal(char *string);