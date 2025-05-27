#include "utilidades.h"
#include "fecha.h"
#include "anfitrion.h"
#include "alojamiento.h"
#include "huesped.h"
#include "reservas.h"
#include <iostream>
#include <fstream>
using namespace std;

bool rangeValidation(int numero, int intIni, int intFin){
    return (numero >= intIni && numero <= intFin);
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
            cout << "Entrada no valida. Intente de nuevo." << endl;
        } else {
            cin.ignore(1000, '\n');
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
            string fechaStr = "";
            cout << "\nIngrese la fecha de corte para generar el historico: \n";
            cin >> fechaStr;
            Fecha fechaCorte = Fecha::fromString(fechaStr);
            actualizarHistorico(reservaciones, totalReservas, huespedes, totalHuespedes, fechaCorte);
        }
        else {
            exit = true;
        }
    }
}

void mostrarMenuHuesped(Huesped* huespedActual, Reservas**& reservaciones, int& totalReservas,
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
        // archivo no pudo abrirse, sin salida a consola
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
        // archivo no pudo abrirse, sin salida a consola
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

void actualizarHistorico(Reservas**& reservasActivas, int& totalReservas, Huesped** huespedes, int totalHuespedes, Fecha& fechaCorte) {
    Reservas** reservasHistorico = new Reservas*[totalReservas];
    int totalReservasHistorico = 0;

    for (int i = 0; i < totalReservas; i++) {
        if (reservasActivas[i] != nullptr) {
            Fecha fechaReserva = Fecha::fromString(reservasActivas[i]->getFechaEntrada());
            if (fechaReserva < fechaCorte || fechaReserva == fechaCorte) {
                reservasHistorico[totalReservasHistorico++] = reservasActivas[i];

                string cedula = reservasActivas[i]->getCedulaHuesped();
                string codigoReserva = reservasActivas[i]->getCodigoReserva();
                for (int j = 0; j < totalHuespedes; j++) {
                    if (huespedes[j] != nullptr && huespedes[j]->getCedulaHuesped() == cedula) {
                        huespedes[j]->eliminarReservaHistorico(codigoReserva);
                        break;
                    }
                }

                reservasActivas[i] = nullptr;
            }
        }
    }

    ofstream archivoHistorico("historico.txt", ios::app);
    if (!archivoHistorico.is_open()) {
        for (int i = 0; i < totalReservasHistorico; i++) {
            delete reservasHistorico[i];
        }
        delete[] reservasHistorico;
        return;
    }

    for (int i = 0; i < totalReservasHistorico; i++) {
        Reservas* r = reservasHistorico[i];
        archivoHistorico << r->getCodigoReserva() << ";"
                         << r->getCodigoAlojamiento() << ";"
                         << r->getCedulaHuesped() << ";"
                         << r->getFechaEntrada() << ";"
                         << to_string(r->getCantNoches()) << ";"
                         << r->getMetodoPago() << ";"
                         << r->getFechaPago() << ";"
                         << r->getMonto() << ";"
                         << r->getAnotaciones() << endl;

        delete r;
    }
    archivoHistorico.close();
    delete[] reservasHistorico;

    int nuevosActivos = 0;
    for (int i = 0; i < totalReservas; i++) {
        if (reservasActivas[i] != nullptr) {
            reservasActivas[nuevosActivos++] = reservasActivas[i];
        }
    }
    for (int i = nuevosActivos; i < totalReservas; i++) {
        reservasActivas[i] = nullptr;
    }
    totalReservas = nuevosActivos;

    guardarReservasActivasArchivo(reservasActivas, totalReservas, "ReservasActivas.txt");
    guardarHuespedesArchivo(huespedes, totalHuespedes, "huespedes.txt");
}
