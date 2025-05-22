#include "utilidades.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

bool rangeValidation(int numero, int intIni, int intFin){
    return (numero >= intIni && numero <= intFin); // Retorna verdadero si el número está dentro del rango, de lo contrario falso
}

int intValidation(int limInf,int limSup){

    int num = 0;

    while (true){
        cout << "Digite la opcion (" << limInf << " - " << limSup << "): ";
        cin >> num;

        // Verificar si la entrada es válida (no contiene caracteres no numéricos)
        if (cin.fail()) {
            cin.clear(); // Limpiar el error de entrada
            cin.ignore(1000, '\n'); //Ignorar caracteres erroneos
            cout << "Entrada no valida. Por favor ingrese un numero." << endl;
        } else if (!rangeValidation(num, limInf, limSup)) {
            cout << "La opcion ingresada esta fuera del rango valido." << endl;
        } else{
            return num;
        }
    }
}




void mostrarMenuAnfitrion(){
    bool exit = false;
    while (!exit){
        cout << "\nMENU ANFITRION:\n1. Consultar reservas\n2. Anular reservacion\n3. Actualizar historico\n4. Salir\n";
        int opc = intValidation(1, 4);

        /*
        if (opc == 1) {
            //consultarReservas();
        }
        else if (opc == 2) {
            //anularReserva();
        }
        else if (opc == 3) {
            //actualizarHistorico();
        }
        else{
            exit = true;
        }
        */

    }
}

void mostrarMenuHuesped(){
    bool exit = false;
    while (!exit){
        cout << "\nMENU HUESPED:\n1. Reservar alojamiento\n2. Anular reservacion\n3. Salir\n";
        int opc = intValidation(1, 3);
        /*
        if (opc == 1) {
            reservarAlojamiento();
        }
        else if (opc == 2) {
            anularReserva();
        }
        else{
            exit = true;
        }
        */
    }
}
