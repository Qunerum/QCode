#include "mainCmds.h"
#include "../common/utility.h"
#include <stdio.h>

void print_qc(char* args) { printf("%s", args); }
void println_qc(char* args) { printf("%s\n", args); }

void find(char* cmd, qcCmd out) { for (int i = 0; i < cmd_count; i++) { if (is(cmds[i].cmd, cmd)) { out = cmds[i]; return; } } }
qcCmd cmds[] = {
    {"print", print_qc},
    {"println", println_qc},
};
int cmd_count = sizeof(cmds) / sizeof(qcCmd);
