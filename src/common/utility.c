
#include "../common/utility.h"

int len(char* text) { int l = 0; while (text[l] != '\0') { l++; } return l; }
int is(char* a, char* b) {
    if (len(a) != len(b)) return 0;
    for (int i = 0; i < len(a); i++) { if (a[i] != b[i]) { return 0; } }
    return 1;
}
