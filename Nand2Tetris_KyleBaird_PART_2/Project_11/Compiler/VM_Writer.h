#ifndef VM_WRITER_H
#define VM_WRITER_H

#include <stdio.h>
/*Writes a VM push command*/
void writePush(FILE *outVM, const char *segment, int index);

/*Writes a VM pop command*/
void writePop(FILE *outVM, const char *segment, int index);

/*Writes a VM arithmetic-logical command*/
void writeArithmetic(FILE *outVM, const char *command);

/*Writes a VM label command*/
void writeLabel(FILE *outVM, const char *label);

/*Writes a VM goto command*/
void writeGoto(FILE *outVM, const char *label);

/*Writes a VM if-goto command*/
void writeIf(FILE *outVM, const char *label);

/*Writes a VM call command*/
void writeCall(FILE *outVM, const char *name, int nArgs);

/*Writes a VM function command*/
void writeFunction(FILE *outVM, const char *className, const char *subroutineName, int nVars);

/*Writes a VM return command*/
void writeReturn(FILE *outVM);


#endif