#ifndef TEXT_H
#define TEXT_H

#define STR_LEN 512
typedef struct { char text[STR_LEN]; } string;

string str(const char* dat);
int len(string str);
int is(string a, string b);
void qprint(string msg);
void cprint(const char* msg);
#endif
