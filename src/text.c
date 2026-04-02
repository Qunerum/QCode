#include "text.h"

string str(const char* dat) { string s; int i = 0; while (i < STR_LEN - 1 && dat[i]) { s.text[i] = dat[i]; i++; } s.text[i] = '\0'; return s; }
int len(string str) {
    int len = 0;
    while (len < STR_LEN && str.text[len]) { len++; }
    return len;
}
int is(string a, string b) {
    int la = len(a);
    if (la != len(b)) return 0;
    for (int i = 0; i < la; i++) { if (a.text[i] != b.text[i]) { return 0; } }
    return 1;
}
void qprint(string msg) {
    int l = len(msg);
    __asm__(
        "mov %0, %%rsi\n"
        "mov %1, %%rdx\n"
        "mov $1, %%rax\n"
        "mov $1, %%rdi\n"
        "syscall\n"
        :
        : "r"(msg.text), "r"(l)
        : "rax", "rdi", "rsi", "rdx"
    );
}
void cprint(const char* msg) {
    int l = len(str(msg));
    __asm__(
        "mov %0, %%rsi\n"
        "mov %1, %%rdx\n"
        "mov $1, %%rax\n"
        "mov $1, %%rdi\n"
        "syscall\n"
        :
        : "r"(msg), "r"(l)
        : "rax", "rdi", "rsi", "rdx"
    );
}

