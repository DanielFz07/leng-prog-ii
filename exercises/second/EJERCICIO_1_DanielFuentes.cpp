/*
    Realice un simulador de cajero automatico, debe empezar con un saldo fijo de $500, el menu debe permitir al usuario, consultar saldo, retirar fondos y depositar. No se debe permitir retirar mas de lo que hay disponible. No puede usar funciones ni arreglos.
*/

#include <iostream>
#include <conio.h>

int main(void)
{
    long long int balance = 500, operation = 0, value = 0;
    char option = 'Y';

    do
    {
        std::cout << std::endl << std::endl << "CAJERO AUTOMATICO" << std::endl;
        std::cout << "1. Consultar saldo"<< std::endl;
        std::cout << "2. Depositar"<< std::endl;
        std::cout << "3. Retirar"<< std::endl;
        std::cout << "Introduzca la accion a realizar. (1, 2, 3): ";
        std::cin >> operation;

        switch(operation)
        {
            case 1:
            {
                std::cout << std::endl << "Su saldo actual es: " << balance << std::endl;
                break;
            }
            case 2:
            {
                std::cout << std::endl << "Introduzca el monto a depositar: ";
                std::cin >> value;
                balance += value;
                std::cout << std::endl << "Su nuevo saldo es: " << balance << std::endl;
                break;
            }
            case 3:
            {
                std::cout << std::endl << "Introduzca el monto a retirar: ";
                std::cin >> value;
                
                if(balance - value < 0 )
                {
                	std::cout << "No puedes retirar mas dinero de lo que tienes." << std::endl;
                	break;
				}
                
                balance -= value;
                std::cout << std::endl << "Su nuevo saldo es: " << balance << std::endl;
                break;
            }
            default: break;
        }

        std::cout << std::endl << "Desea volver a realizar una operacion? (Y/N): ";
        option = getch();
    } while(option == 'Y' || option == 'y');

    return 1;
}
