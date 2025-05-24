#ifndef UTILIDADES_H
#define UTILIDADES_H

#include "alojamiento.h"
#include "anfitrion.h"
#include "reservas.h"
#include "huesped.h"

using namespace std;

bool rangeValidation(int numero, int intIni, int intFin);

int intValidation(int limInf, int limSup);

char charValidation(const string& mensajeStr);

void mostrarMenuAnfitrion(Anfitrion* anfitrionActual, Reservas** reservaciones, int totalReservas);

void mostrarMenuHuesped(Huesped* huespedActual, Huesped** huespedes, int totalHuespedes);

void mostrarReservasPorAlojamiento(Alojamiento** alojamientos, int totalAlojamientos,
                                   Reservas** reservaciones, int totalReservas);

template<typename T>
void liberarArregloDePunteros(T**& arreglo, int& total);
#endif // UTILIDADES_H
