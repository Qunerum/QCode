#ifndef MAIN_H
#define MAIN_H

#define MAX_LINE_SIZE 1024
#define MAX_BLOCK_SIZE 512
#define MAX_FUNCTIONS 256
#define MAX_VARIABLES 256
#define UNKOWN "NULL"
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
    int type; // int , float , char , string
} qcVar;
typedef struct {
    char* name;
    int currentLines;
    char* data[MAX_BLOCK_SIZE + 1];
} qcFunc;

extern char* block[MAX_BLOCK_SIZE + 1];

qcVar* getVar(char* name, int* out);
void addVar(char* name, int type, char* val);
void remVar(char* name);

void run_func(char* name);
int detectType(char* str);

#endif
