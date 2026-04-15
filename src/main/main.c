#include <stdio.h>
#include "main.h"
#include "memory.h"
#include "../common/utility.h"
#include "../commands/mainCmds.h"

char* cmdTemp1;
char* cmdTemp2;

static char* cmd;
static char* args;
static char* temp;

void runCmd(char* line) {
    trimStart(line, ' ', temp);
    splitStart(temp, ' ', cmd, args);
    qcCmd c = find(cmd);
    if (is(c.cmd, "NULL")) printf("Cannot find command '%s'\n", cmd); else c.handler(args);
}
void runBlock(char* ls[]) { int i = 0; while(ls[i] != NULL && ls[i][0] != '\0') { runCmd(ls[i]); i++; } }

// = = = = = = = = = = = = = = = VARIABLES = = = = = = = = = = = = = = =
int varSlots[MAX_VARIABLES]; // 0 - wolne 1 - zajęte
qcVar vars[MAX_VARIABLES];
int varsCount = 0;
int getFirstSlot() { for (int i = 0; i < MAX_VARIABLES; i++) { if (!varSlots[i]) { return i; } } return -1; }
qcVar* getVar(char* name, int* out) {
    for(int i = 0; i < MAX_VARIABLES; i++) { if (!varSlots[i]) continue; if (is(vars[i].name, name)) { if (out != NULL) *out = i; return &vars[i]; } }if (out != NULL) *out = -1; return NULL; }
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

char* blockTypes[] = {"func", NULL};
char* block[MAX_BLOCK_SIZE + 1];

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
    temp = (char*)kmalloc(MAX_LINE_SIZE);
    int collecting = 0, b = 0;

    while (fgets(line_buffer, MAX_LINE_SIZE, file) != NULL) {
        if (line_buffer[0] == '\n' || line_buffer[0] == '\0') continue;
        int l = len(line_buffer);
        if (line_buffer[l - 1] == '\n') line_buffer[l - 1] = '\0';

        if (startWith(line_buffer, "end")) {
            splitStart(line_buffer, ' ', cmd, args);
            if (args != NULL && args[0] != '\0' && isAny(args, blockTypes)) {
                block[b] = NULL;
                collecting = 0;
                if (startWith(args, "func")) { for (int i = 0; i < b; i++) { addLineToNewFunc(block[i]); } funcCount++; }
                b = 0;
                continue;
            }
            printf("Error: End what? end ...\n");
        }
        if (startAny(line_buffer, blockTypes, cmd)) {
            splitStart(line_buffer, ' ', cmd, args);
            if (is(cmd, "func")) { addFunc(args); }
            collecting = 1;
            continue;
        }
        if (collecting)
        {
            block[b] = (char*)kmalloc(MAX_LINE_SIZE);
            copyStr(block[b], line_buffer);
            b++;
        } else runCmd(line_buffer);
    }
    fclose(file);
    kfree(line_buffer);
    kfree(cmdTemp1);
    kfree(cmdTemp2);
    kfree(cmd);
    kfree(args);
    kfree(temp);
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

