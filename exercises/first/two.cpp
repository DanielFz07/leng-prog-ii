/*
    2) Realice un programa que retorne la suma de los diez primeros números
    pares. 

    Respuesta:
    Se podria hacer de dos formas distintas, la primera sería iterar sobre
    los números enteros sumar los que son pares hasta que los numeros sumados sean 10.

    O utilizar la formula para la suma de los numeros pares:
        Fórmula: S=n(n+1)
        Donde: S = suma
            n = numero de elementos

    Siendo esta ultima opcion la mas eficiente.
*/

#include <iostream>

int main(void) 
{
    int sum = (10 * (10 + 1));;

    std::cout << "Suma de los 10 primeros números pares: " << sum << std::endl;

    return sum;
}