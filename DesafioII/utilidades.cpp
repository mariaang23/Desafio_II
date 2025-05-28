/**
 * @file utilidades.cpp
 * @brief Funciones de validación de entrada, gestión de menús para anfitriones y huéspedes,
 *        y funciones para guardar y actualizar archivos relacionados con reservas y huéspedes.
 *
 * Este módulo incluye validación de entradas numéricas y de caracteres, menús interactivos
 * para usuarios anfitriones y huéspedes, así como funciones para manejar la persistencia
 * de datos en archivos de texto y actualizar históricos de reservas.
 */

#include "memoria.h"
#include "utilidades.h"
#include "fecha.h"
#include "anfitrion.h"
#include "alojamiento.h"
#include "huesped.h"
#include "reservas.h"
#include <iostream>
#include <fstream>
using namespace std;

/**Definicion de variables globales para almacenar memoria
*/

int contadorIteracionesGlobal = 0;
size_t memoriaReservadaGlobal = 0;

/**
 * @brief Verifica si un número está dentro de un rango dado.
 *
 * @param numero Número a validar.
 * @param intIni Límite inferior del rango.
 * @param intFin Límite superior del rango.
 * @return true si el número está dentro del rango [intIni, intFin], false en caso contrario.
 */
bool rangeValidation(int numero, int intIni, int intFin){

    return (numero >= intIni && numero <= intFin); // Retorna true si 'numero' está entre 'intIni' y 'intFin' inclusive
}

/**
 * @brief Solicita al usuario ingresar un número entero dentro de un rango válido.
 *
 * Pide repetidamente la entrada hasta que el usuario ingresa un número válido dentro
 * del rango especificado.
 *
 * @param limInf Límite inferior aceptable.
 * @param limSup Límite superior aceptable.
 * @return Número entero válido ingresado por el usuario.
 */
int intValidation(int limInf, int limSup){
    int num = 0;

    while (true){
        incrementarIteraciones();
        cout << "Digite la opcion (" << limInf << " - " << limSup << "): ";
        cin >> num;

        // Verifica si la entrada falló (por ejemplo, no es un número)
        if (cin.fail()) {
            cin.clear();              // Limpia el error de entrada
            cin.ignore(1000, '\n');   // Ignora hasta 1000 caracteres o hasta salto de línea
            cout << "Entrada no valida. Por favor ingrese un numero." << endl;
        } else if (!rangeValidation(num, limInf, limSup)) {
            // Verifica si el número está fuera del rango válido
            cout << "La opcion ingresada esta fuera del rango valido." << endl;
        } else {
            // Número válido dentro del rango
            return num;
        }
    }
}

/**
 * @brief Solicita al usuario ingresar un carácter.
 *
 * Pide repetidamente la entrada hasta que el usuario ingresa un carácter válido.
 *
 * @param mensajeStr Mensaje que se muestra para solicitar la entrada.
 * @return Carácter válido ingresado por el usuario.
 */
char charValidation(const string& mensajeStr) {
    char opc;
    while (true) {
        incrementarIteraciones();
        cout << mensajeStr;
        cin >> opc;

        // Verifica si la entrada falló
        if (cin.fail()) {
            cin.clear();               // Limpia el error
            cin.ignore(1000, '\n');    // Ignora caracteres restantes
            cout << "Entrada no valida. Intente de nuevo." << endl;
        } else {
            cin.ignore(1000, '\n');    // Ignora caracteres restantes hasta salto de línea
            return opc;                // Retorna el carácter válido
        }
    }
}


/**
 * @brief Muestra el menú para las acciones disponibles a un anfitrión.
 *
 * Permite al anfitrión consultar reservas, anular una reservación o actualizar el histórico.
 *
 * @param anfitrionActual Puntero al anfitrión que utiliza el menú.
 * @param reservaciones Referencia al arreglo dinámico de punteros a Reservas.
 * @param totalReservas Referencia al número total de reservas activas.
 * @param huespedes Referencia al arreglo dinámico de punteros a Huespedes.
 * @param totalHuespedes Referencia al número total de huéspedes registrados.
 */
void mostrarMenuAnfitrion(Anfitrion* anfitrionActual, Reservas**& reservaciones, int& totalReservas, Huesped**& huespedes, int& totalHuespedes)
{
    bool exit = false;
    while (!exit){
        incrementarIteraciones();
        cout << "------------------------------" << endl;
        cout << "MENU ANFITRION" << endl;
        cout << "------------------------------" << endl;
        cout << "1. Consultar reservas\n2. Anular reservacion\n3. Actualizar historico\n4. Volver al menu principal\n";
        int opc = intValidation(1, 4);

        if (opc == 1) {
            anfitrionActual->mostrarReservasDeSusAlojamientos(reservaciones, totalReservas);
            mostrarUsoMemoria();
        }
        else if (opc == 2) {
            string codReservaEliminar;
            cout << "Ingrese el codigo de la reserva que desea eliminar: " << endl;
            cin >> codReservaEliminar;
            anfitrionActual->anularReservacion(codReservaEliminar,reservaciones,totalReservas,huespedes,totalHuespedes);
            mostrarUsoMemoria();

        }
        else if (opc == 3) {
            string fechaStr = "";
            cout << "\nIngrese la fecha de corte para generar el historico: \n";
            cin >> fechaStr;
            Fecha fechaCorte = Fecha::fromString(fechaStr);
            actualizarHistorico(reservaciones, totalReservas, huespedes, totalHuespedes, fechaCorte);
            cout << "\nSe ha actualizado el historico correctamente" << endl;
            mostrarUsoMemoria();
        }
        else {
            exit = true;
        }
    }
}

/**
 * @brief Muestra el menú para las acciones disponibles a un huésped.
 *
 * Permite al huésped reservar alojamiento, anular reservación o volver al menú principal.
 *
 * @param huespedActual Puntero al huésped que utiliza el menú.
 * @param reservaciones Referencia al arreglo dinámico de punteros a Reservas.
 * @param totalReservas Referencia al número total de reservas activas.
 * @param alojamientos Arreglo de punteros a alojamientos disponibles.
 * @param totalAlojamientos Total de alojamientos disponibles.
 * @param anfitriones Arreglo de punteros a anfitriones registrados.
 * @param totalAnfitriones Total de anfitriones registrados.
 */
void mostrarMenuHuesped(Huesped* huespedActual, Reservas**& reservaciones, int& totalReservas,
                        Alojamiento** alojamientos, int totalAlojamientos, Anfitrion** anfitriones, int totalAnfitriones){
    bool exit = false;
    while (!exit){
        incrementarIteraciones();
        cout << "------------------------------" << endl;
        cout << "MENU HUESPED" << endl;
        cout << "------------------------------" << endl;
        cout << "1. Reservar alojamiento\n2. Anular reservacion\n3. Volver al menu principal\n";
        int opc = intValidation(1, 3);
        if (opc == 1) {
            huespedActual->reservarAlojamiento(alojamientos,totalAlojamientos,anfitriones,totalAnfitriones,reservaciones,totalReservas);
            mostrarUsoMemoria();
        }
        else if (opc == 2) {
            string codReservaEliminar;
            cout << "Ingrese el codigo de la reserva que desea eliminar: " << endl;
            cin >> codReservaEliminar;
            huespedActual->anularReservacion(codReservaEliminar, reservaciones, totalReservas);
            mostrarUsoMemoria();
        }
        else{
            exit = true;
        }
    }
}

/**
 * @brief Muestra todas las reservas agrupadas por alojamiento.
 *
 * Itera sobre alojamientos y busca reservas asociadas, imprimiendo la información.
 *
 * @param alojamientos Arreglo de punteros a alojamientos.
 * @param totalAlojamientos Número total de alojamientos.
 * @param reservaciones Arreglo de punteros a reservas activas.
 * @param totalReservas Número total de reservas activas.
 */
void mostrarReservasPorAlojamiento(Alojamiento** alojamientos, int totalAlojamientos,
                                   Reservas** reservaciones, int totalReservas) {
    // Itera cada alojamiento
    for (int i = 0; i < totalAlojamientos; ++i) {
        incrementarIteraciones();
        // Obtiene código y nombre del alojamiento
        string codigoAloj = alojamientos[i]->getCodigoAlojamiento();
        cout << "Alojamiento: " << alojamientos[i]->getNombre() << " (Codigo: " << codigoAloj << ")" << endl;

        bool tieneReservas = false;
        // Recorre todas las reservas para ese alojamiento
        for (int j = 0; j < totalReservas; ++j) {
            incrementarIteraciones();
            if (reservaciones[j]->getCodigoAlojamiento() == codigoAloj) {
                reservaciones[j]->mostrarReserva();
                tieneReservas = true;
            }
        }

        if (!tieneReservas) {
            // Indica que no hay reservas para ese alojamiento
            cout << "No tiene reservas asociadas.\n";
        }

        cout << "--------------------------------------" << endl;
    }
}

/**
 * @brief Guarda la información de los huéspedes en un archivo.
 *
 * Escribe todos los datos de los huéspedes en formato delimitado por punto y coma.
 *
 * @param huespedes Arreglo de punteros a huéspedes.
 * @param totalHuespedes Número total de huéspedes.
 * @param archivo Nombre del archivo donde se guardan los datos.
 */
void guardarHuespedesArchivo(Huesped** huespedes, int totalHuespedes, const string &archivo){
    ofstream out(archivo, ios::trunc);
    if (!out.is_open()) {
        // archivo no pudo abrirse, sin salida a consola
        return;
    }

    // Recorre todos los huéspedes y escribe sus datos en archivo
    for (int i = 0; i < totalHuespedes; ++i) {
        incrementarIteraciones();
        out << huespedes[i]->getCedulaHuesped() << ";"
            << huespedes[i]->getClaveHuesped() << ";"
            << huespedes[i]->getAntiguedad() << ";"
            << huespedes[i]->getPuntuacion() << ";";

        string codigos = huespedes[i]->getCodigosReservas();
        if (!codigos.empty()){
            out << codigos;
        }
        out << endl;
    }

    out.close();
}

/**
 * @brief Guarda las reservas activas en un archivo.
 *
 * Escribe los datos de reservas activas en formato delimitado por punto y coma.
 *
 * @param reservas Arreglo de punteros a reservas activas.
 * @param totalReservas Número total de reservas activas.
 * @param archivo Nombre del archivo donde se guardan las reservas.
 */
void guardarReservasActivasArchivo(Reservas** reservas, int totalReservas, const string& archivo){
    ofstream out(archivo, ios::trunc);
    if (!out.is_open()) {
        // archivo no pudo abrirse, sin salida a consola
        return;
    }

    // Recorre todas las reservas activas y escribe sus datos en el archivo
    for (int i = 0; i < totalReservas; ++i) {
        incrementarIteraciones();
        if (reservas[i] != nullptr){
            out << reservas[i]->getCodigoReserva() << ";"
                << reservas[i]->getCodigoAlojamiento() << ";"
                << reservas[i]->getCedulaHuesped() << ";"
                << reservas[i]->getFechaEntrada() << ";"
                << to_string(reservas[i]->getCantNoches()) << ";"
                << reservas[i]->getMetodoPago() << ";"
                << reservas[i]->getFechaPago() << ";"
                << reservas[i]->getMonto() << ";"
                << reservas[i]->getAnotaciones() << endl;

        }
    }
    out.close();
}

/**
 * @brief Actualiza el histórico de reservas moviendo reservas antiguas a un archivo histórico y
 *        eliminándolas de la lista de reservas activas.
 *
 * Esta función revisa las reservas activas y, para cada reserva cuya fecha de entrada
 * sea anterior o igual a una fecha de corte dada, la mueve al archivo histórico "historico.txt".
 * También actualiza la información de los huéspedes para eliminar esas reservas de su historial activo.
 * Finalmente, limpia y actualiza la lista de reservas activas y guarda los archivos correspondientes.
 *
 * @param reservasActivas Referencia a un arreglo dinámico de punteros a reservas activas.
 * @param totalReservas Referencia al total de reservas activas en el arreglo.
 * @param huespedes Arreglo de punteros a huéspedes registrados.
 * @param totalHuespedes Total de huéspedes en el arreglo.
 * @param fechaCorte Fecha límite para determinar qué reservas pasan a histórico.
 */
void actualizarHistorico(Reservas**& reservasActivas, int& totalReservas, Huesped** huespedes, int totalHuespedes, Fecha& fechaCorte) {

    // Crear un arreglo dinámico para almacenar temporalmente las reservas que pasarán a histórico
    Reservas** reservasHistorico = new Reservas*[totalReservas];
    registrarMemoria<Reservas*>(totalReservas);

    int totalReservasHistorico = 0;

    // Recorrer todas las reservas activas para identificar cuáles pasan a histórico
    for (int i = 0; i < totalReservas; i++) {

        incrementarIteraciones();
        // Verificar que la reserva actual no sea nullptr (exista)
        if (reservasActivas[i] != nullptr) {

            // Convertir la fecha de entrada de la reserva a objeto Fecha para comparación
            Fecha fechaReserva = Fecha::fromString(reservasActivas[i]->getFechaEntrada());
            if (fechaReserva < fechaCorte || fechaReserva == fechaCorte) {

                // Guardar la reserva en el arreglo de reservas para histórico
                reservasHistorico[totalReservasHistorico++] = reservasActivas[i];

                // Obtener la cédula del huésped y el código de la reserva para actualizar datos
                string cedula = reservasActivas[i]->getCedulaHuesped();
                string codigoReserva = reservasActivas[i]->getCodigoReserva();

                // Buscar el huésped correspondiente para eliminar la reserva del histórico activo
                for (int j = 0; j < totalHuespedes; j++) {
                    if (huespedes[j] != nullptr && huespedes[j]->getCedulaHuesped() == cedula) {

                        // Eliminar la reserva del historial activo del huésped
                        huespedes[j]->eliminarReservaHistorico(codigoReserva);
                        break; // Salir del ciclo después de encontrar y eliminar la reserva
                    }
                }

                // Marcar la reserva activa como nullptr para indicar que ya no está activa
                reservasActivas[i] = nullptr;
            }
        }
    }

    ofstream archivoHistorico("historico.txt", ios::app);
    if (!archivoHistorico.is_open()) {
        for (int i = 0; i < totalReservasHistorico; i++) {
            incrementarIteraciones();
            delete reservasHistorico[i];
            liberarMemoria<Reservas>(1);
        }
        delete[] reservasHistorico;
        liberarMemoria<Reservas*>(totalReservasHistorico);
        return;
    }

    // Guardar en el archivo histórico todas las reservas que se movieron
    for (int i = 0; i < totalReservasHistorico; i++) {
        incrementarIteraciones();
        Reservas* r = reservasHistorico[i];

        // Escribir cada dato de la reserva separado por punto y coma
        archivoHistorico << r->getCodigoReserva() << ";"
                         << r->getCodigoAlojamiento() << ";"
                         << r->getCedulaHuesped() << ";"
                         << r->getFechaEntrada() << ";"
                         << to_string(r->getCantNoches()) << ";"
                         << r->getMetodoPago() << ";"
                         << r->getFechaPago() << ";"
                         << r->getMonto() << ";"
                         << r->getAnotaciones() << endl;

        delete r;    // Liberar la memoria de la reserva que ya se archivó
        liberarMemoria<Reservas>(1);
    }
    archivoHistorico.close();
    delete[] reservasHistorico;  // Liberar el arreglo temporal de reservas para histórico
    liberarMemoria<Reservas*>(totalReservasHistorico);

    int nuevosActivos = 0;
    // Compactar el arreglo de reservas activas eliminando los nullptrs
    for (int i = 0; i < totalReservas; i++) {
        incrementarIteraciones();
        if (reservasActivas[i] != nullptr) {
            reservasActivas[nuevosActivos++] = reservasActivas[i];
        }
    }

    // Poner nullptr en las posiciones restantes después de compactar
    for (int i = nuevosActivos; i < totalReservas; i++) {
        incrementarIteraciones();
        reservasActivas[i] = nullptr;
    }

    // Actualizar el total de reservas activas para reflejar el nuevo tamaño
    totalReservas = nuevosActivos;

    guardarReservasActivasArchivo(reservasActivas, totalReservas, "ReservasActivas.txt");
    guardarHuespedesArchivo(huespedes, totalHuespedes, "huespedes.txt");
}



