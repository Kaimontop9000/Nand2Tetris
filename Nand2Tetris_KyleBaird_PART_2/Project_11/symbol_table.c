#include <string.h>  // for memset, strncpy

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