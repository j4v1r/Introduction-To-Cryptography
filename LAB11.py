import os
import base64
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes

#Generate AES keys of 128, 192, and 256 bits
def generate_key(bits):
    """
    Generates AES key of:
    128 bits -> 16 bytes
    192 bits -> 24 bytes
    256 bits -> 32 bytes
    """

    if bits not in [128, 192, 256]:
        raise ValueError("AES supports only 128, 192, or 256 bits")

    key = os.urandom(bits // 8)

    print(f"\nAES-{bits} KEY (Base64):")
    print(base64.b64encode(key).decode())

    return key

def main():
    while True:
        print("---- LAB11 - AES-CTR ----")
        print("Select an option: ")
        print("\n1. Generate random key")
        print("\n2. Decode Base64 String to Binary")
        print("\n3. Generate Random Key")
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

                
                
if __name__ == "__main__":
    main()