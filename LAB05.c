#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getNTH();
void setNTH();
void countZ();


int main(){

    unsigned int num;
    int count = 0;
    int total_bits = sizeof(num) * 8;

    printf("Enter an option: ");
    scanf("%u", &num);

    switch(num){
        case 1:
            getNTH();
        break;
        case 2:
            setNTH();
        break;
        case 3:
            countZ();
        break;
    }

    return 0;
}

void getNTH(){
    unsigned int num;
    int n = 0;
    int total_bits = sizeof(num) * 8;

    printf("Enter a number: ");
    scanf("%u", &num);

    printf("Enter the bit position (n): ");
    scanf("%d", &n);

    unsigned int bit = (num >> n) & 1;

    printf("The bit at position %d is: %u\n", n, bit);
}

void setNTH(){
    unsigned int num;
    int n;

    printf("Enter a number: ");
    scanf("%u", &num);

    printf("Enter the bit position to set (n): ");
    scanf("%d", &n);

    unsigned int result = num | (1 << n);

    printf("Result after setting bit %d: %u\n", n, result);
}

void countZ(){
    unsigned int num;
    int count = 0;
    int total_bits = sizeof(num) * 8;

    printf("Enter a number: ");
    scanf("%u", &num);

    for (int i = total_bits - 1; i >= 0; i--) {
        if ((num >> i) & 1)
            break;
        count++;
    }

    printf("Number of leading zeros: %d\n", count);
}