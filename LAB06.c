#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

void generateS(int l);
bool loadS(unsigned char S[256]);
void substituteM4();
void substituteM8();
void substituteK8();
void substituteK4();

unsigned char s_box[256];
unsigned char S1[256];
unsigned char S2[256];


int main(){

    bool seguir = true;

    while(seguir){
        printf("\nLAB06 - S-Boxes\n");

        printf("1. Generate S-Box\n");
        printf("2. Substitute char array 'M'\n");
        printf("3. Substitute K (8 bits)\n");
        printf("4. Substitute K (4 bits)\n");
        printf("5. Exit\n");

        int option;
        printf("Select an option: ");
        scanf("%i", &option);

        int l;

        switch(option){
            case 1:
                printf("Enter l, must be a power of 2: ");
                scanf("%i", &l);

                generateS(l);
                break;
            case 2:
                substituteM4();
                substituteM8();
                break;
            case 3:
                substituteK8();
                break;
            case 4:
                substituteK4();
                break;
            case 5:
                seguir = false;
                break;
            default:
                printf("Invalid option\n");
                return 0;
        }
    }

    return 0;
}

void generateS(int l){

    FILE *fptr;
    srand(time(NULL));
    int size = 1 << l;
    int S[size];
    char filename[100];

    printf("Enter the file name to store the S-Box: ");
    scanf("%99s", filename);

    for (int i = 0; i < size; i++){
        S[i] = i;
    }

    //Algoritmo de Fisher-Yates para mezclar el arreglo S
    for (int i = size - 1; i > 0; i--){
        int j = rand() % (i + 1);

        int temp = S[i];
        S[i] = S[j];
        S[j] = temp;
    }

    fptr = fopen(filename, "w");

    if(fptr==NULL){
        printf("Failed to open file\n");
        exit(1);
    }

    printf("S-Box table stored in %s\n", filename);

    for (int i = 0; i < size; i++){
        fprintf(fptr, "%X -> %X\n", i, S[i]);
    }

    fclose(fptr);
}

//Substitutes each character of a string 'M' using an 4-bit S-Box, where each character is substituted by the corresponding value in the S-Box
void substituteM4(){
    char M[100];
    unsigned char S[256];
    unsigned char substitutedM[100];

    printf("Enter the string M: ");
    scanf("%99s", M);

    if(loadS(S)){

        int i;

        for (i = 0; M[i] != '\0'; i++){
            unsigned char originalChar = (unsigned char)M[i];

            printf("\nOriginal char M[%d]: %c (hex: %02X)\n", i, M[i], originalChar);
            
            unsigned char highNibble = (originalChar >> 4);
            unsigned char lowNibble = originalChar;

            printf("8-bit High nibble=M[%d]>>4: %X\n", i, highNibble);
            printf("8-bit Low nibble=M[%d]: %X\n", i, lowNibble);

            highNibble = highNibble & 0x0F;
            lowNibble = lowNibble & 0x0F;

            printf("Original high nibble=8-bit High nibble & 0x0F: %X\n", highNibble);
            printf("Original low nibble=8-bit Low nibble & 0x0F: %X\n", lowNibble);

            unsigned char substitutedHigh = S[highNibble];
            unsigned char substitutedLow = S[lowNibble];

            printf("Substituted high nibble: %X\n", substitutedHigh);
            printf("Substituted low nibble: %X\n", substitutedLow);

            substitutedM[i] = ((substitutedHigh << 4) | substitutedLow);

            printf("Substituted char M[%d](hex): %02X\n\n", i, substitutedM[i]);
        }

        printf("\nSubstituted M(hex): ");
        for (int j = 0; j < i; j++){
            printf("%02X ", substitutedM[j]);
        }
        printf("\n");

    } else {
        printf("Failed to load S-Box.\n");
        return;
    }
}

//Substitutes each character of a string 'M' using an 8-bit S-Box, where each character is substituted by the corresponding value in the S-Box
void substituteM8(){
    char M[100];
    unsigned char S[256];
    unsigned char substitutedM[100];

    printf("\n\nEnter the string M: ");
    scanf("%99s", M);

    if(loadS(S)){

        int i;
        for (i = 0; M[i] != '\0'; i++){
            unsigned char originalChar = (unsigned char)M[i];

            printf("\nOriginal char M[%d]: %c (hex: %02X)\n", i, M[i], originalChar);

            unsigned char substitutedChar = S[originalChar];

            printf("Substituted char M[%d](hex): %02X\n", i, substitutedChar);

            substitutedM[i] = substitutedChar;
        }

        printf("\nSubstituted M(hex): ");
        for (int j = 0; j < i; j++){
            printf("%02X ", substitutedM[j]);
        }
        printf("\n");

    } else {
        printf("Failed to load S-Box.\n");
        return;
    }
}

bool loadS(unsigned char S[256]){

    char filename[100];
    FILE *fptr;

    printf("Enter the file name to load the S-Box: ");
    scanf("%99s", filename);

    fptr = fopen(filename, "r");
    if (!fptr) {
        printf("Error: No se pudo abrir el archivo %s\n", filename);
        return false;
    }

    int entrada, salida;
    while (fscanf(fptr, " %x -> %x", &entrada, &salida) == 2) {
        if (entrada >= 0 && entrada < 256) {
            S[entrada] = (unsigned char)salida;
        }
    }

    fclose(fptr);

    return true;
}

//Subtitutes a 16-bit unsigned short int 'K' using 2 different 8-bit S-Boxes, each for the 2 bytes(MSB and LSB) of K
void substituteK8(){
    
    unsigned short int K;
    unsigned char S1[256];
    unsigned char S2[256];

    printf("Enter K: ");
    scanf("%i", &K);

    if(loadS(S1) && loadS(S2)){

        unsigned char msb = (K >> 8) ;
        unsigned char lsb = K;

        printf("\n16-bit MSB=K>>8: %04X\n", msb);
        printf("16-bit LSB=K: %04X\n\n", lsb);

        msb = msb & 0xFF;
        lsb = lsb & 0xFF;

        printf("Original MSB=16-bit msb&0xFF: %02X\n", msb);
        printf("Original LSB=16-bit lsb&0xFF: %02X\n\n", lsb);

        unsigned char substitutedMSB = S1[msb];
        unsigned char substitutedLSB = S2[lsb];

        printf("Substituted MSB: %02X\n", substitutedMSB);
        printf("Substituted LSB: %02X\n\n", substitutedLSB);

        unsigned short int substitutedK = (substitutedMSB << 8) | substitutedLSB;
        printf("Substituted K: %04X\n\n", substitutedK);

    } else {
        printf("Failed to load S-Boxes.\n");
        return;
    }

}

//Subtitutes a 16-bit unsigned short int 'K' using a 4 S-Box, each for 4 bits of K
void substituteK4(){
    
    unsigned short int K;
    unsigned char S[256];

    printf("\nEnter K: ");
    scanf("%i", &K);

    if(loadS(S)){

        unsigned char n1 = (K >> 12);
        unsigned char n2 = (K >> 8);
        unsigned char n3 = (K >> 4);
        unsigned char n4 = K;

        printf("\n16-bit n1=K>>12: %04X\n", n1);
        printf("16-bit n2=K>>8: %04X\n", n2);
        printf("16-bit n3=K>>4: %04X\n", n3);
        printf("16-bit n4=K: %04X\n\n", n4);

        n1 = n1 & 0x0F;
        n2 = n2 & 0x0F;
        n3 = n3 & 0x0F;
        n4 = n4 & 0x0F;

        printf("Original n1=16-bit n1&0x0F: %04X\n", n1);
        printf("Original n2=16-bit n2&0x0F: %04X\n", n2);
        printf("Original n3=16-bit n3&0x0F: %04X\n", n3);
        printf("Original n4=16-bit n4&0x0F: %04X\n\n", n4);

        unsigned char substitutedN1 = S[n1];
        unsigned char substitutedN2 = S[n2];
        unsigned char substitutedN3 = S[n3];
        unsigned char substitutedN4 = S[n4];

        printf("Substituted n1: %02X\n", substitutedN1);
        printf("Substituted n2: %02X\n", substitutedN2);
        printf("Substituted n3: %02X\n", substitutedN3);
        printf("Substituted n4: %02X\n\n", substitutedN4);

        unsigned short int substitutedK = (substitutedN1 << 12) | (substitutedN2 << 8) | (substitutedN3 << 4) | substitutedN4;
        printf("Substituted K: %04X\n\n", substitutedK);

    } else {
        printf("Failed to load S-Boxes.\n");
        return;
    }

}