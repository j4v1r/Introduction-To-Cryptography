#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

void generateS(int l);
bool loadS(unsigned char S[256]);
void key_expansion();

int main(){

    bool seguir = true;

    while(seguir){
        printf("\nLAB07 - Tiny Block Cipher\n");

        printf("1. Key Expansion\n");
        printf("5. Exit\n");

        int option;
        printf("Select an option: ");
        scanf("%i", &option);

        int l;

        switch(option){
            case 1:
                key_expansion();
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


//Generates a S-Bx of size l and stores it in a file, then performs key expansion using the generated S-Box
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

//Loads a S-Box from a file into an array, returns true if successful, false otherwise
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

//Performs XOR operations on a short int 'K' with the substituted values of its nibbles using a 4-bit S-Box, and then substitutes the result using the same S-Box
void key_expansion(){

    unsigned short int K;
    unsigned char S[256];
    unsigned char w0;
    unsigned char w1;
    unsigned char w2;
    unsigned char w3;
    unsigned char w4;
    unsigned char w5;

    printf("\nEnter K: ");
    scanf("%i", &K);

    generateS(8);

    if(loadS(S)){

        unsigned char high = (K >> 8);
        unsigned char low = K;

        //K=k0,k1,k2,k3,...,k15
        w0 = high & 0xFF;
        w1 = low & 0xFF;

        unsigned char rw1 = (w1 << 4) | (w1 >> 4); //printf("Original: 0x%02X\n", w1);printf("Invertido: 0x%02X\n", rw1);
        w2=w0 ^ 0x80 ^ S[rw1];

        w3=w2 ^ w1;

        unsigned char rw3 = (w3 << 4) | (w3 >> 4); printf("Original: 0x%02X\n", w3);printf("Invertido: 0x%02X\n", rw3);
        w4=w2 ^ 0x30 ^ S[rw3];

        w5=w4 ^ w3;
    } else {
        printf("Failed to load S-Box.\n");
    }

    printf("\nExpanded Key:\n");
    printf("w0: %02X\n", w0);   
    printf("w1: %02X\n", w1);
    printf("w2: %02X\n", w2);
    printf("w3: %02X\n", w3);
    printf("w4: %02X\n", w4);
    printf("w5: %02X\n", w5);

    return;
}