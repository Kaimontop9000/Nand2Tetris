#include <stdio.h>
#include <string.h>
#include <ctype.h>

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

int main(int argc, char const *argv[])
{
	//shows how to use the program if argument line is not correctly used
	if(argc != 2){
		printf("Usage: %s filename.jack\n", argv[0]);
		return 1;
	}

	//get the file name without extension
	char *dot = strrchr(argv[1], '.');
	size_t len = (dot != NULL)?
	dot - argv[1] : strlen(argv[1]);
	char base_name[256];
	strncpy(base_name, argv[1], len);
	base_name[len] = '\0';

	//creates output file name 
	char output_File[280];
	snprintf(output_File, sizeof(output_File), "%sT.xml", base_name);
	
	//open input file so it is ready to be read in
	FILE *inputFile = fopen(argv[1], "r");
		if(inputFile == NULL) {
			printf("Error opening file %s\n", argv[1]);
			return 1;
	}

	//open the output file, name is based off of input name. Example : prog.jack becomes progT.xml
	FILE *outputFile = fopen(output_File, "w");
	if(outputFile == NULL){
		printf("Error opening output file\n");
		return 1;
	} 

	return 0;
}