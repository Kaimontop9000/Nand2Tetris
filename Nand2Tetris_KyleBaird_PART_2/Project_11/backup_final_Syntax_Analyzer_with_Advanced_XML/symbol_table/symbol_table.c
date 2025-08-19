#include <string.h>  // for memset, strncpy
#include "SYMBOL_TABLE.h"

void construct_Symbol_Table(SymbolTable *table) {
    table->count = 0;
    table->staticCount = 0;
    table->fieldCount = 0;
    table->argCount = 0;
    table->varCount = 0;

    // Optional: clear symbols
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        table->symbols[i].name[0] = '\0';
        table->symbols[i].type[0] = '\0';
        table->symbols[i].kind = NONE;
        table->symbols[i].index = -1;
    }
}