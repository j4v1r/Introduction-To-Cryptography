import time
from sympy import isprime
from cryptography.hazmat.primitives.asymmetric import rsa

#Calculates the discrete logarithm of a modulo mod that gives result
def logarithm(a,mod, result):
    value = 1
    
    for i in range(mod):
        if value == result:
            return i
        value = (value * a) % mod
    return None

#Finds two prime numbers that multiply give n
def prime_numbers(n):
    for p in range(2, int(n**0.5) + 1):
        if n % p == 0:
            q = n // p

            if isprime(p) and isprime(q):
                return p, q

    return None, None

#Generates large prime numbers of 512 bits
def generate_prime_512():
    """
    Generates and returns a single 512-bit prime number
    using the cryptography library.
    """

    private_key = rsa.generate_private_key(
        public_exponent=65537,
        key_size=1024
    )

    numbers = private_key.private_numbers()

    return numbers.p


def main():
    
    while True:
        print("\n---- LAB12 - HARD COMPUTATIONAL PROBLEMS ----")
        print("\nSelect an option: \n1. Calculate Discrete Logarithm\n2. Find p,q such that n = p*q \n3. Find large prime numbers of 512 bits \n4. Exit")

        option = input("Option: ")
        
        match option:
            case '1':
                a = int(input("Enter the base (a): "))
                mod = int(input("Enter the modulus (mod): "))
                result = int(input("Enter the result for the logarithm: "))
    
                start = time.perf_counter()
                result = logarithm(a, mod, result)
                end = time.perf_counter()
    
                if result is not None:
                    print(f"The discrete logarithm of {a} modulo {mod} is: {result}")
                else:
                    print(f"No solution found for a={a}, mod={mod}, result={result}")
                    
                print(f"Execution time: {end - start:.6f} seconds")
                    
            case '2':
                n = int(input("Enter the number n to factor: "))
                
                start = time.perf_counter()
                p, q = prime_numbers(n)
                end = time.perf_counter()
                
                if p and q:
                    print(f"The prime factors of {n} are: p={p}, q={q}")
                else:
                    print(f"No prime factors found for {n}.")
                
                print(f"Execution time: {end - start:.6f} seconds")
                
            case '3':  
                start = time.perf_counter()
                p = generate_prime_512()
                end = time.perf_counter()
                print(f"Generated prime number: p={hex(p)}")
                print(f"Bits: {p.bit_length()}")
                print(f"Execution time: {end - start:.6f} seconds")
            case '4':
                print("Exiting...")
                break
    
if __name__ == "__main__":
    main()