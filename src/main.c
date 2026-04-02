#include "text.h"

void qexit() {
    __asm__(
        "mov $60, %rax\n"
        "xor %rdi, %rdi\n"
        "syscall"
    );
}
int q_open_file(const char* path) {
    int fd;
    __asm__(
        "mov $2, %%rax\n"   // SYS_OPEN
        "mov %1, %%rdi\n"   // path to %1
        "mov $0, %%rsi\n"   // flags (O_RDONLY)
    "syscall"
    : "=a"(fd)          // %0: rax (wynik)
    : "r"(path)         // %1: dowolny rejestr dla ścieżki
    : "rdi", "rsi"      // informujemy kompilator, że zmieniamy te rejestry
    );
    return fd;
}
void q_read_to_string(int fd, string* out) {
    long bytes_read;
    __asm__(
        "mov $0, %%rax\n"
        "mov %1, %%rdi\n"
        "mov %2, %%rsi\n"
        "mov %3, %%rdx\n"
        "syscall"
        : "=a"(bytes_read)
        : "r"(fd), "r"(out->text), "i"(STR_LEN - 1)
        : "rdi", "rsi", "rdx", "rcx", "r11" // Dodaj rcx i r11!
    );
    if (bytes_read > 0) out->text[bytes_read] = '\0';
    else out->text[0] = '\0';
}
void _start() {
    long argc;
    char **argv;

    // Pobieramy argc i argv bezpośrednio ze stosu
    __asm__ (
        "mov (%%rsp), %0\n"
        "lea 8(%%rsp), %1\n"
        : "=r" (argc), "=r" (argv)
    );

    if (argc < 2) {
        cprint("Podaj plik: ./bar.sh test.qc\n");
        qexit();
    }

    // argv[1] to ścieżka, którą wpisałeś w terminalu
    int fd = q_open_file(argv[1]);

    if (fd < 0) {
        cprint("Dalej nie widze pliku: ");
        cprint(argv[1]);
        cprint("\n");
        qexit();
    }

    string s;
    for(int i=0; i<512; i++) s.text[i] = 0; // Czyszczenie
    q_read_to_string(fd, &s);
    qprint(s);

    qexit();
}
