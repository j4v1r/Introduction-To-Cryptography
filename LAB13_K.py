import math
from Crypto.Util import number

def generate_RSA_pair(num_bits):

    if num_bits not in [32, 512, 2048]:
        raise ValueError("Number of bits must be 32, 512, or 2048.")

    p = number.getPrime(num_bits)
    q = number.getPrime(num_bits)
    n = p * q
    phi = (p - 1) * (q - 1)

    while True:
        e = number.getRandomRange(2, phi)
        if math.gcd(e, phi) == 1:
            break

    d = number.inverse(e, phi)

    return (e, n), (d, n)


def main():
    
    print("\n---- LAB13 - Schoolbook RSA Generate Keys ----")

    while True:
        try:
            n = input("Enter the key size (32, 512 or 2048): ")
            n = int(n)
            if n in [32, 512, 2048]:
                break
            else:
                print("Please enter a valid key size (32, 512 or 2048).")
        except ValueError:
            print("Please enter a valid integer.")

    public_key, private_key = generate_RSA_pair(n)
    with open("RSAKeys.txt", "w") as file:
        file.write(f"e = {public_key[0]}\n")
        file.write(f"n = {public_key[1]}\n")
        file.write(f"d = {private_key[0]}\n")
    print("Keys generated and saved to RSAKeys.txt")

if __name__ == "__main__":
    main()
