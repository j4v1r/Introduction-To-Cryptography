#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void generateS(int l);
void base64Encode(unsigned char input[], char output[]);
int base64Value(char c);
void generateKey();
void generateKeyAndSBox();
bool loadS(unsigned char S[256]);
bool loadKey(unsigned short int *K);
bool loadPermutation(int perm[8]);
void key_expansion(unsigned short int K, unsigned char S[256], unsigned char W[4]);
void generatePermutation();
unsigned char permutate(unsigned char s, int P[8]);
unsigned short int TBC(unsigned short int K, char M[3]);
void CTR_E(unsigned short int K);
void CTR_D();


int main(){

    bool seguir = true;

    while(seguir){
        printf("\nTBC16-CTR\n");

        printf("1. Generate random key 'K'\n");
        printf("2. Cipher plaintext\n");
        printf("3. Decipher ciphertext\n");
        printf("4. Exit\n");

        int option;
        printf("Select an option: ");
        scanf("%i", &option);

        srand(time(NULL));
        unsigned short int K;

        switch(option){
            case 1:
                generateKeyAndSBox();
                break;
            case 2:
                loadKey(&K);
                CTR_E(K);
                break;
            case 3:
                break;
            case 4:
                seguir=false;
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
    char filename[100]="JACA_sbox.txt";

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

//Encodes in base-64 the random generated key 'K'
void base64Encode(unsigned char input[], char output[]) {

    int i, j = 0, length=2;

    for (i = 0; i < length; i += 3) {

        unsigned int value = input[i] << 16;

        if (i + 1 < length)
            value |= input[i + 1] << 8;

        if (i + 2 < length)
            value |= input[i + 2];

        output[j++] = base64_table[(value >> 18) & 0x3F];
        output[j++] = base64_table[(value >> 12) & 0x3F];

        if (i + 1 < length)
            output[j++] = base64_table[(value >> 6) & 0x3F];
        else
            output[j++] = '=';

        if (i + 2 < length)
            output[j++] = base64_table[value & 0x3F];
        else
            output[j++] = '=';
    }

    output[j] = '\0';
}

//Decodes in base-64 the loaded key from the file 'KEY.txt'
int base64Value(char c) {

    if (c >= 'A' && c <= 'Z')
        return c - 'A';

    if (c >= 'a' && c <= 'z')
        return c - 'a' + 26;

    if (c >= '0' && c <= '9')
        return c - '0' + 52;

    if (c == '+')
        return 62;

    if (c == '/')
        return 63;

    return -1;
}

//Generates a random 16-bit key and stores it in a file named 'KEY.txt'
void generateKey() {

    FILE *fptr;

    srand((unsigned int)time(NULL));

    unsigned short int K;

    K = ((unsigned short int)(rand() & 0xFF) << 8) |
        (unsigned short int)(rand() & 0xFF);

    unsigned char keyBytes[2];

    keyBytes[0] = (K >> 8) & 0xFF;
    keyBytes[1] = K & 0xFF;

    char encoded[10];

    base64Encode(keyBytes, encoded);

    fptr = fopen("KEY.txt", "w");

    if (fptr == NULL) {
        printf("Failed to open file\n");
        exit(1);
    }

    fprintf(fptr, "%s\n", encoded);

    fclose(fptr);

    //printf("Generated key (HEX): %04X\n", K);
    //printf("Base64 stored in KEY.txt: %s\n", encoded);
}

//Calls generateKey() to create a random key 'K' and store it in a file, then calls generateS() to create a random S-Box of l=8and store it in a file
void generateKeyAndSBox(){
    generateS(8);
    generateKey();
}

//Loads an S-Box from a file into an array, returns true if successful, false otherwise
bool loadS(unsigned char S[256]){

    char filename[100]="JACA_sbox.txt";
    FILE *fptr;

    printf("Enter the file name where the S-Box is stored: ");
    scanf("%99s", filename);

    int entrada, salida;
    while (fscanf(fptr, " %x -> %x", &entrada, &salida) == 2) {
        if (entrada >= 0 && entrada < 256) {
            S[entrada] = (unsigned char)salida;
        }
    }

    fclose(fptr);

    return true;
}

//Loads an 16-bit key 'K' from a file named 'KEY.txt' into a variable, returns true if successful, false otherwise
bool loadKey(unsigned short int *K) {

    char filename[100];

    FILE *fptr;

    char encoded[10];

    unsigned char decoded[2];

    printf("Enter the file name where 'K' is stored: ");
    scanf("%99s", filename);

    fptr = fopen(filename, "r");

    if (!fptr) {
        printf("Error opening file %s\n", filename);
        return false;
    }

    if (fscanf(fptr, "%9s", encoded) != 1) {
        printf("Error reading Base64\n");
        fclose(fptr);
        return false;
    }

    fclose(fptr);

    int b0 = base64Value(encoded[0]);
    int b1 = base64Value(encoded[1]);
    int b2 = base64Value(encoded[2]);

    decoded[0] = (b0 << 2) | (b1 >> 4);
    decoded[1] = ((b1 & 0x0F) << 4) | (b2 >> 2);

    *K = ((unsigned short int)decoded[0] << 8) |
          decoded[1];

    //printf("Loaded key (HEX): %04X\n", *K);

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
unsigned short int TBC(unsigned short int K, char M[3]){

    int perm[8] = {4,1,7,0,6,2,5,3};
    unsigned short int KEYS[3];

    unsigned char W[4];
    unsigned char S[256];
    
    unsigned char C[2];

    unsigned char m0 = (unsigned char)M[0];
    unsigned char m1 = (unsigned char)M[1];

    if(loadS(S)){

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

        //printf("\nCiphertext: %02X%02X\n", (unsigned char)C[0], (unsigned char)C[1]);
        //printf("\nCiphertext (as ascii): %c%c\n", C[0], C[1]);
        return (C[0] << 8) | C[1];

    } else {
        printf("Failed to load S-Box or Key.\n");
    }   
}

//Ciphers a plaintext obtained from a file given by the user using a key 'K' given by the user with the CTR Mode of Operation 
void CTR_E(unsigned short int K){
    return;
}

//Deciphers a ciphertext obtained from a file given by the user using the counter used for the CTR Mode of Operation ciphering
void CTR_D(){
    return;
}