#include "mainCmds.h"
#include "../common/utility.h"
#include <stdio.h>

// int


void print_qc(char* args) { printf("%s", args); }
void println_qc(char* args) { printf("%s\n", args); }

static qcCmd nullCmd = {"NULL", NULL};
qcCmd find(char* cmd) { for (int i = 0; i < cmd_count; i++) { if (is(cmds[i].cmd, cmd)) { return cmds[i]; } } return nullCmd; }
qcCmd cmds[] = {
    {"print", print_qc},
    {"println", println_qc},
};
int cmd_count = sizeof(cmds) / sizeof(qcCmd);
