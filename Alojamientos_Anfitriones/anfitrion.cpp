#include "anfitrion.h"
#include "alojamiento.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Anfitrion::Anfitrion(const string& _cedula, const string& _clave, const string& _antiguedad, const string& _puntos, const string& _codigosAlojamientos)
    : cedula(_cedula),clave(_clave), antiguedad(_antiguedad), puntuacion(_puntos), codigosAlojamientos(_codigosAlojamientos) {
}

const string& Anfitrion::getCedula() const {
    return cedula;
}

void Anfitrion::cargarAnfitriones(Anfitrion**& anfitriones, int& totalAnfitriones) {
    ifstream archivo("anfitriones.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de Anfitriones.\n";
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

    if (alojamientosAnfitrion != nullptr) {
        delete[] alojamientosAnfitrion;
    }

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
