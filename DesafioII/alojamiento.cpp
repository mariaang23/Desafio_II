#include "alojamiento.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Alojamiento::Alojamiento(const string& _codigo, const string& _nombre, const string& _nombreAnf,
                         const string& _departamento, const string& _municipio, const string& _tipo,
                         const string& _direccion, const string& _precioStr, const string& _amenidades)
    : codigo(_codigo), nombre(_nombre), nombreAnf(_nombreAnf), departamento(_departamento), municipio(_municipio),
    tipo(_tipo), direccion(_direccion), precio(_precioStr), amenidades(_amenidades){}

const string& Alojamiento::getCodigoAlojamiento() const {
    return codigo;
}

const string& Alojamiento::getPrecio() const{
    return precio;
}

void Alojamiento::mostrarAlojamientos() const {
    cout << "    - " << nombre << " (" << tipo << ", $" << precio << ")\n";
}

void Alojamiento::cargarAlojamientos(Alojamiento**& alojamientos, int& totalAlojamientos) {
    ifstream archivo("alojamientos.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de Alojamientos.\n";
    }

    string linea;
    int total = 0;

    while (getline(archivo, linea)) total++;
    archivo.clear();
    archivo.seekg(0);

    alojamientos = new Alojamiento*[total];
    totalAlojamientos=total;
    int i = 0;

    while (getline(archivo, linea)) {
        stringstream frase(linea);
        string codigo_, nombre_, nombreAnf_, departamento_, municipio_, tipo_, direccion_, precio_, amenidades_;
        getline(frase, codigo_, ';');
        getline(frase, nombre_, ';');
        getline(frase, nombreAnf_, ';');
        getline(frase, departamento_, ';');
        getline(frase, municipio_, ';');
        getline(frase, tipo_, ';');
        getline(frase, direccion_, ';');
        getline(frase, precio_, ';');
        getline(frase, amenidades_);
        Alojamiento * nuevoAlojamiento= new Alojamiento(codigo_, nombre_, nombreAnf_, departamento_, municipio_, tipo_, direccion_, precio_, amenidades_);
        alojamientos[i++] = nuevoAlojamiento;
    }
    archivo.close();
}
