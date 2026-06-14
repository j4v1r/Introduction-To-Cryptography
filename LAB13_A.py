from Crypto.Util import number
import math

def generate_prime_numbers():

    prime_number_16 = number.getPrime(16)
    prime_number_32 = number.getPrime(32)
    prime_number_512 = number.getPrime(512)
    prime_number_2048 = number.getPrime(2048)

    print("16-bit prime number:", prime_number_16)
    print("32-bit prime number:", prime_number_32)
    print("512-bit prime number:", prime_number_512)
    print("2048-bit prime number:", prime_number_2048)
    
    
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

if __name__ == "__main__":
    
    generate_prime_numbers()
    
    print("\nGenerating RSA key pairs:")
    for bits in [32, 512, 2048]:
        public_key, private_key = generate_RSA_pair(bits)
        print(f"{bits}-bit RSA key pair:")
        print("Public Key (e, n):", public_key)
        print("Private Key (d):", private_key[0])
        print()