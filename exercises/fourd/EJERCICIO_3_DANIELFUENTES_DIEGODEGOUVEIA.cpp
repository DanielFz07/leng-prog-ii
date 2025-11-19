/*
    Realice un programa que permita al usuario:
     *Convertir $ a Bs
     *Convertir Bs a Euros
     *Calcular cuanto tiempo necesitaria ahorrar para lograr una meta
     monetaria ingresada por teclado, partiendo de un salario mensual base
     ingresado por telcado y gastos mensuales fijos ingresados por teclado
     *Calcular cuanto tardaria en alcanzar un peso deseado, partiendo de un peso actual ingresado
     por teclado y una medida de kg que es capaz de comprometerse a perder por mes
*/
#include <iostream>
#include <conio.h>

using namespace std;
int main(void)
{

    int bolivares = 0;
    int euros = 275.81;
    int dolares = 237.75;
    int conversion = 0;
    int opcion = 0;
    char salir = 'Y';

    do
    {
        cout<<"Que acción desea realizar?"<<endl;
        cout<<"Opcion 1: Convertir dolares a bolivares"<<endl;
        cout<<"Opcion 2: Convertir bolivares a euros"<<endl;
        cout<<"Opcion 3: Calcular cuanto tiempo necesitaria ahorrar para lograr una meta \nmonetaria ingresada por teclado, partiendo de un salario mensual base ingresado por teclado y\ngastos mensuales fijos ingresados por teclado : "<<endl;
        cout<<"Opcion 4: Calcular cuanto tardaria en alcanzar un peso deseado, partiendo de un peso actual ingresado\n por teclado y una medida de kg que es capaz de comprometerse a perder por mes: "<<endl;
        cin>>opcion;

        switch (opcion)
        {
            case 1:
            {
                cout<<"Ingrese la cantidad de dolares que quiere pasar a bolivares"<<endl;
                cin>>bolivares;

                conversion = dolares * bolivares;
                cout<<"Tus dolares en bolivares son: "<<conversion;
                break;
            }
            case 2:
            {

                cout<<"Ingrese la cantidad de bolivares que quiere pasar a euros: "<<endl;
                cin>>bolivares;
            
                conversion = bolivares / euros;
                cout<<"Tus bolivares en euros son: "<<conversion;
                break;
            }
            case 3:
            {
                int metaMonetaria = 0;
                int salarioMensual = 0;
                int gastosMensuales = 0;
                int ahorromensual;
                int tiemponecesario;

                cout<<"¿Cual es su meta monetaria?"<<endl;
                cin>>metaMonetaria;

                cout<<"¿Cual es tu salario mensual base?"<<endl;
                cin>>salarioMensual;

                cout<<"¿Cuanto es tu gasto mensual fijo?"<<endl;
                cin>>gastosMensuales;

                ahorromensual = salarioMensual - gastosMensuales;

                tiemponecesario = metaMonetaria / ahorromensual;

                cout<<"Calculo del ahorro mensual: "<<ahorromensual<<endl;

                cout<<"Tiempo necesario para alcanzar la meta (en meses): "<<tiemponecesario<<endl;
                break;
            }
            case 4:
            {
                int pesoIdeal = 0;
                int pesoActual = 0;
                int compromiso = 0;
                int tiempo = 0;
                int pesoPerder = 0;
                int pesoGanar = 0;

                cout<<"Ingrese su peso deseado en kg: ";
                cin>>pesoIdeal;

                cout<<"Ingrese la cantidad de kg que te comprometes a bajar/subir al mes: ";
                cin>>compromiso;

                cout<<"Ingresa tu peso actual: ";
                cin>>pesoActual;

                if(pesoIdeal == pesoActual)
                {
                    cout<<"Ya estas en tu peso ideal!!";
                }
                if (pesoIdeal > pesoActual)
                {
                    pesoGanar = pesoIdeal - pesoActual;

                    cout<<"Los kg que necesitas ganar son: "<<pesoGanar<<endl;
                
                    tiempo = (pesoIdeal - pesoActual) / compromiso;
                }
                else 
                {
                    pesoPerder = pesoActual - pesoIdeal;

                    cout<<"Los kg que necesitas perder son: "<<pesoPerder<<endl;

                    tiempo =  pesoPerder / compromiso;

                }
                cout<<"\nEl tiempo necesario (en meses) para" <<  (pesoIdeal > pesoActual ? "subir" : "bajar") << " de peso son: "<<tiempo<<endl;
    
                break;
            }
        }
        cout << "Quieres salir?? (Y/n): ";
        salir = getch();
    } while (salir == 'y' || salir == 'Y');
    return 1;
}
