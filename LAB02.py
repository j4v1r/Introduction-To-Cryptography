#LAB02.py - Hill Cipher
#February 26, 2026
#Authors: Colunga Aguilar Javier Alejandro
#         Valencia Rojas Berenice

import random

#Función para calcular el inverso multiplicativo de a en Zn*.
def invMult(a, n):
    a0 = n
    b0 = a
    t0 = 0
    t = 1
    q = a0//b0
    r = a0 - q * b0

    while r > 0:
        temp = (t0 - q * t) % n
        t0 = t
        t = temp
        a0 = b0
        b0 = r
        q = a0//b0
        r = a0 - q * b0
    
    if b0 != 1:
        return -1
    else:
        return t

#Función generadora de la llave aleatoria K[[k11, k12], [k21, k22]] para el Hill Cipher.
def keyGeneration(n):
    while True:
        k11 = random.randrange(n)
        k12 = random.randrange(n)
        k21 = random.randrange(n)
        k22 = random.randrange(n)

        det_k = (k11*k22 - k21*k12) % n #Mantiene a det_k dentro del rango.

        if invMult(det_k, n) != -1:
            break
    
    key = [[k11, k12], [k21, k22]]
    return key

#Función que devuelve el inverso de una llave del Hill Cipher.
def invKey(n, key):
    k11, k12 = key[0]
    k21, k22 = key[1]

    det_k = (k11*k22 - k21*k12) % n
    det_k_inv = invMult(det_k, n)

    adj = [[k22, -k12], [-k21, k11]]

    k_inv = [[(det_k_inv * adj[i][j]) % n for j in range(2)]
             for i in range(2)]

    return k_inv

#Función de cifrado
def hill_cipher(plaintext, key, ciphertext):
    ciphered = ""
    n = 95
    k11, k12 = key[0]
    k21, k22 = key[1]
    values = []
    specialChar = {}
    values_enc = []

    with open(plaintext, 'r', encoding='utf-8') as fuente:
        text = fuente.read()

    for i, caracter in enumerate(text):
        if (caracter == '\n')  or (caracter == '\t'):
            specialChar[i] = caracter
        else:
            values.append(ord(caracter) - 32) # Mantiene los caracteres dentro del rango 32 - 126
    
    if len(values) % 2 != 0:
        values.append(0)

    for i in range(0, len(values), 2):
        m1 = values[i]
        m2 = values[i+1]

        c1 = (m1 * k11 + m2 * k12) % n
        c2 = (m1 * k21 + m2 * k22) % n

        values_enc.append(chr(c1 + 32)) # Mantiene los caracteres dentro del rango 32 - 126
        values_enc.append(chr(c2 + 32)) # Mantiene los caracteres dentro del rango 32 - 126
    
    j = 0
    for i in range(len(text)):
        if i in specialChar:
            ciphered += specialChar[i]
        else:
            ciphered += values_enc[j]
            j += 1        
    
    with open(ciphertext, 'w', encoding='utf-8') as salida:
        salida.write(ciphered)
    
    print("Encrypted message saved in: ", ciphertext)

#Función de descifrado
def decipher(ciphertext, key, plaintext):
    deciphered = ""
    n = 95
    inv_key = invKey(n, key)
    k11, k12 = inv_key[0]
    k21, k22 = inv_key[1]
    values = []
    values_dec = []
    specialChar = {}
    
    with open(ciphertext, 'r', encoding='utf-8') as fuente:
        cipher = fuente.read()

    for i, caracter in enumerate(cipher):
        if (caracter == '\n')  or (caracter == '\t'):
            specialChar[i] = caracter
        else:
            values.append(ord(caracter) - 32) # Mantiene los caracteres dentro del rango 32 - 126

    if len(values) % 2 != 0:
        values.append(0)

    for i in range(0, len(values), 2):
        c1 = values[i]
        c2 = values[i+1]

        m1 = (c1 * k11 + c2 * k12) % n
        m2 = (c1 * k21 + c2 * k22) % n

        values_dec.append(chr(m1 + 32)) # Mantiene los caracteres dentro del rango 32 - 126
        values_dec.append(chr(m2 + 32)) # Mantiene los caracteres dentro del rango 32 - 126
    
    j = 0
    for i in range(len(cipher)):
        if i in specialChar:
            deciphered += specialChar[i]
        else:
            deciphered += values_dec[j]
            j += 1 
    
    with open(plaintext, 'w', encoding='utf-8') as salida:
        salida.write(deciphered)

    print("Decrypted message saved in", plaintext)

def main():
    while True:
        print("---- LAB02 - HILL CIPHER ----")
        print("Select an option: \n1. Multiplicative Inverse \n2. Key Generation \n3. Inverse Key \n4. Encrypt message \n5. Decrypt message  \n6. Exit")

        option = input("Option: ")
        
        match option:
            case '1':
                while True:
                    try:
                        n = int(input("Enter a number greater than or equal to 2: "))
                        a = int(input("Enter the value of n: "))
                        if (n >= 2) and ((a >= 0 ) and (a < n)):
                            break
                        elif n < 2:
                            print("The number must be greater or equal to 2.")
                        else:
                            print("a it's not part of Zn. Enter a different number")
                    except ValueError:
                        print("Please enter an integer.")
                
                a_inv = invMult(a, n)

                if a_inv == -1:
                    print(a, " has no inverse module ", n)
                else:
                    print("a_inv: ", a_inv)

            case '2':
                while True:
                    try: 
                        n = int(input("Enter a number greater than or equal to 2: "))
                        if n >= 2:
                            break
                        else:
                            print("The number must be greater or equal to 2.")
                    except ValueError:
                        print("Please enter an integer.") 
                key = keyGeneration(n)
                print("K = ",key)

            case '3':
                while True:
                    try: 
                        n = int(input("Enter a number greater than or equal to 2: "))
                        if n >= 2:
                            break
                        else:
                            print("The number must be greater or equal to 2.")
                    except ValueError:
                        print("Please enter an integer.") 
                
                #Verificación de la llave ingresada
                while True:
                    try:
                        print("Enter the key matrix (2x2): ")
                        k11 = int(input("K11: "))
                        k12 = int(input("K12: "))
                        k21 = int(input("K21: "))
                        k22 = int(input("K22: "))

                        det_k = (k11*k22 - k12*k21) % n

                        if invMult(det_k, n) != -1:
                            break
                        else:
                            print("Invalid Key. Try again.")
                    except ValueError:
                        print("Please enter only integers.") 
                
                key = [[k11, k12], [k21, k22]]
                inv_key = invKey(n, key)
                print("Inverse key: ", inv_key)
            
            case '4':
                n = 95
                #Verificación de la llave ingresada
                while True:
                    try:
                        print("Enter the key matrix (2x2): ")
                        k11 = int(input("K11: "))
                        k12 = int(input("K12: "))
                        k21 = int(input("K21: "))
                        k22 = int(input("K22: "))

                        det_k = (k11*k22 - k12*k21) % n

                        if invMult(det_k, n) != -1:
                            break
                        else:
                            print("Invalid Key. Try again.")
                    except ValueError:
                        print("Please enter only integers.")
                
                #Verificación de la existencia del archivo fuente
                while True:
                    fuente = input("Enter the source file name + extension: ")
                    try:
                        with open(fuente, 'r') as f:
                            f.close()
                            break
                    except FileNotFoundError:
                        print("The file doesn't exist. Try again.")

                destino = input("Enter the destination file name + extension: ")

                key = [[k11, k12], [k21, k22]]
                hill_cipher(fuente, key, destino)
            
            case '5':
                n = 95
                while True:
                    #Verificación de la llave ingresada
                    try:
                        print("Enter the key matrix (2x2): ")
                        k11 = int(input("K11: "))
                        k12 = int(input("K12: "))
                        k21 = int(input("K21: "))
                        k22 = int(input("K22: "))

                        det_k = (k11*k22 - k12*k21) % n

                        if invMult(det_k, n) != -1:
                            break
                        else:
                            print("Invalid Key. Try again.")
                    except ValueError:
                        print("Please enter only integers.")
                
                #Verificación de la existencia del archivo fuente
                while True:
                    fuente = input("Enter the source file name + extension: ")
                    try:
                        with open(fuente, 'r') as f:
                            f.close()
                            break
                    except FileNotFoundError:
                        print("The file doesn't exist. Try again.")

                destino = input("Enter the destination file name + extension: ")

                key = [[k11, k12], [k21, k22]]
                decipher(fuente, key, destino)

            case '6':
                print("Exiting the program...")
                break

            case _:
                print("Invalid option. Please, enter an option from 1 to 6.")


                        


if __name__ == "__main__":

    main()
