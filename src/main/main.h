#ifndef MAIN_H
#define MAIN_H

#define MAX_LINE_SIZE 1024
#define MAX_BLOCK_SIZE 256
#define MAX_FUNCTIONS 256
#define MAX_VARIABLES 256
#define UNKOWN "NULL"

#define INT 1
#define FLOAT 2
#define CHAR 3
#define STRING 4

typedef struct {
    char* cmd;
    void (*handler)(char*);
} qcCmd;
typedef struct {
    char* name;
    char* data[MAX_BLOCK_SIZE + 1];
} qcFunc;
typedef struct {
    char* name;
    char* value;
    int type; // int , float , char , string
} qcVar;

#endif
