#include "VM_Writer.h"


void writePush(FILE *outVM, const char *segment, int index) {
    fprintf(outVM, "push %s %d\n", segment, index);
}

void writePop(FILE *outVM, const char *segment, int index) {
    fprintf(outVM, "pop %s %d\n", segment, index);
}

void writeArithmetic(FILE *outVM, const char *command) {
    fprintf(outVM, "%s\n", command);
}

void writeLabel(FILE *outVM, const char *label) {
    fprintf(outVM, "label %s\n", label);
}

void writeGoto(FILE *outVM, const char *label) {
    fprintf(outVM, "goto %s\n", label);
}

void writeIf(FILE *outVM, const char *label) {
    fprintf(outVM, "if-goto %s\n", label);
}

void writeFunction(FILE *outVM, const char *name, int nVars) {
    fprintf(outVM, "function %s %d\n", name, nVars);
}

void writeCall(FILE *outVM, const char *name, int nArgs) {
    fprintf(outVM, "call %s %d\n", name, nArgs);
}

void writeReturn(FILE *outVM) {
    fprintf(outVM, "return\n");
}