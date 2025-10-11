/*
    Se trata de escribir el algoritmo que permita emitir la factura
    correspondiente a una compra de un artículo determinado, del que se
    adquieren una o varias unidades. El IVA a aplicar es del 16% y si el
    precio bruto (precio venta más IVA) es mayor de 54.300Bs, se debe
    realizar un descuento del 10%. La factura a mostrar en pantalla debe
    incluir:
        a. Su nombre (El de usted, no uno ingresado por teclado).
        b. SubTotal
        c. IVA
        d. Descuento
        e. Total. 
*/
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

    // Tengo entendido que el descuento se aplica al subtotal, es decir, al precio bruto (antes de impuestos)
    if(subtotal > 54300) discount = subtotal * 0.10;

    iva = subtotal * 0.16;
    total = subtotal - discount + iva;

    std::cout << std::endl << "Nombre: Daniel Fuentes" << std::endl;
    std::cout << "SubTotal: " << subtotal << std::endl;
    std::cout << "IVA: " << iva << std::endl;
    std::cout << "Descuento: " << discount << std::endl;
    std::cout << "Total: " << total << std::endl;

    return 1;
}