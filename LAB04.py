import base64
from unittest import case

def bin_b64(bin_string):
    
    byte_data = int(bin_string, 2).to_bytes((len(bin_string) + 7) // 8, byteorder='big')
    
    b64 = base64.b64encode(byte_data)
    
    return b64.decode()


def b64_bin(base64_string):
    
    byte_data = base64.b64decode(base64_string)
    
    binary_string = ''.join(format(byte, '08b') for byte in byte_data)
    
    return binary_string



def main():
    while True:
        print("---- LAB03 - PERMUTATION CIPHER ----")
        print("Select an option: \n1. Encode Binary String to Base64 \n2. Decode Base64 String to Binary \n3. Encrypt Message \n4. Decrypt Message \n5. Exit")

        option = input("Option: ")
        
        match option:
            case '1':
                while True:
                    try:
                        n = input("Enter a binary string of 8 bits: ")
                        if (len(n) == 8):
                            break
                        else:
                            print("The binary string must be 8 bits long.")
                    except ValueError:
                        print("Please enter only 0s and 1s.")
                
                base64 = bin_b64(n)
                print("The binary string", n, "encoded in base 64 is:", base64)


            case '2':
                while True:
                    try:
                        n = input("Enter a string in base 64: ")
                        if (len(n) % 4 == 0):
                            break
                        else:
                            print("The base 64 string must be a multiple of 4 characters long.")
                    except ValueError:
                        print("Please enter a valid base 64 string.")
                
                binary = b64_bin(n)
                print("The binary string of", n, "encoded in base 64 is:", binary)
                
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
                print("Exiting the program...")
                break
            
            case _:
                print("Invalid option. Please, enter an option from 1 to 6.")
                
                
if __name__ == "__main__":
    main()