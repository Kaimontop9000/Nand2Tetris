//======================================================================================|
//Kyle Baird. Created : 23/04/2025 Last edited : 09/05/2025								|
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
----------------------------------------------------------------------------------------|
|close			|None			|None			|Close the output file/stream			|
----------------------------------------------------------------------------------------|*/ 
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define BUFFER_SIZE 100//USED TO HOLD THE BUFFER FROM FILE INPUT TO FILE OUTPUT
char line[BUFFER_SIZE];	//THE INITIAL LINE FROM FILE INPUT IS READ INTO THIS LINE
char current_command[BUFFER_SIZE]; //THE INITIAL LINE IS READ INTO THIS LINE
char arg1_string[BUFFER_SIZE]; //used in the arg1 function
char arg2_value[BUFFER_SIZE];					//used in the arg2 function
int arg2_int;
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

int labelCount = 0;

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

char pushText[BUFFER_SIZE]; 

char popText[BUFFER_SIZE];

char ifText[BUFFER_SIZE];

char gotoText[BUFFER_SIZE];

char functionText[BUFFER_SIZE];



							
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
        removeWhitespace(line_name);
		return 1;
		//line_name[strcspn(line_name, "\n")] = 0;
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
int commandType(char current_command_line[]){
	char temp_push[BUFFER_SIZE]; 
	char temp_pop[BUFFER_SIZE];
	char temp_label[BUFFER_SIZE];
	char temp_if[BUFFER_SIZE];
	char temp_Goto[BUFFER_SIZE];
	char temp_Function[BUFFER_SIZE];

	strcpy(temp_push,current_command_line);
	temp_push[4] = '\0';

	strcpy(temp_pop,current_command_line);
	temp_pop[3] = '\0';

	strcpy(temp_label,current_command_line);
	temp_label[5] = '\0';

	strcpy(temp_if,current_command_line);
	temp_if[2] = '\0';

	strcpy(temp_Goto, current_command_line);
	temp_Goto[4] = '\0';

	strcpy(temp_Function,current_command_line);
	temp_Function[8] = '\0';

	if(strcmp(current_command_line,"add") == 0 || strcmp(current_command_line,"sub")==0
	||strcmp(current_command_line,"neg")==0 || strcmp(current_command_line,"eq")==0
	||strcmp(current_command_line,"gt")==0||strcmp(current_command_line,"lt")==0
	||strcmp(current_command_line,"and")==0|| strcmp(current_command_line,"or")==0
	||strcmp(current_command_line,"not")==0){
		return C_ARITHMETIC;
	}else if(strcmp(temp_push,"push")==0){
		return C_PUSH;
	}else if(strcmp(temp_pop,"pop")==0){
		return C_POP;
	}else if(strcmp(temp_label,"label")==0){
		return C_LABEL;
	}else if(strcmp(temp_if,"if")==0){
		return C_IF;
	}else if(strcmp(temp_Goto,"goto")==0){
		return C_GOTO;
	}else if(strcmp(temp_Function,"function")==0){
		return C_FUNCTION;
	}else{
		return INVALID_COMMAND;
	}
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

		sprintf(pushText,"@%s\n",argue2);
		strcat(pushText,"D=A\n");
		strcat(pushText,"@STATIC\n");
		strcat(pushText,"A=M\n");
		strcat(pushText,"A=D+A\n");
		strcat(pushText,"D=M\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"A=M\n");
		strcat(pushText,"M=D\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"M=M+1\n");		

		fprintf(output_file,"%s", pushText);


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

   		strcat(popText, "@");
   		strcat(popText, argue2);       
	    strcat(popText, "\nD=A\n");

	    strcat(popText, "@STATIC\n");       
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
void writeLabel(char command[]){
	char temp[BUFFER_SIZE];

char *label = command+5;

	strcpy(temp, label);

	fprintf(output_file,"(");
	fprintf(output_file,"%s", temp);
	fprintf(output_file,")\n");
	
} 
//---------------------------------------------------------------------------------------------
//CodeWriter- writeIf
//---------------------------------------------------------------------------------------------
	void writeIf(char command[]){
		char temp[BUFFER_SIZE];

	char *label = command+7;
	
		ifText[0] = '\0';

		strcat(ifText,"@SP\n");
		strcat(ifText,"M=M-1\n");
		strcat(ifText,"@SP\n");
		strcat(ifText,"A=M\n");
		strcat(ifText,"D=M\n");
		//strcat(ifText,"@SP\n"); perhaps ifgoto is supposed to consume its previoulsy pushed value.
		//strcat(ifText,"M=M+1\n");
		strcat(ifText,"@");
		strcat(ifText,label);
		strcat(ifText,"\nD;JGT\n");
		fprintf(output_file,"%s", ifText);
}
//--------------------------------------------------------------------------------------------
//CodeWriter - writeGoto
//--------------------------------------------------------------------------------------------
void writeGoto(char command[]){

	char *label = command+4;
	
		gotoText[0] = '\0';

		strcat(gotoText,"@");
		strcat(gotoText,label);
		strcat(gotoText,"\n0;JMP\n");
		fprintf(output_file,"%s", gotoText);
}
//---------------------------------------------------------------------------------------------
//CodeWriter-writeFunction
//---------------------------------------------------------------------------------------------
void writeFunction(char command[]){

	char nArgs[BUFFER_SIZE];
	strcpy(nArgs,command);//copy command into nArgs to extract the nVars local variable count

 	size_t len = strlen(nArgs);
	size_t end = len;
    while (end > 0 && isdigit(nArgs[end - 1])) {
        end--;
    }
    size_t digit_len = len - end;
    char *number = malloc(digit_len + 1);
	
	strncpy(number, nArgs + end, digit_len);
    number[digit_len] = '\0';//number now holds the nVars(n is number of local variables)
	int nNum = atoi(number);

	char *function = command+8;

	size_t len1 = strlen(function);
    while (len1 > 0 && isdigit(function[len1 - 1])) {
        len1--; // move back over digits
    }
    function[len1] = '\0'; // truncate the digits
    
    functionText[0] = '\0';
    
    strcat(functionText,"(");
    strcat(functionText,function);
    strcat(functionText,")\n");

    for(int i; i < nNum; i++){
    	strcat(functionText,"push 0\n");
    }


}
//---------------------------------------------------------------------------------------------
//CodeWriter-close
//---------------------------------------------------------------------------------------------
void close(){
	fclose(input_file);
	fclose(output_file);

}
//---------------------------------------------------------------------------------------------
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
   		}else printf("failed\n");
   	}	

   	close();
	return 0;
}