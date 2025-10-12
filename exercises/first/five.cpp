#include <iostream>
#include <string>

int main(void) 
{
    int avg = 0, rating = 0;
    std::string name;

    std::cout << "Ingrese su nombre: ";
    // Necesario para poder introducir el nombre con espacios, ya que cin se detiene en el primer espacio
    getline(std::cin, name);

    std::cout << "Ingrese 5 notas (separados por espacios o presionar ENTER en cada una): ";
    for (int i = 0; i < 5; i++)
    {
        std::cin >> rating;
        avg += rating;
    }
    
    avg /= 5;
    if (avg < 10) 
    {
        std::cout << "Usted está reprobado " << name << std::endl;
        return avg;
    }

    std::cout << "Usted está aprobado " << name << std::endl;

    return avg;
}