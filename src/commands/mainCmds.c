#include "mainCmds.h"
#include "../main/main.h"
#include "../common/utility.h"
#include <stdio.h>

extern char* cmdTemp1;
extern char* cmdTemp2;

void int_qc(char* args) {
    // ct1 - name
    // ct2 - val
    splitStart(args, ' ', cmdTemp1, cmdTemp2);
    if (cmdTemp2 == NULL || cmdTemp2[0] == '\0') { copyStr(cmdTemp2, "0"); }
    int t = detectType(cmdTemp2);
    if (t != INT) { printf("Error: Value '%s' is not an integer!\n", cmdTemp1); return; }
    addVar(cmdTemp1, t, cmdTemp2);
}

void run_qc(char* args) { run_func(args); }

void print_qc(char* args) { printf("%s", args); }
void println_qc(char* args) { printf("%s\n", args); }

static qcCmd nullCmd = {"NULL", NULL};
qcCmd find(char* cmd) { for (int i = 0; i < cmd_count; i++) { if (is(cmds[i].cmd, cmd)) { return cmds[i]; } } return nullCmd; }
qcCmd cmds[] = {
    {"int", int_qc},
    {"run", run_qc},
    {"print", print_qc},
    {"println", println_qc},
};
int cmd_count = sizeof(cmds) / sizeof(qcCmd);
