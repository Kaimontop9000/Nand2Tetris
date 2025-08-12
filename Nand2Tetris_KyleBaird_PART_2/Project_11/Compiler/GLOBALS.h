#ifndef GLOBALS_H   // If GLOBALS_H is NOT yet defined
#define GLOBALS_H   // Define GLOBALS_H now

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


extern char token[250];			//used to hold the token 

extern int stringFlag;				//used for string constants, because we remove the 
							//"" from the string, this flag helps us to differentiate
							//between identifiers. Flag is set in advance() and is 
							//tested in the tokenType(), i could take the flag and
							//make it a local variable for the keyword type function. 
							//by copying the string with """ and then by removing them when needed.

#define LEN 250				//used in advance function

#endif // End of include guard
