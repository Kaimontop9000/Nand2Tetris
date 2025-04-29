//======================================================================================|
//Kyle Baird. Created : 23/04/2025 Last edited : 24/04/2025								|
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
#define C_ARITHMETIC 0
#define C_PUSH 1
#define C_POP 2
#define C_LABEL 3	//branching command perhaps
#define C_GOTO 4	//branching command perhaps
#define C_IF 5		//branching command perhaps
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
				"D=M+D\n"
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

char eqText[]=  "@SP\n"
				"M=M-1\n"
				"A=M\n"
				"D=M\n"
				"@SP\n"
				"M=M-1\n"
				"A=M\n"
				"D=D-M\n"
				"M=D\n"
				"@SP\n"
				"M=M+1\n"
				"D;JEQ\n";

char gtText[]= "@SP\n"
				"M=M-1\n"
				"A=M\n"
				"D=M\n"
				"@SP\n"
				"M=M-1\n"
				"A=M\n"
				"D=D-M\n"
				"M=D\n"
				"@SP\n"
				"M=M+1\n"
				"D;JGT\n";

char ltText[]= "@SP\n"
				"M=M-1\n"
				"A=M\n"
				"D=M\n"
				"@SP\n"
				"M=M-1\n"
				"A=M\n"
				"D=D-M\n"
				"M=D\n"
				"@SP\n"
				"M=M+1\n"
				"D;JLT\n";	

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
				"@Sp\n"
				"M=m-1\n"
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
//-----------------------------------------------------------------------------
//Parser Routine-hasMoreLines
//-----------------------------------------------------------------------------
int hasMoreLines(FILE *input_filename, char line_name[]){
	while(fgets(line_name, BUFFER_SIZE, input_filename)) {
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
	}
}
//-------------------------------------------------------------------------------
//Parser Routine-CommandType
int commandType(char current_command_line[]){
	char temp_push[BUFFER_SIZE]; 
	char temp_pop[BUFFER_SIZE];
	strcpy(temp_push,current_command_line);
	temp_push[4] = '\0';
	strcpy(temp_pop,current_command_line);
	temp_pop[3] = '\0';

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
	}else if(strstr(current_command_line,"local")){
		strcpy(arg1,"local");
		return arg1;
	}else if(strstr(current_command_line,"arguement")){
		strcpy(arg1,"arguement");
		return arg1;
	}else if(strstr(current_command_line,"this")){
		strcpy(arg1,"this");
		return arg1;
	}else if(strcpy(current_command_line,"that")){
		strcpy(arg1,"that");
		return arg1;
	}else if(strcpy(current_command_line,"temp")){
		strcpy(arg1,"temp");
		return arg1;
	}else if(strcpy(current_command_line,"pointer")){
		strcpy(arg1,"pointer");
		return arg1;
	}
}//----------------------------------------------------------
//Parser Routine-arg2
//-----------------------------------------------------------
int arg2(char current_command_line[],char arg2_v[],int arg2_integer){
int i;
int j = 0;
for(i = 0; i<BUFFER_SIZE; i++) {
	if(isdigit(current_command_line[i])){
		arg2_v[j++] = current_command_line[i];
	}
	arg2_v[j]='\0';
}
arg2_integer = atoi(arg2_v);
return arg2_integer;
}
//-----------------------------------------------------------------------
//CodeWriter writeArithmetic
//-----------------------------------------------------------------------
void writeArithmetic(char command[]){
	if(strcmp(command,"add")){
		fprintf(output_file, "%s",addText);
	}else if(strcmp(command,"sub")){
		fprintf(output_file,"%s", subText);
	}else if(strcmp(command,"neg")){
		fprintf(output_file,"%s",negText);
	}else if(strcmp(command,"eq")){
		fprintf(output_file,"%s",eqText);
	}else if(strcmp(command,"gt")){
		fprintf(output_file,"%s",gtText);
	}else if(strcmp(command,"lt")){
		fprintf(output_file,"%s",ltText);
	}else if(strcmp(command,"and")){
		fprintf(output_file,"%s",andText);
	}else if(strcmp(command,"or")){
		fprintf(output_file,"%s",orText);
	}else if(strcmp(command,"not")){
		fprintf(output_file,"%s",notText);
	}
}
//------------------------------------------------------------------------
//CodeWriter-writePushPop
//------------------------------------------------------------------------
void writePushPop(int command, char argue1[], char argue2[])
{
	if(command == C_PUSH && (strcmp(argue1,"constant") != 0)){
		pushText[0] = '\0';
		sprintf(pushText, "@%s\n", argue1);
		strcat(pushText,"A=D\n");
		char temp[BUFFER_SIZE];
		sprintf(temp, "%s\n",argue2);
		strcat(pushText,temp);
		strcat(pushText,"D=A+D\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"A=M\n");
		strcat(pushText,"M=D\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"M=M+1\n");		
		fprintf(output_file,"%s", pushText);

	}else if(command == C_POP){
		popText[0] = '\0';
		strcat(popText,"@SP\n");
		strcat(popText,"M=M-1\n");
		sprintf(popText,"@%s\n", argue2);
		strcat(popText,"D=A\n");
		char temp[BUFFER_SIZE];
		sprintf(temp,"@%s\n", argue1);
		strcat(popText,temp);
		strcat(popText,"D=A+D\n");	
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
		strcat(pushText,"A=D\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"A=M\n");
		strcat(pushText,"M=D\n");
		strcat(pushText,"@SP\n");
		strcat(pushText,"M=M+1\n");		
		fprintf(output_file,"%s", pushText);
	}
}
//might have to add logic for constant, if segment is constant then arg1 needs to be looked at
//---------------------------------------------------------------------------------------------
//CodeWriter-close
//---------------------------------------------------------------------------------------------
void close(){
	fclose(input_file);
	fclose(output_file);

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
   
    while(hasMoreLines(input_file,line)){
    	advance(line,current_command);
    	if(commandType(current_command)== C_ARITHMETIC){
    		writeArithmetic(current_command);
    	}else if(commandType(current_command)== C_PUSH){
    		writePushPop(C_PUSH,arg1_string,arg2_value);
    	}else if(commandType(current_command)== C_POP){
   			writePushPop(C_POP,arg1_string,arg2_value);
   		}
   	}
   	close();
	return 0;
}