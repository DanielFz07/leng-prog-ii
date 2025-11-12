/*
	Realice una calculadora que permita al usuario elegir entre sumar, restar, multiplicar y dividir en un menu. No puede usar 
	funciones ni arreglos
*/
#include <iostream>
#include <conio.h>

int main(void)
{
	int value_a = 0, value_b = 0, operation = 0, result = 0;
    char option = 'Y';

    do
    {
        std::cout << std::endl << std::endl << "CALCULADORA" << std::endl;
        std::cout << "1. Sumar" << std::endl;
        std::cout << "2. Restar" << std::endl;
        std::cout << "3. Multiplicar" << std::endl;
        std::cout << "4. Dividir" << std::endl;
        std::cout << "Introduzca la accion a realizar. (1, 2, 3, 4): ";
        std::cin >> operation;
        
        std::cout << std::endl << "Introduzca el primer valor: ";
        std::cin >> value_a;
        std::cout << std::endl << "Introduzca el segundo valor: ";
        std::cin >> value_b;
        
        switch(operation)
        {
        	case 1:
    		{
    			result = value_a+value_b;
    			std::cout << "El resultado de la suma de " << value_a << " y " << value_b << " es igual a " << result;
    			break;
			}
			case 2:
    		{
    			result = value_a-value_b;
    			std::cout << "El resultado de la resta de " << value_a << " y " << value_b << " es igual a " << result;
    			break;
			}
			case 3:
    		{
    			result = value_a*value_b;
    			std::cout << "El resultado de la multiplicacion de " << value_a << " y " << value_b << " es igual a " << result;
    			break;
			}
			case 4:
    		{
    			if(value_b == 0) 
    			{
    				std::cout << "No es posible dividir entre 0.";
    				break;
				}
    			result = value_a/value_b;
    			std::cout << "El resultado de la division de " << value_a << " y " << value_b << " es igual a " << result;
    			break;
			}
			default: break;
		}
    	
        std::cout << std::endl << "Desea volver a realizar una operacion? (Y/N): ";
        option = getch();
    } while(option == 'Y' || option == 'y');
	return 1;
}
