#include "../include/mainCmds.h"
#include "../include/main.h"
#include "../include/utility.h"
#include <stdio.h>

int checkArgs(char* args, char* outFunc) {
    char c1[MAX_LINE_SIZE];
    char c2[MAX_LINE_SIZE];
    char c3[MAX_LINE_SIZE];
    char c4[MAX_LINE_SIZE];
    char c5[MAX_LINE_SIZE];
    splitStart(args, ' ', c1, c2);
    splitStart(c2, ' ', c3, c4);
    splitStart(c4,' ' , c5, outFunc);
    int at = detectType(c1), bt = detectType(c5);
    float a = 0, b = 0;
    if (at == INT || at == FLOAT) strToFloat(c1, &a);
    if (bt == INT || bt == FLOAT) strToFloat(c5, &b);
    if (at == STRING) a = len(c1);
    if (bt == STRING) a = len(c5);

    if (is(c3, "==")) { return is(c1, c5); }
    else if (is(c3, "!=")) { return !is(c1, c5); }
    else if (is(c3, "<=")) { return a <= b; }
    else if (is(c3, "<")) { return a < b; }
    else if (is(c3, ">=")) { return a >= b; }
    else if (is(c3, ">")) { return a > b; }
    return 0;
}

void if_qc(char* args) { char c1[MAX_LINE_SIZE]; if (checkArgs(args, c1)) { run_func(c1); } }
void for_qc(char* args) {
    char c1[MAX_LINE_SIZE];
    char c2[MAX_LINE_SIZE];
    splitStart(args, ' ', c1, c2);
    char funcName[MAX_LINE_SIZE];
    copyStr(funcName, c2);
    int c = -1;
    strToInt(c1, &c); if (c < 0) return;
    for (int i = 0; i < c; i++) run_func(funcName);
}

void mfmqcc(char* args, int m) {
    ctc(0); ctc(1); ctc(2); ctc(3); ctc(4);
    splitStart(args, ' ', ct[0], ct[1]);
    splitStart(ct[1], ' ', ct[2], ct[3]);
    int vi = -1;
    qcVar* v = getVar(ct[0], &vi);
    if (v != NULL && vi >= 0) {
        if (v->type == INT) {
            int a, b, c = 0;
            strToInt(v->value, &a);
            strToInt(ct[2], &b);
            if (m == 1) c = a + b;
            if (m == 2) c = a - b;
            if (m == 3) c = a * b;
            if (m == 4) c = a / b;
            intToStr(c, ct[4]);
        } else if (v->type == FLOAT) {
            float a, b, c = 0.0f;
            strToFloat(v->value, &a);
            strToFloat(ct[2], &b);
            if (m == 1) c = a + b;
            if (m == 2) c = a - b;
            if (m == 3) c = a * b;
            if (m == 4) c = a / b;
            floatToStr(c, ct[4]);
        } else if (v->type == STRING) { addStr(v->value, ct[2]); return; }
        copyStr(v->value, ct[4]);
    }
}

// = = = = = = = = = = = = = = = OUC = = = = = = = = = = = = = = =
void int_qc(char* args) {
    ctc(0); ctc(1);
    splitStart(args, ' ', ct[0], ct[1]);
    if (ct[1][0] == '\0') { copyStr(ct[1], "0"); }
    int t = detectType(ct[1]);
    if (t != INT) { printf("Error: Value '%s' is not an integer!\n", ct[1]); return; }
    addVar(ct[0], INT, ct[1]);
}
void float_qc(char* args) {
    ctc(0); ctc(1);
    splitStart(args, ' ', ct[0], ct[1]);
    if (ct[1][0] == '\0') { copyStr(ct[1], "0"); }
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
    splitStart(args, ' ', ct[0], ct[1]);
    qcVar* v = getVar(ct[0], NULL);
    if (v != NULL) copyStr(v->value, ct[1]);
}

void list_qc(char* args) {
    ctc(0); ctc(1);
    splitStart(args, ' ', ct[0], ct[1]);
    int t = -1;
    if (is(ct[0], "int")) t = INT;
    else if (is(ct[0], "float")) t = FLOAT;
    else if (is(ct[0], "string")) t = STRING;
    if (t < 0) { printf("Error! Please set type! (int , float , string)"); return; }
    addList(ct[1], t);
}
void addl_qc(char* args) {
    ctc(0); ctc(1);
    splitStart(args, ' ', ct[0], ct[1]);
    int t = detectType(ct[1]);
    qcList* l = getList(ct[0], NULL);
    if (l == NULL) { printf("Error: List not found!\n"); return; }
    int tl = l->type;
    if (tl == t || tl == STRING) { addToList(ct[0], ct[1]); }
}
void setl_qc(char* args) {
    ctc(0); ctc(1); ctc(2);
    splitStart(args, ' ', ct[0], ct[3]);
    splitStart(ct[3], ' ', ct[1], ct[2]);
    qcList* l = getList(ct[0], NULL);
    if (l == NULL) { printf("Error: List '%s' not found!\n", ct[0]); return; }
    int id = -1;
    strToInt(ct[1], &id);
    if (id >= 0 && id < MAX_LIST_LEN) {
        int valType = detectType(ct[2]);
        if (l->type == valType || l->type == STRING) { setToList(l->name, id, ct[2]); }
        else { printf("Error: Type mismatch!\n"); }
    }
}
void reml_qc(char* args) { remList(args); }
void remli_qc(char* args) {
    ctc(0); ctc(1);
    splitStart(args, ' ', ct[0], ct[1]);
    qcList* l = getList(ct[0], NULL);
    if (l == NULL) { printf("Error: List not found!\n"); return; }
    int i = -1;
    strToInt(ct[1], &i);
    if (i >= 0 && i < MAX_LIST_LEN) { remiList(l->name, i); }
    else { printf("Error: Invalid index %d\n", i); }
}

void add_qc(char* args) { mfmqcc(args, 1); }
void sub_qc(char* args) { mfmqcc(args, 2); }
void mul_qc(char* args) { mfmqcc(args, 3); }
void div_qc(char* args) { mfmqcc(args, 4); }
void mod_qc(char* args) {
    ctc(0); ctc(1); ctc(2); ctc(3);
    splitStart(args, ' ', ct[0], ct[3]);
    splitStart(ct[3], ' ', ct[1], ct[2]);
    ctc(3);
    int at = detectType(ct[0]);
    int bt = detectType(ct[1]);
    int vi = -1;
    qcVar* v = getVar(ct[2], &vi);
    if (vi <= -1 || at != INT || bt != INT || (v->type != INT && v->type != FLOAT)) { return; }
    int a = 0; strToInt(ct[0], &a);
    int b = 0; strToInt(ct[1], &b);
    if (b == 0) { printf("Error: Modulo by zero!\n"); return; }
    int c = a % b;
    intToStr(c, ct[3]);
    copyStr(v->value, ct[3]);
}

void print_qc(char* args) { printf("%s", args); }
void println_qc(char* args) { printf("%s\n", args); }

// = = = = = = = = = = = = = = = END OUC = = = = = = = = = = = = = = =

void run_qc(char* args) { run_func(args); }

void input_qc(char* args) {
    if (args == NULL || args[0] == '\0') { printf("Error: 'input' command needs a variable name!\n"); return; }
    char buffer[MAX_LINE_SIZE];
    if (fgets(buffer, MAX_LINE_SIZE, stdin)) {
        int l = len(buffer);
        if (l > 0 && buffer[l - 1] == '\n') { buffer[l - 1] = '\0'; }
        int idx = -1;
        qcVar* v = getVar(args, &idx);
        if (v != NULL && idx >= 0) { if (v->type == STRING) { copyStr(v->value, buffer); } else { printf("Error! Variable isn't a string!"); } }
    }
}
void split_qc(char* args) {
    ctc(0); ctc(1); ctc(2); ctc(3);
    splitStart(args, ' ', ct[0], ct[2]);
    splitStart(ct[2], ' ', ct[1], ct[4]);
    ctc(2); splitStart(ct[4], ' ', ct[2], ct[3]);
    int ini = -1, ai = -1, bi = -1;
    qcVar* in = getVar(ct[0], &ini);
    qcVar* a = getVar(ct[2], &ai);
    qcVar* b = getVar(ct[3], &bi);
    if (in == NULL || a == NULL || b == NULL) { printf("Error! "); } else {
        if (in->type != STRING || a->type != STRING || b->type != STRING) { printf("Error! "); return; }
        int l = len(ct[1]);
        if (l != 1 && l != 2) { printf("Error! "); return; }
        char d = ct[1][0];
        if (l == 2 && is(ct[1], "__")) { d = ' '; }
        splitStart(in->value, d, a->value, b->value);
    }
}

static qcCmd nullCmd = {"NULL", NULL};
qcCmd find(char* cmd) { for (int i = 0; i < cmd_count; i++) { if (is(cmds[i].cmd, cmd)) { return cmds[i]; } } return nullCmd; }
qcCmd cmds[] = {
    {"int", int_qc}, {"float", float_qc}, {"string", string_qc}, {"rem", rem_qc}, {"set", set_qc},
    {"add", add_qc}, {"sub", sub_qc}, {"mul", mul_qc}, {"div", div_qc}, {"mod", mod_qc},
    {"list", list_qc}, {"addl", addl_qc}, {"setl", setl_qc}, {"reml", reml_qc}, {"remli", remli_qc},

    {"run", run_qc},
    {"if", if_qc}, {"for", for_qc},

    {"print", print_qc}, {"println", println_qc},
    {"input", input_qc}, {"split", split_qc}
};
int cmd_count = sizeof(cmds) / sizeof(qcCmd);
