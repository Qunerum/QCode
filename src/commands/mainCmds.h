#ifndef MAINCMD_H
#define MAINCMD_H

typedef struct {
    char* cmd;
    void (*handler)(char*);
} qcCmd;

void find(char* cmd, qcCmd out);
extern qcCmd cmds[];
extern int cmd_count;

#endif
