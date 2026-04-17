#ifndef MAIN_H
#define MAIN_H

#define MAX_LINE_SIZE 1024
#define MAX_BLOCK_SIZE 512
#define MAX_FUNCTIONS 256
#define MAX_VARIABLES 256
#define MAX_LISTS 256
#define MAX_LIST_LEN 256
#define UNKNOWN "NULL"
#define CT_COUNT 10

#define INT 1
#define FLOAT 2
#define STRING 3

typedef struct {
    char* cmd;
    void (*handler)(char*);
} qcCmd;
typedef struct {
    char* name;
    char* value;
    int type;
} qcVar;
typedef struct {
    char* name;
    int count;
    char* values[MAX_LIST_LEN];
    int type;
} qcList;
typedef struct {
    char* name;
    int currentLines;
    char* data[MAX_BLOCK_SIZE + 1];
} qcFunc;

extern char* ct[CT_COUNT];
void ctc(int i);
extern char* block[MAX_BLOCK_SIZE + 1];

qcVar* getVar(char* name, int* out);
void addVar(char* name, int type, char* val);
void remVar(char* name);

qcList* getList(char* name, int* out);
void addList(char* name, int type);
void addToList(char* listName, char* value);

void run_func(char* name);
int detectType(char* str);
void runLine(char* line);

#endif
