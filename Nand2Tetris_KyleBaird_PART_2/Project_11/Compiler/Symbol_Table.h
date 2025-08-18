#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_SYMBOLS 256
#define MAX_NAME_LEN 64
#define MAX_TYPE_LEN 64

typedef enum {
    STATIC,
    FIELD,
    ARG,
    VAR,
    NONE
} Kind;

typedef struct {
    char name[MAX_NAME_LEN];
    char type[MAX_TYPE_LEN];
    Kind kind;
    int index;
} Symbol;

typedef struct {
    Symbol symbols[MAX_SYMBOLS];
    int count;
    int staticCount;
    int fieldCount;
    int argCount;
    int varCount;
} SymbolTable;



//funtion prototypes
/*Creates a new symbol table*/
void construct_Symbol_Table(SymbolTable *table);

/*Empty's the symbol table, resets the four indexes to 0.
Called when starting to compile a  subroutine declaration*/
void reset_Symbol_Table();

/*Defines(adds to the table) a new variable of the given
name,type and kind. Assigns to it the index value of that kind 
and adds 1 to the index*/
void define(SymbolTable *table, const char name[], const char type[], Kind kind);

/*Returns the number of variables of the given kind already in 
the table*/
int varCount(SymbolTable *table, Kind kind);

/*Returns the kind of the named identifier. If identifier
is not found, returns NONE*/
Kind kindOf(SymbolTable *table, const char *name);

/*Returns the type of the named variable*/
char *typeOf(SymbolTable *table, const char *name);

/*Returns the index of the variable*/
int indexOf(SymbolTable *table, const char *name);

#endif