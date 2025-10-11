/*
    Realice un programa que reciba la tasa del dólar del día (BCV) en Bs y un
    valor monetario en $ y retorne la conversión.
    Ej: Tasa:189,32Bs * valorMonetario: 5$ = 946,60Bs (2 puntos). 
*/
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