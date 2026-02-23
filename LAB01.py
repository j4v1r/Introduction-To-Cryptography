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
    
    
if __name__ == "__main__":
    main()