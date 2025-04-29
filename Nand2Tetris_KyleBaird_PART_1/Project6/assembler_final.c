//------------------------------------------------------------------------//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
//------------------------------------------------------------------------//
#define BUFFER_SIZE 100 //USED TO READ IN ONE LINE AT A TIME
                        //FROM INPUT DOCUMENT XXX.ASM

#define BUFFER_SIZE_C_INSTRUCTION 256
#define TABLE_SIZE 100

char line[BUFFER_SIZE];
char dest[BUFFER_SIZE], comp[BUFFER_SIZE], jump[BUFFER_SIZE], aaa[BUFFER_SIZE];
char c_instruction[BUFFER_SIZE];
//-------------------------------------------------------------------------//
//ALL BELOW IS RELATED TO HASH MAP
typedef struct Entry {
    char *key;
    int value;
    struct Entry *next;
} Entry;

typedef struct HashMap {
    Entry **buckets;
} HashMap;

unsigned int hash(const char *key) {
    unsigned int hashVal = 0;
    while (*key) {
        hashVal = (hashVal << 5) + *key++;
    }
    return hashVal % TABLE_SIZE;
}

HashMap *createHashMap() {
    HashMap *map = (HashMap *)malloc(sizeof(HashMap));
    map->buckets = (Entry **)calloc(TABLE_SIZE, sizeof(Entry *));
    return map;
}

void insert(HashMap *map, const char *key, int value) {
    unsigned int index = hash(key);
    Entry *newEntry = (Entry *)malloc(sizeof(Entry));
    newEntry->key = strdup(key);
    newEntry->value = value;
    newEntry->next = map->buckets[index];
    map->buckets[index] = newEntry;
}

int get(HashMap *map, const char *key, int *found) {
    unsigned int index = hash(key);
    Entry *entry = map->buckets[index];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            *found = 1;
            return entry->value;
        }
        entry = entry->next;
    }
    *found = 0;
    return 0;
}

void freeHashMap(HashMap *map) {
    for (int i = 0; i < TABLE_SIZE; ++i) {
        Entry *entry = map->buckets[i];
        while (entry != NULL) {
            Entry *temp = entry;
            entry = entry->next;
            free(temp->key);
            free(temp);
        }
    }
    free(map->buckets);
    free(map);
}
//ALL ABOVE IS RELATED TO HASH MAP
//------------------------------------------------------------------------//
void parseInstructionC(const char *line, char *dest, char *comp,
                        char *jump, char *aaa) {

    char temp[BUFFER_SIZE_C_INSTRUCTION]; 
    strcpy(temp, line); // HOLDS THE LINE VALUE IN TEMP FOR MANIPULATION

    dest[0] = comp[0] = jump[0] = '\0'; // SETS FIRST VALUE IN ARRAY TO '\0'

    //CHECK WETHER INSTRUCTION HAS COMPUTE AND DESTINATION OR IF 
    //INSTRUCTION HAS COMPUTE AND JUMP. 
    //COMPUTE IS MANDATORY
    //IF DEST IS EMPTY, = IS OMMITED
    //IF JUMP IS EMPTY, ; IS OMMITED

    char *eqPos = strchr(temp, '=');
    char *scPos = strchr(temp, ';');

    if (eqPos) {
        *eqPos = '\0';
        strcpy(dest, temp);
        if (scPos) {
            *scPos = '\0';
            strcpy(comp, eqPos + 1);
            strcpy(jump, scPos + 1);
        } else {
            strcpy(comp, eqPos + 1);
        }
    } else {
        if (scPos) {
            *scPos = '\0';
            strcpy(comp, temp);
            strcpy(jump, scPos + 1);
        } else {
            strcpy(comp, temp);
        }
    }

    if (dest[0] == '\0') {
        strcpy(dest, "000");
    } else {
        char newDest[4] = "000";
        if (strchr(dest, 'A')) newDest[0] = '1';
        if (strchr(dest, 'D')) newDest[1] = '1';
        if (strchr(dest, 'M')) newDest[2] = '1';
        strcpy(dest, newDest);
    }
    //WE HAVE SEPERATED VALUES FOR DEST, COMP, JUMP
    //THEY ARE NOW READY TO BE TRANSLATED

    // comp + aaa mapping
    struct CompMapping {
        const char *mnemonic;
        const char *binary;
        const char *a;
    } compTable[] = {
        {"0",   "101010", "0"}, {"1",   "111111", "0"}, {"-1",  "111010", "0"},
        {"D",   "001100", "0"}, {"A",   "110000", "0"}, {"!D",  "001101", "0"},
        {"!A",  "110001", "0"}, {"-D",  "001111", "0"}, {"-A",  "110011", "0"},
        {"D+1", "011111", "0"}, {"A+1", "110111", "0"}, {"D-1", "001110", "0"},
        {"A-1", "110010", "0"}, {"D+A", "000010", "0"}, {"D-A", "010011", "0"},
        {"A-D", "000111", "0"}, {"D&A", "000000", "0"}, {"D|A", "010101", "0"},
        {"M",   "110000", "1"}, {"!M",  "110001", "1"}, {"-M",  "110011", "1"},
        {"M+1", "110111", "1"}, {"M-1", "110010", "1"}, {"D+M", "000010", "1"},
        {"D-M", "010011", "1"}, {"M-D", "000111", "1"}, {"D&M", "000000", "1"},
        {"D|M", "010101", "1"},
        {NULL, NULL, NULL}
    };
    //LOOPS THROUGH THE TABLE COMPARING COMP AND IF FOUND TRANSLATING IT.
    int found = 0;
    for (int i = 0; compTable[i].mnemonic != NULL; ++i) {
        if (strcmp(comp, compTable[i].mnemonic) == 0) {
            strcpy(comp, compTable[i].binary);
            strcpy(aaa, compTable[i].a);
            found = 1;
            break;
        }
    }
    //IF NO MATCH IN THE COMPTABLE THEN SETS COMP AND A TO '000000' AND '0'
    if (!found) {
        strcpy(comp, "000000");
        strcpy(aaa, "0");
    }

    // jump mapping
    struct JumpMapping {
        const char *mnemonic;
        const char *binary;
    } jumpTable[] = {
        {"",    "000"},
        {"JGT", "001"},
        {"JEQ", "010"},
        {"JGE", "011"},
        {"JLT", "100"},
        {"JNE", "101"},
        {"JLE", "110"},
        {"JMP", "111"},
        {NULL, NULL}
    };
    //LOOPS THROUGH JUMPTABLE AND COMPARES IT WITH JUMP
    //WHEN VALUE IS MATCHED, TRANSLATION TAKES PLACE TO ITS BINARY VERSION
    for (int i = 0; jumpTable[i].mnemonic != NULL; ++i) { 
        if (strcmp(jump, jumpTable[i].mnemonic) == 0) {
            strcpy(jump, jumpTable[i].binary);
            break;
        }
    }
}
//-------------------------------------------------------//
//STRSTR() FINS THE FIRST OCCURENCE OF //
//STRING STRING. SEARCHES FOR A STRING WITHIN A STRING
//REPLACES THE FIURS / IN // WITH THE NULL TERMINATOR '\0', 
//EFFECTIVELY TRUNCATING THE STRING AT THAT POINT.
void removeComment(char *str) {
    char *commentPos = strstr(str, "//");
    if (commentPos) {
        *commentPos = '\0';
    }
}
//------------------------------------------------------//
//IF THERE IS NO SPACE THEN COPIED STRING AS IS AND GOES TO NEXT CHAR IN LINE
//IF THERE IS A SPACE THEN IGNORES IT AND CHECKS THE NEXT CHAR IN LINE
void removeWhitespace(char *str) {
    int i = 0, j = 0;
    while (str[i]) {
        if (!isspace((unsigned char)str[i])) {
            str[j++] = str[i];
        }
        i++;
    }
    str[j] = '\0';
}
//-------------------------------------------------------//
//ALLOWS FOR @XXX A-INSTRUCTIONS TO BE PARSED INTO BINARY VALUES
void decimalToBinary(int num, char *binaryStr) {
    for (int i = 15; i >= 0; i--) {
        binaryStr[15 - i] = (num & (1 << i)) ? '1' : '0';
    }
    binaryStr[16] = '\0';
}
//----------------------------------------------------------//
//OPENS FILE TO BE READ IN, ONLT KEEPING TRACK OF (XXX) AND THEIR ROM ADDRESS
void firstPass(HashMap *map, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file\n");
        return;
    }

    char line[BUFFER_SIZE];     //READS IN LINE
    int romAddress = 0;         //KEEPS TRACK OF ROM LINE 

    while (fgets(line, BUFFER_SIZE, file)) {
        line[strcspn(line, "\n")] = 0; //REMOVES '\n' FROM THE STRING
        removeComment(line);
        removeWhitespace(line);
        if (strlen(line) == 0) continue; //IF LINE IS EMPTY THEN CONTINUE
                                        //BEGIN NEXT ITERATION OF THIS WHILE LOOP

        //FIRST PASS ONLY CHECKING FOR (XXX) AND INSERTING THESE VALUES TO
        //HASH MAP ALONG WITH THEIR LINE NUMBER(ROM ADDRESS)
        if (line[0] == '(' && line[strlen(line) - 1] == ')') {
            line[strlen(line) - 1] = '\0'; //GETS RID OF ), LEFT WITH (XXX
            insert(map, line + 1, romAddress); //TAKES FROM AFTER (, SO TAKES XXX
        } else {
            romAddress++;
        }
    }
    fclose(file);
}
//----------------------------------------------------------------------------//
//OPEN BOTH INPUT AND OUTPUT FILES
void processFile(HashMap *map, const char *input_filename, 
                        const char *output_filename) {
    FILE *input_file = fopen(input_filename, "r");
    FILE *output_file = fopen(output_filename, "w");

    if (input_file == NULL || output_file == NULL) {
        printf("File open error\n");
        return;
    }

    char line[BUFFER_SIZE];
    int variableAddress = 16;
//REMOVES COMMENTS AND WHITE SPACES FROM THE READ IN LINE
    while (fgets(line, BUFFER_SIZE, input_file)) {
        line[strcspn(line, "\n")] = 0;
        removeComment(line);
        removeWhitespace(line);

//TESTS IF THE LINE IS NULL OR (XXX) AND IF EITHER WILL SKIP THESE LINES
        if (strlen(line) == 0 || (line[0] == '(' && line[strlen(line)-1] == ')'))
                         continue;

        if (line[0] == '@') {
            const char *symbol = line + 1;
            int address;    

            //CHECKS IF AFTER @ IS INT VALUE OR IF ITS SYMBOLIC VALUE
            int isNumber = 1;
            for (const char *p = symbol; *p; ++p) {
                if (!isdigit(*p)) {
                    isNumber = 0;
                    break;
                }
            }
            //IF @XXX IS AN ASSCI NUMBER, TRANSLATES IT INTO ITS INTEGER VALUE
            if (isNumber) {
                address = atoi(symbol);
            } else {
            //CHECKS IF SYMBOLIC @XXX IS IN HASH TABLE OR NOT
            //ADDS NEW ENTRY IF NOT FOUND
                int found;
                address = get(map, symbol, &found);
                if (!found) {
                    insert(map, symbol, variableAddress);
                    address = variableAddress++;
                }
            }
            //CONVERTS THE INTEGER VALUE OF ADDRESS INTO ITS BINARY FORMAT
            char binaryStr[17];
            decimalToBinary(address, binaryStr);
            //PRINTS OUT AN @ A - INSTRUCTION
            fprintf(output_file, "%s\n", binaryStr);
        } else {
            //IF C INSTRUCTION, THEN PARSES C INSTRUCTION
            parseInstructionC(line, dest, comp, jump, aaa);
            char comp_full[8];
            //STITCHES TOGETHER THE ENTIRE PARSED C INSTRUCTION
            snprintf(comp_full, sizeof(comp_full), "%.1s%.6s", aaa, comp);
            snprintf(c_instruction, sizeof(c_instruction), "111%.7s%.3s%.3s", comp_full, dest, jump);
            fprintf(output_file, "%s\n", c_instruction);
        }
    }

    //CLOSE THE FILES
    fclose(input_file);
    fclose(output_file);
    printf("Conversion Complete!\n");
}

int main(int argc, const char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <input file,Prog.asm> <output file,Prog.hack>\n", argv[0]);
        return 1;
    }

    HashMap *map = createHashMap();

    // Predefined symbols
    
    insert(map, "SP", 0);
    insert(map, "LCL", 1);
    insert(map, "ARG", 2);
    insert(map, "THIS", 3);
    insert(map, "THAT", 4);
    insert(map, "SCREEN", 16384);
    insert(map, "KBD", 24576);

    //INSERTS R0 TO R15 TO HASH MAP
    for (int i = 0; i <= 15; ++i) {
        char reg[4];
        sprintf(reg, "R%d", i);
        insert(map, reg, i);
    }

    firstPass(map, argv[1]);
    processFile(map, argv[1], argv[2]);
    freeHashMap(map);

    return 0;
}