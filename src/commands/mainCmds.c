#include "mainCmds.h"
#include "../main/main.h"
#include "../common/utility.h"
#include <stdio.h>

extern char* ct[CT_COUNT];
// ct0 - VARs - name
// ct1 - VARs - val
// ct2 - BLOCKs - arg1
// ct3 - BLOCKs - oper
// ct4 - BLOCKs - arg2

void if_qc(char* args) {

}

void int_qc(char* args) {
    splitStart(args, ' ', ct[0], ct[1]);
    if (ct[1] == NULL || ct[1][0] == '\0') { copyStr(ct[1], "0"); }
    int t = detectType(ct[1]);
    if (t != INT) { printf("Error: Value '%s' is not an integer!\n", ct[0]); return; }
    addVar(ct[0], t, ct[1]);
}
void rem_qc(char* args) { remVar(args); }
void run_qc(char* args) { run_func(args); }

void print_qc(char* args) { printf("%s", args); }
void println_qc(char* args) { printf("%s\n", args); }

static qcCmd nullCmd = {"NULL", NULL};
qcCmd find(char* cmd) { for (int i = 0; i < cmd_count; i++) { if (is(cmds[i].cmd, cmd)) { return cmds[i]; } } return nullCmd; }
qcCmd cmds[] = {
    {"if", if_qc},
    {"int", int_qc},
    {"rem", rem_qc},
    {"run", run_qc},
    {"print", print_qc},
    {"println", println_qc},
};
int cmd_count = sizeof(cmds) / sizeof(qcCmd);
