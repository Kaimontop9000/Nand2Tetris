//======================================================================================|
//Kyle Baird. Created : 23/04/2025 Last edited : 16/05/2025								|
//VMTranslator that takes .vm and parses and translates into .asm 						|
//To use in command line execute the follwoing - VMTranslator <Prog.vm> <Prog.asm>		|
//======================================================================================|
//--------------------------------------------------------------------------------------|
/*Parser																				|
//--------------------------------------------------------------------------------------|
|Routine		|Arguements		|Returns		|Function								|
----------------------------------------------------------------------------------------|
|Constructor/   |Input file/    |No return      |Opens the input file stream, and 		|
|Initialiser	|stream			|value	        |gets ready to parse it 				|
|---------------------------------------------------------------------------------------|
|hasMoreLines   |No arguements  |boolean        |Are there more lines in the input?		|
|---------------------------------------------------------------------------------------|
|advance        |None           |None           |Reads the next command from the 		|
|               |               |               |input and makes it the current 		|
|      			|				|				|commnand. This routine should be 		|
|               |  				|				|called only of hasMoreLine is true. 	|
|				|				|				|Initially there is no current command 	|
----------------------------------------------------------------------------------------|
|CommandType    |None			|C_ARITHMETIC,	|Returns a constant representing the 	|
|				|				|C_PUSH,C_POP,  |type of the current command.If 		|
|				|				|C_LABEL,C_GOTO,|current command is arithmetic-logical 	|
|				|				|C_IF,C_Function|command, returns C_ARITHMETIC 			|
|				|				|,C_RETURN,		| 										|
|				|				|C_CALL(constant| 										|
----------------------------------------------------------------------------------------|
|arg1			|None			|String			|Returns the first arguement of the  	|
|				|				|				|current command. In the case of 		|
|				|				|				|C_ARITHMETIC, the command itself is 	|
|				|				|				|returned. Should not be called if the 	|
|				|				|				|current command is C_RETURN 			|
----------------------------------------------------------------------------------------|
|arg2  			|None			|Int 			|Returns the second arguement of the    |
|				|				|				|current command. Should be called		|
|				|				|				|only if the current command is a  		|
|				|				|				|C_PUSH,C_POP,C_FUNCTION or C_CALL 		|
----------------------------------------------------------------------------------------|
|																						|
|																						|
//--------------------------------------------------------------------------------------|
CodeWriter 																				|
//--------------------------------------------------------------------------------------|
|Routine 		|Arguements		|Returns		|Function 								|
----------------------------------------------------------------------------------------|
|Constructor/	|Output file/	|None			|Opens an output file/stream and 		|
|Initialiser	|stream			|				|gets ready to write into it 			|
----------------------------------------------------------------------------------------|
|writeArithmetic|command(string)|None			|Writes to the output file, the assembly|
|				|				|				|code that implements the given  		|
|				|				|				|arithmetic-logical commands 			|
----------------------------------------------------------------------------------------|
|writePushPop   |command(C_Push |None			|Writes to the output file the assembly |
|				|/C_Pop),segment|				|code that implements the given push 	|
|				|(string),index |				|or pop command 						|
|				|(int)			|				| 										|
----------------------------------------------------------------------------------------|
|writeLabel		|label(string)	|None			|Writes assembly code that effects the 	|
|				|				| 				|label command 							|
----------------------------------------------------------------------------------------|
|writeGoto		|label(string)	|None			|Writes assembly code that effects the  |
|				|				|				|goto command 							|
----------------------------------------------------------------------------------------|
|writeIf		|label(string)	|None			|Writes assembly code that effects the 	|
|				|				|				|if-goto command 						|
----------------------------------------------------------------------------------------|
|writeFunction	|functionName(	|None 			|Writes assembly code that effects		|
|				|string),nArgs	|				|the function command 					|
|				|(int)			|				|										|
|---------------|---------------|---------------|---------------------------------------|
|writeCall      |functionName	|None			|Writes assembly code that effects the  |
|				|(string),nArgs |				|call command							|
|				|(int)			|				|										|
----------------------------------------------------------------------------------------|
|writeReturn	|None 			|None 			|Writes assembly code that effects the  |
|				|				|				|function return command 				|
----------------------------------------------------------------------------------------|
|close			|None			|None			|Close the output file/stream			|
----------------------------------------------------------------------------------------|*/ 
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#define BUFFER_SIZE 100//USED TO HOLD THE BUFFER FROM FILE INPUT TO FILE OUTPUT
char line[BUFFER_SIZE];	//THE INITIAL LINE FROM FILE INPUT IS READ INTO THIS LINE
char current_command[BUFFER_SIZE]; //THE INITIAL LINE IS READ INTO THIS LINE
char arg1_string[BUFFER_SIZE]; //used in the arg1 function
char arg2_value[BUFFER_SIZE];					//used in the arg2 function
int arg2_int;
char currentFileName[BUFFER_SIZE];
char currentFunction[BUFFER_SIZE] = "";

#define C_ARITHMETIC 0 //arithmetic/logical
#define C_PUSH 1
#define C_POP 2
#define C_LABEL 3	//branching command 
#define C_GOTO 4	//branching command 
#define C_IF 5		//branching command
#define C_FUNCTION 6//function command
#define C_RETURN 7	//function command
#define C_CALL 8 	//function command
#define INVALID_COMMAND 9

FILE *input_file;
FILE *output_file;
char addText[]=	"@SP\n"
				"M=M-1\n"
				"A=M\n"
				"D=M\n"
				"@SP\n"
				"M=M-1\n"
				"A=M\n"
				"D=D+M\n"
				"@SP\n"
				"A=M\n"
				"M=D\n"
				"@SP\n"
				"M=M+1\n";

char subText[]="@SP\n"
				"M=M-1\n"
				"A=M\n"
				"D=M\n"
				"@SP\n"
				"M=M-1\n"
				"A=M\n"
				"D=M-D\n"
				"@SP\n"
				"A=M\n"
				"M=D\n"
				"@SP\n"
				"M=M+1\n";

char negText[]="@SP\n"
				"M=M-1\n"
				"A=M\n"
				"M=-M\n"
				"@SP\n"
				"M=M+1\n";

int labelCount = 0;		//used in logical comparisons to create unique addresses
int retAddrNum = 0;		//used in writeCall function to create unique return addresses

void writeEq(FILE* out) {
    fprintf(out,
        "@SP\n"
        "M=M-1\n"
        "A=M\n"
        "D=M\n"
        "@SP\n"
        "M=M-1\n"
        "A=M\n"
        "D=M-D\n"
        "@EQ_TRUE%d\n"
        "D;JEQ\n"
        "@SP\n"
        "A=M\n"
        "M=0\n"
        "@EQ_CONTINUE%d\n"
        "0;JMP\n"
        "(EQ_TRUE%d)\n"
        "@SP\n"
        "A=M\n"
        "M=-1\n"
        "(EQ_CONTINUE%d)\n"
        "@SP\n"
        "M=M+1\n",
        labelCount, labelCount, labelCount, labelCount);
    labelCount++;
}

void writeGt(FILE* out) {
    fprintf(out,
        "@SP\n"
        "M=M-1\n"
        "A=M\n"
        "D=M\n"
        "@SP\n"
        "M=M-1\n"
        "A=M\n"
        "D=M-D\n"
        "@GT_TRUE%d\n"
        "D;JGT\n"
        "@SP\n"
        "A=M\n"
        "M=0\n"
        "@GT_CONTINUE%d\n"
        "0;JMP\n"
        "(GT_TRUE%d)\n"
        "@SP\n"
        "A=M\n"
        "M=-1\n"
        "(GT_CONTINUE%d)\n"
        "@SP\n"
        "M=M+1\n",
        labelCount, labelCount, labelCount, labelCount);
    labelCount++;
}

void writeLt(FILE* out) {
    fprintf(out,
        "@SP\n"
        "M=M-1\n"
        "A=M\n"
        "D=M\n"
        "@SP\n"
        "M=M-1\n"
        "A=M\n"
        "D=M-D\n"
        "@LT_TRUE%d\n"
        "D;JLT\n"
        "@SP\n"
        "A=M\n"
        "M=0\n"
        "@LT_CONTINUE%d\n"
        "0;JMP\n"
        "(LT_TRUE%d)\n"
        "@SP\n"
        "A=M\n"
        "M=-1\n"
        "(LT_CONTINUE%d)\n"
        "@SP\n"
        "M=M+1\n",
        labelCount, labelCount, labelCount, labelCount);
    labelCount++;
}

char andText[]= "@SP\n"
				"M=M-1\n"
				"A=M\n"
				"D=M\n"
				"@SP\n"
				"M=M-1\n"
				"A=M\n"
				"M=D&M\n"
				"@SP\n"
				"M=M+1\n";

char orText[]= 	"@SP\n"
				"M=M-1\n"
				"A=M\n"
				"D=M\n"
				"@SP\n"
				"M=M-1\n"
				"A=M\n"
				"M=D|M\n"
				"@SP\n"
				"M=M+1\n";

char notText[]= "@SP\n"
				"M=M-1\n"
				"A=M\n"
				"M=!M\n"
				"@SP\n"
				"M=M+1\n";

char pushText[256]; 

char popText[256];

char ifText[BUFFER_SIZE];

char gotoText[BUFFER_SIZE];

char functionText[BUFFER_SIZE];


///////////////////////////////////////////////////////////////////////////////
//PARSER-PARSER-PARSER-PARSER-PARSER-PARSER-PARSER-PARSER-PARSER-PARSER-PARSER-
///////////////////////////////////////////////////////////////////////////////
							
//-----------------------------------------------------------------------------
//Parser Routine-Constructor/Initialiser
//-----------------------------------------------------------------------------
/*void initialiser(const char *input_filename, const char *output_filename){
	FILE *input_file = fopen(input_filename, "r");
	if(input_file == NULL){
		printf("Error reading file\n");
		return;
	}
	FILE *output_file = fopen(output_filename,"w");
	if(output_file == NULL){
		printf("Error writing file");
		return;
	}
}*/
void removeComment(char *str) {
    char *commentPos = strstr(str, "//");
    if (commentPos) {
        *commentPos = '\0';
    }
}
void removeWhitespace(char *str) {
    int i = 0, j = 0;
    while (str[i]) {
        if (!isspace((unsigned char)str[i])) {
            str[j++] = str[i];
        }
        i++;
    }
    str[j] = '\0';
}
//-----------------------------------------------------------------------------
//Parser Routine-hasMoreLines
//-----------------------------------------------------------------------------
int hasMoreLines(FILE *input_filename, char line_name[]){
	while(fgets(line_name, BUFFER_SIZE, input_filename)) {
		removeComment(line_name);
        
        if (strlen(line_name) == 0) {
        // empty line or comment-only, skip
        continue;
        }
	
		line[strcspn(line, "\r\n")] = '\0';
            return 1;
	}
	return 0;
}
//------------------------------------------------------------------------------
//Parser routine-advance
void advance(char line_name[],char current_command_line[]){
	current_command_line[0] = '\0';
	strcpy(current_command_line, line_name);{
	current_command_line[strcspn(current_command_line, "\n")] = 0;
	current_command_line[strcspn(current_command_line, "\r")] = 0;
	}
}
//-------------------------------------------------------------------------------
//Parser Routine-CommandType
int commandType(char *current_command_line) {
    // Make a local copy because strtok modifies the string
    char buf[BUFFER_SIZE];
    strncpy(buf, current_command_line, BUFFER_SIZE);
    buf[BUFFER_SIZE-1] = '\0';

    // Extract the first token (up to first space)
    char *tok = strtok(buf, " ");
    if (!tok) {
        fprintf(stderr, "  → NO TOKENS\n");
        return INVALID_COMMAND;
    }
    //fprintf(stderr, "  → first token = \"%s\"\n", tok);

    if (strcmp(tok, "add") == 0
     || strcmp(tok, "sub") == 0
     || strcmp(tok, "neg") == 0
     || strcmp(tok, "eq")  == 0
     || strcmp(tok, "gt")  == 0
     || strcmp(tok, "lt")  == 0
     || strcmp(tok, "and") == 0
     || strcmp(tok, "or")  == 0
     || strcmp(tok, "not") == 0) {
        return C_ARITHMETIC;
    }
    else if (strcmp(tok, "push")    == 0) return C_PUSH;
    else if (strcmp(tok, "pop")     == 0) return C_POP;
    else if (strcmp(tok, "label")   == 0) return C_LABEL;
    else if (strcmp(tok, "goto")    == 0) return C_GOTO;
    else if (strcmp(tok, "if-goto") == 0) return C_IF;
    else if (strcmp(tok, "function")== 0) return C_FUNCTION;
    else if (strcmp(tok, "return")  == 0) return C_RETURN;
    else if (strcmp(tok, "call")    == 0) return C_CALL;
    else                                  return INVALID_COMMAND;
}
//---------------------------------------------------------------------------
//Parser Routine-arg1
//----------------------------------------------------------------------------
char *arg1(char current_command_line[], char arg1[]){
arg1[0]='\0';
if(strcmp(current_command_line,"add") == 0 || strcmp(current_command_line,"sub")==0
	||strcmp(current_command_line,"neg")==0 || strcmp(current_command_line,"eq")==0
	||strcmp(current_command_line,"gt")==0||strcmp(current_command_line,"lt")==0
	||strcmp(current_command_line,"and")==0|| strcmp(current_command_line,"or")==0
	||strcmp(current_command_line,"not")==0){
	strcpy(arg1,current_command_line);
	return arg1;
	}else if(strstr(current_command_line,"constant")){
		strcpy(arg1,"constant");
		return arg1;
	}else if(strstr(current_command_line,"static")){
		strcpy(arg1,"static");
		return arg1;
	}else if(strstr(current_command_line,"local")){
		strcpy(arg1,"local");
		return arg1;
	}else if(strstr(current_command_line,"argument")){
		strcpy(arg1,"argument");
		return arg1;
	}else if(strstr(current_command_line,"this")){
		strcpy(arg1,"this");
		return arg1;
	}else if(strstr(current_command_line,"that")){
		strcpy(arg1,"that");
		return arg1;
	}else if(strstr(current_command_line,"temp")){
		strcpy(arg1,"temp");
		return arg1;
	}else if(strstr(current_command_line,"pointer")){
		strcpy(arg1,"pointer");
		return arg1;
	}
}//----------------------------------------------------------
//Parser Routine-arg2
//-----------------------------------------------------------
int arg2(char current_command_line[],char arg2_v[],int arg2_integer){
/*int i;
int j = 0;
for(i = 0; i<BUFFER_SIZE; i++) {
	if(isdigit(current_command_line[i])){
		arg2_v[j++] = current_command_line[i];
	}
	arg2_v[j]='\0';
}
arg2_integer = atoi(arg2_v);
return arg2_integer;
}*/ int i = 0, j = 0;
while (current_command_line[i]) {
    if (isdigit(current_command_line[i])) {
        arg2_v[j++] = current_command_line[i];
    }
    i++;
}
arg2_v[j] = '\0';
return atoi(arg2_v);
}

// Call this immediately after advance(), before commandType() or dispatch
void trimLine(char *s) {
    char *start = s;
    // Skip leading whitespace
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != s) {
        memmove(s, start, strlen(start) + 1);
    }
    // Trim trailing whitespace
    char *end = s + strlen(s) - 1;
    while (end >= s && isspace((unsigned char)*end)) {
        *end = '\0';
        end--;
    }
}
/////////////////////////////////////////////////////////////////////////
//CODE-WRITER-CODE-WRITER-CODE-WRITER-CODE-WRITER-CODE-WRITER-CODE-WRITER-
/////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------
//CodeWriter-setFileName
//-----------------------------------------------------------------------
void setFileName(const char *file_path) {
    const char *lastSlash = strrchr(file_path, '/');
    const char *lastBackslash = strrchr(file_path, '\\');
    const char *fileNameStart = lastSlash ? lastSlash + 1 :
                              (lastBackslash ? lastBackslash + 1 : file_path);
    
    char fileName[100];
    strcpy(fileName, fileNameStart);

    // Remove extension
    char *dot = strrchr(fileName, '.');
    if (dot != NULL) {
        *dot = '\0';
    }

    strcpy(currentFileName, fileName);
}
//------------------------------------------------------------------------
//CodeWrite- endsWithVM
//------------------------------------------------------------------------
int endsWithVM(const char *filename) {
    size_t len = strlen(filename);
    if (len < 3) return 0;
    return strcmp(filename + len - 3, ".vm") == 0;
}
//-----------------------------------------------------------------------
//CodeWriter writeArithmetic
//-----------------------------------------------------------------------
void writeArithmetic(char command[]){
	if(strcmp(command,"add") == 0){
		fprintf(output_file, "%s",addText);
	}else if(strcmp(command,"sub")==0){
		fprintf(output_file,"%s", subText);
	}else if(strcmp(command,"neg")==0){
		fprintf(output_file,"%s",negText);
	} else if (strcmp(command, "eq") == 0) {
        writeEq(output_file);
    } else if (strcmp(command, "gt") == 0) {
        writeGt(output_file);
    } else if (strcmp(command, "lt") == 0) {
        writeLt(output_file);
	}else if(strcmp(command,"and")==0){
		fprintf(output_file,"%s",andText);
	}else if(strcmp(command,"or")==0){
		fprintf(output_file,"%s",orText);
	}else if(strcmp(command,"not")==0){
		fprintf(output_file,"%s",notText);
	}
}
//------------------------------------------------------------------------
//CodeWriter-writePushPop
//------------------------------------------------------------------------
void writePushPop(int command, char argue1[], char argue2[])
{
	if(command == C_PUSH && (strcmp(argue1,"this") == 0)){
		pushText[0] = '\0';
		sprintf(pushText,"@%s\n",argue2);
		strcat(pushText,"D=A\n");
		strcat(pushText,"@THIS\n");
		strcat(pushText,"A=M\n");
		strcat(pushText,"A=D+A\n");
		strcat(pushText,"D=M\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"A=M\n");
		strcat(pushText,"M=D\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"M=M+1\n");		
		

		fprintf(output_file,"%s", pushText); //should be correct

	}else if(command == C_PUSH && (strcmp(argue1,"local") == 0)){
		pushText[0] = '\0';

		sprintf(pushText,"@%s\n",argue2);
		strcat(pushText,"D=A\n");
		strcat(pushText,"@LCL\n");
		strcat(pushText,"A=M\n");
		strcat(pushText,"A=D+A\n");
		strcat(pushText,"D=M\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"A=M\n");
		strcat(pushText,"M=D\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"M=M+1\n");		

		fprintf(output_file,"%s", pushText);


	}else if(command == C_PUSH && (strcmp(argue1,"argument") == 0)){
		pushText[0] = '\0';

		sprintf(pushText,"@%s\n",argue2);
		strcat(pushText,"D=A\n");
		strcat(pushText,"@ARG\n");
		strcat(pushText,"A=M\n");
		strcat(pushText,"A=D+A\n");
		strcat(pushText,"D=M\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"A=M\n");
		strcat(pushText,"M=D\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"M=M+1\n");		

		fprintf(output_file,"%s", pushText);

	}else if(command == C_PUSH && (strcmp(argue1,"static") == 0)){
		pushText[0] = '\0';

	 // Format: @FileName.index
  	 sprintf(pushText, "@%s.%s\n", currentFileName, argue2);
  	 strcat(pushText, "D=M\n");
   	 strcat(pushText, "@SP\n");
   	 strcat(pushText, "A=M\n");
   	 strcat(pushText, "M=D\n");
     strcat(pushText, "@SP\n");
   	 strcat(pushText, "M=M+1\n");

    fprintf(output_file, "%s", pushText);

	}else if(command == C_PUSH && (strcmp(argue1,"temp") == 0)){
		pushText[0] = '\0';

		sprintf(pushText,"@%s\n",argue2);
		strcat(pushText,"D=A\n");
		strcat(pushText,"@5\n");
		strcat(pushText,"A=D+A\n");
		strcat(pushText,"D=M\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"A=M\n");
		strcat(pushText,"M=D\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"M=M+1\n");		

		fprintf(output_file,"%s", pushText);

	}else if(command == C_PUSH && (strcmp(argue1,"that") == 0)){
		pushText[0] = '\0';

		sprintf(pushText,"@%s\n",argue2);
		strcat(pushText,"D=A\n");
		strcat(pushText,"@THAT\n");
		strcat(pushText,"A=M\n");
		strcat(pushText,"A=D+A\n");
		strcat(pushText,"D=M\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"A=M\n");
		strcat(pushText,"M=D\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"M=M+1\n");		

		fprintf(output_file,"%s", pushText);

	}else if(command == C_PUSH && strcmp(argue1,"pointer") == 0 && (strcmp(argue2,"0") == 0)){
		pushText[0] = '\0';
		sprintf(pushText, "@3\n");
		strcat(pushText,"D=M\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"A=M\n");
		strcat(pushText,"M=D\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"M=M+1\n");		
		fprintf(output_file,"%s", pushText); 

	}else if(command == C_PUSH && (strcmp(argue1,"pointer") == 0) && (strcmp(argue2,"1") == 0)){
		pushText[0] = '\0';
		sprintf(pushText, "@4\n");
		strcat(pushText,"D=M\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"A=M\n");
		strcat(pushText,"M=D\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"M=M+1\n");		
		fprintf(output_file,"%s", pushText); //i think its ok
	
	}else if(command == C_POP && strcmp(argue1,"pointer") == 0 && (strcmp(argue2,"0") == 0)){
		popText[0] = '\0';
		strcat(popText,"@SP\n");
		strcat(popText,"M=M-1\n");
		strcat(popText,"A=M\n");
		strcat(popText,"D=M\n");
		strcat(popText,"@3\n");
		strcat(popText,"M=D\n");
	
		fprintf(output_file,"%s", popText);

	}else if(command == C_POP && (strcmp(argue1,"pointer") == 0 && (strcmp(argue2,"1") == 0))){
		popText[0] = '\0';
		strcat(popText,"@SP\n");
		strcat(popText,"M=M-1\n");
		strcat(popText,"A=M\n");
		strcat(popText,"D=M\n");
		strcat(popText,"@4\n");
		strcat(popText,"M=D\n");
	

		fprintf(output_file,"%s", popText);

	}else if(command == C_POP && strcmp(argue1, "this") == 0){
    	popText[0] = '\0';

   		 strcat(popText, "@");
   		 strcat(popText, argue2);       
	    strcat(popText, "\nD=A\n");

	    strcat(popText, "@THIS\n");       
	    strcat(popText, "A=M\n");
	    strcat(popText, "D=D+A\n");     

	    strcat(popText, "@R13\n");
	    strcat(popText, "M=D\n");       

	    strcat(popText, "@SP\n");
	    strcat(popText, "M=M-1\n");
	    strcat(popText, "A=M\n");
	    strcat(popText, "D=M\n");       

	    strcat(popText, "@R13\n");
	    strcat(popText, "A=M\n");
	    strcat(popText, "M=D\n");	

	    strcat(popText, "@R13\n");
		strcat(popText, "M=0\n");


	    fprintf(output_file, "%s", popText);

	}else if(command == C_POP && strcmp(argue1, "local") == 0){
    	popText[0] = '\0';

   		 strcat(popText, "@");
   		 strcat(popText, argue2);       
	    strcat(popText, "\nD=A\n");

	    strcat(popText, "@LCL\n");       
	    strcat(popText, "A=M\n");
	    strcat(popText, "D=D+A\n");     

	    strcat(popText, "@R13\n");
	    strcat(popText, "M=D\n");       

	    strcat(popText, "@SP\n");
	    strcat(popText, "M=M-1\n");
	    strcat(popText, "A=M\n");
	    strcat(popText, "D=M\n");       

	    strcat(popText, "@R13\n");
	    strcat(popText, "A=M\n");
	    strcat(popText, "M=D\n");	

	    strcat(popText, "@R13\n");
		strcat(popText, "M=0\n");


	    fprintf(output_file, "%s", popText);

	}else if(command == C_POP && strcmp(argue1, "static") == 0){
    	popText[0] = '\0'; 

    	char symbol[128];   // For FileName.index (e.g., Foo.3)

   		// Build the static label: e.g., Foo.3
   		snprintf(symbol, sizeof(symbol), "%s.%s", currentFileName, argue2);

   		// Use symbol to compute the address and store value
  		sprintf(popText, "@%s\n", symbol);       // Load the address symbol (Foo.3)
   		strcat(popText, "D=A\n");
   		strcat(popText, "@STATIC\n");            // STATIC base (points to beginning of static segment)
       	strcat(popText, "A=M\n");
    	strcat(popText, "D=D+A\n");              // Compute full address

    	strcat(popText, "@R13\n");
    	strcat(popText, "M=D\n");                // Store address in R13

	    strcat(popText, "@SP\n");
    	strcat(popText, "M=M-1\n");
	    strcat(popText, "A=M\n");
    	strcat(popText, "D=M\n");                // Get value from top of stack

    	strcat(popText, "@R13\n");
    	strcat(popText, "A=M\n");
    	strcat(popText, "M=D\n");                // Store value at static address

    	strcat(popText, "@R13\n");	
    	strcat(popText, "M=0\n");                // Clear R13 (optional cleanup)

    	fprintf(output_file, "%s", popText);

	}else if(command == C_POP && strcmp(argue1, "argument") == 0){
    	popText[0] = '\0';

   		strcat(popText, "@");
   		strcat(popText, argue2);       
	    strcat(popText, "\nD=A\n");

	    strcat(popText, "@ARG\n");       
	    strcat(popText, "A=M\n");
	    strcat(popText, "D=D+A\n");     

	    strcat(popText, "@R13\n");
	    strcat(popText, "M=D\n");       

	    strcat(popText, "@SP\n");
	    strcat(popText, "M=M-1\n");
	    strcat(popText, "A=M\n");
	    strcat(popText, "D=M\n");       

	    strcat(popText, "@R13\n");
	    strcat(popText, "A=M\n");
	    strcat(popText, "M=D\n");	

	    strcat(popText, "@R13\n");
		strcat(popText, "M=0\n");


	    fprintf(output_file, "%s", popText);

	}else if(command == C_POP && strcmp(argue1, "temp") == 0){
    	popText[0] = '\0';

   		strcat(popText, "@");
   		strcat(popText, argue2);       
	    strcat(popText, "\nD=A\n");

	    strcat(popText, "@5\n");       
	    strcat(popText, "D=D+A\n");     

	    strcat(popText, "@R13\n");
	    strcat(popText, "M=D\n");       

	    strcat(popText, "@SP\n");
	    strcat(popText, "M=M-1\n");
	    strcat(popText, "A=M\n");
	    strcat(popText, "D=M\n");       

	    strcat(popText, "@R13\n");
	    strcat(popText, "A=M\n");
	    strcat(popText, "M=D\n");	

	    strcat(popText, "@R13\n");
		strcat(popText, "M=0\n");


	    fprintf(output_file, "%s", popText);

	}else if(command == C_POP && strcmp(argue1, "that") == 0){
    	popText[0] = '\0';

   		strcat(popText, "@");
   		strcat(popText, argue2);       
	    strcat(popText, "\nD=A\n");

	    strcat(popText, "@THAT\n");       
	    strcat(popText, "A=M\n");
	    strcat(popText, "D=D+A\n");     

	    strcat(popText, "@R13\n");
	    strcat(popText, "M=D\n");       

	    strcat(popText, "@SP\n");
	    strcat(popText, "M=M-1\n");
	    strcat(popText, "A=M\n");
	    strcat(popText, "D=M\n");       

	    strcat(popText, "@R13\n");
	    strcat(popText, "A=M\n");
	    strcat(popText, "M=D\n");	

	    strcat(popText, "@R13\n");
		strcat(popText, "M=0\n");


	    fprintf(output_file, "%s", popText);
	}
	else if(command == C_POP){
		popText[0] = '\0';
		char temp[BUFFER_SIZE];
		strcat(popText,"@SP\n");
		strcat(popText,"M=M-1\n");
		sprintf(temp,"@%s\n", argue2);
		strcat(popText,temp);
		strcat(popText,"D=A\n");
		temp[0] = '\0';
		sprintf(temp,"@%s\n", argue1);
		strcat(popText,temp);
		strcat(popText,"D=D+A\n");	
		strcat(popText,"@R13\n");
		strcat(popText,"M=D\n");
		strcat(popText,"@SP\n");
		strcat(popText,"A=M\n");
		strcat(popText,"D=M\n");
		strcat(popText,"@R13\n");
		strcat(popText,"A=M\n");
		strcat(popText,"M=D\n");
		fprintf(output_file,"%s", popText);
	}
	if (command == C_PUSH && (strcmp(argue1,"constant") == 0)){
		pushText[0] = '\0';
		sprintf(pushText, "@%s\n", argue2);
		strcat(pushText,"D=A\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"A=M\n");
		strcat(pushText,"M=D\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"M=M+1\n");		
		fprintf(output_file,"%s", pushText); //should be correct for push constant 
	}
}
//--------------------------------------------------------------------------------------------
//CodeWriter-writeLabel	
//--------------------------------------------------------------------------------------------
void writeLabel(char command[]) {
    // skip “label ” (6 chars)
    char *label = command + 6;
    while (*label == ' ') label++;        // trim leading spaces
    char *end = label + strlen(label)-1;
    while (end > label && isspace((unsigned char)*end)) *end-- = '\0';

    // build scoped name: currentFunction$label
    char uniqueLabel[256];
    snprintf(uniqueLabel, sizeof(uniqueLabel), "%s$%s", currentFunction, label);

    fprintf(output_file, "(%s)\n", uniqueLabel);
}

//---------------------------------------------------------------------------------------------
//CodeWriter- writeIf
//---------------------------------------------------------------------------------------------
void writeIf(char command[]) {
    // skip “if-goto ” (8 chars)
    char *label = command + 8;
    while (*label == ' ') label++;
    char *end = label + strlen(label)-1;
    while (end > label && isspace((unsigned char)*end)) *end-- = '\0';

    char target[256];
    snprintf(target, sizeof(target), "%s$%s", currentFunction, label);
    fprintf(output_file,
        "@SP\n"
        "M=M-1\n"
        "A=M\n"
        "D=M\n"
        "@%s\n"
        "D;JNE\n", target);
}
//--------------------------------------------------------------------------------------------
//CodeWriter - writeGoto
//--------------------------------------------------------------------------------------------
void writeGoto(char command[]) {
    // skip “goto ” (5 chars)
    char *label = command + 5;
    while (*label == ' ') label++;
    char *end = label + strlen(label)-1;
    while (end > label && isspace((unsigned char)*end)) *end-- = '\0';

    char target[256];
    snprintf(target, sizeof(target), "%s$%s", currentFunction, label);
    fprintf(output_file, "@%s\n0;JMP\n", target);
}//---------------------------------------------------------------------------------------------
//CodeWriter-writeFunction
//---------------------------------------------------------------------------------------------
void writeFunction(char command[]) {
    char functionName[BUFFER_SIZE];
    int nVars;
	//printf(" writeFunction got: '%s'\n", command);
	//fflush(stdout);
    // Parse command like: "function Sys.init 0"
    if (sscanf(command, "function %s %d", functionName, &nVars) != 2) {
      fprintf(stderr, "Invalid function command format: %s\n", command);
       return;
    }

    // update the global “currentFunction” so label/goto/if-goto get scoped
    strncpy(currentFunction, functionName, BUFFER_SIZE-1);
    currentFunction[BUFFER_SIZE-1] = '\0';

    // Write function label
    fprintf(output_file, "(%s)\n", functionName);

    // Initialize local variables to 0
    for (int i = 0; i < nVars; i++) {
        fprintf(output_file,
            "@0\n"
            "D=A\n"
            "@SP\n"
            "A=M\n"
            "M=D\n"
            "@SP\n"
            "M=M+1\n"
        );
    }
}
//--------------------------------------------------------------------------------------------
//CodeWriter-writeCall
//---------------------------------------------------------------------------------------------
void writeCall(char string[], int nArgs)
{
	char callText[256];
	callText[0] = '\0';
	char numStr[12];  // Enough to hold integer as string

	strcat(callText, "@");               //PUSH RETURN ADDRESS
	strcat(callText, "returnAddress");  
	sprintf(numStr, "%d", retAddrNum++); 
	strcat(callText, numStr);           
	strcat(callText, "\n");
	strcat(callText,"D=A\n");
	strcat(callText,"@SP\n");
	strcat(callText,"A=M\n");
	strcat(callText,"M=D\n");
	strcat(callText,"@SP\n");
	strcat(callText,"M=M+1\n");

	strcat(callText,"@LCL\n");			//PUSH LOCAL
	strcat(callText,"D=M\n");	
	strcat(callText,"@SP\n");
	strcat(callText,"A=M\n");
	strcat(callText,"M=D\n");
	strcat(callText,"@SP\n");
	strcat(callText,"M=M+1\n");

	strcat(callText,"@ARG\n");			//PUSH ARGUMENT
	strcat(callText,"D=M\n");	
	strcat(callText,"@SP\n");
	strcat(callText,"A=M\n");
	strcat(callText,"M=D\n");
	strcat(callText,"@SP\n");
	strcat(callText,"M=M+1\n");

	strcat(callText,"@THIS\n");			//PUSH THIS
	strcat(callText,"D=M\n");	
	strcat(callText,"@SP\n");
	strcat(callText,"A=M\n");
	strcat(callText,"M=D\n");
	strcat(callText,"@SP\n");
	strcat(callText,"M=M+1\n");

	strcat(callText,"@THAT\n");			//PUSH THAT
	strcat(callText,"D=M\n");	
	strcat(callText,"@SP\n");
	strcat(callText,"A=M\n");
	strcat(callText,"M=D\n");
	strcat(callText,"@SP\n");
	strcat(callText,"M=M+1\n");

	strcat(callText,"@");				//ARG = SP - 5 - NARGS
	char numString[12];  				// Enough space for integer + null terminator
	sprintf(numString, "%d", nArgs); 	// Convert integer to string
	strcat(callText, numString);      	// Concatenate to target string
	strcat(callText,"\nD=A\n");	
	strcat(callText,"@5\n");
	strcat(callText,"D=D+A\n");
	strcat(callText,"@SP\n");
	strcat(callText,"D=M-D\n");
	strcat(callText,"@ARG\n");
	strcat(callText,"M=D\n");
	strcat(callText,"@SP\n");
	strcat(callText,"D=M\n");
	strcat(callText,"@LCL\n");
	strcat(callText,"M=D\n");

	strcat(callText, "@");
	strcat(callText, string);  // Function name
	strcat(callText, "\n0;JMP\n");

	// Declare the return address label
	strcat(callText, "(");
	strcat(callText, "returnAddress");
	strcat(callText, numStr);  // same return address number as before
	strcat(callText, ")\n");

	fprintf(output_file,"%s",callText);
}
//---------------------------------------------------------------------------------------------
//CodeWriter-writeReturn
//---------------------------------------------------------------------------------------------
void writeReturn(){
	char returnText[1000];

	strcpy(returnText,"@LCL\n"); 	//FRAME = LOCAL
	strcat(returnText,"D=M\n");
	strcat(returnText,"@R13\n");
	strcat(returnText,"M=D\n");

	strcat(returnText,"@5\n"); 		//retAdress = *(frame-5)
	strcat(returnText,"D=A\n");
	strcat(returnText,"@R13\n");
	strcat(returnText,"D=M-D\n");
	strcat(returnText,"A=D\n");
	strcat(returnText,"D=M\n");
	strcat(returnText,"@R14\n");
	strcat(returnText,"M=D\n");

	strcat(returnText,"@ARG\n");	//*Arg = pop()
	strcat(returnText,"A=M\n");
	strcat(returnText,"D=A\n");
	strcat(returnText,"@R15\n");
	strcat(returnText,"M=D\n");
	strcat(returnText,"@SP\n");
	strcat(returnText,"M=M-1\n");
	strcat(returnText,"A=M\n");
	strcat(returnText,"D=M\n");
	strcat(returnText,"@15\n");
	strcat(returnText,"A=M\n");
	strcat(returnText,"M=D\n");

	strcat(returnText,"@ARG\n");	//SP = Arg + 1
	strcat(returnText,"D=M\n");
	strcat(returnText,"@1\n");
	strcat(returnText,"D=D+A\n");
	strcat(returnText,"@SP\n");
	strcat(returnText,"M=D\n");

	strcat(returnText,"@R13\n"); 	//THAT = *(frame-1)
	strcat(returnText,"D=M\n");
	strcat(returnText,"@1\n");
	strcat(returnText,"A=D-A\n");
	strcat(returnText,"D=M\n");
	strcat(returnText,"@THAT\n");
	strcat(returnText,"M=D\n");

	strcat(returnText,"@R13\n"); 	//THIS = *(frame-2)
	strcat(returnText,"D=M\n");
	strcat(returnText,"@2\n");
	strcat(returnText,"A=D-A\n");
	strcat(returnText,"D=M\n");
	strcat(returnText,"@THIS\n");
	strcat(returnText,"M=D\n");

	strcat(returnText,"@R13\n"); 	//ARG = *(frame-3)
	strcat(returnText,"D=M\n");
	strcat(returnText,"@3\n");
	strcat(returnText,"A=D-A\n");
	strcat(returnText,"D=M\n");
	strcat(returnText,"@ARG\n");
	strcat(returnText,"M=D\n");

	strcat(returnText,"@R13\n"); 	//LCL = *(frame-4)
	strcat(returnText,"D=M\n");
	strcat(returnText,"@4\n");
	strcat(returnText,"A=D-A\n");
	strcat(returnText,"D=M\n");
	strcat(returnText,"@LCL\n");
	strcat(returnText,"M=D\n");

	strcat(returnText,"@R14\n"); 	//goto retAddr
	strcat(returnText,"A=M\n");
	strcat(returnText,"0;JMP\n");

	fprintf(output_file,"%s",returnText);
}
//---------------------------------------------------------------------------------------------
//CodeWriter - writeInit
//---------------------------------------------------------------------------------------------
void writeInit() {
    // Write assembly code to set SP = 256
    fprintf(output_file, "// bootstrap code: initialize SP=256\n");
    fprintf(output_file, "@256\n");
    fprintf(output_file, "D=A\n");
    fprintf(output_file, "@SP\n");
    fprintf(output_file, "M=D\n\n");
}
//---------------------------------------------------------------------------------------------
//CodeWriter-close
//---------------------------------------------------------------------------------------------

void processFile(const char *path) {
	//fprintf(stderr, "=== Processing VM file: %s ===\n", path);
    input_file = fopen(path, "r");
    if (!input_file) {
        fprintf(stderr, "Error reading file: %s\n", path);
        return;
    }
    setFileName(path);

    while (hasMoreLines(input_file, line)) {
        advance(line, current_command);

        trimLine(current_command);
        if (current_command[0] == '\0') continue;

        // *** DEBUG PRINT THE RAW, TRIMMED COMMAND ***
   // fprintf(stderr, "RAW CMD → \"%s\"\n", current_command);

        int type = commandType(current_command);
        switch (type) {
            case C_ARITHMETIC:
                writeArithmetic(current_command);
                break;
            case C_PUSH:
            case C_POP:
                arg1(current_command, arg1_string);
                arg2_int = arg2(current_command, arg2_value, 0);
                writePushPop(type, arg1_string, arg2_value);
                break;
            case C_LABEL:
                writeLabel(current_command);
                break;
            case C_GOTO:
                writeGoto(current_command);
                break;
            case C_IF:
                writeIf(current_command);
                break;
            case C_FUNCTION:
                writeFunction(current_command);
                break;
            case C_RETURN:
                writeReturn();
                break;
            case C_CALL: {
                char fn[BUFFER_SIZE];
                int nargs;
                sscanf(current_command, "call %s %d", fn, &nargs);
                writeCall(fn, nargs);
                break;
            }
            default:
                fprintf(stderr, "Unknown command: %s\n", current_command);
        }
    }

    fclose(input_file);
}

//---------------------------------------------------------------------------------------------

int main(int argc, const char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file or folder> <output file>\n", argv[0]);
        return 1;
    }

    // 1) Determine file vs. directory
    struct stat path_stat;
    if (stat(argv[1], &path_stat) != 0) {
        perror("stat");
        return 1;
    }

    // 2) Open output file
    output_file = fopen(argv[2], "w");
    if (!output_file) {
        perror("fopen output");
        return 1;
    }

    // 3) Bootstrap
    writeInit();
    writeCall("Sys.init", 0);

    

    if (S_ISDIR(path_stat.st_mode)) {
        // 1) Process Sys.vm first
        char sysPath[1024];
        snprintf(sysPath, sizeof(sysPath), "%s/Sys.vm", argv[1]);
        struct stat st;
        if (stat(sysPath, &st) == 0 && S_ISREG(st.st_mode)) {
            processFile(sysPath);
        }

        // 2) Then process all other .vm files in the directory
        DIR *dir = opendir(argv[1]);
        if (!dir) {
            perror("opendir");
            return 1;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            // Skip non-.vm files and Sys.vm (already done)
            if (!endsWithVM(entry->d_name) ||
                strcmp(entry->d_name, "Sys.vm") == 0) {
                continue;
            }

            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s",
                     argv[1], entry->d_name);
            processFile(full_path);
        }
        closedir(dir);
    }
}
/*Remove all debug prints (fprintf(stderr, …)) now that you’ve verified ordering.

Delete the RAW CMD and first token logging as well as the 🛠 writeFunction got: line.

Recompile and run on the directory again—your out.asm will be clean, with just the Hack assembly.
*/



/*int main(int argc, const char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input file or folder> <output file, Prog.asm>\n", argv[0]);
        return 1;
    }

    struct stat path_stat;
    stat(argv[1], &path_stat);

    output_file = fopen(argv[2], "w");
    if (output_file == NULL) {
        printf("Error writing file\n");
        return 1;
    }

    writeInit();  
    // Call Sys.init with 0 arguments
    writeCall("Sys.init", 0);

    if (S_ISREG(path_stat.st_mode)) {
        // Single .vm file
        input_file = fopen(argv[1], "r");
        if (input_file == NULL) {
            printf("Error reading file: %s\n", argv[1]);
            return 1;
        }

        setFileName(argv[1]);

        while (hasMoreLines(input_file, line)) {
    advance(line, current_command);

    // 1) Trim off any tabs/spaces around the command
    trimLine(current_command);

    // 2) Skip blank lines
    if (current_command[0] == '\0') continue;

    // 3) Debug the cleaned command
    int type = commandType(current_command);
    printf("DEBUG: command=\"%s\" → type=%d\n", current_command, type);
    fflush(stdout);

    // 4) Dispatch based on type
    switch (type) {
        case C_ARITHMETIC:
            writeArithmetic(current_command);
            break;
        case C_PUSH:
        case C_POP:
            arg1(current_command, arg1_string);
            arg2_int = arg2(current_command, arg2_value, 0);
            writePushPop(type, arg1_string, arg2_value);
            break;
        case C_LABEL:
            writeLabel(current_command);
            break;
        case C_GOTO:
            writeGoto(current_command);
            break;
        case C_IF:
            writeIf(current_command);
            break;
        case C_FUNCTION:
            writeFunction(current_command);
            break;
        case C_RETURN:
            writeReturn();
            break;
        case C_CALL: {
            char fn[BUFFER_SIZE];
            int nargs;
            sscanf(current_command, "call %s %d", fn, &nargs);
            writeCall(fn, nargs);
            break;
        }
        default:
            printf("failed\n");
    }
}

        fclose(input_file);
    } else if (S_ISDIR(path_stat.st_mode)) {
        // Folder containing .vm files
        DIR *dir = opendir(argv[1]);
        if (!dir) {
            printf("Error opening directory: %s\n", argv[1]);
            return 1;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (!endsWithVM(entry->d_name)) continue;

            char full_path[1024];
            snprintf(full_path, sizeof(full_path), "%s/%s", argv[1], entry->d_name);

            input_file = fopen(full_path, "r");
            if (!input_file) {
                printf("Error reading file: %s\n", full_path);
                continue;
            }

          
          //Set file name for context (used in label scoping etc.)
    setFileName(full_path);

    while (hasMoreLines(input_file, line)) {
    advance(line, current_command);

    // 1) Trim off any tabs/spaces around the command
    trimLine(current_command);

    // 2) Skip blank lines
    if (current_command[0] == '\0') continue;

    // 3) Debug the cleaned command
    int type = commandType(current_command);
    printf("DEBUG: command=\"%s\" → type=%d\n", current_command, type);
    fflush(stdout);

    // 4) Dispatch based on type
    switch (type) {
        case C_ARITHMETIC:
            writeArithmetic(current_command);
            break;
        case C_PUSH:
        case C_POP:
            arg1(current_command, arg1_string);
            arg2_int = arg2(current_command, arg2_value, 0);
            writePushPop(type, arg1_string, arg2_value);
            break;
        case C_LABEL:
            writeLabel(current_command);
            break;
        case C_GOTO:
            writeGoto(current_command);
            break;
        case C_IF:
            writeIf(current_command);
            break;
        case C_FUNCTION:
            writeFunction(current_command);
            break;
        case C_RETURN:
            writeReturn();
            break;
        case C_CALL: {
            char fn[BUFFER_SIZE];
            int nargs;
            sscanf(current_command, "call %s %d", fn, &nargs);
            writeCall(fn, nargs);
            break;
        }
        default:
            printf("failed\n");
    }
}
   	
}

    closedir(dir);
    closeFiles();
	return 0;
    } else {
        printf("Invalid input: not a file or folder\n");
        return 1;
    }

    closeFiles();
    fclose(output_file);
    return 0;
}






int main(int argc,const char *argv[]) {
	if (argc != 3) {
        printf("Usage: %s <input file,Prog.vm> <output file,Prog.asm>\n", argv[0]);
        return 1;
    }
    input_file = fopen(argv[1], "r");
	if(input_file == NULL){
		printf("Error reading file\n");
	}
	output_file = fopen(argv[2],"w");
	if(output_file == NULL){
		printf("Error writing file");
	}
   
	//Set file name for context (used in label scoping etc.)
    setFileName(argv[1]);

    while(hasMoreLines(input_file,line)){
    	advance(line,current_command);
    	if (strlen(line) == 0) {
    	 continue;
    	}else if((commandType(current_command) == C_ARITHMETIC)){
    		writeArithmetic(current_command);
    	}else if(commandType(current_command)== C_PUSH){
    		arg1(current_command, arg1_string);
			int index = arg2(current_command, arg2_value, 0);
    		writePushPop(C_PUSH,arg1_string,arg2_value);
    	}else if(commandType(current_command)== C_POP){
    		arg1(current_command, arg1_string);
			int index = arg2(current_command, arg2_value, 0);
   			writePushPop(C_POP,arg1_string,arg2_value);
   		}else if(commandType(current_command)==C_LABEL){
   			writeLabel(current_command);
   		}else if(commandType(current_command)==C_IF){
   			writeIf(current_command);
   		}else if(commandType(current_command)==C_GOTO){
   			writeGoto(current_command);
   		}else if(commandType(current_command)==C_FUNCTION){
   			writeFunction(current_command);
   		}else if(commandType(current_command)==C_RETURN){
   			writeReturn();
   		}else if(commandType(current_command)==C_CALL){
   			
   			char functionName[50];
    		int nArgs;

    		// Skip the "call" keyword and extract function name and nArgs
   		 	sscanf(current_command, "call %s %d", functionName, &nArgs);

   			writeCall(functionName, nArgs);
   		}else printf("failed\n");
   	}	

   	close();
	return 0;
}*/