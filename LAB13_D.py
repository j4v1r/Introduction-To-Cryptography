import random

def RSA_decrypt():
    print("Enter the private key:")
    d = int(input("d: "))
    n = int(input("n: "))
    print("Enter the ciphertext:")
    c = int(input("c: "))
    p = pow(c, d, n)
    return p

def main():
    print("\n---- LAB13 - Schoolbook RSA Decrypt ----")
    
    while True:
        try:
            plaintext = RSA_decrypt()
            print("Recovered plaintext:", plaintext)
            break
        except:
            print("Please enter a valid private key.")

if __name__ == "__main__":
    main()