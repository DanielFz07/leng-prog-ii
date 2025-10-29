/*
	Realice un programa que permita al usuario jugar a la adivinanza de numeros. El programa debe generar un numero aleatorio y 
	permitir al usuaraio 5 intentos para tratar de adivinarlo. Cuando el usuario ingrese uno muy alto o omuy bajo respecto al numero 
	a encontrar, el programa debe informarlo.
*/

#include <iostream>
#include <cstdlib>
#include <conio.h>
#include <ctime>

int main(void)
{
	srand(time(0));
	
	int tries = 5, number = 0;
	char option = 'Y';
	
	int random = 0;

    do
    {
    	random = rand() % 101;
		do
		{	
			std::cout << std::endl << "Tienes " << tries << " intentos disponibles." << std::endl;
			std::cout << "Introduce un numero para jugar: ";
			std::cin >> number;
			
			tries --;
			
			const int distance = abs(random - number);
			
			if(distance == 0) std::cout << std::endl << "\033[32mHas adivinado el numero correctamente.\033[0m"<< std::endl;
			else if(distance > 70) std::cout << std::endl<< "\033[34mCONGELADO. \033[0mVuelve a intentarlo." << std::endl;
			else if(distance > 50) std::cout << std::endl<< "\033[34mMuy frio. \033[0mVuelve a intentarlo." << std::endl;
			else if(distance > 30) std::cout << std::endl<< "\033[34mFrio. \033[0mVuelve a intentarlo." << std::endl;
			else if(distance > 10) std::cout << std::endl<< "\033[34mTibio. \033[0mVuelve a intentarlo." << std::endl;
			else if(distance > 5) std::cout << std::endl<< "\033[31mCALIENTE. \033[0mVuelve a intentarlo." << std::endl;
			else if(distance > 1) std::cout << std::endl<< "\033[31mTE QUEMAS. \033[0mVuelve a intentarlo." << std::endl;
			else std::cout << std::endl<< "\033[34mCONGELADO. \033[0mVuelve a intentarlo." << std::endl;
		}
		while(tries > 0);
		
		tries = 5;
		std::cout << std::endl << std::endl << "\033[31mHas perdido, no pudiste adivinar el numero." << std::endl;
		
	   	std::cout << "\033[0mQuieres volver a jugar? (Y/N): ";
        option = getch();
    } while(option == 'Y' || option == 'y');
	
	return 1;
}
