#LAB01.py - Affine Cipher
#February 19, 2026
#Authors: Colunga Aguilar Javier Alejandro
#         Valencia Rojas Berenice

import random

#Función para calcular el máximo común divisor (gcd) de dos números a y b utilizando el algoritmo de Euclides.
def gcd(a, b):
    
    r=a%b
    
    if(r==0):
        return b
    else:
        return gcd(b, r)    

#Función para calcular el conjunto Zn* de los enteros menores que n que son coprimos a n.
def Zn_asterisco(n):
    
    Zna=[]
        
    for i in range(0, n):
        if(gcd(i, n)==1):
            #print(i)
            Zna.append(i)

    return Zna
    
#Función para calcular el inverso multiplicativo de a en Zn*.   
def inversoPrimo(n, a, Zn):
    
    for i in Zn:
        if((i*a)%n==1):
            inverso = i
            break
            
    return inverso
       
#Función generadora de la llave aleatoria K(a, b) para el Affine Cipher.
def keygen():
    
    Znkey = Zn_asterisco(95)
    
    a = random.choice(Znkey)
    b = inversoPrimo(95, a, Znkey)
    
    return a, b

#Función de cifrado
def affine_cipher(plaintext, a, b, ciphertext):
    ciphered = ""
    n = 95

    with open(plaintext, 'r', encoding='utf-8') as fuente:
        text = fuente.read()

    for caracter in text:
        m = ord(caracter) - 32 
        cipher = ((a * m + b) % n) + 32 # Mantiene los caracteres dentro del rango 32 - 126
        print(cipher)
        ciphered += chr(cipher)
    
    
    with open(ciphertext, 'w', encoding='utf-8') as salida:
        salida.write(ciphered)
    
    print("El mensaje fue encriptado en ", ciphertext)

#Función de descifrado
def decipher(ciphertext, a, b, plaintext):
    deciphered = ""
    n = 95

    Zn = Zn_asterisco(n)

    a_inv = inversoPrimo(n, a, Zn)

    with open(ciphertext, 'r', encoding='utf-8') as fuente:
        cipher = fuente.read()

    for caracter in cipher:
        c = ord(caracter) - 32
        text = ((a_inv * (c-b)) % n) + 32 # Mantiene los caracteres dentro del rango 32 - 126
        deciphered += chr(text)
    print("destino")
    
    with open(plaintext, 'w', encoding='utf-8') as salida:
        salida.write(deciphered)

    print("El mensaje ha sido decifrado en ", plaintext)
            
    
def main():
    
    #Validar que el usuario ingrese un número entero mayor o igual a 2 para n.
    while True:
        try:
            n = int(input("Ingresa un número mayor o igual a 2: "))
            if n >= 2:
                break;
            else:
                print("Entrada no válida. Por favor, ingresa un número mayor o igual a 2.")
        except ValueError:
            print("Entrada no válida. Por favor, ingresa un número entero.")

    Zna=Zn_asterisco(n)
    print("El conjunto Z"+str(n)+"* es:", Zna)
    
    
    #Validar que el usuario ingrese un número entero perteneciente a Zn* para a.
    while True:
        try:
            a = int(input("Ingresa un número mayor o igual a 2: "))
            if a in Zna:
                break;
            else:
                print("Entrada no válida. Por favor, ingresa un número perteneciente a Z"+str(n)+"*.")
        except ValueError:
            print("Entrada no válida. Por favor, ingresa un número entero.")

    b =  inversoPrimo(n, a, Zna)
    print("El inverso multiplicativo de", a, "en Z"+str(n)+"* es:", b)
    
    #Se desempaqueta la tupla devuelta por la función keygen()
    a, b = keygen()
    print("La clave generada es: K("+str(a)+","+str(b)+")")
    
    #Verificación de llave ingresada por el usuario
    while True:
        try:
            k_a = int(input("Ingresa el valor de a: "))
            k_b = int(input("Ingresa el valor de b: "))
            if (k_a in Zna) and (k_b <= 9595):
                break
            else:
                print("La llave no es válida. Intente de nuevo.")
        except ValueError:
            print("Entrada no válida. Por favor, ingresa un número entero.")

    #Verificación de la existencia del archivo fuente
    while True:
        fuente = input("Ingrese el nombre del archivo fuente + extensión: ")
        try:
            with open(fuente, 'r') as f:
                f.close()
                break
        except FileNotFoundError:
            print("El archivo no existe. Intente de nuevo")
        
    destino = input("Ingrese el nombre del archivo destino + extensión: ")

    #Menú de opciones
    while True:
        try:
            opcion = input("-- Selecciona una opción -- \nA. Cifrar \nB. Descifrar\n").upper()
            if opcion == 'A':
                affine_cipher(fuente, k_a, k_b, destino)
                break
            elif opcion == 'B':
                decipher(fuente, k_a, k_b, destino)
                break
            else:
                print("Opción no válida. Por favor, ingrese una de las opciones disponibles.")
        except ValueError:
            print("Entrada no válida. Por favor, ingresa un caracter.")
    
if __name__ == "__main__":

    main()
