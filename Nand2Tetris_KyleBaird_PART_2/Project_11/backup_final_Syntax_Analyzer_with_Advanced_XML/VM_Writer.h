#ifndef VM_WRITER_H
#define VM_WRITER_H
/*Writes a VM push command*/
void writePush(FILE *out, const char *segment, int index);

/*Writes a VM pop command*/
void writePop(FILE *out, const char *segment, int index);

/*Writes a VM arithmetic-logical command*/
void writeArithmetic(FILE *out, const char *command);

/*Writes a VM label command*/
void writeLabel(FILE *out, const char *label);

/*Writes a VM goto command*/
void writeGoto(FILE *out, const char *label);

/*Writes a VM if-goto command*/
void writeIf(FILE *out, const char *label);

/*Writes a VM call command*/
void writeCall(FILE *out, const char *name, int nArgs);

/*Writes a VM function command*/
void writeFunction(FILE *out, const char *name, int nVars);

/*Writes a VM return command*/
void writeReturn(FILE *out);
#endif