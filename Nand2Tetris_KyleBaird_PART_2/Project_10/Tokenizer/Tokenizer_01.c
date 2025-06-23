#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

#define SYMBOL 0
#define KEYWORD 1
#define IDENTIFIER 2
#define INT_CONST 3
#define STRING_CONST 4

char token[250];
int stringFlag;

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
int tokenType(char *token, int flag){
	if(strcmp(token,"{") == 0 || strcmp(token,"}") == 0 || strcmp(token,"(") == 0 || strcmp(token,")") == 0 || strcmp(token, "[") == 0
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
	else if (token != NULL && *token != '\0') {
    const char *str = token;
    while (*str) {
        if (!isdigit((unsigned char)*str)) {
            break;
        }
        str++;
    }
    if (*str == '\0') {
        return INT_CONST; // All digits
    }
}

	else{
		return IDENTIFIER;
	}
}
	

int getToken(FILE *in, char *string, int maxLen, int flag){
	flag = 0;
	int x = 1;
	int c;
	int index=0;
	while(x){
		c=fgetc(in);

		if (c == '/') {
    	int next = fgetc(in);
    	if (next == '/') {
        	// Skip until end of line
        	while ((c = fgetc(in)) != EOF && c != '\n');
        	continue;  // start over, get next character
    		} else {
        	ungetc(next, in);  // not a comment, treat '/' as a symbol later
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
            	if (index < maxLen - 1) {
                string[index++] = c;
            	}
        	}
        	string[index] = '\0';
        	flag = 1;
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
		else{
		string[index++]=c;
		}
		
	}
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


int main(int argc, char const *argv[])
{
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

		/*process files here{

		
		}
		
		*/
	
		fclose(inputFile);
		fclose(outputFile);
	}

	else{
		//Step 1: open the folder 
		DIR *folder = opendir(argv[1]);
		if(folder == NULL) {
			perror("Error opening folder");
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
					perror("Error opening folder's file");
					continue;
				}

				//create the output file name
				char folder_Output_File[280];
				make_output_filename(entry->d_name, folder_Output_File, sizeof(folder_Output_File));	

				//open the output file, name is based off of input name. Example : prog.jack becomes progT.xml
				FILE *F_OutputFile = fopen(folder_Output_File, "w");
				if(F_OutputFile == NULL){
					printf("Error opening output file\n");
					return 1;
				}

				/*
				process files here{

				

				}
				*/

				fclose(folderFile);
				fclose(F_OutputFile);


			}

		}
		closedir(folder);
	}

	return 0;
}