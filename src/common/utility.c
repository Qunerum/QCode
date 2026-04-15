#include <stdio.h>
#include "../common/utility.h"

int len(char* text) { int l = 0; while (text[l] != '\0') { l++; } return l; }
int is(char* a, char* b) {
    if (len(a) != len(b)) return 0;
    for (int i = 0; i < len(a); i++) { if (a[i] != b[i]) { return 0; } }
    return 1;
}
int isAny(char* a, char* b[]) {
    int i = 0;
    while (b[i] != NULL) { if (is(a, b[i])) return 1; i++; }
    return 0;
}
void splitStart(char* in, char delimiter, char* outA, char* outB) {
    int i = 0, ia = 0, ib = 0, found = 0;
    outA[0] = '\0'; outB[0] = '\0';
    if (in == 0 || in[0] == '\0') return;
    while(in[i] != '\0') {
            if (in[i] == delimiter && !found) found = 1;
            else if (!found) { outA[ia] = in[i]; ia++; }
            else if (found) { outB[ib] = in[i]; ib++; }
            i++;
    }
    outA[ia] = '\0'; outB[ib] = '\0';
}
void copyStr(char* target, char* source) { int i = 0; while (source[i] != '\0') { target[i] = source[i]; i++; } target[i] = '\0'; }
int startWith(char* text, char* start) { while (*start != '\0') { if (*text == '\0' || *text != *start) return 0; text++; start++; } return 1; }
int startAny(char* a, char* b[], char* out) {
    int i = 0;
    while (b[i] != NULL) { if (startWith(a, b[i])) { copyStr(out, b[i]); return 1; } i++; }
    return 0;
}
void cutStart(char* t, int c, char* out) {
    int i = 0;
    if (c >= len(t)) { out[0] = '\0'; return; }
    while (t[c] != '\0') { out[i] = t[c]; i++; c++; }
    out[i] = '\0';
}
void trimStart(char* t, char c, char* out) {
    int i = 0;
    while (t[i] == c) { i++; }
    cutStart(t, i, out);
}
