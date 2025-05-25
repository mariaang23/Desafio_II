#ifndef UTILIDADES_H
#define UTILIDADES_H

#include "alojamiento.h"
#include "reservas.h"
#include "huesped.h"
#include "anfitrion.h"

using namespace std;

bool rangeValidation(int numero, int intIni, int intFin);

int intValidation(int limInf, int limSup);

char charValidation(const string& mensajeStr);

void mostrarMenuHuesped(Huesped* huespedActual, Huesped** huespedes, int totalHuespedes, Reservas**& reservaciones, int& totalReservas);

void mostrarReservasPorAlojamiento(Alojamiento** alojamientos, int totalAlojamientos,
                                   Reservas** reservaciones, int totalReservas);

template <typename T>
void liberarArregloDePunteros(T**& arreglo, int& total){
    if (arreglo == nullptr) return;

    for (int i = 0; i < total; ++i) {
        delete arreglo[i];  // Liberar el objeto
        arreglo[i] = nullptr;
    }
    delete[] arreglo;       // Liberar el arreglo de punteros
    arreglo = nullptr;
    total = 0;
}

#endif // UTILIDADES_H
