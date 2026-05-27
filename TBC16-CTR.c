#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void generateS(int l);
void base64Encode(unsigned char input[], int length, char output[]);
int base64Decode(char input[], unsigned char output[]);
int base64Value(char c);
void generateKey();
void generateKeyAndSBox();
bool loadS(unsigned char S[256]);
bool loadKey(char *filename, unsigned short int *K);
bool loadPermutation(int perm[8]);
char *readText(char *filename, int *output_size);
void key_expansion(unsigned short int K, unsigned char S[256], unsigned char W[4]);
void generatePermutation();
unsigned char permutate(unsigned char s, int P[8]);
unsigned short int TBC(unsigned short int K, char M[2]);
void CTR_E(char *TextFile, char *KeyFile);
void CTR_D(char *TextFile, char *KeyFile);
void saveFile(unsigned char *text, int length, char *filename, int mode);

int main()
{

    bool seguir = true;
    char KeyFile[100];
    char TextFile[100];

    while (seguir)
    {
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

        switch (option)
        {
        case 1:
            generateKeyAndSBox();
            break;
        case 2:
            printf("Enter the file name where the key is stored: ");
            scanf("%s", KeyFile);
            printf("Enter the file name you'd like to encrypt: ");
            scanf("%s", TextFile);
            CTR_E(TextFile, KeyFile);
            break;
        case 3:
            printf("Enter the file name where the key is stored: ");
            scanf("%s", KeyFile);
            printf("Enter the file name you'd like to decrypt: ");
            scanf("%s", TextFile);
            CTR_D(TextFile, KeyFile);
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

// Generates a S-Bx of size l and stores it in a file, then performs key expansion using the generated S-Box
void generateS(int l)
{

    FILE *fptr;
    srand(time(NULL));
    int size = 1 << l;
    int S[size];
    char filename[100] = "JACA_sbox.txt";

    for (int i = 0; i < size; i++)
    {
        S[i] = i;
    }

    // Algoritmo de Fisher-Yates para mezclar el arreglo S
    for (int i = size - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);

        int temp = S[i];
        S[i] = S[j];
        S[j] = temp;
    }

    fptr = fopen(filename, "w");

    if (fptr == NULL)
    {
        printf("Failed to open file\n");
        exit(1);
    }

    printf("S-Box table stored in %s\n", filename);

    for (int i = 0; i < size; i++)
    {
        fprintf(fptr, "%X -> %X\n", i, S[i]);
    }

    fclose(fptr);
}

// Decodes in base-64 the random generated key 'K'
void base64Encode(unsigned char input[], int length, char output[])
{
    int i, j = 0;

    for (i = 0; i < length; i += 3)
    {

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

// Encodes in base-64 the random generated key 'K'
int base64Decode(char input[], unsigned char output[])
{
    int i, j = 0;
    int length = strlen(input);
    int val[4];
    unsigned int value;

    for (i = 0; i < length; i += 4)
    {

        val[0] = base64Value(input[i]);
        val[1] = base64Value(input[i + 1]);
        val[2] = (input[i + 2] == '=') ? 0 : base64Value(input[i + 2]);
        val[3] = (input[i + 3] == '=') ? 0 : base64Value(input[i + 3]);

        value = (val[0] << 18) | (val[1] << 12) | (val[2] << 6) | val[3];

        output[j++] = (value >> 16) & 0xFF;

        if (input[i + 2] != '=')
            output[j++] = (value >> 8) & 0xFF;

        if (input[i + 3] != '=')
            output[j++] = value & 0xFF;
    }

    return j;
}

// Decodes in base-64 the loaded key from the file 'KEY.txt'
int base64Value(char c)
{

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

// Generates a random 16-bit key and stores it in a file named 'KEY.txt'
void generateKey()
{

    FILE *fptr;

    srand((unsigned int)time(NULL));

    unsigned short int K;

    K = ((unsigned short int)(rand() & 0xFF) << 8) |
        (unsigned short int)(rand() & 0xFF);

    unsigned char keyBytes[2];

    keyBytes[0] = (K >> 8) & 0xFF;
    keyBytes[1] = K & 0xFF;

    char encoded[10];

    base64Encode(keyBytes, 2, encoded);

    fptr = fopen("KEY.txt", "w");

    if (fptr == NULL)
    {
        printf("Failed to open file\n");
        exit(1);
    }

    fprintf(fptr, "%s\n", encoded);

    fclose(fptr);

    printf("Generated key stored in KEY.txt\n");
}

// Calls generateKey() to create a random key 'K' and store it in a file, then calls generateS() to create a random S-Box of l=8and store it in a file
void generateKeyAndSBox()
{
    generateS(8);
    generateKey();
}

// Loads an S-Box from a file into an array, returns true if successful, false otherwise
bool loadS(unsigned char S[256])
{

    char filename[100] = "JACA_sbox.txt";
    FILE *fptr;

    fptr = fopen(filename, "r");

    int entrada, salida;
    while (fscanf(fptr, " %x -> %x", &entrada, &salida) == 2)
    {
        if (entrada >= 0 && entrada < 256)
        {
            S[entrada] = (unsigned char)salida;
        }
    }

    fclose(fptr);

    return true;
}

// Loads an 16-bit key 'K' from a file named 'KEY.txt' into a variable, returns true if successful, false otherwise
bool loadKey(char *filename, unsigned short int *K)
{

    FILE *fptr;

    char encoded[10];

    unsigned char decoded[2];

    fptr = fopen(filename, "r");

    if (!fptr)
    {
        printf("Error opening file %s\n", filename);
        return false;
    }

    if (fscanf(fptr, "%9s", encoded) != 1)
    {
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

    *K = ((unsigned short int)decoded[0] << 8) | decoded[1];

    return true;
}

// Reads the text of a file given by the user.
char *readText(char *filename, int *output_size)
{

    FILE *fptr = fopen(filename, "rb");
    if (!fptr)
    {
        printf("Error opening file %s\n", filename);
        return false;
    }

    int capacity = 1024;
    int size = 0;
    char *buffer = malloc(capacity);

    char temp[512];
    int read;

    while ((read = fread(temp, 1, 512, fptr)) > 0)
    {
        if (size + read >= capacity)
        {
            capacity *= 2;
            buffer = realloc(buffer, capacity);
        }
        for (int i = 0; i < read; i++)
        {
            buffer[size++] = temp[i];
        }
    }

    fclose(fptr);
    *output_size = size;
    return buffer;
}

// Performs XOR operations on a short int 'K' with the substituted values of its nibbles using a 4-bit S-Box, and then substitutes the result using the same S-Box
void key_expansion(unsigned short int K, unsigned char S[256], unsigned char W[4])
{

    unsigned char w0;
    unsigned char w1;
    unsigned char w2;
    unsigned char w3;
    unsigned char w4;
    unsigned char w5;

    unsigned char high = (K >> 8);
    unsigned char low = K;

    // K=k0,k1,k2,k3,...,k15
    w0 = high & 0xFF;
    w1 = low & 0xFF;

    unsigned char rw1 = (w1 << 4) | (w1 >> 4);
    w2 = w0 ^ 0x80 ^ S[rw1];

    w3 = w2 ^ w1;

    unsigned char rw3 = (w3 << 4) | (w3 >> 4);
    w4 = w2 ^ 0x30 ^ S[rw3];

    w5 = w4 ^ w3;

    // Store the expanded key values in the W array
    W[0] = w2;
    W[1] = w3;
    W[2] = w4;
    W[3] = w5;

    return;
}

// Permutes the bits of a byte 'x' according to a given permutation 'P' and returns the permuted byte
unsigned char permutate(unsigned char s, int P[8])
{
    unsigned char y = 0;

    for (int i = 0; i < 8; i++)
    {
        unsigned char bit = (s >> P[i]) & 1;
        y |= (bit << i);
    }

    return y;
}

// Ciphers a plaintext 'M' using the expanded key generated from a random key 'K' and a random S-Box of l=8, then prints the ciphertext
unsigned short int TBC(unsigned short int K, char M[3])
{

    int perm[8] = {4, 1, 7, 0, 6, 2, 5, 3};
    unsigned short int KEYS[3];

    unsigned char W[4];
    unsigned char S[256];

    unsigned char C[2];

    unsigned char m0 = (unsigned char)M[0];
    unsigned char m1 = (unsigned char)M[1];

    if (loadS(S))
    {

        key_expansion(K, S, W);

        KEYS[0] = K;
        KEYS[1] = (W[0] << 8) | W[1];
        KEYS[2] = (W[2] << 8) | W[3];

        for (int j = 0; j <= 2; j++)
        {
            m0 = m0 ^ (KEYS[j] >> 8);
            m1 = m1 ^ (KEYS[j] & 0xFF);

            m0 = S[m0];
            m1 = S[m1];

            m0 = permutate(m0, perm);
            m1 = permutate(m1, perm);
        }

        C[0] = m0;
        C[1] = m1;

        return (C[0] << 8) | C[1];
    }
    else
    {
        printf("Failed to load S-Box or Key.\n");
    }
}

// Ciphers a plaintext obtained from a file given by the user using a key 'K' given by the user with the CTR Mode of Operation
void CTR_E(char *TextFile, char *KeyFile)
{
    char filename[100];
    int len = 0;
    char *plaintext = readText(TextFile, &len);

    unsigned char c0 = rand() % 256;
    unsigned char c1 = 0;
    unsigned short int enc, K;

    unsigned char *ciphertext = malloc(len + 1);
    ciphertext[0] = c0;

    loadKey(KeyFile, &K);

    int aux = 1;
    for (int i = 0; i < len; i += 2)
    {
        unsigned char counter[2];
        counter[0] = c0;
        counter[1] = c1;

        enc = TBC(K, counter);
        unsigned char msb = (enc >> 8) & 0xFF;
        unsigned char lsb = enc & 0xFF;

        ciphertext[aux++] = plaintext[i] ^ msb;

        if (i + 1 < len)
        {
            ciphertext[aux++] = plaintext[i + 1] ^ lsb;
        }

        c1++;
    }

    printf("Enter the name of the file where the ciphertext will be stored: ");
    scanf("%s", filename);

    saveFile(ciphertext, aux, filename, 0);

    printf("Succesfully encripted in %s.\n", filename);

    free(plaintext);
    free(ciphertext);
    return;
}

// Deciphers a ciphertext obtained from a file given by the user using the counter used for the CTR Mode of Operation ciphering
void CTR_D(char *TextFile, char *KeyFile)
{
    char filename[100];
    unsigned char c0, c1 = 0;
    unsigned short int enc, K;
    int inputLen = 0;

    char *input = readText(TextFile, &inputLen);
    unsigned char *ciphertext = malloc(inputLen);
    int totalBytes = base64Decode((char *)input, ciphertext);

    c0 = ciphertext[0];

    unsigned char *plaintext = malloc(totalBytes);

    loadKey(KeyFile, &K);

    int aux = 0;

    for (int i = 1; i < totalBytes; i += 2)
    {
        unsigned char counter[2];
        counter[0] = c0;
        counter[1] = c1;

        enc = TBC(K, (char *)counter);
        unsigned char msb = (enc >> 8) & 0xFF;
        unsigned char lsb = enc & 0xFF;

        plaintext[aux++] = ciphertext[i] ^ msb;

        if (i + 1 < totalBytes)
        {
            plaintext[aux++] = ciphertext[i + 1] ^ lsb;
        }

        c1++;
    }

    plaintext[aux] = '\0';

    printf("Enter the name of the file where the plaintext will be stored: ");
    scanf("%s", filename);

    saveFile(plaintext, aux, filename, 1);

    printf("Succesfully decripted in %s.\n", filename);

    free(input);
    free(ciphertext);
    free(plaintext);
    return;
}

// Stores the corresponding text in the file specified by the user
void saveFile(unsigned char *text, int length, char *filename, int mode)
{
    FILE *fptr = fopen(filename, "wb");

    if (fptr == NULL)
    {
        printf("Failed to open file\n");
        exit(1);
    }

    // Ciphertext -> Mode = 0
    if (mode == 0)
    {
        int encodedSize = (length * 4 / 3) + 4;
        char *encoded = malloc(encodedSize);

        base64Encode(text, length, encoded);
        fwrite(encoded, 1, strlen(encoded), fptr);
        free(encoded);
    }
    // Plaintext -> Mode = 1
    else if (mode == 1)
    {
        fwrite(text, 1, length, fptr);
    }

    fclose(fptr);
}