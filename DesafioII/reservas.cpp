#include "reservas.h"
#include "fecha.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Reservas::Reservas(const string& _codigoReserva, const string& _fechaEntrada, const string& _cantNoches, const string& _codigoAlojamiento,
                   const string& _cedulaHuesped, const string& _metodoPago, const string& _fechaPago, const string& _monto, const string& _anotaciones)
    : codigoReserva(_codigoReserva), fechaEntrada(_fechaEntrada), cantNoches(_cantNoches), codigoAlojamiento(_codigoAlojamiento),
    cedulaHuesped(_cedulaHuesped), metodoPago(_metodoPago), fechaPago(_fechaPago), monto(_monto), anotaciones(_anotaciones){

};

Reservas::~Reservas() {
    delete[] fechasReservadas;
}

const string& Reservas::getCodigoReserva() const {
    return codigoReserva;
}

const string& Reservas::getCodigoAlojamiento() const {
    return codigoAlojamiento;
}

const string& Reservas::getCantNoches() const {
    return cantNoches;
}

void Reservas::cargarReservas(Reservas**& reservaciones, int& totalReservas ){
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
        Reservas* nuevaReserva= new Reservas(codigoReserva_, fechaEntrada_, cantNoches_, codigoAlojamiento_, cedulaHuesped_, metodoPago_, fechaPago_, monto_, anotaciones_);
        reservaciones[i++] = nuevaReserva;
    }

    archivo.close();

}

void Reservas::mostrarReservas() const{
    cout << "Código de reserva: " << codigoReserva << " - ";
    cout << "Fecha de entrada: " << fechaEntrada << " - ";
    cout << "Cantidad de noches: " << cantNoches << " - ";
    cout << "Código de alojamiento: " << codigoAlojamiento << " - ";
    cout << "Método de pago: " << metodoPago << " - ";
    cout << "Fecha de pago: " << fechaPago << " - ";
    cout << "Monto: " << monto << " - ";
    cout << "Anotaciones: " << anotaciones << endl;
}


void Reservas::asociarFechasReservadas(){
    int noches = stoi(cantNoches);
    string fechaStr = fechaEntrada + '/'; //Concantenar '/' para procesar el año
    int partes[3] = {0, 0, 0};  // dia, mes, año
    int parteActual = 0;
    string subStrFec = "";


    // Iteramos sobre cada carácter de fechaStr
    for (char c : fechaStr){
        if (c == '/'){
            if (parteActual < 3){
                // Validar que subStrFec no esté vacío antes de convertirlo
                if (subStrFec.empty()) {
                    cout << "Error: subStrFec esta vacío antes de la conversion a int.\n";
                    return;  // Salir si subStrFec está vacío
                }
                partes[parteActual++] = stoi(subStrFec);  // Convertir a entero
                subStrFec = "";  // Limpiar subStrFec para la siguiente parte
            }
        } else {
            subStrFec += c;  // Agregar el carácter a subStrFec
        }
    }

    // Crear la fecha de inicio con las partes extraídas
    Fecha inicio(partes[0], partes[1], partes[2]);

    // Crear el arreglo de fechas reservadas
    fechasReservadas = new Fecha*[noches];

    // Llenar el arreglo de fechas con las fechas reservadas (sumando días)
    for (int i = 0; i < noches; i++) {
        fechasReservadas[i] = new Fecha(inicio + i);  // Sumar i días a la fecha de inicio
    }
}


void Reservas::mostrarFechasReservadas() const {
    cout << "Fechas reservadas para la reserva " << codigoReserva << endl;
    for (int i = 0; i < stoi(cantNoches); ++i) {
        fechasReservadas[i]->imprimir();
        cout << endl;
    }

}


