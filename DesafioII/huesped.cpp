#include "huesped.h"
#include "reservas.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Huesped::Huesped(const string& _cedula, const string& _clave, const string& _antiguedad,
                 const string& _puntuacion, const string& _codigosReservas)
    : cedula(_cedula), clave(_clave), antiguedad(_antiguedad), puntuacion(_puntuacion), codigosReservas(_codigosReservas){}


Huesped::~Huesped(){
    if (reservasHuesped != nullptr) {
        delete[] reservasHuesped; // Eliminar el arreglo de reservas
        reservasHuesped = nullptr; // Evitar el puntero colgante
    }
}

const string& Huesped::getCedulaHuesped() const {
    return cedula;
}

const string& Huesped::getClaveHuesped() const{
    return clave;
}


void Huesped::cargarHuespedes(Huesped**& huespedes, int& totalHuespedes){
    ifstream archivo("huespedes.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de huespedes.\n";
        return;
    }

    string linea;
    int total = 0;

    while (getline(archivo, linea)) total++;
    archivo.clear(); archivo.seekg(0);

    huespedes = new Huesped*[total];
    totalHuespedes = total;
    int i = 0;

    while (getline(archivo, linea)) {
        stringstream frase(linea);
        string cedula_, clave_, antiguedad_, puntuacion_, codigosReservas_;
        getline(frase, cedula_, ';');
        getline(frase, clave_, ';');
        getline(frase, antiguedad_, ';');
        getline(frase, puntuacion_, ';');
        getline(frase, codigosReservas_, ';');
        Huesped* nuevoHuesped= new Huesped(cedula_, clave_, antiguedad_, puntuacion_, codigosReservas_);
        huespedes[i++] = nuevoHuesped;
    }
    archivo.close();
}

void Huesped::asociarReservas(Reservas** listaReservas, int totalReservas){
    int numReservas = 0;
    for (char c : codigosReservas){
        if (c == ',') numReservas++; // Contar la cantidad de reservas que tiene cada huesped
    }
    //   std::cout << numReservas << std::endl;
    /*
    if (reservasHuesped != nullptr) {
        delete[] reservasHuesped;  // Limpiar las reservas existentes, si las hay
    }
    */

    reservasHuesped = new Reservas*[numReservas]; // Arreglo para almacenar reservas que le corresponden a cada huesped
    cantidadReservas = 0;

    // Asociar los códigos de reservas con las instancias de Reservas
    size_t start = 0;
    while (start < codigosReservas.length()){
        size_t end = codigosReservas.find(',', start);
        if (end == string::npos) {
            end = codigosReservas.length();
        }
        string codReserva = codigosReservas.substr(start, end - start); //Obtener cada codigo de reserva que le pertenece al huesped
        if (codReserva.empty()) {
            break; // o continue, si quieres seguir procesando
        }

        //Buscar la reserva correspondiente en la lista de reservas
        if (!codReserva.empty()){    for (int i = 0; i < totalReservas; i++){
                //cout << "objRes: "<< listaReservas[i]->getCodigoReserva() << " - " << "codResHuesp:" <<codReserva <<endl;
                if (listaReservas[i]->getCodigoReserva() == codReserva){
                    //cout <<"Entra" << endl;
                    reservasHuesped[cantidadReservas++] = listaReservas[i]; // Asignar la reserva al arreglo de reservas del Huesped (enlazar al huesped con sus reservas)
                    break;
                }
            }
        }
        start = end + 1;
    }
}

void Huesped::mostrarReservasHuesped(){
    std::cout << "Reservas para huesped " << cedula << ":\n";
    for (int j = 0; j < cantidadReservas; j++){
        std::cout << "Reservas: " << cantidadReservas << std::endl;
        reservasHuesped[j]->mostrarReservas();

    }

}

