#LAB03.py - Permutation Cipher
#March 5, 2026
#Authors: Colunga Aguilar Javier Alejandro
#         Valencia Rojas Berenice

import math
import random

from LAB02 import keyGeneration

def random_perm(n):
    
    pi = []
    exists=False
    
    for i in range(1, n+1):
        
        while(not exists):
            
            k=random.randrange(1, n+1)
         
            if k not in pi:
                pi.append(k)
                exists=True
                
        exists=False
    
    return pi
    

def inv_perm(pi):
    
    inv_pi = [0] * len(pi)
    
    for i in range(len(pi)):
        inv_pi[pi[i]-1] = i + 1
        
    return inv_pi


def perm_cipher(pi, fuente):
    
    fuente = fuente.lstrip()
    
    block_size = len(pi)
    
    while len(fuente)%block_size != 0:
        fuente += "&"

    ciphertext = ""
    for i in range(0, len(fuente), block_size):
        block = fuente[i:i+block_size]
        perm_block = ""
        for j in range(block_size):
            if(block[pi[j]-1] != " "):
                perm_block += block[pi[j]-1]
            else:
                perm_block += "&"
            #perm_block += block[pi[j]-1]
        ciphertext += perm_block

    return ciphertext


def perm_decipher(pi, cifrado):
    
    inv_pi = inv_perm(pi)
    
    block_size = len(pi)
    
    plaintext = ""
    for i in range(0, len(cifrado), block_size):
        block = cifrado[i:i+block_size]
        perm_block = ""
        for j in range(block_size):
            if(block[inv_pi[j]-1] != "&"):
                perm_block += block[inv_pi[j]-1]
            else:
                perm_block += " "
        plaintext += perm_block

    return plaintext.strip()
    
def main():
    while True:
        print("---- LAB03 - PERMUTATION CIPHER ----")
        print("Select an option: \n1. Compute Random Permutation \n2. Compute Inverse Permutation \n3. Encrypt Message \n4. Decrypt Message \n5. Exit")

        option = input("Option: ")
        
        match option:
            case '1':
                while True:
                    try:
                        n = int(input("Enter a number greater than or equal to 3: "))
                        if (n >= 3):
                            break
                        else:
                            print("The number must be greater or equal to 3.")
                    except ValueError:
                        print("Please enter an integer.")
                
                permutation = random_perm(n)
                print("Random Permutation of size", n, ": ", permutation)

            case '2':
                while True:
                    try:
                        n = int(input("Enter a number greater than or equal to 3: "))
                        i=0
                        pi = []
                        if (n >= 3):
                            while(True):
                                try:
                                    k=int(input("Perm[" + str(i) + "]: "))
                            
                                    if (k <= 0 or k > n):
                                        print("The number must be between 1 and ", n)
                                    else:
                                        pi.append(k)
                                        i+=1
                                    
                                    if (i == n):
                                        break
                        
                                except ValueError:
                                    print("Please enter an integer.")
                        
                            break
                        else:
                            print("The number must be greater or equal to 3.")
                    except ValueError:
                        print("Please enter an integer.")
        
                inv_pi = inv_perm(pi)
                print("The inverse permutation of ", pi, " is:", inv_pi)
                
            case '3':
                while True:
                    try:
                        n = int(input("Enter the size of the permutation pi: "))
                        if (n >= 3):
                            i=0
                            pi = []
                            while(True):
                                try:
                                    k=int(input("Perm[" + str(i) + "]: "))
                            
                                    if (k <= 0 or k > n):
                                        print("The number must be between 1 and ", n)
                                    else:
                                        pi.append(k)
                                        i+=1
                                    
                                    if (i == n):
                                        break
                        
                                except ValueError:
                                    print("Please enter an integer.")
                        
                            break       
                        else:
                            print("The number must be greater or equal to 3.")
                    except ValueError:
                        print("Please enter an integer.")
                        
                while True:
                    fuente = input("Enter the message to cipher (English alphabet): ")
                    if all(c.isalpha() or c.isspace() for c in fuente):
                        break
                    else:
                        print("Please enter a message containing only letters and spaces.")
                        
                cifrado = perm_cipher(pi, fuente)
                print("Ciphered message: ", cifrado)
                
            case '4':
                while True:
                    try:
                        n = int(input("Enter the size of the permutation pi: "))
                        if (n >= 3):
                            i=0
                            pi = []
                            while(True):
                                try:
                                    k=int(input("Perm[" + str(i) + "]: "))
                            
                                    if (k <= 0 or k > n):
                                        print("The number must be between 1 and ", n)
                                    else:
                                        pi.append(k)
                                        i+=1
                                    
                                    if (i == n):
                                        break
                        
                                except ValueError:
                                    print("Please enter an integer.")
                        
                            break       
                        else:
                            print("The number must be greater or equal to 3.")
                    except ValueError:
                        print("Please enter an integer.")
                        
                while True:
                    fuente = input("Enter the message to decrypt (English alphabet): ")
                    if all(c.isalpha() or c.isspace() or c=='&' for c in fuente):
                        break
                    else:
                        print("Please enter a message containing only letters and spaces.")
                        
                decifrado = perm_decipher(pi, fuente)
                print("Deciphered message: ", decifrado)
                
            case '5':
                print("Exiting the program...")
                break

            case _:
                print("Invalid option. Please, enter an option from 1 to 6.")


if __name__ == "__main__":
    main()