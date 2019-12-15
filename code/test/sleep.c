#include "syscall.h"
main() {
    int i;
    for(i = 0; i < 5; i++) {
        Sleep(1000000);
        PrintInt(666667777);
    }
    return 0;
}
