#include "reservas.h"
#include "fecha.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Reservas::Reservas(const string& _codigoReserva, const string& _fechaEntrada, int _cantNoches, const string& _codigoAlojamiento,
                   const string& _cedulaHuesped, const string& _metodoPago, const string& _fechaPago, const string& _monto, const string& _anotaciones)
    : codigoReserva(_codigoReserva), fechaEntrada(_fechaEntrada), cantNoches(_cantNoches), codigoAlojamiento(_codigoAlojamiento),
      alojamientoPtr(nullptr), cedulaHuesped(_cedulaHuesped), metodoPago(_metodoPago), fechaPago(_fechaPago), monto(_monto), anotaciones(_anotaciones) {};

Reservas::~Reservas() {
    for (int i = 0; i < cantNoches; i++){
        delete fechasReservadas[i];
        fechasReservadas[i] = nullptr;
    }
    delete[] fechasReservadas;
    fechasReservadas = nullptr;
    alojamientoPtr = nullptr;
}

const string& Reservas::getCodigoReserva() const {
    return codigoReserva;
}

const string& Reservas::getCodigoAlojamiento() const {
    return codigoAlojamiento;
}

const string& Reservas::getCedulaHuesped() const{
    return cedulaHuesped;
}

const string& Reservas::getFechaEntrada() const{
    return fechaEntrada;
}

int Reservas::getCantNoches() const {
    return cantNoches;
}

const string& Reservas::getMetodoPago() const{
    return metodoPago;
}

const string& Reservas::getFechaPago() const{
    return fechaPago;
}

const string& Reservas::getMonto() const{
    return monto;
}

const string& Reservas::getAnotaciones() const{
    return anotaciones;
}

Alojamiento* Reservas::getAlojamiento() const {
    return alojamientoPtr;
}

Fecha** Reservas::getFechasReservadas() const {
    return fechasReservadas;
}

void Reservas::setFechasReservadas(Fecha ** nuevasFechas){
    fechasReservadas = nuevasFechas;
}

void Reservas::setAlojamientoPtr(Alojamiento* nuevoPtr){
    alojamientoPtr = nuevoPtr;
}

void Reservas::enlazarAlojamiento(Alojamiento** alojamientos, int totalAlojamientos) {
    for (int i = 0; i < totalAlojamientos; ++i) {
        if (alojamientos[i]->getCodigoAlojamiento() == codigoAlojamiento) {
            alojamientoPtr = alojamientos[i];
            break;
        }
    }
}

void Reservas::cargarReservas(Reservas**& reservaciones, int& totalReservas) {
    ifstream archivo("ReservasActivas.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de reservaciones.\n";
        return;
    }

    string linea;
    int total = 0;

    while (getline(archivo, linea)) total++;
    archivo.clear(); archivo.seekg(0);

    reservaciones = new Reservas*[total];
    totalReservas = total;
    int i = 0;

    while (getline(archivo, linea)) {
        stringstream frase(linea);
        string codigoReserva_, codigoAlojamiento_, cedulaHuesped_, fechaEntrada_, cantNochesStr, metodoPago_, fechaPago_, monto_, anotaciones_;
        getline(frase, codigoReserva_, ';');
        getline(frase, codigoAlojamiento_, ';');
        getline(frase, cedulaHuesped_, ';');
        getline(frase, fechaEntrada_, ';');
        getline(frase, cantNochesStr, ';');
        getline(frase, metodoPago_, ';');
        getline(frase, fechaPago_, ';');
        getline(frase, monto_, ';');
        getline(frase, anotaciones_, ';');

        int cantNochesInt = stoi(cantNochesStr);
        Reservas* nuevaReserva = new Reservas(codigoReserva_, fechaEntrada_, cantNochesInt, codigoAlojamiento_, cedulaHuesped_, metodoPago_, fechaPago_, monto_, anotaciones_);
        reservaciones[i++] = nuevaReserva;
    }

    archivo.close();
}

void Reservas::mostrarReservas() const {
    cout << "Código de reserva: " << codigoReserva << " - ";
    cout << "Fecha de entrada: " << fechaEntrada << " - ";
    cout << "Cantidad de noches: " << cantNoches << " - ";
    cout << "Código de alojamiento: " << codigoAlojamiento << " - ";
    cout << "Método de pago: " << metodoPago << " - ";
    cout << "Fecha de pago: " << fechaPago << " - ";
    cout << "Monto: " << monto << " - ";
    cout << "Anotaciones: " << anotaciones << endl;
}

void Reservas::mostrarReserva() const {
    cout << "   - Reserva para fecha: " << fechaEntrada << ", cedula huesped: " << cedulaHuesped << endl;
}

void Reservas::asociarFechasReservadas() {
    Fecha inicio = Fecha::fromString(fechaEntrada);

    fechasReservadas = new Fecha*[cantNoches];

    for (int i = 0; i < cantNoches; i++) {
        fechasReservadas[i] = new Fecha(inicio + i);  // Asumiendo que operator+ está definido
    }
}

void Reservas::mostrarComprobante() const {
    Fecha fechaInicio = Fecha::fromString(fechaEntrada);

    int noches = cantNoches;
    Fecha fechaFin = fechaInicio + noches;

    cout << "\n========== COMPROBANTE DE RESERVA ==========\n";
    cout << "Codigo de Reserva:     " << codigoReserva << endl;
    cout << "Codigo de Alojamiento: " << codigoAlojamiento << endl;
    cout << "Cedula del Huesped:    " << cedulaHuesped << endl;
    cout << "Fecha de Inicio:       ";
    fechaInicio.imprimirConDia();
    cout << "Fecha de Finalizacion: ";
    fechaFin.imprimirConDia();
    cout << "Monto Total:           $" << monto << endl;
    cout << "============================================\n";
}
