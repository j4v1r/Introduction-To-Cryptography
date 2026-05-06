#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

void generateS(int l);
bool loadS(unsigned char S[256]);
bool loadKey(unsigned short int *K);
bool loadPermutation(int perm[8]);
void key_expansion(unsigned short int K, unsigned char S[256], unsigned char W[4]);
void generateKey();
void generatePermutation();
unsigned char permutate(unsigned char s, int P[8]);
void generateKeyAndSBox();
void cipher(unsigned char M[3]);
void decipher(unsigned short int C);

int main(){

    bool seguir = true;

    while(seguir){
        printf("\nLAB08 - Tiny Block Cipher II\n");

        printf("1. Key Expansion\n");
        printf("2. Generate random key 'K' and S-Box(8))\n");
        printf("3. Cipher 'M' plaintext\n");
        printf("4. Decipher 'C' ciphertext\n");
        printf("5. Exit\n");

        int option;
        printf("Select an option: ");
        scanf("%i", &option);

        int l;
        unsigned char M[3];
        unsigned char W[4];
        unsigned char S[256];
        unsigned short int C;
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
                printf("\nEnter M(2 ascii chars): ");
                scanf("%2s", (char*)M);
                cipher(M);
                break;
            case 4:
                printf("\nEnter C(hex): ");
                scanf("%hx", &C);
                decipher(C);
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

//Loads a permutation of size 8 from a file into an array, returns true if successful, false otherwise
bool loadPermutation(int perm[8]) {
    char filename[100];
    printf("Enter the file name where the permutation is stored: ");
    scanf("%99s", filename);
    FILE *f = fopen(filename, "r");
    if (!f){
        return false;
    }

    for (int i = 0; i < 8; i++) {
        if (fscanf(f, "%d", &perm[i]) != 1) {
            fclose(f);
            return false;
        }
    }

    fclose(f);
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

    /*
    printf("\nExpanded Key:\n");
    printf("w0: %02X\n", w0);   
    printf("w1: %02X\n", w1);
    printf("w2: %02X\n", w2);
    printf("w3: %02X\n", w3);
    printf("w4: %02X\n", w4);
    printf("w5: %02X\n", w5);*/

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

//Generates a random permutation of size 8 and stores it in an array, then prints the permutation
void generatePermutation() {
    int perm[8];
    char filename[100];

    for (int i = 0; i < 8; i++) {
        perm[i] = i;
    }

    for (int i = 7; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = perm[i];
        perm[i] = perm[j];
        perm[j] = temp;
    }

    printf("Enter the file name to store the permutation: ");
    scanf("%99s", filename);
    FILE *f = fopen(filename, "w");

    if (!f) {
        printf("Error opening file\n");
        return;
    }

    for (int i = 0; i < 8; i++) {
        fprintf(f, "%d ", perm[i]);
    }

    fclose(f);
    printf("Permutation stored in %s\n", filename);
}

//Calls generateKey() to create a random key 'K' and store it in a file, then calls generateS() to create a random S-Box of l=8and store it in a file
void generateKeyAndSBox(){
    generateS(8);
    generateKey();
    generatePermutation();
}

//Permutes the bits of a byte 'x' according to a given permutation 'P' and returns the permuted byte
unsigned char permutate(unsigned char s, int P[8]) {
    unsigned char y = 0;

    for (int i = 0; i < 8; i++) {
        unsigned char bit = (s >> P[i]) & 1; 
        y |= (bit << i);
    }

    return y;
}

//Ciphers a plaintext 'M' using the expanded key generated from a random key 'K' and a random S-Box of l=8, then prints the ciphertext
void cipher(unsigned char M[3]){

    int perm[8];
    unsigned short int K;
    unsigned short int KEYS[3];

    unsigned char W[4];
    unsigned char S[256];
    
    unsigned char C[2];

    unsigned char m0 = (unsigned char)M[0];
    unsigned char m1 = (unsigned char)M[1];

    if(loadS(S) && loadKey(&K) && loadPermutation(perm)){

        key_expansion(K,S,W);

        KEYS[0] = K;
        KEYS[1] = (W[0] << 8) | W[1];
        KEYS[2] = (W[2] << 8) | W[3];

        for(int j=0; j<=2; j++){
            m0 = m0 ^ (KEYS[j] >> 8);
            m1 = m1 ^ (KEYS[j] & 0xFF);
            //printf("\nRound %d\n", j);
           // printf("After XOR: %02X %02X\n", m0, m1);

            m0 = S[m0];
            m1 = S[m1];
            //printf("After S: %02X %02X\n", m0, m1);

            m0 = permutate(m0, perm);
            m1 = permutate(m1, perm);
            //printf("After P: %02X %02X\n", m0, m1);
        }

        C[0] = m0;
        C[1] = m1;

        printf("\nCiphertext: %02X%02X\n", (unsigned char)C[0], (unsigned char)C[1]);
        //printf("\nCiphertext (as ascii): %c%c\n", C[0], C[1]);

    } else {
        printf("Failed to load S-Box or Key.\n");
    }   
}

//Deciphers a ciphertext 'C' using the expanded key generated from a random key 'K' and a random S-Box of l=8, then prints the plaintext
void decipher(unsigned short int C){

    int perm[8];
    int perm_inv[8];
    unsigned short int K;
    unsigned short int KEYS[3];

    unsigned char W[4];
    unsigned char S[256];
    unsigned char S_INV[256];
    
    char M[3];

    unsigned char c0 = C>>8;
    unsigned char c1 = C&0xFF;

    if(loadS(S) && loadKey(&K) && loadPermutation(perm)){

        key_expansion(K,S,W);

        KEYS[0] = K;
        KEYS[1] = (W[0] << 8) | W[1];
        KEYS[2] = (W[2] << 8) | W[3];

        for(int i=0; i<256; i++){
            S_INV[S[i]] = i;
        }

        for(int i = 0; i < 8; i++) {
            perm_inv[perm[i]] = i;
        }

        for(int j=2; j>=0; j--){
            c0 = permutate(c0, perm_inv);
            c1 = permutate(c1, perm_inv);
            c0 = S_INV[c0];
            c1 = S_INV[c1];
            c0 = c0 ^ (KEYS[j] >> 8);
            c1 = c1 ^ (KEYS[j] & 0xFF);
        }

        M[0] = c0;
        M[1] = c1;
        M[2] = '\0';

        printf("\nPlaintext: %s\n", M); 

    } else {
        printf("Failed to load S-Box or Key.\n");
    }   
}

