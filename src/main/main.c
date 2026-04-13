#include <stdio.h>
#include "memory.h"
#include "../common/utility.h"
#include "../commands/mainCmds.h"

#define MAX_LINE_SIZE 1024

int main(int argc, char* argv[]) {
    init_memory();
    if (argc < 2) { printf("Using: qcode <plik.qc>\n"); return 1; }
    FILE* file = fopen(argv[1], "r");
    if (file == NULL) { printf("Error: Cannot open file!\n"); return 1; }
    char* line_buffer = (char*)kmalloc(MAX_LINE_SIZE);

        qcCmd cmd;
    while (fgets(line_buffer, MAX_LINE_SIZE, file) != NULL) {
        if (line_buffer[0] == '\n' || line_buffer[0] == '\0') continue;
        int l = len(line_buffer);
        if (line_buffer[l - 1] == '\n') line_buffer[l - 1] = '\0';
        // CMD
        printf("%d -> %s\n", len(line_buffer), line_buffer);
        find("print", cmd);
        cmd.handler("siema");
        cmd.handler("nox");
    }
    kfree(line_buffer);
    fclose(file);
    return 0;
}
