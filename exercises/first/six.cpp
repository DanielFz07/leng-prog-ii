#include <iostream>

int main(void)
{
    int quantity = 0, price = 0, iva = 0, subtotal = 0, total = 0, discount = 0;

    std::cout << "Ingrese la cantidad de unidades a comprar: ";
    std::cin >> quantity;

    // No especifico el precio en el enunciado del ejercicio, asi que, asumo que se debe introducir.
    std::cout << "Ingrese el precio de la unidad: ";
    std::cin >> price;

    subtotal = quantity * price;

    iva = subtotal * 0.16;


    if((subtotal + iva) > 54300) discount = (subtotal + iva) * 0.10;

    total = subtotal - discount + iva;

    std::cout << std::endl << "Nombre: Daniel Fuentes" << std::endl;
    std::cout << "SubTotal: " << subtotal << std::endl;
    std::cout << "IVA: " << iva << std::endl;
    std::cout << "Descuento: " << discount << std::endl;
    std::cout << "Total: " << total << std::endl;

    return 1;
}