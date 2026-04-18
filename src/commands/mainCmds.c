#include "mainCmds.h"
#include "../main/main.h"
#include "../common/utility.h"
#include <stdio.h>
// ct0 - VARs - name
// ct1 - VARs - val
// ct2 - BLOCKs - arg1
// ct3 - BLOCKs - oper
// ct4 - BLOCKs - arg2
// ct5 - Split temp
// ct6 - Split temp 2

int checkArgs(char* args, char* outFunc) { // arg1 oper arg2 func
    ctc(2); ctc(3); ctc(4); ctc(5); ctc(6);
    splitStart(args, ' ', ct[2], ct[5]);
    splitStart(ct[5], ' ', ct[3], ct[6]);
    splitStart(ct[6],' ' , ct[4], outFunc);
    int at = detectType(ct[2]), bt = detectType(ct[4]);
    float a = 0, b = 0;

    if (at == INT || at == FLOAT) strToFloat(ct[2], &a);
    if (bt == INT || bt == FLOAT) strToFloat(ct[4], &b);
    if (at == STRING) a = len(ct[2]); if (bt == STRING) a = len(ct[4]);

    if (is(ct[3], "==")) { return is(ct[2], ct[4]); }
    else if (is(ct[3], "!=")) { return !is(ct[2], ct[4]); }
    else if (is(ct[3], "<=")) { return a <= b; }
    else if (is(ct[3], "<")) { return a < b; }
    else if (is(ct[3], ">=")) { return a >= b; }
    else if (is(ct[3], ">")) { return a > b; }
    return 0;
}

void if_qc(char* args) { ctc(0); if (checkArgs(args, ct[0])) { run_func(ct[0]); } }
void for_qc(char* args) {
    ctc(2); ctc(6);
    splitStart(args, ' ', ct[2], ct[6]);
    char funcName[MAX_LINE_SIZE];
    copyStr(funcName, ct[6]);
    int c = -1;
    strToInt(ct[2], &c); if (c < 0) return;
    for (int i = 0; i < c; i++) run_func(funcName);
}

void int_qc(char* args) {
    ctc(0); ctc(1);
    splitStart(args, ' ', ct[0], ct[1]);
    if (ct[1] == NULL || ct[1][0] == '\0') { copyStr(ct[1], "0"); }
    int t = detectType(ct[1]);
    if (t != INT) { printf("Error: Value '%s' is not an integer!\n", ct[1]); return; }
    addVar(ct[0], INT, ct[1]);
}
void float_qc(char* args) {
    ctc(0); ctc(1);
    splitStart(args, ' ', ct[0], ct[1]);
    if (ct[1] == NULL || ct[1][0] == '\0') { copyStr(ct[1], "0"); }
    int t = detectType(ct[1]);
    if (t != INT && t != FLOAT) { printf("Error: Value '%s' is not an float!\n", ct[1]); return; }
    addVar(ct[0], FLOAT, ct[1]);
}
void string_qc(char* args) {
    ctc(0); ctc(1);
    splitStart(args, ' ', ct[0], ct[1]);
    addVar(ct[0], STRING, ct[1]);
}
void rem_qc(char* args) { remVar(args); }
void set_qc(char* args) {
    ctc(0); ctc(1);
    splitStart(args, ' ', ct[0], ct[1]); // Używaj 0 i 1, nie 7 i 8!
    qcVar* v = getVar(ct[0], NULL);
    if (v != NULL) copyStr(v->value, ct[1]);
}

void list_qc(char* args) {
    ctc(0); ctc(1);
    splitStart(args, ' ', ct[1], ct[0]);
    int t = -1;
    if (is(ct[1], "int")) t = INT;
    else if (is(ct[1], "float")) t = FLOAT;
    else if (is(ct[1], "string")) t = STRING;
    if (t < 0) { printf("Error! Please set type! (int , float , string)"); return; }
    addList(ct[0], t);
}
void addl_qc(char* args) { // addl name val
    ctc(0); ctc(1);
    splitStart(args, ' ', ct[0], ct[1]);
    int tl = getList(ct[0], NULL)->type, t = detectType(ct[1]);
    if (tl == t) { addToList(ct[0], ct[1]); }
}

void mfmqcc(char* args, int m) {
    ctc(0); ctc(1); ctc(2); ctc(3); ctc(5);
    splitStart(args, ' ', ct[0], ct[5]);
    splitStart(ct[5], ' ', ct[1], ct[2]);
    int t = detectType(ct[1]);
    int vi = -1;
    qcVar* v = getVar(ct[0], &vi);
    if (v != NULL && vi >= 0) {
        if (v->type == INT) {
            int a, b, c;
            strToInt(v->value, &a);
            strToInt(ct[1], &b);
            if (m == 1) c = a + b;
            if (m == 2) c = a - b;
            if (m == 3) c = a * b;
            if (m == 4) c = a / b;
            intToStr(c, ct[3]);
        } else if (v->type == FLOAT) {
            float a, b, c;
            strToFloat(v->value, &a);
            strToFloat(ct[1], &b);
            if (m == 1) c = a + b;
            if (m == 2) c = a - b;
            if (m == 3) c = a * b;
            if (m == 4) c = a / b;
            floatToStr(c, ct[3]);
        } else if (v->type == STRING) { addStr(v->value, ct[1]); return; }
        copyStr(v->value, ct[3]);
    }
}

void add_qc(char* args) { mfmqcc(args, 1); }
void sub_qc(char* args) { mfmqcc(args, 2); }
void mul_qc(char* args) { mfmqcc(args, 3); }
void div_qc(char* args) { mfmqcc(args, 4); }

void run_qc(char* args) { run_func(args); }

void print_qc(char* args) { printf("%s", args); }
void println_qc(char* args) { printf("%s\n", args); }

void input_qc(char* args) {
    if (args == NULL || args[0] == '\0') { printf("Error: input command needs a variable name!\n"); return; }
    char buffer[MAX_LINE_SIZE];
    if (fgets(buffer, MAX_LINE_SIZE, stdin)) {
        int l = len(buffer);
        if (l > 0 && buffer[l - 1] == '\n') { buffer[l - 1] = '\0'; }
        int idx = -1;
        qcVar* v = getVar(args, &idx);
        if (v != NULL && idx >= 0) { if (v->type == STRING) { copyStr(v->value, buffer); } else { printf("Error! variable isn't a string!"); } }
    }
}

static qcCmd nullCmd = {"NULL", NULL};
qcCmd find(char* cmd) { for (int i = 0; i < cmd_count; i++) { if (is(cmds[i].cmd, cmd)) { return cmds[i]; } } return nullCmd; }
qcCmd cmds[] = {
    {"if", if_qc}, {"for", for_qc},

    {"int", int_qc}, {"float", float_qc}, {"string", string_qc}, {"rem", rem_qc}, {"set", set_qc},

    {"list", list_qc}, {"addl", addl_qc},

    {"add", add_qc}, {"sub", sub_qc}, {"mul", mul_qc}, {"div", div_qc},

    {"run", run_qc},
    {"print", print_qc},
    {"println", println_qc},
    {"input", input_qc},
};
int cmd_count = sizeof(cmds) / sizeof(qcCmd);
