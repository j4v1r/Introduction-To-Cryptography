#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

void generateS(int l);
bool loadS(unsigned char S[256]);
void key_expansion(unsigned short int K, unsigned char S[256], unsigned char W[4]);
void generateKey();
void generateKeyAndSBox();
void cipher();

int main(){

    bool seguir = true;

    while(seguir){
        printf("\nLAB07 - Tiny Block Cipher\n");

        printf("1. Key Expansion\n");
        printf("2. Generate random key 'K' and S-Box(8))\n");
        printf("3. Cipher 'M' plaintext\n");
        printf("4. Exit\n");

        int option;
        printf("Select an option: ");
        scanf("%i", &option);

        int l;
        unsigned char W[4];
        unsigned char S[256];
        unsigned short int K;

        switch(option){
            case 1:
                printf("\nEnter K: ");
                scanf("%i", &K);
                loadS(S);
                key_expansion(K,S,W);
                break;
            case 2:
                generateKeyAndSBox();
                break;
            case 3:
                cipher();
                break;
            case 4:
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

//Loads an S-Box from a file into an array, returns true if successful, false otherwise
bool loadS(unsigned char S[256]){

    char filename[100];
    FILE *fptr;

    printf("Enter the file name where the S-Box is stored: ");
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

//Loads an 16-bit key 'K' from a file into a variable, returns true if successful, false otherwise
bool loadKey(unsigned short int *K){

    char filename[100];
    FILE *fptr;

    printf("Enter the file name where 'K' is stored: ");
    scanf("%99s", filename);

    fptr = fopen(filename, "r");
    if (!fptr) {
        printf("Error: No se pudo abrir el archivo %s\n", filename);
        return false;
    }

    if (fscanf(fptr, "%hx", K) != 1) {
        printf("Error: No se pudo leer la clave del archivo %s\n", filename);
        fclose(fptr);
        return false;
    }

    fclose(fptr);
    return true;
}

//Performs XOR operations on a short int 'K' with the substituted values of its nibbles using a 4-bit S-Box, and then substitutes the result using the same S-Box
void key_expansion(unsigned short int K, unsigned char S[256], unsigned char W[4]){

    unsigned char w0;
    unsigned char w1;
    unsigned char w2;
    unsigned char w3;
    unsigned char w4;
    unsigned char w5;

    unsigned char high = (K >> 8);
    unsigned char low = K;

    //K=k0,k1,k2,k3,...,k15
    w0 = high & 0xFF;
    w1 = low & 0xFF;

    unsigned char rw1 = (w1 << 4) | (w1 >> 4); 
    //printf("Original: 0x%02X\n", w1);
    //printf("Invertido: 0x%02X\n", rw1);
    w2=w0 ^ 0x80 ^ S[rw1];

    w3=w2 ^ w1;

    unsigned char rw3 = (w3 << 4) | (w3 >> 4); 
    //printf("Original: 0x%02X\n", w3);
    //printf("Invertido: 0x%02X\n", rw3);
    w4=w2 ^ 0x30 ^ S[rw3];

    w5=w4 ^ w3;

    // Store the expanded key values in the W array
    W[0] = w2;
    W[1] = w3;
    W[2] = w4;
    W[3] = w5;

    
    printf("\nExpanded Key:\n");
    printf("w0: %02X\n", w0);   
    printf("w1: %02X\n", w1);
    printf("w2: %02X\n", w2);
    printf("w3: %02X\n", w3);
    printf("w4: %02X\n", w4);
    printf("w5: %02X\n", w5);

    return;
}

//Generates a random 16-bit key and stores it in a file
void generateKey(){

    FILE *fptr;
    
    srand(time(NULL));
    unsigned short int K;
    K = ((unsigned short)rand() << 8) | (rand() & 0xFF);
    
    char filename[100];
    printf("Enter the file name to store the key: ");
    scanf("%99s", filename);

    fptr = fopen(filename, "w");

    if(fptr==NULL){
        printf("Failed to open file\n");
        exit(1);
    }

    fprintf(fptr, "%04X\n", K);
    printf("Key stored in %s\n", filename);

    fclose(fptr);
}

//Calls generateKey() to create a random key 'K' and store it in a file, then calls generateS() to create a random S-Box of l=8and store it in a file
void generateKeyAndSBox(){
    generateKey();
    generateS(8);
}

//Ciphers a plaintext 'M' using the expanded key generated from a random key 'K' and a random S-Box of l=8, then prints the ciphertext
void cipher(){

    unsigned short int K;
    unsigned short int KEYS[3];

    unsigned char W[4];
    unsigned char S[256];
    
    char M[3];
    char C[2];
    unsigned char m0;
    unsigned char m1;

    printf("\nEnter M(2 ascii chars): ");
    scanf("%2s", M);
    m0 = M[0];
    m1 = M[1];

    if(loadS(S) && loadKey(&K)){

        key_expansion(K,S,W);

        KEYS[0] = K;
        KEYS[1] = (W[0] << 8) | W[1];
        KEYS[2] = (W[2] << 8) | W[3];

        for(int j=0; j<=2; j++){
            m0 = m0 ^ (KEYS[j] >> 8);
            m1 = m1 ^ (KEYS[j] & 0xFF);
            m0 = S[m0];
            m1 = S[m1];
        }

        C[0] = m0;
        C[1] = m1;

        printf("\nCiphertext: %02X%02X\n", (unsigned char)C[0], (unsigned char)C[1]);

    } else {
        printf("Failed to load S-Box or Key.\n");
    }   
}
