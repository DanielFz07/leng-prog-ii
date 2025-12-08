/*
    Realice un programa que muestre en pantalla un rectangulo SIN RELLENO (solo los bordes), y que dentro del mismo aparezca el año de su nacimiento.
*/
#include <windows.h>
#include <iostream>
#include <CONIO.H>

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void draw2(int pos_x, int pos_y, int size)
{
    for(int x = 0; x < size; x ++)
    {
        for(int y = 0; y < size; y ++)
        {
            if(y == 0 || y == size - 1 || (x == size - 1 && y < size / 2) || (x == 0 && y > size / 2) || (y == size / 2))
            {
                gotoxy(pos_x+x, pos_y+y);
                std::cout << ".";
            }
        }
    }
}

void draw0(int pos_x, int pos_y, int size)
{
    for(int x = 0; x < size; x ++)
    {
        for(int y = 0; y < size; y ++)
        {
            if(y == 0 || y == size - 1 || x == 0 || x == size - 1)
            {
                gotoxy(pos_x+x, pos_y+y);
                std::cout << ".";
            }
        }
    }
}

void draw7(int pos_x, int pos_y, int size)
{
    for(int x = 0; x < size; x ++)
    {
        for(int y = 0; y < size; y ++)
        {
            if(x == size - 1 || y == 0)
            {
                gotoxy(pos_x+x, pos_y+y);
                std::cout << ".";
            }
        }
    }
}

int main(void)
{
    char option = 'Y';
    do
    {
        int length = 0;
        std::cout << "Introduce la longitud del cuadrado: ";
        std::cin >> length;
        
        if(length < 20) 
        {
            std::cout << "la longitud debe ser mayor a 20.";
            return 0;
        }

        system("cls");

        for(int x = 0; x < length*3; x++)
        {
            for(int y = 0; y < length; y ++)
            {
                if(x == 0 || y == 0 || x == (length*3) - 1 || y == length - 1)
                {
                    gotoxy(x, y);
                    std::cout << "*";
                }
                else if(x == length / 2 && y == length / 2)
                {
                    //gotoxy(x, y);
                    draw2(x, y-3, length/4);
                    draw0(x+(length / 2)+2, y-3, (length / 4));
                    draw0(x+2*(length / 2)+4, y-3, (length / 4));
                    draw7(x+3*(length / 2)+5, y-3, (length / 4));
                }
            }
        }
    } while (option != 'Y' || option != 'y');
    
    return 1;
}
