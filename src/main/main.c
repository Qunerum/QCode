#include <stdio.h>
#include "main.h"
#include "memory.h"
#include "../common/utility.h"
#include "../commands/mainCmds.h"

static char* cmd;
static char* args;

void runCmd(char* line) {
    splitStart(line, ' ', cmd, args);
    qcCmd c = find(cmd);
    if (is(c.cmd, "NULL")) printf("Cannot find command '%s'\n", cmd); else c.handler(args);
}

qcVar vars[MAX_VARIABLES];
int varsCount = 0;
qcFunc funcs[MAX_FUNCTIONS];
int funcCount = 0;

qcFunc* getFunc(char* name) { for (int i = 0; i < funcCount; i++) { if (is(funcs[i].name, name)) { return &funcs[i]; } } return NULL; }
char* getFuncLine(char* name, int line) {
    if (line >= MAX_BLOCK_SIZE) return "";
    qcFunc* f = getFunc(name);
    if (f.data[line] != NULL) return f.data[line];
    return "";
}
int addFunc(char* name)
{
    if (funcCount >= MAX_FUNCTIONS) { printf("Error: Too many functions! (Max: %d)\n", MAX_FUNCTIONS); return 0; }
    funcs[funcCount].name = (char*)kmalloc(len(name) + 1);
    copyStr(funcs[funcCount].name, name);
    for (int i = 0; i < MAX_BLOCK_SIZE; i++) funcs[funcCount].data[i] = NULL;
    return 1;
}
void addLineToFunc(char* name, char* line) {
    qcFunc* f; getFunc(name, f);
    if (f.currentLines >= MAX_BLOCK_SIZE) return;
    f.data[f.currentLines] = (char*)kmalloc(len(line));
    copyStr(f.data[f.currentLines], line);
}

int main(int argc, char* argv[]) {
    init_memory();
    if (argc < 2) { printf("Using: qcode <plik.qc>\n"); return 1; }
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) { printf("Error: Cannot open file!\n"); return 1; }
    char* line_buffer = (char*)kmalloc(MAX_LINE_SIZE);
    cmd = (char*)kmalloc(MAX_LINE_SIZE);
    args = (char*)kmalloc(MAX_LINE_SIZE);
    int collecting = 0;
    int b = 0;
    char* block[MAX_BLOCK_SIZE + 1];
    while (fgets(line_buffer, MAX_LINE_SIZE, file) != NULL) {
        if (line_buffer[0] == '\n' || line_buffer[0] == '\0') continue;
        int l = len(line_buffer);
        if (line_buffer[l - 1] == '\n') line_buffer[l - 1] = '\0';

        if (collecting)
        {
            // a
        } else runCmd(line_buffer);
    }
    fclose(file);
    kfree(line_buffer);
    kfree(cmd);
    kfree(args);
    return 0;
}

void runBlock(char* ls[]) { int i = 0; while(ls[i] != NULL && ls[i][0] != '\0') { runCmd(ls[i]); i++; } }
