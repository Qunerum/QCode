#ifndef MAINCMD_H
#define MAINCMD_H

#include "../main/main.h"

qcCmd find(char* cmd);
extern qcCmd cmds[];
extern int cmd_count;

#endif
