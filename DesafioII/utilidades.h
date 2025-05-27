#ifndef UTILIDADES_H
#define UTILIDADES_H

#include "alojamiento.h"
#include "reservas.h"
#include "huesped.h"
#include "anfitrion.h"

using namespace std;

// Validaciones básicas
bool rangeValidation(int numero, int intIni, int intFin);
int intValidation(int limInf, int limSup);
char charValidation(const string& mensajeStr);

// Menús principales
void mostrarMenuAnfitrion(Anfitrion* anfitrionActual, Reservas**& reservaciones, int& totalReservas,
                          Huesped**& huespedes, int& totalHuespedes);

void mostrarMenuHuesped(Huesped* huespedActual, Reservas**& reservaciones, int& totalReservas,
                        Alojamiento** alojamientos, int totalAlojamientos,
                        Anfitrion** anfitriones, int totalAnfitriones);

// Utilidades de reserva
void mostrarReservasPorAlojamiento(Alojamiento** alojamientos, int totalAlojamientos,
                                   Reservas** reservaciones, int totalReservas);

void guardarHuespedesArchivo(Huesped** huespedes, int totalHuespedes, const string& archivo);
void guardarReservasActivasArchivo(Reservas** reservas, int totalReservas, const string& archivo);
void actualizarHistorico(Reservas**& reservasActivas, int& totalReservas,
                         Huesped** huespedes, int totalHuespedes, Fecha& fechaCorte);

// Liberar arreglos de punteros
template <typename T>
void liberarArregloDePunteros(T**& arreglo, int& total) {
    if (arreglo == nullptr) return;

    for (int i = 0; i < total; ++i) {
        delete arreglo[i];
        arreglo[i] = nullptr;
    }

    delete[] arreglo;
    arreglo = nullptr;
    total = 0;
}

#endif // UTILIDADES_H
