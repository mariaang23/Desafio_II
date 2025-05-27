#include "anfitrion.h"
#include "alojamiento.h"
#include "huesped.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Anfitrion::Anfitrion(const string& _cedula, const string& _clave, const string& _antiguedad, const string& _puntos, const string& _codigosAlojamientos)
    : cedula(_cedula),clave(_clave), antiguedad(_antiguedad), puntuacion(_puntos), codigosAlojamientos(_codigosAlojamientos) {
}

const string& Anfitrion::getCedulaAnfitrion() const {
    return cedula;
}

const string& Anfitrion::getClaveAnfitrion() const{
    return clave;
}

const string& Anfitrion::getPuntuacion() const{
    return puntuacion;
}

void Anfitrion::cargarAnfitriones(Anfitrion**& anfitriones, int& totalAnfitriones) {
    ifstream archivo("anfitriones.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de Anfitriones\n";
        return;
    }

    string linea;
    int total = 0;

    while (getline(archivo, linea)) total++;
    archivo.clear();
    archivo.seekg(0);

    anfitriones = new Anfitrion*[total];
    totalAnfitriones = total;
    int i = 0;

    while (getline(archivo, linea)) {
        stringstream frase(linea);
        string cedula_, clave_, antiguedad_, puntuacion_, codigosAlojamientos_;
        getline(frase, cedula_, ';');
        getline(frase, clave_, ';');
        getline(frase, antiguedad_, ';');
        getline(frase, puntuacion_, ';');
        getline(frase, codigosAlojamientos_, ';');
        Anfitrion* nuevoAnfitrion= new Anfitrion(cedula_, clave_, antiguedad_, puntuacion_, codigosAlojamientos_);
        anfitriones[i++] = nuevoAnfitrion;
    }
    archivo.close();
}

void Anfitrion::asociarAlojamientos(Alojamiento** listaAlojamientos, int totalAlojamientos){
    int numAlojamientos = 0;
    for (char c : codigosAlojamientos){
        if (c == ',') numAlojamientos++;
    }
    /*
    if (alojamientosAnfitrion != nullptr) {
        delete[] alojamientosAnfitrion;
    }
*/

    alojamientosAnfitrion = new Alojamiento*[numAlojamientos];
    cantidadAlojamientos = 0;

    size_t start = 0;
    while (start < codigosAlojamientos.length()){
        size_t end = codigosAlojamientos.find(',', start);
        if (end == string::npos) {
            end = codigosAlojamientos.length();
        }
        string codAlojamiento = codigosAlojamientos.substr(start, end - start);


        // Limpieza de espacios al inicio y al final
        while (!codAlojamiento.empty() && codAlojamiento.front() == ' ')
            codAlojamiento.erase(0, 1);
        while (!codAlojamiento.empty() && codAlojamiento.back() == ' ')
            codAlojamiento.pop_back();

        //Buscar la reserva correspondiente en la lista de reservas
        if (!codAlojamiento.empty()){
            for (int i = 0; i < totalAlojamientos; i++){
                if (listaAlojamientos[i]->getCodigoAlojamiento() == codAlojamiento){
                    alojamientosAnfitrion[cantidadAlojamientos++] = listaAlojamientos[i];
                    break;
                }
            }
        }
        start = end + 1;
    }
}

void Anfitrion::mostrarAlojamientosAnfitrion(){
    std::cout << "Anfitrion " << cedula << ":\n";
    for (int j = 0; j < cantidadAlojamientos; j++){
        std::cout << "Alojamientos: " << j+1 << std::endl;
        alojamientosAnfitrion[j]->mostrarAlojamientos();
    }
}

void Anfitrion::mostrarReservasDeSusAlojamientos(Reservas** reservaciones, int totalReservas) {
    for (int i = 0; i < cantidadAlojamientos; ++i) {

        string codAloj = alojamientosAnfitrion[i]->getCodigoAlojamiento();
        cout << "- Alojamiento: " << alojamientosAnfitrion[i]->getNombre()
             << " (Codigo: " << codAloj << ")" << endl;

        bool tieneReservas = false;

        for (int j = 0; j < totalReservas; ++j) {
            if (reservaciones[j]->getCodigoAlojamiento() == codAloj) {
                reservaciones[j]->mostrarReserva();
                tieneReservas = true;
            }
        }

        if (!tieneReservas) {
            cout << "   No tiene reservas\n";
        }
    }
}

void Anfitrion::anularReservacion(const string& _codigoReserva, Reservas **&reservas, int& totalReservas, Huesped** huespedes, int totalHuespedes) {
    // Buscar la reserva
    int indexReserva = -1;
    string codigoAlojamientoReserva = "";
    string cedulaHuespedActualizar = "";

    for (int i = 0; i < totalReservas; ++i) {
        if (reservas[i]->getCodigoReserva() == _codigoReserva) {
            indexReserva = i;
            codigoAlojamientoReserva = reservas[i]->getCodigoAlojamiento();
            cedulaHuespedActualizar = reservas[i]->getCedulaHuesped();
            break;
        }
    }

    if (indexReserva == -1) {
        cout << "Reserva no encontrada.\n";
        return;
    }

    // Verificar que la reserva este asociada a un alojamiento del anfitrion
    bool perteneceAlAnfitrion = false;
    for (int i = 0; i < cantidadAlojamientos; ++i) {
        if (alojamientosAnfitrion[i]->getCodigoAlojamiento() == codigoAlojamientoReserva) {
            perteneceAlAnfitrion = true;
            break;
        }
    }

    if (!perteneceAlAnfitrion) {
        cout << "Error: No tienes permiso para eliminar esta reserva, ya que no pertenece a tus alojamientos.\n";
        return;
    }

    // Buscar el huesped correspondiente
    Huesped* huespedActualizado = nullptr;
    for (int i = 0; i < totalHuespedes; ++i) {
        if (huespedes[i]->getCedulaHuesped() == cedulaHuespedActualizar) {
            huespedActualizado = huespedes[i];
            break;
        }
    }

    // Eliminar reserva del huésped
    huespedActualizado->liberarReservasHuesped(_codigoReserva);

    // Eliminar del arreglo global
    delete reservas[indexReserva];
    for (int i = indexReserva; i < totalReservas - 1; ++i) {
        reservas[i] = reservas[i + 1];
    }
    totalReservas--;
    reservas[totalReservas] = nullptr;

    // Reasociar reservas locales al huésped
    huespedActualizado->asociarReservas(reservas, totalReservas);

    cout << "Reserva " << _codigoReserva << " eliminada exitosamente.\n";
}

bool Anfitrion::poseeAlojamiento(const string& codigoAloj) const {
    for (int i = 0; i < cantidadAlojamientos; ++i) {
        if (alojamientosAnfitrion[i]->getCodigoAlojamiento() == codigoAloj) {
            return true;
        }
    }
    return false;
}


