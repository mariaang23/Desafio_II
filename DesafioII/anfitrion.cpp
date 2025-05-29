/**
 * @file anfitrion.cpp
 * @brief Implementación de la clase Anfitrión, que gestiona la información y operaciones relacionadas con los anfitriones
 *        responsables de alojamientos, incluyendo atributos personales y métodos para acceso y manipulación de datos.
 */

#include "memoria.h"
#include "anfitrion.h"
#include "alojamiento.h"
#include "huesped.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

/**
 * @brief Constructor de la clase Anfitrion.
 * @param _cedula Cédula del anfitrión.
 * @param _clave Clave del anfitrión.
 * @param _antiguedad Antigüedad como anfitrión.
 * @param _puntos Puntuación obtenida.
 * @param _codigosAlojamientos Códigos de alojamientos asociados separados por comas.
 */
Anfitrion::Anfitrion(const string& _cedula, const string& _clave, const string& _antiguedad, const string& _puntos, const string& _codigosAlojamientos)
    : cedula(_cedula),clave(_clave), antiguedad(_antiguedad), puntuacion(_puntos), codigosAlojamientos(_codigosAlojamientos) {
}


/**
 * @brief Obtiene la cédula del anfitrión.
 * @return Cédula como string.
 */
const string& Anfitrion::getCedulaAnfitrion() const {
    return cedula;
}

/**
 * @brief Obtiene la clave del anfitrión.
 * @return Clave como string.
 */
const string& Anfitrion::getClaveAnfitrion() const{
    return clave;
}

/**
 * @brief Obtiene la puntuación del anfitrión.
 * @return Puntuación como string.
 */
const string& Anfitrion::getPuntuacion() const{
    return puntuacion;
}

/**
 * @brief Carga los datos de todos los anfitriones desde un archivo de texto.
 * @param anfitriones Arreglo de punteros donde se almacenarán los anfitriones.
 * @param totalAnfitriones Número total de anfitriones leídos.
 */
void Anfitrion::cargarAnfitriones(Anfitrion**& anfitriones, int& totalAnfitriones) {
    ifstream archivo("anfitriones.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de Anfitriones\n";
        return;
    }

    string linea;
    int total = 0;

    // Cuenta la cantidad total de líneas (anfitriones) en el archivo
    while (getline(archivo, linea)) total++;
    archivo.clear();
    archivo.seekg(0);

    // Reserva memoria para el arreglo de punteros a Anfitrion
    anfitriones = new Anfitrion*[total];
    registrarMemoria<Anfitrion*>(total);
    totalAnfitriones = total;
    int i = 0;

    // Lee línea por línea, separa cada dato y crea un nuevo objeto Anfitrion
    while (getline(archivo, linea)) {
        stringstream frase(linea);
        string cedula_, clave_, antiguedad_, puntuacion_, codigosAlojamientos_;
        getline(frase, cedula_, ';');
        getline(frase, clave_, ';');
        getline(frase, antiguedad_, ';');
        getline(frase, puntuacion_, ';');
        getline(frase, codigosAlojamientos_, ';');
        Anfitrion* nuevoAnfitrion= new Anfitrion(cedula_, clave_, antiguedad_, puntuacion_, codigosAlojamientos_);
        registrarMemoria<Anfitrion>(1);
        anfitriones[i++] = nuevoAnfitrion;
        incrementarIteraciones();
    }
    archivo.close();

}

/**
 * @brief Asocia alojamientos al anfitrión en función de los códigos almacenados.
 * @param listaAlojamientos Lista global de alojamientos disponibles.
 * @param totalAlojamientos Cantidad total de alojamientos disponibles.
 */
void Anfitrion::asociarAlojamientos(Alojamiento** listaAlojamientos, int totalAlojamientos){
    int numAlojamientos = 0;
    // Cuenta la cantidad de alojamientos sumando las comas
    for (char c : codigosAlojamientos){
        if (c == ',') numAlojamientos++;
    }

     // Reserva memoria para alojamientos asociados (nota: la cantidad de alojamientos es numAlojamientos + 1)
    alojamientosAnfitrion = new Alojamiento*[numAlojamientos];
    registrarMemoria<Alojamiento*>(numAlojamientos);
    cantidadAlojamientos = 0;

    size_t start = 0;
    // Recorre la cadena de códigos separando por comas
    while (start < codigosAlojamientos.length()){
        incrementarIteraciones();
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

        // Busca el alojamiento con el código dado en la lista global y lo asocia si lo encuentra
        if (!codAlojamiento.empty()){
            for (int i = 0; i < totalAlojamientos; i++){
                incrementarIteraciones();
                if (listaAlojamientos[i]->getCodigoAlojamiento() == codAlojamiento){
                    alojamientosAnfitrion[cantidadAlojamientos++] = listaAlojamientos[i];
                    break;
                }
            }
        }
        start = end + 1;
    }
}

/**
 * @brief Muestra los alojamientos que tiene asociados el anfitrión.
 */
void Anfitrion::mostrarAlojamientosAnfitrion(){
    std::cout << "Anfitrion " << cedula << ":\n";
    for (int j = 0; j < cantidadAlojamientos; j++){
        incrementarIteraciones();
        std::cout << "Alojamientos: " << j+1 << std::endl;
        alojamientosAnfitrion[j]->mostrarAlojamientos();
    }
}

/**
 * @brief Muestra las reservas asociadas a los alojamientos del anfitrión.
 * @param reservaciones Arreglo de todas las reservas del sistema.
 * @param totalReservas Número total de reservas registradas.
 */
void Anfitrion::mostrarReservasDeSusAlojamientos(Reservas** reservaciones, int totalReservas) {
    for (int i = 0; i < cantidadAlojamientos; ++i) {
        incrementarIteraciones();
        string codAloj = alojamientosAnfitrion[i]->getCodigoAlojamiento();
        cout << "- Alojamiento: " << alojamientosAnfitrion[i]->getNombre()
             << " (Codigo: " << codAloj << ")" << endl;

        bool tieneReservas = false;

        for (int j = 0; j < totalReservas; ++j) {
            incrementarIteraciones();
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

/**
 * @brief Anula una reserva si pertenece a uno de los alojamientos del anfitrión.
 * @param _codigoReserva Código de la reserva a eliminar.
 * @param reservas Arreglo de todas las reservas (modificable).
 * @param totalReservas Número total de reservas (modificable).
 * @param huespedes Arreglo de todos los huéspedes del sistema.
 * @param totalHuespedes Número total de huéspedes registrados.
 */
void Anfitrion::anularReservacion(const string& _codigoReserva, Reservas **&reservas, int& totalReservas, Huesped** huespedes, int totalHuespedes) {
    // Buscar la reserva en la lista global
    int indexReserva = -1;
    string codigoAlojamientoReserva = "";
    string cedulaHuespedActualizar = "";

    for (int i = 0; i < totalReservas; ++i) {
        incrementarIteraciones();
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

    // Verificar que la reserva pertenece a uno de los alojamientos del anfitrión
    bool perteneceAlAnfitrion = false;
    for (int i = 0; i < cantidadAlojamientos; ++i) {
        incrementarIteraciones();
        if (alojamientosAnfitrion[i]->getCodigoAlojamiento() == codigoAlojamientoReserva) {
            perteneceAlAnfitrion = true;
            break;
        }
    }

    if (!perteneceAlAnfitrion) {
        cout << "Error: No tiene permiso para eliminar la reserva, ya que no pertenece a sus alojamientos.\n";
        return;
    }

    // Buscar el huésped asociado para actualizar su lista de reservas
    Huesped* huespedActualizado = nullptr;
    for (int i = 0; i < totalHuespedes; ++i) {
        incrementarIteraciones();
        if (huespedes[i]->getCedulaHuesped() == cedulaHuespedActualizar) {
            huespedActualizado = huespedes[i];
            break;
        }
    }

    if (!huespedActualizado) {
        cout << "Huesped asociado no encontrado.\n";
        return;
    }

    // Llama al método del huésped para liberar la reserva
    huespedActualizado->liberarReservasHuesped(_codigoReserva);

    //liberar la memoria de la reserva antes de eliminarla del arreglo
    delete reservas[indexReserva];
    liberarMemoria<Reservas>(1);

    // Elimina la reserva del arreglo global y desplaza los elementos para llenar el hueco
    for (int i = indexReserva; i < totalReservas - 1; ++i) {
        incrementarIteraciones();
        reservas[i] = reservas[i + 1];
    }
    totalReservas--;
    reservas[totalReservas] = nullptr;

    // Actualiza las reservas del huésped con el arreglo modificado
    huespedActualizado->asociarReservas(reservas, totalReservas);

    cout << "Reserva " << _codigoReserva << " eliminada exitosamente.\n";
}

/**
 * @brief Verifica si un alojamiento pertenece al anfitrión.
 * @param codigoAloj Código del alojamiento.
 * @return true si el alojamiento pertenece al anfitrión, false en caso contrario.
 */
bool Anfitrion::poseeAlojamiento(const string& codigoAloj) const {
    for (int i = 0; i < cantidadAlojamientos; ++i) {
        incrementarIteraciones();
        if (alojamientosAnfitrion[i]->getCodigoAlojamiento() == codigoAloj) {
            return true;
        }
    }
    return false;
}


