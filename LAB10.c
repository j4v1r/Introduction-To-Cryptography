#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define IRRED 0x002B

unsigned short int xtimesfx(unsigned short int f);
unsigned short int gf_mult(unsigned short int f, unsigned short int g);

int main(){

    bool seguir = true;

    while(seguir){
        printf("\nLAB10 - Multiplication on GF(2^n)\n");

        printf("1. Multiply x*f(x), f(x)->GF(2^16)\n");
        printf("2. Multiply f(x)*g(x), f(x), g(x)->GF(2^16)\n");
        printf("3. Exit\n");

        int option;
        printf("Select an option: ");
        scanf("%i", &option);

        unsigned short int f,g,result;

        switch(option){
            case 1:
                printf("Enter f(x) in hexadecimal: ");
                scanf("%hx", &f);
                result = xtimesfx(f);
                printf("Result of x*%04X mod %04X: 0x%04X\n", f, IRRED, result);
                break;
            case 2:
                printf("Enter f(x) in hexadecimal: ");
                scanf("%hx", &f);
                printf("Enter g(x) in hexadecimal: ");
                scanf("%hx", &g);
                result = gf_mult(f, g);
                printf("Result of %04X*%04X mod %04X: 0x%04X\n", f, g, IRRED, result);
                break;
            case 3:
                seguir = false;
                break;
            default:
                printf("Invalid option.\n");
                break;
        }
    }

    return 0;
}

unsigned short int xtimesfx(unsigned short int f){
    
    unsigned short int carry = f & 0x8000;

    f <<= 1;
    //f &= 0xFFFF;

    if(carry){
        f ^= IRRED;
    }
    return f;
}

unsigned short int gf_mult(unsigned short int f, unsigned short int g){
    unsigned short int result = 0;

    for(int i=0; i<16; i++){
        
        if(g & 1){
            result ^= f;
        }

        unsigned short int carry = f & 0x8000;

        f <<= 1;
        //f &= 0xFFFF;
        
        if (carry) {
            f ^= IRRED;
        }

        g >>= 1;
    }

    return result;
}
