from fileinput import filename
import os
import base64
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes

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
    print("\n---- LAB11 - AES-CTR DECRYPT ----")
        
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
                
if __name__ == "__main__":
    main()