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


#Ciphertext is generated using AES-CTR mode with a random IV (nonce) and the provided key. The IV and ciphertext are printed in Base64 encoding for easy handling.
def encrypt_ctr(plaintext, key):
    """
    Encrypts plaintext using AES-CTR
    Random IV (nonce) is generated automatically
    """

    # CTR mode uses 16-byte nonce/counter
    iv = os.urandom(16)

    cipher = Cipher(
        algorithms.AES(key),
        modes.CTR(iv)
    )

    encryptor = cipher.encryptor()

    ciphertext = encryptor.update(
        plaintext.encode()
    ) + encryptor.finalize()

    print("\nIV used (Base64):")
    print(base64.b64encode(iv).decode())

    print("\nCiphertext (Base64):")
    print(base64.b64encode(ciphertext).decode())

    return iv, ciphertext


#Decryption is performed using the same key and IV (nonce) that were used for encryption. The ciphertext is decoded from Base64, and the plaintext is recovered and printed.
def decrypt_ctr(ciphertext_b64, key, iv_b64):
    """
    Decrypts AES-CTR ciphertext encoded in Base64
    """

    ciphertext = base64.b64decode(ciphertext_b64)
    iv = base64.b64decode(iv_b64)

    cipher = Cipher(
        algorithms.AES(key),
        modes.CTR(iv)
    )

    decryptor = cipher.decryptor()

    plaintext = decryptor.update(
        ciphertext
    ) + decryptor.finalize()

    print("\nRecovered Plaintext:")
    print(plaintext.decode())



if __name__ == "__main__":

    # Generate AES Keys
    key128 = generate_key(128)
    key192 = generate_key(192)
    key256 = generate_key(256)

    # Use AES-256 for encryption/decryption
    plaintext = input("\nEnter plaintext: ")

    iv, ciphertext = encrypt_ctr(plaintext, key256)

    # Convert to Base64 for decryption test
    ciphertext_b64 = base64.b64encode(ciphertext).decode()
    iv_b64 = base64.b64encode(iv).decode()

    # Decrypt
    decrypt_ctr(ciphertext_b64, key256, iv_b64)