import random

def RSA_encrypt():
    print("Enter the public key:")
    e = int(input("e: "))
    n = int(input("n: "))
    r = random.getrandbits(16) 
    print(f"Random message (r): {r}")
    c = pow(r, e, n)
    return c

def main():
    print("\n---- LAB13 - Schoolbook RSA Encrypt ----")
    
    while True:
        try:
            ciphertext = RSA_encrypt()
            print("Ciphertext:", ciphertext)
            break
        except:
            print("Please enter a valid public key.")

if __name__ == "__main__":
    main()