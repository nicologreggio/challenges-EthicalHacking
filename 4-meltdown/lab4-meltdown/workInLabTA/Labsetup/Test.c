#include <stdio.h>

int main() {
    char *kernel_data_addr = (char*)0xcfccc030;
    char kernel_data = *kernel_data_addr;
    printf("I have reached here.\n");
    return 0;
}