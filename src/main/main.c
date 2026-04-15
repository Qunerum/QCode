#include <stdio.h>
#include "main.h"
#include "memory.h"
#include "../common/utility.h"
#include "../commands/mainCmds.h"

char* cmdTemp1;
char* cmdTemp2;

static char* cmd;
static char* args;
static int tempCount = 6;
static char* temp[6]; // 0-runCmd 1-if-args-all 2-arg1 3-arg2 4-arg3 5-temp
int collecting = 0, ifCount = 0, b = 0;
char* blockTypes[] = {"func", "if", NULL};
char* block[MAX_BLOCK_SIZE + 1];


int checkArgs(char* t) { // arg1 == arg2
    splitStart(t, ' ', temp[2], temp[3]);

    return 0;
}

void runCmd(char* line) {
    trimStart(line, ' ', temp[0]);
    splitStart(temp[0], ' ', cmd, args);
    qcCmd c = find(cmd);
    if (is(c.cmd, "NULL")) printf("Cannot find command '%s'\n", cmd); else c.handler(args);
}
void runBlock(char* ls[]) { int i = 0; while(ls[i] != NULL && ls[i][0] != '\0') { runCmd(ls[i]); i++; } }

// = = = = = = = = = = = = = = = VARIABLES = = = = = = = = = = = = = = =
int varSlots[MAX_VARIABLES]; // 0 - wolne 1 - zajęte
qcVar vars[MAX_VARIABLES];
int varsCount = 0;
int getFirstSlot() { for (int i = 0; i < MAX_VARIABLES; i++) { if (!varSlots[i]) { return i; } } return -1; }

qcVar* getVar(char* name, int* out) { for(int i = 0; i < MAX_VARIABLES; i++)
{ if (!varSlots[i]) continue; if (is(vars[i].name, name)) { if (out != NULL) *out = i; return &vars[i]; } }if (out != NULL) *out = -1; return NULL; }

void addVar(char* name, int type, char* val) {
    int s = getFirstSlot();
    if (varsCount >= MAX_VARIABLES || s <= -1) { printf("Error! Too many variables! (Max: %d)\n", MAX_VARIABLES); return; }
    vars[s].name = (char*)kmalloc(len(name) + 1);
    copyStr(vars[s].name, name);
    vars[s].type = type;
    vars[s].value = (char*)kmalloc(MAX_LINE_SIZE);
    copyStr(vars[s].value, val);
    varSlots[s] = 1;
    varsCount++;
}
void remVar(char* name) {
    int o = 0;
    qcVar* v = getVar(name, &o);
    if (o <= -1) { printf("Error! Variable '%s' not found!\n", name); return; }
    kfree(v->name);
    kfree(v->value);
    varSlots[o] = 0;
}
// = = = = = = = = = = = = = = = END VARIABLES = = = = = = = = = = = = = = =

// = = = = = = = = = = = = = = = FUNCTIONS = = = = = = = = = = = = = = =
qcFunc funcs[MAX_FUNCTIONS];
int funcCount = 0;

    qcFunc* getFunc(char* name) { for (int i = 0; i < funcCount; i++) { if (is(funcs[i].name, name)) { return &funcs[i]; } } return NULL; }
    char* getFuncLine(char* name, int line) {
    if (line >= MAX_BLOCK_SIZE) return "";
    qcFunc* f = getFunc(name);
    if (f == NULL) return "";
    if (f->data[line] != NULL) return f->data[line];
    return "";
}
void addFunc(char* name) {
    if (funcCount >= MAX_FUNCTIONS) { printf("Error: Too many functions! (Max: %d)\n", MAX_FUNCTIONS); return; }
    funcs[funcCount].name = (char*)kmalloc(len(name) + 1);
    copyStr(funcs[funcCount].name, name);
    for (int i = 0; i < MAX_BLOCK_SIZE; i++) funcs[funcCount].data[i] = NULL;
    funcs[funcCount].currentLines = 0;
}
void addLineToNewFunc(char* line) {
    qcFunc* f = &funcs[funcCount];
    if (f->currentLines >= MAX_BLOCK_SIZE) return;
    f->data[f->currentLines] = line;
    f->currentLines++;
}
void run_func(char* name) { qcFunc* f = getFunc(name); if (f != NULL) { runBlock(f->data); } else { printf("Error: Function '%s' not found!\n", name); } }
// = = = = = = = = = = = = = = = END FUNCTIONS = = = = = = = = = = = = = = =

void runLine(char* line) {
    int l = len(line);
    if (line[l - 1] == '\n') line[l - 1] = '\0';

    if (startWith(line, "end")) {
        splitStart(line, ' ', cmd, args);
        if (args != NULL && args[0] != '\0' && isAny(args, blockTypes)) {
            block[b] = NULL;
            collecting = 0;
            if (startWith(args, "func")) { for (int i = 0; i < b; i++) { addLineToNewFunc(block[i]); } funcCount++; }
            else if(startWith(args, "if")) {
                if (ifCount > 0) {
                    if (checkArgs(temp[1])) {
                        for (int i = 0; i < b; i++) {
                            runLine(block[i]);
                            kfree(block[i]);
                        }
                    } else { for (int i = 0; i < b; i++) kfree(block[i]); }
                    ifCount--;
                }
                b = 0;
                collecting = 0;
                return;
            }
            b = 0;
            return;
        }
        printf("Error: End what? end ...\n");
    }
    if (startAny(line, blockTypes, cmd)) {
        splitStart(line, ' ', cmd, args);
        if (is(cmd, "func")) { addFunc(args); }
        else if (is(cmd, "if")) { copyStr(temp[1], args); ifCount++; }
        collecting = 1;
        return;
    }
    if (collecting)
    {
        block[b] = (char*)kmalloc(MAX_LINE_SIZE);
        copyStr(block[b], line);
        b++;
    } else runCmd(line);
}

int main(int argc, char* argv[]) {
    init_memory();
    if (argc < 2) { printf("Using: qcode <plik.qc>\n"); return 1; }
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) { printf("Error: Cannot open file!\n"); return 1; }

    char* line_buffer = (char*)kmalloc(MAX_LINE_SIZE);
    cmdTemp1 = (char*)kmalloc(MAX_LINE_SIZE);
    cmdTemp2 = (char*)kmalloc(MAX_LINE_SIZE);
    cmd = (char*)kmalloc(MAX_LINE_SIZE);
    args = (char*)kmalloc(MAX_LINE_SIZE);
    for (int i = 0; i < tempCount; i++) temp[i] = (char*)kmalloc(MAX_LINE_SIZE);

    while (fgets(line_buffer, MAX_LINE_SIZE, file) != NULL) {
        if (line_buffer[0] == '\n' || line_buffer[0] == '\0') continue;
        runLine(line_buffer);
    }
    fclose(file);
    kfree(line_buffer);
    kfree(cmdTemp1);
    kfree(cmdTemp2);
    kfree(cmd);
    kfree(args);
    for (int i = 0; i < tempCount; i++) kfree(temp[i]);
    return 0;
}

int detectType(char* str) {
    int i = 0;
    int dotCount = 0;
    int digitCount = 0;
    if (str[0] == '-') i++;
    while (str[i] != '\0') {
        if (str[i] == '.') { dotCount++; }
        else if (str[i] >= '0' && str[i] <= '9') { digitCount++; }
        else { return STRING; }
        i++;
    }
    if (digitCount > 0 && dotCount == 0) return INT;
    if (digitCount > 0 && dotCount == 1) return FLOAT;
    return STRING;
}
