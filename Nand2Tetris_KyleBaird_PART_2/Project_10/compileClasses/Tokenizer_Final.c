/*Created June 01 2025- Last edited June 24 2025
Jack Tokenizer
Usage - ./Tokenizer_01 <filename.jack/folderName>
----------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

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
							//between identifiers. Flag is set in advance() and is tested in the tokenType()
int len = 250;

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

/* advance() only called if hasMoreTokens() returns true. Takes a FILE pointer to read in the next token, 
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
        		// Block comment: skip until closing */
        		int prev = 0;
        		while ((c = fgetc(in)) != EOF) {
            		if (prev == '*' && c == '/') {
                		break;  // found the end of the comment
            		}
            	prev = c;
        		}
        	continue;  // start over to get next character
   			} 
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
		else if(isspace(c)){
			string[index] = '\0';
			x = 0;
			return 1;

		}
		else if(c == '{' || c == '}' || c == '(' || c == ')'|| c == '[' || c == ']' || c == '.' || c == ',' || c == ';' || c == '+' 
			|| c == '-' || c == '*' || c == '/' || c == '&' || c == '|' || c == '<'|| c == '>' || c == '=' || c == '~'){
			//already building a token
			if(index>0){
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
    if (base_len + 5 >= size) base_len = size - 6;

    strncpy(output_filename, input_filename, base_len);
    output_filename[base_len] = '\0';
    strncat(output_filename, "T.xml", size - base_len - 1);
}

void printXmlToken(int tokenDefinedbyX, FILE *outXML){
	int x;
	if(x == SYMBOL){
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
	else if(x == KEYWORD){				
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
	else if(x == STRING_CONST){
		fprintf(outXML, "<stringConst> %s </stringConst>\n",token);
	}else if(x == INT_CONST){
		fprintf(outXML, "<intConst> %s </intConst>\n",token);
	}else if(x == IDENTIFIER){
		fprintf(outXML, "<identifier> %s </identifier>\n",token);
	}
}	


void process(char *process,  FILE *in,  FILE *out) {
	if(strcmp(token,process)==0) {

		int x = tokenType(token, stringFlag);
		printXmlToken(x, out);
		
		if(hasMoreTokens(in)){
			advance(in, token, &stringFlag);
		}
	}
}
void compileClass(FILE *in, FILE *out){
	fprintf(out, "<class>");
	process("class", in, out);
	


	process("{", in, out);
	process("}", in, out);

	fprintf(out, "</class>");
}

void compileClassVarDec(FILE *out){
	fprintf(out, "<classVarDec>");
	fprintf(out, "</classVarDec>");
}

void compileSubroutineDec(FILE *out){
	fprintf(out, "<subroutineDec>");
	fprintf(out, "</subroutineDec>");
}

void compileParamaterList(FILE *out){
	fprintf(out, "<ParameterList>");
	fprintf(out, "</ParameterList>");
}

void compileSubroutineBody(FILE *out){
	fprintf(out, "<subroutineBody>");
	fprintf(out, "</subroutineBody>");
}

void compileStatements(FILE *out){
	fprintf(out, "<statements>");
	fprintf(out, "</statements>");
}

void compileLet(FILE *out){
	fprintf(out, "<letStatement>");
	fprintf(out, "</letStatement>");
}

void compileIf(FILE *out){
	fprintf(out, "<ifStatement>");
	fprintf(out, "</ifStatement>");
}

void compileWhile(FILE *out){
	fprintf(out, "<whileStatement>");
	fprintf(out, "</whileStatement>");
}

void compileDo(FILE *out){
	fprintf(out, "<doStatement>");
	fprintf(out, "</doStatement>");
}

void compileReturn(FILE *out){
	fprintf(out, "<returnStatement>");
	fprintf(out, "</returnStatement>");
}

void compileExpression(FILE *out){
	fprintf(out, "<expression>");
	fprintf(out, "</expression>");
}

void compileTerm(FILE *out){
	fprintf(out, "<term>");
	fprintf(out, "</term>");
}

void compileExpressionList(FILE *out){
	fprintf(out, "<expressionList>");
	fprintf(out, "</expressionList>");
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
		fprintf(outputFile, "<tokens>\n");
		while(hasMoreTokens(inputFile)){
			
			int x;
			char xmlOut[250];

			advance(inputFile, token, &stringFlag);
			x = tokenType(token, stringFlag);
			if (x == -1) continue; // skip empty or invalid tokens

		}
		fprintf(outputFile, "</tokens>\n");
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
				fprintf(F_OutputFile, "<tokens>\n");
				while(hasMoreTokens(folderFile)){
					
					int x;
					
					char xmlOut[250];
					
					//advance gets the next token and places it in *token
					advance(folderFile, token, &stringFlag);
					//tokenType returns the type of token that is stored in *token and sets x to one of the #define token values
					x = tokenType(token, stringFlag);
					if (x == -1) continue; // skip empty or invalid tokens


				}

				fprintf(F_OutputFile, "</tokens>\n");
//================================================================================================================================
				fclose(folderFile);
				fclose(F_OutputFile);
			}
		}
		closedir(folder);
	}

	return 0;
}


		