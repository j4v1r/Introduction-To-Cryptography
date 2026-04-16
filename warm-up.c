#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){

    unsigned char data2 = 0xB5;
    unsigned char result = data2 & 0x08;

    printf("4th LSB -> Hex: %X | Int: %d\n", result, result);


    return 0;
}

/*
    
    unsigned char data = 'z';

    printf("The value of %c is: %c\n", data, data);
    printf("The value of %c in hexadecimal is: %x\n", data, data);
    printf("The value of %c in integer is: %d\n", data, data);

    for (int i = 0; i < 8; i++) {
        printf("Shift %d:\n", i);
        printf("Character: %c\n", data);
        printf("Hexadecimal: %x\n", data);
        printf("Integer: %d\n\n", data);

        data = data << 1;
    }

    for (int i = 0; i < 8; i++) {
        printf("Shift %d:\n", i);
        printf("Character: %c\n", data);
        printf("Hexadecimal: %x\n", data);
        printf("Integer: %d\n\n", data);

        data = data >> 1;
    }

    unsigned char v1 = 'A';
    unsigned char v2 = 'B';

    printf("v1: %c, v2: %c\n", v1, v2);
    printf("v1 & v2: %x\n", v1 & v2);
    printf("v1 & v2: %d\n", v1 & v2);
    printf("v1 | v2: %x\n", v1 | v2);
    printf("v1 | v2: %d\n", v1 | v2);
    printf("v1 ^ v2: %x\n", v1 ^ v2);
    printf("v1 ^ v2: %d\n", v1 ^ v2);

*/