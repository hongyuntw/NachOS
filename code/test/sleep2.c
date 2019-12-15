#include "syscall.h"
main() {
    int i;
    for(i = 0; i < 20; i++) {
        Sleep(100000);
        PrintInt(10);
    }
    return 0;
}
