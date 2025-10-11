/*
    Realice un programa que reciba 10 números por teclado y retorne su suma. 
*/

#include <iostream>

int main(void) 
{
    int sum = 0;
    int n;

    std::cout << "Ingrese 10 numeros (separados por espacios o presionar ENTER en cada uno): ";
    for (int i = 0; i < 10; i++)
    {
        std::cin >> n;
        sum += n;
    }

    std::cout << "Suma de los 10 numeros introducidos: " << sum << std::endl;

    return sum;
}