#include <stdio.h>
#include "main.h"
#include "memory.h"
#include "../common/utility.h"
#include "../commands/mainCmds.h"

char* ct[CT_COUNT];
void ctc(int i) { ct[i][0] = '\0'; }

static char* cmd;
static char* args;
static char* temp;
static char* temp2;
int collecting = 0,b = 0;
char* blockTypes[] = {"func", "if", NULL};
char* block[MAX_BLOCK_SIZE + 1];

void runCmd(char* line) {
    splitStart(line, ' ', cmd, args);
    qcCmd c = find(cmd);
    if (is(c.cmd, "NULL")) printf("Cannot find command '%s'\n", cmd); else c.handler(args);
}
void runBlock(char* ls[]) { int i = 0; while(ls[i] != NULL && ls[i][0] != '\0') { runLine(ls[i]); i++; } }

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
char* typeToStr(int t) {
    if (t == INT) return "INT";
    else if (t == FLOAT) return "FLOAT";
    else if (t == STRING) return "STRING";
    return UNKNOWN;
}
void runLine(char* line) {
    copyStr(temp2, line);
    if (contains(temp2, "v(")) {
        for (int i = 0; i < MAX_VARIABLES; i++) {
            if (varSlots[i]) {
                temp[0] = '\0'; addStr(temp, "v("); addStr(temp, vars[i].name); addStr(temp, ")");
                while(contains(temp2, temp) && !is(vars[i].value, temp)) { copyStr(ct[5], temp2); replace(ct[5], temp, vars[i].value, temp2); }
            }
        }
    }
    if (contains(temp2, "t(")) {
        for (int i = 0; i < MAX_VARIABLES; i++) {
            if (varSlots[i]) {
                temp[0] = '\0'; addStr(temp, "t("); addStr(temp, vars[i].name); addStr(temp, ")");
                while(contains(temp2, temp) && !is(vars[i].value, temp)) { copyStr(ct[5], temp2); replace(ct[5], temp, typeToStr(vars[i].type), temp2); }
            }
        }
    }

    trimStart(temp2, ' ', temp);
    int l = len(temp);
    if (temp[l - 1] == '\n') temp[l - 1] = '\0';
    if (startWith(temp, "end")) {
        if (!collecting) { printf("Error! End what?\n"); } else {
            block[b] = NULL;
            int j = 0;
            while(block[j] != NULL) { addLineToNewFunc(block[j]); j++; }
            funcCount++;
            b = 0;
            for (int i = 0; i <= MAX_BLOCK_SIZE; i++) block[i] = NULL;
            collecting = 0;
        }
    } else if (startWith(temp, "func")) {
        if (collecting) { printf("Error! Cannot make function in function!\n"); } else {
            splitStart(temp, ' ', cmd, args);
            addFunc(args);
            collecting = 1;
        }
    } else if (collecting && b <= MAX_BLOCK_SIZE) {
        block[b] = (char*)kmalloc(MAX_LINE_SIZE);
        copyStr(block[b], temp);
        b++;
    } else runCmd(temp);
}

int main(int argc, char* argv[]) {
    init_memory();
    if (argc < 2) { printf("Using: qcode <plik.qc>\n"); return 1; }
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) { printf("Error: Cannot open file!\n"); return 1; }

    char* line_buffer = (char*)kmalloc(MAX_LINE_SIZE);
    for (int i = 0; i < CT_COUNT; i++) ct[i] = kmalloc(MAX_LINE_SIZE);
    cmd = (char*)kmalloc(MAX_LINE_SIZE);
    args = (char*)kmalloc(MAX_LINE_SIZE);
    temp = (char*)kmalloc(MAX_LINE_SIZE);
    temp2 = (char*)kmalloc(MAX_LINE_SIZE);
    while (fgets(line_buffer, MAX_LINE_SIZE, file) != NULL) { if (line_buffer[0] == '\n' || line_buffer[0] == '\0') continue; runLine(line_buffer); }
    fclose(file);
    kfree(line_buffer);
    for (int i = 0; i < CT_COUNT; i++) kfree(ct[i]);
    kfree(cmd);
    kfree(args);
    kfree(temp);
    kfree(temp2);
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
