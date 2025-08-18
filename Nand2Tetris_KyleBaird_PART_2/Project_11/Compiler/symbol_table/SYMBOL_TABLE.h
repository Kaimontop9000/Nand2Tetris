#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
/*Creates a new symbol table*/
void construct_Symbol_Tabel();

/*Empty's the symbol table, resets the four indexes to 0.
Called when starting to compile a  subroutine declaration*/
void reset_Symbol_Table();

/*Defines(adds to the table) a new variable of the given
name,type and kind. Assigns to it the index value of that kind 
and adds 1 to the index*/
void define(char name[], char type[], int kind);

/*Returns the number of variables of the given kind already in 
the table*/
int varCount(int kind);

/*Returns the kind of the named identifier. If identifier
is not found, returns NONE*/
int kindOf(char name[]);

/*Returns the type of the named variable*/
char *typeOf(char name[]);

/*Returns the index of the variable*/
int indexOf(char name[]);

#endif