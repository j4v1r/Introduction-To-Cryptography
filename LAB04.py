import base64

def bin_b64(binary_string):
    
    byte_data = int(binary_string, 2).to_bytes((len(binary_string) + 7) // 8, byteorder='big')
    
    # encode to base64
    b64 = base64.b64encode(byte_data)
    
    return b64.decode()


def b64_to_bin(base64_string):
    # decode base64 to bytes
    byte_data = base64.b64decode(base64_string)
    
    # convert bytes to binary string
    binary_string = ''.join(format(byte, '08b') for byte in byte_data)
    
    return binary_string



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
                        case '5':
                print("Exiting the program...")
                break

            case '4':
                print("Exiting the program...")
                break
            
            case _:
                print("Invalid option. Please, enter an option from 1 to 6.")
                
                
if __name__ == "__main__":
    main()