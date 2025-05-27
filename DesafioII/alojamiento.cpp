#include "alojamiento.h"
#include "fecha.h"
#include "reservas.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Alojamiento::Alojamiento(const string& _codigo, const string& _nombre, const string& _nombreAnf,
                         const string& _departamento, const string& _municipio, const string& _tipo,
                         const string& _direccion, const int &_precioStr, const string& _amenidades)
    : codigo(_codigo), nombre(_nombre), nombreAnf(_nombreAnf), departamento(_departamento), municipio(_municipio),
      tipo(_tipo), direccion(_direccion), precio(_precioStr), amenidades(_amenidades) {}

const string& Alojamiento::getCodigoAlojamiento() const {
    return codigo;
}

const int& Alojamiento::getPrecio() const {
    return precio;
}

const string& Alojamiento::getNombre() const {
    return nombre;
}


const string& Alojamiento::getMunicipio() const {
    return municipio;
}

void Alojamiento::mostrarAlojamientos() const {
    cout << "    - " << nombre << " (" << tipo << ", $" << precio << ")\n";
}

void Alojamiento::cargarAlojamientos(Alojamiento**& alojamientos, int& totalAlojamientos) {
    ifstream archivo("alojamientos.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de Alojamientos\n";
        return;
    }

    string linea;
    int total = 0;

    while (getline(archivo, linea)) total++;
    archivo.clear();
    archivo.seekg(0);

    alojamientos = new Alojamiento*[total];
    totalAlojamientos = total;
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
        int precioInt_ = stoi(precio_);
        Alojamiento* nuevoAlojamiento = new Alojamiento(codigo_, nombre_, nombreAnf_, departamento_, municipio_, tipo_, direccion_, precioInt_, amenidades_);
        alojamientos[i++] = nuevoAlojamiento;
    }
    archivo.close();
}


bool Alojamiento::estaDisponible(const string& fechaEntrada, int cantNoches, Reservas** reservas, int totalReservas)
{
    bool solapa = false;
    Fecha fechaInicio = Fecha::fromString(fechaEntrada);
    Fecha** fechasIngresadas = new Fecha*[cantNoches];

    for (int j = 0; j < cantNoches; j++) {
        fechasIngresadas[j] = new Fecha(fechaInicio + j);
    }

    for (int i = 0; i < totalReservas && !solapa; i++) {
        int cantidadExistente = reservas[i]->getCantNoches();
        Fecha reservaInicio = Fecha::fromString(reservas[i]->getFechaEntrada());
        Fecha** fechasExistentes = new Fecha*[cantidadExistente];

        for (int j = 0; j < cantidadExistente; j++) {
            fechasExistentes[j] = new Fecha(reservaInicio + j);
        }

        // Comparar todas las fechas entre la reserva y las nuevas fechas
        for (int j = 0; j < cantNoches && !solapa; j++) {
            for (int k = 0; k < cantidadExistente && !solapa; k++) {
                if (*fechasIngresadas[j] == *fechasExistentes[k]) {  // Comparacion por contenido
                    //si hay solapamiento el alojamiento no esta disponible
                    solapa = true;
                }
            }
        }

        // Liberar memoria de fechasExistentes
        for (int j = 0; j < cantidadExistente; j++) {
            delete fechasExistentes[j];
        }
        delete[] fechasExistentes;
    }

    // Liberar memoria de fechasIngresadas
    for (int j = 0; j < cantNoches; j++) {
        delete fechasIngresadas[j];
    }
    delete[] fechasIngresadas;

    //si no hay solapamiento el alojamiento esta disponible
    return !solapa;
}

