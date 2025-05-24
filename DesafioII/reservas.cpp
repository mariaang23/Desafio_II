#include "reservas.h"
#include "fecha.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Reservas::Reservas(const string& _codigoReserva, const string& _fechaEntrada, const string& _cantNoches, const string& _codigoAlojamiento,
                   const string& _cedulaHuesped, const string& _metodoPago, const string& _fechaPago, const string& _monto, const string& _anotaciones)
    : codigoReserva(_codigoReserva), fechaEntrada(_fechaEntrada), cantNoches(_cantNoches), codigoAlojamiento(_codigoAlojamiento),
      alojamientoPtr(nullptr), cedulaHuesped(_cedulaHuesped), metodoPago(_metodoPago), fechaPago(_fechaPago), monto(_monto), anotaciones(_anotaciones) {};

Reservas::~Reservas() {
    int noches = stoi(cantNoches);
    for (int i = 0; i < noches; i++){
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

const string& Reservas::getCantNoches() const {
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

float Reservas::calcularMonto(Alojamiento* _alojamientoPtr, string& _cantNoches) {
    float preNoche = stof(_alojamientoPtr->getPrecio());
    int cantidadNoches = stoi(_cantNoches);
    float montoPagar = preNoche * cantidadNoches;
    return montoPagar;
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
        string codigoReserva_, codigoAlojamiento_, cedulaHuesped_, fechaEntrada_, cantNoches_, metodoPago_, fechaPago_, monto_, anotaciones_;
        getline(frase, codigoReserva_, ';');
        getline(frase, codigoAlojamiento_, ';');
        getline(frase, cedulaHuesped_, ';');
        getline(frase, fechaEntrada_, ';');
        getline(frase, cantNoches_, ';');
        getline(frase, metodoPago_, ';');
        getline(frase, fechaPago_, ';');
        getline(frase, monto_, ';');
        getline(frase, anotaciones_, ';');

        Reservas* nuevaReserva = new Reservas(codigoReserva_, fechaEntrada_, cantNoches_, codigoAlojamiento_, cedulaHuesped_, metodoPago_, fechaPago_, monto_, anotaciones_);
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
    int noches = stoi(cantNoches);
    string fechaStr = fechaEntrada + '/';
    int partes[3] = {0, 0, 0};
    int parteActual = 0;
    string subStrFec = "";

    for (char c : fechaStr) {
        if (c == '/') {
            if (parteActual < 3) {
                if (subStrFec.empty()) {
                    cout << "Error: subStrFec esta vacío antes de la conversión a int.\n";
                    return;
                }
                partes[parteActual++] = stoi(subStrFec);
                subStrFec = "";
            }
        } else {
            subStrFec += c;
        }
    }

    Fecha inicio(partes[0], partes[1], partes[2]);

    fechasReservadas = new Fecha*[noches];

    for (int i = 0; i < noches; i++) {
        fechasReservadas[i] = new Fecha(inicio + i);
    }
}

void Reservas::mostrarFechasReservadas() const {
    cout << "Fechas reservadas para la reserva " << codigoReserva << endl;
    for (int i = 0; i < stoi(cantNoches); ++i) {
        fechasReservadas[i]->imprimir();
        cout << endl;
    }
}

void Reservas::guardarReservasActivasArchivo(Reservas** reservas, int totalReservas, const string& archivo){
    ofstream out(archivo, ios::trunc);
    if (!out.is_open()) {
        cerr << "No se pudo abrir el archivo: " << archivo << endl;
        return;
    }
    cout << totalReservas << endl;

    for (int i = 0; i < totalReservas; ++i) {
        out << reservas[i]->getCodigoReserva() << ";"
            << reservas[i]->getCodigoAlojamiento() << ";"
            << reservas[i]->getCedulaHuesped() << ";"
            << reservas[i]->getFechaEntrada() << ";"
            << reservas[i]->getCantNoches() << ";"
            << reservas[i]->getMetodoPago() << ";"
            << reservas[i]->getFechaPago() << ";"
            << reservas[i]->getMonto() << ";"
            << reservas[i]->getAnotaciones() << endl;
    }

    out.close();
}

