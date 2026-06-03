import base64
import os
from Crypto.Random import get_random_bytes

def generateKey(n):
    key = base64.b64encode(get_random_bytes(n))
    return key

def saveKey(key):
    filename = input("Enter the file name + extension where the key will be stored: ")
    with open(filename, 'wb') as salida:
        salida.write(key)


def main():
    while True:
        print("\n---- AES KEY GENERATOR ----")
        size = input("Enter the size of the key in bits: ")
        
        match size:
            case '128':
                key = generateKey(16)
                saveKey(key)
                break
            case '192':
                key = generateKey(24)
                saveKey(key)
                break
            case '256':
                key = generateKey(32)
                saveKey(key)
                break
            case _:
                print("\nThe key must be one of the following sizes: 128-bit, 192-bit, 256-bit")
                
                
if __name__ == "__main__":
    main()