/*Created August 8th 2025- Last edited August 11th 2025
Jack Compiler
=================================================
Usage - ./Compiler <filename.jack or folderName>

Testing for XML(Project 10) : Usage for TextComparer - 
bash TextComparer.sh Folder/file.xml Folder_test/file.xml 
----------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <stdlib.h>
#include "Token_Creation_Functions.h"
#include "Class_Compilation_Functions.h"
#include "File_Processing_Functions.h"
#include "GLOBALS.h"

//===========================================
int main(int argc, char const *argv[])
{
//============================================
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
//=============================================
		compileClass(inputFile, outputFile);
//==============================================
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
				compileClass(folderFile, F_OutputFile);
//================================================================================================================================
				fclose(folderFile);
				fclose(F_OutputFile);
			}
		}
		closedir(folder);
	}

	return 0;
}


