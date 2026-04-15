#ifndef UTILITY_H
#define UTILITY_H

int len(char* text);
int is(char* a, char* b);
int isAny(char* a, char* b[]);
void splitStart(char* in, char delimiter, char* outA, char* outB);
void copyStr(char* target, char* source);
int startWith(char* text, char* start);
int startAny(char* a, char* b[], char* out);
void cutStart(char* t, int c, char* out);
void trimStart(char* t, char c, char* out);

#endif
