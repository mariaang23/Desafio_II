#include "utilidades.h"
#include "fecha.h"
#include "anfitrion.h"
#include "alojamiento.h"
#include "huesped.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

bool rangeValidation(int numero, int intIni, int intFin){
    return (numero >= intIni && numero <= intFin); // Retorna verdadero si el número está dentro del rango, de lo contrario falso
}

int intValidation(int limInf, int limSup){
    int num = 0;

    while (true){
        cout << "Digite la opcion (" << limInf << " - " << limSup << "): ";
        cin >> num;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Entrada no valida. Por favor ingrese un numero." << endl;
        } else if (!rangeValidation(num, limInf, limSup)) {
            cout << "La opcion ingresada esta fuera del rango valido." << endl;
        } else {
            return num;
        }
    }
}

char charValidation(const string& mensajeStr){
    char opc;
    while (true){
        cout << mensajeStr;
        cin >> opc;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Entrada no válida. Intente de nuevo." << endl;
        } else {
            cin.ignore(1000, '\n'); // Limpiar el buffer por si escribieron más de un carácter
            return opc;
        }
    }
}

void mostrarMenuAnfitrion(Anfitrion* anfitrionActual, Reservas**& reservaciones, int& totalReservas, Huesped**& huespedes, int& totalHuespedes)
{
    bool exit = false;
    while (!exit){
        cout << "------------------------------" << endl;
        cout << "MENU ANFITRION" << endl;
        cout << "------------------------------" << endl;
        cout << "1. Consultar reservas\n2. Anular reservacion\n3. Actualizar historico\n4. Volver al menu principal\n";
        int opc = intValidation(1, 4);

        if (opc == 1) {
            anfitrionActual->mostrarReservasDeSusAlojamientos(reservaciones, totalReservas);
        }
        else if (opc == 2) {
            string codReservaEliminar;
            cout << "Ingrese el codigo de la reserva que desea eliminar: " << endl;
            cin >> codReservaEliminar;
            anfitrionActual->anularReservacion(codReservaEliminar,reservaciones,totalReservas,huespedes,totalHuespedes);

        }
        else if (opc == 3) {
            cout << "Funcion actualizar historico aun no esta implementada.\n";
        }
        else {
            exit = true;
        }
    }
}

void mostrarMenuHuesped(Huesped* huespedActual, Huesped** huespedes, int totalHuespedes,
                        Reservas**& reservaciones, int& totalReservas,
                        Alojamiento** alojamientos, int totalAlojamientos, Anfitrion** anfitriones, int totalAnfitriones){
    bool exit = false;
    while (!exit){
        cout << "------------------------------" << endl;
        cout << "MENU HUESPED" << endl;
        cout << "------------------------------" << endl;
        cout << "1. Reservar alojamiento\n2. Anular reservacion\n3. Volver al menu principal\n";
        int opc = intValidation(1, 3);
        if (opc == 1) {
            huespedActual->reservarAlojamiento(alojamientos,totalAlojamientos,anfitriones,totalAnfitriones,reservaciones,totalReservas);
        }
        else if (opc == 2) {
            string codReservaEliminar;
            cout << "Ingrese el codigo de la reserva que desea eliminar: " << endl;
            cin >> codReservaEliminar;
            huespedActual->anularReservacion(codReservaEliminar, reservaciones, totalReservas);
        }
        else{
            exit = true;
        }
    }
}

void mostrarReservasPorAlojamiento(Alojamiento** alojamientos, int totalAlojamientos,
                                   Reservas** reservaciones, int totalReservas) {
    for (int i = 0; i < totalAlojamientos; ++i) {
        string codigoAloj = alojamientos[i]->getCodigoAlojamiento();
        cout << "Alojamiento: " << alojamientos[i]->getNombre() << " (Codigo: " << codigoAloj << ")" << endl;

        bool tieneReservas = false;
        for (int j = 0; j < totalReservas; ++j) {
            if (reservaciones[j]->getCodigoAlojamiento() == codigoAloj) {
                reservaciones[j]->mostrarReserva();
                tieneReservas = true;
            }
        }

        if (!tieneReservas) {
            cout << "   No tiene reservas asociadas.\n";
        }

        cout << "--------------------------------------" << endl;
    }
}

void guardarHuespedesArchivo(Huesped** huespedes, int totalHuespedes, const string &archivo){
    ofstream out(archivo, ios::trunc);
    if (!out.is_open()) {
        cerr << "No se pudo abrir el archivo: " << archivo << endl;
        return;
    }

    for (int i = 0; i < totalHuespedes; ++i) {
        out << huespedes[i]->getCedulaHuesped() << ";"
            << huespedes[i]->getClaveHuesped() << ";"
            << huespedes[i]->getAntiguedad() << ";"
            << huespedes[i]->getPuntuacion() << ";";

        string codigos = huespedes[i]->getCodigosReservas();
        if (!codigos.empty()){
            out << codigos;
        }
        out << endl;
    }

    out.close();
}
void guardarReservasActivasArchivo(Reservas** reservas, int totalReservas, const string& archivo){
    ofstream out(archivo, ios::trunc);
    if (!out.is_open()) {
        cerr << "No se pudo abrir el archivo: " << archivo << endl;
        return;
    }

    for (int i = 0; i < totalReservas; ++i) {
        if (reservas[i] != nullptr){
            out << reservas[i]->getCodigoReserva() << ";"
                << reservas[i]->getCodigoAlojamiento() << ";"
                << reservas[i]->getCedulaHuesped() << ";"
                << reservas[i]->getFechaEntrada() << ";"
                << to_string(reservas[i]->getCantNoches()) << ";"
                << reservas[i]->getMetodoPago() << ";"
                << reservas[i]->getFechaPago() << ";"
                << reservas[i]->getMonto() << ";"
                << reservas[i]->getAnotaciones() << endl;

        }
    }
    out.close();
}
