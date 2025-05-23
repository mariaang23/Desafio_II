#include "reservas.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Reservas::Reservas(const string& _codigoReserva, const string& _fechaEntrada, const string& _cantNoches, const string& _codigoAlojamiento,
                   const string& _cedulaHuesped, const string& _metodoPago, const string& _fechaPago, const string& _monto, const string& _anotaciones)
    : codigoReserva(_codigoReserva), fechaEntrada(_fechaEntrada), cantNoches(_cantNoches), codigoAlojamiento(_codigoAlojamiento), alojamientoPtr(nullptr),
    cedulaHuesped(_cedulaHuesped), metodoPago(_metodoPago), fechaPago(_fechaPago), monto(_monto), anotaciones(_anotaciones){};


const string& Reservas::getCodigoReserva() const {
    return codigoReserva;
}

const string& Reservas::getCodigoAlojamiento() const {
    return codigoAlojamiento;
}

const string& Reservas::getCantidadNoches() const{
    return cantNoches;
}

Alojamiento* Reservas::getAlojamiento() const {
    return alojamientoPtr;
}

const string& Reservas::getFechaEntrada() const{
    return fechaEntrada;
}
void Reservas::enlazarAlojamiento(Alojamiento** alojamientos, int totalAlojamientos) {
    for (int i = 0; i < totalAlojamientos; ++i) {
        if (alojamientos[i]->getCodigoAlojamiento() == codigoAlojamiento) {
            alojamientoPtr = alojamientos[i];
            break;
        }
    }
}

float Reservas::calcularMonto(Alojamiento* _alojamientoPtr, string& _cantNoches){
    float preNoche = stof(_alojamientoPtr->getPrecio());
    float cantidadNoches = stof(_cantNoches);
    float montoPagar=preNoche*cantidadNoches;

    return montoPagar;
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
        string codigoReserva_, fechaEntrada_, cantNoches_, codigoAlojamiento_, cedulaHuesped_, metodoPago_, fechaPago_, monto_, anotaciones_;
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

void Reservas::mostrarReserva() const {
    cout << "   - Reserva para fecha: " << fechaEntrada << ", nombre huesped: " << cedulaHuesped << endl;
}


