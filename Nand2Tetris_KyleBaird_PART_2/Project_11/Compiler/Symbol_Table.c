#include <string.h>  // for memset, strncpy
#include <stdio.h>
#include <stdlib.h>
#include "Symbol_Table.h"

void construct_Symbol_Table(SymbolTable *table) {
    table->count = 0; //total number of symbols in the table
    table->staticCount = 0;
    table->fieldCount = 0;
    table->argCount = 0;
    table->varCount = 0;

    // Optional: clear symbols (memory may contain garbage).
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        table->symbols[i].name[0] = '\0';
        table->symbols[i].type[0] = '\0';
        table->symbols[i].kind = KIND_NONE;
        table->symbols[i].index = -1;
    }
}

void reset_Symbol_Table(SymbolTable *table) {
    table->count = 0;
    table->argCount = 0;
    table->varCount = 0;

    for (int i = 0; i < MAX_SYMBOLS; i++) {
        table->symbols[i].name[0] = '\0';
        table->symbols[i].type[0] = '\0';
        table->symbols[i].kind = KIND_NONE;
        table->symbols[i].index = -1;
    }
}

void define(SymbolTable *table, const char name[], const char type[], Kind kind) {
    if (table->count >= MAX_SYMBOLS) {
        fprintf(stderr, "Error: Symbol table full\n");
        exit(1);
    }

    Symbol *sym = &table->symbols[table->count];

    strncpy(sym->name, name, MAX_NAME_LEN);
    sym->name[MAX_NAME_LEN - 1] = '\0'; // ensure null termination
    strncpy(sym->type, type, MAX_TYPE_LEN);
    sym->type[MAX_TYPE_LEN - 1] = '\0';
    sym->kind = kind;

    // Assign index based on kind
    switch (kind) {
        case KIND_STATIC:
            sym->index = table->staticCount++;
            break;
        case KIND_FIELD:
            sym->index = table->fieldCount++;
            break;
        case KIND_ARG:
            sym->index = table->argCount++;
            break;
        case KIND_VAR:
            sym->index = table->varCount++;
            break;
        default:
            sym->index = -1;
            break;
    }
    table->count++;
}

int varCount(SymbolTable *table, Kind kind) {
    switch (kind) {
        case KIND_STATIC: return table->staticCount;
        case KIND_FIELD:  return table->fieldCount;
        case KIND_ARG:    return table->argCount;
        case KIND_VAR:    return table->varCount;
        default:     return 0; // for NONE or invalid
    }
}

Kind kindOf(SymbolTable *table, const char *name) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->symbols[i].name, name) == 0) {
            return table->symbols[i].kind; // found it
        }
    }
    return KIND_NONE; // not found
}

int indexOf(SymbolTable *table, const char *name) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->symbols[i].name, name) == 0) {
            return table->symbols[i].index;  // found it
        }
    }
    return -1;  // not found
}

char* typeOf(SymbolTable *table, const char *name) {
    for (int i = 0; i < table->count; i++) {
        if (strcmp(table->symbols[i].name, name) == 0) {
            return table->symbols[i].type;  // return type string
        }
    }
    return NULL;  // not found
}
