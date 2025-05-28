/**
 * @file alojamiento.cpp
 * @brief Implementación de la clase Alojamiento, que representa un alojamiento turístico con atributos como código, nombre,
 *        ubicación, tipo, precio, amenidades y métodos para carga, consulta y verificación de disponibilidad.
 */

#include "memoria.h"
#include "alojamiento.h"
#include "fecha.h"
#include "reservas.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

/**
 * @brief Constructor de la clase Alojamiento.
 *
 * Inicializa un objeto Alojamiento con los datos especificados.
 *
 * @param _codigo Código único identificador del alojamiento.
 * @param _nombre Nombre comercial del alojamiento.
 * @param _nombreAnf Nombre del anfitrión responsable del alojamiento.
 * @param _departamento Departamento donde se ubica el alojamiento.
 * @param _municipio Municipio donde se ubica el alojamiento.
 * @param _tipo Tipo de alojamiento (e.g., casa, apartamento).
 * @param _direccion Dirección física del alojamiento.
 * @param _precioStr Precio por noche del alojamiento.
 * @param _amenidades Lista o descripción de amenidades disponibles.
 */
Alojamiento::Alojamiento(const string& _codigo, const string& _nombre, const string& _nombreAnf,
                         const string& _departamento, const string& _municipio, const string& _tipo,
                         const string& _direccion, const int &_precioStr, const string& _amenidades)
    : codigo(_codigo), nombre(_nombre), nombreAnf(_nombreAnf), departamento(_departamento), municipio(_municipio),
      tipo(_tipo), direccion(_direccion), precio(_precioStr), amenidades(_amenidades) {}

/**
 * @brief Destructor de la clase Alojamiento.
 *
 * Actualmente no realiza operaciones explicitas, ya que la clase no gestiona memoria dinamica internamente.
 * Se incluye como buena practica para facilitar futuras extensiones, depuracion y garantizar una destruccion segura.
 */
Alojamiento::~Alojamiento() {}


/**
 * @brief Retorna el código único del alojamiento.
 *
 * @return Referencia constante al string con el código del alojamiento.
 */
const string& Alojamiento::getCodigoAlojamiento() const {
    return codigo;
}

/**
 * @brief Retorna el precio por noche del alojamiento.
 *
 * @return Referencia constante al entero que representa el precio.
 */
const int& Alojamiento::getPrecio() const {
    return precio;
}

/**
 * @brief Retorna el nombre del alojamiento.
 *
 * @return Referencia constante al string con el nombre del alojamiento.
 */
const string& Alojamiento::getNombre() const {
    return nombre;
}


/**
 * @brief Retorna el municipio donde se ubica el alojamiento.
 *
 * @return const string& Nombre del municipio.
 */
const string& Alojamiento::getMunicipio() const {
    return municipio;
}

/**
 * @brief Muestra la informacion basica del alojamiento por consola.
 */
void Alojamiento::mostrarAlojamientos() const {
    cout << "    - " << nombre << " (" << tipo << ", $" << precio << ")\n";
}

/**
 * @brief Carga todos los alojamientos desde el archivo "alojamientos.txt".
 *
 * El archivo debe tener el formato:
 * código;nombre;anfitrión;departamento;municipio;tipo;dirección;precio;amenidades
 *
 * @param alojamientos Puntero doble donde se almacenarán los objetos Alojamiento.
 * @param totalAlojamientos Variable donde se guardará el número total de alojamientos cargados.
 */
void Alojamiento::cargarAlojamientos(Alojamiento**& alojamientos, int& totalAlojamientos) {
    ifstream archivo("alojamientos.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de Alojamientos\n";
        return;
    }

    string linea;
    int total = 0;

    // Contar líneas para conocer cuántos alojamientos hay
    while (getline(archivo, linea)) total++;
    archivo.clear();
    archivo.seekg(0);

    alojamientos = new Alojamiento*[total];
    registrarMemoria<Alojamiento*>(total);

    totalAlojamientos = total;
    int i = 0;

    // Leer y construir cada objeto Alojamiento
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
        registrarMemoria<Alojamiento>(1);
        alojamientos[i++] = nuevoAlojamiento;
        incrementarIteraciones();
    }
    archivo.close();

}

/**
 * @brief Verifica si el alojamiento está disponible para una fecha y cantidad de noches dadas.
 *
 * La función compara las fechas deseadas con las fechas reservadas para ese alojamiento.
 *
 * @param fechaEntrada Fecha de entrada en formato "dd/mm/aaaa".
 * @param cantNoches Número de noches deseadas.
 * @param reservas Arreglo de reservas existentes.
 * @param totalReservas Número de reservas existentes.
 * @return true si el alojamiento está disponible en esas fechas, false si hay solapamiento.
 */
bool Alojamiento::estaDisponible(const string& fechaEntrada, int cantNoches, Reservas** reservas, int totalReservas)
{
    bool solapa = false;
    Fecha fechaInicio = Fecha::fromString(fechaEntrada);
    Fecha** fechasIngresadas = new Fecha*[cantNoches];
    registrarMemoria<Fecha*>(cantNoches);

    // Generar arreglo con todas las fechas de la nueva solicitud
    for (int j = 0; j < cantNoches; j++) {
        fechasIngresadas[j] = new Fecha(fechaInicio + j);
    }

    // Comparar con cada reserva existente
    for (int i = 0; i < totalReservas && !solapa; i++) {
        incrementarIteraciones();
        int cantidadExistente = reservas[i]->getCantNoches();
        Fecha reservaInicio = Fecha::fromString(reservas[i]->getFechaEntrada());
        Fecha** fechasExistentes = new Fecha*[cantidadExistente];
        registrarMemoria<Fecha*>(cantidadExistente);

        // Generar arreglo con todas las fechas de la reserva existente
        for (int j = 0; j < cantidadExistente; j++) {
            fechasExistentes[j] = new Fecha(reservaInicio + j);
        }

         // Comparar fechas de solicitud vs fechas reservadas
        for (int j = 0; j < cantNoches && !solapa; j++) {
            incrementarIteraciones();
            for (int k = 0; k < cantidadExistente && !solapa; k++) {
                if (*fechasIngresadas[j] == *fechasExistentes[k]) {  // Comparacion por contenido
                    //si hay solapamiento el alojamiento no esta disponible
                    solapa = true;
                }
            }
        }

        // Liberar memoria de fechas existentes
        for (int j = 0; j < cantidadExistente; j++) {
            delete fechasExistentes[j];
        }
        delete[] fechasExistentes;
        liberarMemoria<Fecha>(cantidadExistente);
    }

    // Liberar memoria de fechas ingresadas
    for (int j = 0; j < cantNoches; j++) {
        delete fechasIngresadas[j];
    }
    delete[] fechasIngresadas;
    liberarMemoria<Fecha>(cantNoches);

    return !solapa; // true si no hay solapamiento
}

