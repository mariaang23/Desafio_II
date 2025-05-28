/**
 * @file reservas.cpp
 * @brief Implementación de la clase Reservas para gestión de reservaciones en un sistema de alojamientos.
 *
 * Esta clase permite manejar la información de las reservas activas,
 * cargar datos desde archivo, enlazar con alojamientos, gestionar fechas reservadas,
 * mostrar información relevante y manejar la memoria asociada.
 */
#include "memoria.h"
#include "reservas.h"
#include "fecha.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

/**
 * @brief Constructor de la clase Reservas.
 *
 * Inicializa una reserva con los datos proporcionados, dejando punteros a nullptr.
 *
 * @param _codigoReserva Código único de la reserva.
 * @param _fechaEntrada Fecha de entrada a la reserva en formato string.
 * @param _cantNoches Cantidad de noches reservadas.
 * @param _codigoAlojamiento Código del alojamiento asociado.
 * @param _cedulaHuesped Cédula del huésped que realizó la reserva.
 * @param _metodoPago Método de pago utilizado.
 * @param _fechaPago Fecha en que se realizó el pago.
 * @param _monto Monto pagado.
 * @param _anotaciones Anotaciones adicionales sobre la reserva.
 */
Reservas::Reservas(const string& _codigoReserva, const string& _fechaEntrada, int _cantNoches, const string& _codigoAlojamiento,
                   const string& _cedulaHuesped, const string& _metodoPago, const string& _fechaPago, const string& _monto, const string& _anotaciones)
    : codigoReserva(_codigoReserva), fechaEntrada(_fechaEntrada), cantNoches(_cantNoches), codigoAlojamiento(_codigoAlojamiento),
    alojamientoPtr(nullptr), cedulaHuesped(_cedulaHuesped), metodoPago(_metodoPago), fechaPago(_fechaPago), monto(_monto), anotaciones(_anotaciones), fechasReservadas(nullptr) {};

/**
 * @brief Destructor de la clase Reservas.
 *
 * Libera memoria dinámica asociada a fechasReservadas y desvincula puntero a alojamiento.
 */
Reservas::~Reservas() {
    if (fechasReservadas != nullptr) {
        for (int i = 0; i < cantNoches; ++i) {
            incrementarIteraciones();
            delete fechasReservadas[i];
            liberarMemoria<Fecha>(1);
            fechasReservadas[i] = nullptr;
        }
        delete[] fechasReservadas;
        liberarMemoria<Fecha*>(cantNoches);
        fechasReservadas = nullptr;
    }
    alojamientoPtr = nullptr;  // desvincular, no eliminar
}


/**
 * @brief Obtiene el código de la reserva.
 * @return Referencia constante al código de la reserva.
 */
const string& Reservas::getCodigoReserva() const {
    return codigoReserva;
}

/**
 * @brief Obtiene el código del alojamiento asociado a la reserva.
 * @return Referencia constante al código del alojamiento.
 */
const string& Reservas::getCodigoAlojamiento() const {
    return codigoAlojamiento;
}

/**
 * @brief Obtiene la cédula del huésped que realizó la reserva.
 * @return Referencia constante a la cédula del huésped.
 */
const string& Reservas::getCedulaHuesped() const{
    return cedulaHuesped;
}

/**
 * @brief Obtiene la fecha de entrada (inicio) de la reserva.
 * @return Referencia constante a la fecha de entrada.
 */
const string& Reservas::getFechaEntrada() const{
    return fechaEntrada;
}

/**
 * @brief Obtiene la cantidad de noches que dura la reserva.
 * @return Entero con la cantidad de noches.
 */
int Reservas::getCantNoches() const {
    return cantNoches;
}

/**
 * @brief Obtiene el método de pago utilizado en la reserva.
 * @return Referencia constante al método de pago.
 */
const string& Reservas::getMetodoPago() const{
    return metodoPago;
}

/**
 * @brief Obtiene la fecha en que se realizó el pago de la reserva.
 * @return Referencia constante a la fecha de pago.
 */
const string& Reservas::getFechaPago() const{
    return fechaPago;
}

/**
 * @brief Obtiene el monto total pagado por la reserva.
 * @return Referencia constante al monto.
 */
const string& Reservas::getMonto() const{
    return monto;
}

/**
 * @brief Obtiene las anotaciones adicionales relacionadas con la reserva.
 * @return Referencia constante a las anotaciones.
 */
const string& Reservas::getAnotaciones() const{
    return anotaciones;
}

/**
 * @brief Obtiene el puntero al alojamiento asociado a la reserva.
 * @return Puntero a un objeto Alojamiento.
 */
Alojamiento* Reservas::getAlojamiento() const {
    return alojamientoPtr;
}

/**
 * @brief Obtiene el arreglo dinámico de punteros a objetos Fecha que representan las fechas reservadas.
 * @return Puntero doble a Fecha.
 */
Fecha** Reservas::getFechasReservadas() const {
    return fechasReservadas;
}

/**
 * @brief Asigna un nuevo arreglo de fechas reservadas.
 *
 * Libera previamente el arreglo anterior para evitar fugas.
 *
 * @param nuevasFechas Puntero al nuevo arreglo dinámico de fechas.
 */
void Reservas::setFechasReservadas(Fecha** nuevasFechas) {
    if (fechasReservadas != nullptr) {
        for (int i = 0; i < cantNoches; i++) {
            incrementarIteraciones();
            delete fechasReservadas[i];
            liberarMemoria<Fecha>(1);
        }
        delete[] fechasReservadas;
        liberarMemoria<Fecha*>(cantNoches);
    }
    fechasReservadas = nuevasFechas;
}

/**
 * @brief Establece el puntero al alojamiento asociado.
 *
 * @param nuevoPtr Puntero al objeto Alojamiento.
 */
void Reservas::setAlojamientoPtr(Alojamiento* nuevoPtr){
    alojamientoPtr = nuevoPtr;
}

/**
 * @brief Enlaza la reserva con un alojamiento existente en el arreglo proporcionado.
 *
 * Busca el alojamiento cuyo código coincida con el de la reserva y enlaza el puntero.
 *
 * @param alojamientos Arreglo de punteros a alojamientos disponibles.
 * @param totalAlojamientos Cantidad total de alojamientos en el arreglo.
 */
void Reservas::enlazarAlojamiento(Alojamiento** alojamientos, int totalAlojamientos) {
    for (int i = 0; i < totalAlojamientos; ++i) {
        incrementarIteraciones();
        if (alojamientos[i]->getCodigoAlojamiento() == codigoAlojamiento) {
            alojamientoPtr = alojamientos[i];
            break;
        }
    }

}

/**
 * @brief Carga las reservas activas desde un archivo de texto.
 *
 * Lee línea por línea el archivo "ReservasActivas.txt" y crea objetos Reservas.
 * Almacena los punteros en un arreglo dinámico, que se asigna por referencia.
 *
 * @param reservaciones Referencia al arreglo de punteros a Reservas que será asignado.
 * @param totalReservas Referencia a la variable donde se almacenará la cantidad total de reservas leídas.
 */
void Reservas::cargarReservas(Reservas**& reservaciones, int& totalReservas) {
    ifstream archivo("ReservasActivas.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de reservaciones.\n";
        return;
    }

    string linea;
    int total = 0;

    while (getline(archivo, linea)) total++;
    archivo.clear(); archivo.seekg(0);


    // Reservar memoria para el arreglo de punteros a Reservas
    reservaciones = new Reservas*[total];
    registrarMemoria<Reservas*>(total);
    totalReservas = total;
    int i = 0;

    // Leer cada línea y extraer los campos separados por ';'
    while (getline(archivo, linea)) {
        stringstream frase(linea);
        string codigoReserva_, codigoAlojamiento_, cedulaHuesped_, fechaEntrada_, cantNochesStr, metodoPago_, fechaPago_, monto_, anotaciones_;
        getline(frase, codigoReserva_, ';');
        getline(frase, codigoAlojamiento_, ';');
        getline(frase, cedulaHuesped_, ';');
        getline(frase, fechaEntrada_, ';');
        getline(frase, cantNochesStr, ';');
        getline(frase, metodoPago_, ';');
        getline(frase, fechaPago_, ';');
        getline(frase, monto_, ';');
        getline(frase, anotaciones_, ';');

        // Convertir cantidad de noches de string a int
        int cantNochesInt = stoi(cantNochesStr);
        // Crear nuevo objeto Reserva con los datos extraídos
        Reservas* nuevaReserva = new Reservas(codigoReserva_, fechaEntrada_, cantNochesInt, codigoAlojamiento_, cedulaHuesped_, metodoPago_, fechaPago_, monto_, anotaciones_);
        registrarMemoria<Reservas>(1);
        reservaciones[i++] = nuevaReserva;  // Guardar puntero en el arreglo
        incrementarIteraciones();
    }

    archivo.close();
}

void Reservas::mostrarReserva() const {
    cout << "   - Reserva para fecha: " << fechaEntrada << ", cedula huesped: " << cedulaHuesped << endl;
}

/**
 * @brief Crea y asigna el arreglo dinámico fechasReservadas con todas las fechas ocupadas.
 *
 * Cada fecha es un objeto Fecha que comienza en fechaEntrada y avanza un día por cada noche reservada.
 * Evita reasignar si ya está asignado para evitar fugas de memoria.
 * Valida que la cantidad de noches esté en rango válido.
 */
void Reservas::asociarFechasReservadas() {

     // Verificar si ya están asignadas las fechas para evitar fugas de memoria
    if (fechasReservadas != nullptr) {
        return;
    }

    // Validar que la cantidad de noches sea válida
    if (fechasReservadas != nullptr) {
        // Ya asignado; no volver a asignar para evitar fugas
        return;
    }
    if (cantNoches <= 0 || cantNoches > 365) {
        cout << "asociarFechasReservadas: cantNoches invalido (" << cantNoches << ") en " << codigoReserva << endl;
        return;
    }

    // Convertir la fecha de entrada de string a objeto Fecha y crear arreglo dinamico para fechas reservadas
    Fecha inicio = Fecha::fromString(fechaEntrada);
    fechasReservadas = new Fecha*[cantNoches];
    registrarMemoria<Fecha*>(cantNoches);

     // Asignar cada fecha consecutiva sumando días a la fecha inicial
    for (int i = 0; i < cantNoches; i++) {
        incrementarIteraciones();
        fechasReservadas[i] = new Fecha(inicio + i);
        registrarMemoria<Fecha>(1);
    }

}


/**
 * @brief Muestra un comprobante de la reserva con detalles completos.
 *
 * Imprime la información clave en formato legible, incluyendo fechas con día de la semana.
 */
void Reservas::mostrarComprobante() const {
    Fecha fechaInicio = Fecha::fromString(fechaEntrada);

    int noches = cantNoches;
    Fecha fechaFin = fechaInicio + noches;

    cout << "\n========== COMPROBANTE DE RESERVA ==========\n";
    cout << "Codigo de Reserva:     " << codigoReserva << endl;
    cout << "Codigo de Alojamiento: " << codigoAlojamiento << endl;
    cout << "Cedula del Huesped:    " << cedulaHuesped << endl;
    cout << "Fecha de Inicio:       ";
    fechaInicio.imprimirConDia();
    cout << "Fecha de Finalizacion: ";
    fechaFin.imprimirConDia();
    cout << "Monto Total:           $" << monto << endl;
    cout << "============================================\n";
}
