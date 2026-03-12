import base64
from unittest import case




from Crypto.Util.Padding import unpad
from Crypto.Cipher import DES

def bin_b64(bin_string):
    
    byte_data = int(bin_string, 2).to_bytes((len(bin_string) + 7) // 8, byteorder='big')
    
    b64 = base64.b64encode(byte_data)
    
    return b64.decode()


def b64_bin(base64_string):
    
    byte_data = base64.b64decode(base64_string)
    
    binary_string = ''.join(format(byte, '08b') for byte in byte_data)
    
    return binary_string


def decrypt_file(key, fuente, destino):
    
    with open(fuente, 'rb') as f:
        encoded = f.read()
    
    data = base64.b64decode(encoded)
    
    iv = data[:8]
    ciphertext = data[8:]
    
    cipher = DES.new(key, DES.MODE_CBC, iv=iv)
    
    plaintext = unpad(cipher.decrypt(ciphertext), DES.block_size)
    
    with open(destino, 'wb') as f:
        f.write(plaintext)
    
    print("File decrypted successfully.")

def main():
    while True:
        print("---- LAB04 - Block Ciphers 1 ----")
        print("Select an option: \n1. Encode Binary String to Base64 \n2. Decode Base64 String to Binary \n3. Generate Random Key")
        print("4. Encrypt File \n5. Decrypt File \n6. Exit")

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
                key = generateKey()
                print("The random generated key is: ", key)

            case '4':
                while True:
                    fuente = input("Enter the source file name + extension: ")
                    try:
                        with open(fuente, 'r') as f:
                            f.close()
                            try:
                                key = input("Enter the key to encrypt the file: ")
                            except Exception as e:
                                print("An error occurred during encryption:", str(e))
                            break
                    except FileNotFoundError:
                        print("The file doesn't exist. Try again.")

                destino = input("Enter the destination file name + extension: ")
                
                encrypt_file(fuente, destino, key)
                print("File encrypted successfully on", destino)
                
            case '5':
                while True:
                    fuente = input("Enter the source file name + extension: ")
                    try:
                        with open(fuente, 'r') as f:
                            f.close()
                            try:
                                key = input("Enter the key to encrypt the file: ")
                            except Exception as e:
                                print("An error occurred during encryption:", str(e))
                            break
                    except FileNotFoundError:
                        print("The file doesn't exist. Try again.")

                destino = input("Enter the destination file name + extension: ")
                decrypt_file(key, fuente, destino)
                print("File decrypted successfully on", destino)
                
            case '6':
                print("Exiting the program...")
                break
            
            case _:
                print("Invalid option. Please, enter an option from 1 to 6.")
                
                
if __name__ == "__main__":
    main()