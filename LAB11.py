from fileinput import filename
import os
import base64
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes

#Generate AES keys of 128, 192, and 256 bits
def generate_key(bits, filename):
    """
    Generates AES key of:
    128 bits -> 16 bytes
    192 bits -> 24 bytes
    256 bits -> 32 bytes
    """

    if bits not in [128, 192, 256]:
        raise ValueError("AES supports only 128, 192, or 256 bits")

    key = os.urandom(bits // 8)

    key_b64 = base64.b64encode(key).decode()

    with open(filename, "w") as f:
        f.write(key_b64)

    #print(f"AES-{bits} key stored in '{filename}'")

#Ciphertext is generated using AES-CTR mode with a random IV (nonce) and the provided key. The IV and ciphertext are printed in Base64 encoding for easy handling.
def encrypt_ctr(plaintext_file, key, ciphertext_file):
    """
    Encrypts plaintext using AES-CTR
    Random IV (nonce) is generated automatically
    """
    
    # Read plaintext from file on binary mode
    with open(plaintext_file, "rb") as f:
        plaintext = f.read()

    # CTR mode uses 16-byte nonce/counter
    iv = os.urandom(16)

    cipher = Cipher(
        algorithms.AES(key),
        modes.CTR(iv)
    )

    encryptor = cipher.encryptor()

    ciphertext = encryptor.update(
        plaintext
    ) + encryptor.finalize()


    # Save ciphertext to file
    with open(ciphertext_file, "w") as f:
        f.write(base64.b64encode(iv + ciphertext).decode())

#Decryption is performed using the same key and IV (nonce) that were used for encryption. The ciphertext is decoded from Base64, and the plaintext is recovered and printed.
def decrypt_ctr(ciphertext_file, key, plaintext_file):

    # Read ciphertext from file
    with open(ciphertext_file, "r") as f:
        source = base64.b64decode(f.read())

    # Obtains the 16-byte nonce/counter used in CTR mode
    iv = source[:16]
    ciphertext = source[16:]

    cipher = Cipher(algorithms.AES(key), modes.CTR(iv))

    decryptor = cipher.decryptor()

    plaintext = decryptor.update(ciphertext) + decryptor.finalize()

    #Save recovered plaintext to file
    with open(plaintext_file, "wb") as f:
        f.write(plaintext)                         
    

def main():
    while True:
        print("\n---- LAB11 - AES-CTR ----")
        print("\nSelect an option: \n1. Generate random key\n2. Encrypt File \n3. Decrypt File \n4. Exit")

        option = input("Option: ")
        
        match option:
            case '1':
                while True:
                    try:
                        n = input("Enter the key size (128, 192, or 256): ")
                        n = int(n)
                        if n in [128, 192, 256]:
                            name = input("Enter a name for the key file: ")
                            break
                        else:
                            print("Please enter a valid key size (128, 192, or 256).")
                    except ValueError:
                        print("Please enter a valid integer.")
                        
                generate_key(n, name)
                
            case '2':
                while True:
                    try:
                        key_file = input("Enter the file containing the AES key: ")
                        with open(key_file, "r") as f:
                            key_b64 = f.read().strip()
                        key = base64.b64decode(key_b64)
                        plaintext_file = input("Enter the plaintext file name: ")
                        ciphertext_file = input("Enter the name for the output ciphertext file: ")
                        break
                    except (FileNotFoundError, ValueError):
                        print("Please enter a valid key file.")
                        
                encrypt_ctr(plaintext_file, key, ciphertext_file)
                
            case '3':  
                while True:
                    try:
                        key_file = input("Enter the file containing the AES key: ")
                        with open(key_file, "r") as f:
                            key_b64 = f.read().strip()
                        key = base64.b64decode(key_b64)
                        ciphertext_file = input("Enter the ciphertext file name: ")
                        plaintext_file = input("Enter the name for the output plaintext: ")
                        break
                    except (FileNotFoundError, ValueError):
                        print("Please enter a valid key file.")
                        
                decrypt_ctr(ciphertext_file, key, plaintext_file)
            case '4':
                print("Exiting...")
                break
                    
                
                
if __name__ == "__main__":
    main()