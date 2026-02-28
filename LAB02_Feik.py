import random

def inv_mult(a, n):
    a0 = n
    b0 = a
    t0 = 0
    t = 1

    if n == 1:
        return 0

    while b0 != 0:
        q = a0 // b0
        r = a0 - q * b0

        temp = t0 - q * t
        t0 = t
        t = temp

        a0 = b0
        b0 = r

    if a0 != 1:
        return None
    else:
        return t0 % n
    
def gcd(a, b):
    
    r=a%b
    
    if(r==0):
        return b
    else:
        return gcd(b, r)    
    
def key_gen(n):
    
    valida=False
    
    while not valida:
        a = random.randint(1, n-1)
        b = random.randint(0, n-1)
        c = random.randint(1, n-1)
        d = random.randint(0, n-1)

        det = a*d - b*c
        
        if gcd(det, n) == 1:
            valida = True
            
    key = [[a,b],[c,d]]
    return key
     
def inv_k(n,key):
    a = key[0][0]
    b = key[0][1]
    c = key[1][0]
    d = key[1][1]

    det = a*d - b*c
    inv_det = inv_mult(det, n)

    if inv_det is None:
        return None

    inv_key = [[(d * inv_det) % n, (-b * inv_det) % n], [(-c * inv_det) % n, (a * inv_det) % n]]
    
    return inv_key

def main():
    
    #Menu Principal
    while True:
        print("---- LAB02 - Multiplicative Inverse ----")
        print("Select an option: \n1. Find multiplicative inverse of a \n2. Generate random key \n3. Compute inverse of key \n4. Exit")
        
        opcion = input("Option: ")
            
        match opcion:
            case '1':
                
                while True:
                    try:
                        n = int(input("Ingresa un número mayor o igual a 2: "))
                        a = int(input("Ingresa un número perteneciente a Zn: "))   
                        if n >= 2 and a in range(0,n):
                            break;
                        else:
                            print("Entrada no válida. Por favor, ingresa un número mayor o igual a 2 y un número perteneciente a Zn.")
                    except ValueError:
                        print("Entrada no válida. Por favor, ingresa un número entero.")

                inv = inv_mult(a,n)
                print(f"El inverso multiplicativo de {a} en Zn es: {inv}")
                
            case '2':
            
                while True:
                    try:
                        n = int(input("Ingresa un número mayor o igual a 2: "))
                        if n >= 2:
                            break;
                        else:
                            print("Entrada no válida. Por favor, ingresa un número mayor o igual a 2 y un número perteneciente a Zn*.")
                    except ValueError:
                        print("Entrada no válida. Por favor, ingresa un número entero.")
                
                key = key_gen(n)
                print(f"Clave generada: {key}")
                
            case '3':
                
                while True:
                    try:
                        n = int(input("Ingresa un número mayor o igual a 2: "))
                        key = key_gen(n)
                        print(f"Clave generada: {key}")
                        if n >= 2:
                            break;
                        else:
                            print("Entrada no válida. Por favor, ingresa un número mayor o igual a 2 y un número perteneciente a Zn.")
                    except ValueError:
                        print("Entrada no válida. Por favor, ingresa un número entero.")

                inv_key = inv_k(n,key)
                print(f"La clave inversa de {key} en Zn es: {inv_key}")
                
                
                
            #case '4':
                
            #case '5':

            #case '6':
                
            case '7':
                print("Saliendo del programa...")
                break
            
            case _:
                print("Opción no válida. Por favor, selecciona una opción del 1 al 7.")
            
                
if __name__ == "__main__":

    main()