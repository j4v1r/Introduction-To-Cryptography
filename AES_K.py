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

def main():
    print("\n---- LAB11 - AES-CTR KEY GENERATION ----")
        
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
                

if __name__ == "__main__":
    main()