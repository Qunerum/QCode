#include <stdio.h>
#include "../include/utility.h"
#ifdef _WIN32
#include <windows.h>
#endif
#define qva_list __builtin_va_list
#define qva_start(v, l) __builtin_va_start(v, l)
#define qva_arg(v, l) __builtin_va_arg(v, l)
#define qva_end(v) __builtin_va_end(v)

void qprint(const char* format, ...) {
    qva_list args;
    qva_start(args, format);
    char buffer[2048];
    int buf_idx = 0;
    for (int i = 0; format[i] != '\0'; i++) {
        if (format[i] == '%' && format[i + 1] != '\0') {
            i++;
            if (format[i] == 's') { char* s = qva_arg(args, char*); while (*s && buf_idx < 2047) { buffer[buf_idx++] = *s++; } }
            else if (format[i] == 'd') {
                int d = qva_arg(args, int);
                char num_buf[32];
                intToStr(d, num_buf);
                char* n = num_buf;
                while (*n && buf_idx < 2047) { buffer[buf_idx++] = *n++; } }
            else if (format[i] == 'c') { char c = (char)qva_arg(args, int); if (buf_idx < 2047) { buffer[buf_idx++] = c; } }
            else if (format[i] == '%') { if (buf_idx < 2047) { buffer[buf_idx++] = '%'; } } } else { if (buf_idx < 2047) { buffer[buf_idx++] = format[i]; } }
    }
    buffer[buf_idx] = '\0';
    qva_end(args);
    int length = len(buffer);
    #ifdef _WIN32
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD written;
    WriteConsoleA(hStdOut, buffer, length, &written, NULL);
    #else
    __asm__ __volatile__(
        "mov $1, %%rax\n"
        "mov $1, %%rdi\n"
        "mov %0, %%rsi\n"
        "mov %1, %%rdx\n"
        "syscall\n"
        :
        : "r"(buffer), "r"((long)length)
        : "rax", "rdi", "rsi", "rdx"
    );
    #endif
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
int contains(char* str, char* search) {
    int i = 0, j = 0;
    while (str[i] != '\0') {
        if (str[i] == search[j]) {
            int temp = i;
            while (str[temp] == search[j] && search[j] != '\0') { temp++; j++; }
            if (search[j] == '\0') return 1;
            j = 0;
        }
        i++;
    }
    return 0;
}
void replace(char* t, char* w, char* r, char* out) {
    int i = 0, j = 0,
    t_len = len(t),
    w_len = len(w),
    r_len = len(r);
    while (i < t_len) {
        int match = 1;
        for (int m = 0; m < w_len; m++) { if (t[i + m] != w[m]) { match = 0; break; } }
        if (match) { for (int m = 0; m < r_len; m++) { out[j++] = r[m]; } i += w_len; } else { out[j++] = t[i++]; }
    }
    out[j] = '\0';
}
void strToInt(char* str, int* out) {
    int res = 0, sign = 1, i = 0;
    while (str[i] == ' ') i++;
    if (str[i] == '-') { sign = -1; i++; }
    while (str[i] >= '0' && str[i] <= '9') {
        res = res * 10 + (str[i] - '0');
        i++;
    }
    *out = res * sign;
}
void intToStr(int n, char* out) {
    int i = 0, isNegative = 0;
    if (n == 0) {
        out[i++] = '0';
        out[i] = '\0';
        return;
    }
    if (n < 0) { isNegative = 1; n = -n; }
    while (n != 0) { out[i++] = (n % 10) + '0'; n = n / 10; }
    if (isNegative) out[i++] = '-';
    out[i] = '\0';
    int start = 0;
    int end = i - 1;
    while (start < end) {
        char temp = out[start];
        out[start] = out[end];
        out[end] = temp;
        start++;
        end--;
    }
}
void strToFloat(char* str, float* out) {
    float res = 0.0f, sign = 1.0f;
    int i = 0;
    while (str[i] == ' ') i++;
    if (str[i] == '-') { sign = -1.0f; i++; }
    while (str[i] != '\0' && str[i] != '.') { if (str[i] >= '0' && str[i] <= '9') { res = res * 10.0f + (str[i] - '0'); } i++; }
    if (str[i] == '.') {
        i++; float divisor = 10.0f;
        while (str[i] != '\0') { if (str[i] >= '0' && str[i] <= '9') { res += (str[i] - '0') / divisor; divisor *= 10.0f; } i++; }
    }
    *out = res * sign;
}
void floatToStr(float n, char* out) {
    int ipart = (int)n;
    intToStr(ipart, out);
    int i = len(out);
    out[i++] = '.';
    float fpart = n - (float)ipart;
    if (fpart < 0) fpart = -fpart;
    int fpart_int = (int)(fpart * 1000.0f);
    char temp[16];
    intToStr(fpart_int, temp);
    if (fpart_int < 100) out[i++] = '0';
    if (fpart_int < 10) out[i++] = '0';
    out[i] = '\0';
    addStr(out, temp);
}
char* addStr(char* a, char* b) {
    int i = 0, j = 0;
    while (a[i] != '\0') { i++; }
    while (b[j] != '\0') { a[i] = b[j]; i++; j++; }
    a[i] = '\0';
    return a;
}
