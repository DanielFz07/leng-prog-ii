#include <iostream>

int main(void) 
{
    float bcv = 0, value = 0, conversion = 0;
    std::cout << "Ingrese el precio del dolar actual: ";
    std::cin >> bcv;
    std::cout << "Ingrese el valor monetario: ";
    std::cin >> value;

    conversion = bcv * value;
    std::cout << "Conversión: " << conversion << std::endl;

    return conversion;
}