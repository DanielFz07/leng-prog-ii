/*
    Escribir un algoritmo que lea un número y escriba su raíz cuadrada, su valor si se potencia a la 2, a la 3 y a la 5. 
*/
#include <iostream>
#include <cmath>

int main(void) 
{
    int n;
    std::cout << "Ingrese un numero: ";
    std::cin >> n;

    std::cout << "El raíz cuadrada es: " << std::sqrt(n) << std::endl;
    std::cout << "El valor al cuadrado es: " << n * n << std::endl;
    std::cout << "El valor al cubo es: " << n * n * n << std::endl;
    std::cout << "El valor a la 5 es: " << n * n * n * n * n << std::endl;

    return 1;
}