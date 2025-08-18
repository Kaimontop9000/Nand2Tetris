int main() {
    SymbolTable classTable;
    SymbolTable subTable;

    construct_Symbol_Table(&classTable);
    construct_Symbol_Table(&subTable);

    // Now both tables are ready to use without malloc/free
    return 0;
}