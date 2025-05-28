/**
 * @file fecha.cpp
 * @brief Implementación de la clase Fecha, que representa una fecha y permite diversas operaciones como validación,
 *        cálculo de día de la semana, suma de días, comparación y conversión a/desde cadenas de texto.
 */

#include "memoria.h"
#include <iostream>
#include "fecha.h"
using namespace std;

/**
 * @brief Verifica si el año de la fecha actual es bisiesto.
 *
 * @return true si el año es bisiesto, false en caso contrario.
 */
bool Fecha::esBisiesto() const {
    if (anio % 400 == 0) return true;
    if (anio % 100 == 0) return false;
    return anio % 4 == 0;
}

/**
 * @brief Calcula el día de la semana para la fecha actual.
 *
 * Utiliza el algoritmo de Zeller. El resultado se ajusta para retornar:
 * 1 = Lunes, 2 = Martes, ..., 7 = Domingo.
 *
 * @return int Día de la semana como número.
 */
int Fecha::calcularDiaSemana() const {
    int diaCalc = dia;
    int mesCalc = mes;
    int anioCalc = anio;

    // Ajuste para tratar enero y febrero como meses 13 y 14 del año anterior
    if (mesCalc < 3) {
        mesCalc += 12;
        anioCalc -= 1;
    }

    int K = anioCalc % 100;           // Año dentro del siglo
    int J = anioCalc / 100;           // Siglo

    int h = (diaCalc + (13 * (mesCalc + 1)) / 5 + K + (K / 4) + (J / 4) + 5 * J) % 7;

    // Ajuste para retornar en el rango 1=Lunes,...,7=Domingo
    if (h == 0) return 6;      // Sábado
    else if (h == 1) return 7; // Domingo
    else return h - 1;         // Lunes a Viernes
}

/**
 * @brief Retorna la cantidad de días que tiene un mes dado, considerando si el año es bisiesto.
 *
 * @param mes Número del mes (1 a 12).
 * @param anio Año correspondiente.
 * @return int Número de días del mes.
 */
int Fecha::diasDelMes(int mes, int anio) const {
    if (mes == 2) {
        return ( (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0) ) ? 29 : 28;
    }
    else if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        return 30;
    }
    return 31;
}

/**
 * @brief Constructor de la clase Fecha.
 *
 * @param diaInicial Día de la fecha.
 * @param mesInicial Mes de la fecha.
 * @param anioInicial Año de la fecha.
 */
Fecha::Fecha(int diaInicial, int mesInicial, int anioInicial)
    : dia(diaInicial), mes(mesInicial), anio(anioInicial){}

/**
 * @brief Verifica si la fecha actual es válida.
 *
 * Una fecha válida debe estar desde el año 1900 en adelante y tener día y mes válidos.
 *
 * @return true si la fecha es válida, false si no lo es.
 */
bool Fecha::esValida() const {
    if ((anio < 1900) || (mes < 1) || (mes > 12) || (dia < 1)) {
        return false;
    }
    if (dia > diasDelMes(mes, anio)) {
        return false;
    }
    return true;
}

/**
 * @brief Operador de comparación '<' para fechas.
 *
 * Compara dos fechas cronológicamente.
 *
 * @param otraFecha Fecha contra la que se compara.
 * @return true si la fecha actual es anterior a la otra, false en caso contrario.
 */
bool Fecha::operator<(const Fecha& otraFecha) const {
    if (anio != otraFecha.anio) return anio < otraFecha.anio;
    if (mes != otraFecha.mes) return mes < otraFecha.mes;
    return dia < otraFecha.dia;
}

/**
 * @brief Operador de comparación de igualdad '==' para fechas.
 *
 * @param otraFecha Fecha a comparar.
 * @return true si ambas fechas son iguales, false en caso contrario.
 */
bool Fecha::operator==(const Fecha& otraFecha) const {
    return (dia == otraFecha.dia && mes == otraFecha.mes && anio == otraFecha.anio);
}

/**
 * @brief Sobrecarga del operador '+' para sumar días a una fecha.
 *
 * @param diasASumar Número de días a sumar.
 * @return Fecha resultante después de sumar los días.
 */
Fecha Fecha::operator+(int diasASumar) const {
    int d = dia;
    int m = mes;
    int a = anio;

    // Sumar los días
    d += diasASumar;

    // Ajustar el día y avanzar meses/años si es necesario
    while (d > diasDelMes(m, a)) {
        incrementarIteraciones();
        d -= diasDelMes(m, a);
        m++;
        if (m > 12) {
            m = 1;
            a++;   // Pasamos al siguiente año
        }
    }

    // Retornar la nueva fecha con los días sumados
    return Fecha(d, m, a);
}

/**
 * @brief Imprime la fecha con el nombre del día y del mes.
 *
 * Ejemplo: "Lunes, 15 de Abril del 2025"
 */
void Fecha::imprimirConDia() const {
    const char* nombresDias[7] = {"Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado", "Domingo"};
    const char* nombresMeses[12] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
                                    "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};

    if (!esValida()) {
        std::cout << "\nFecha invalida\n";
        return;
    }

    int indiceDia = calcularDiaSemana() - 1;  // Ajuste índice para array (0-based)

    std::cout << nombresDias[indiceDia] << ", " << dia << " de " << nombresMeses[mes - 1] << " del " << anio << std::endl;
}

/**
 * @brief Crea un objeto Fecha a partir de un string con formato "dd/mm/aaaa".
 *
 * @param fechaStr Cadena con la fecha.
 * @return Fecha construida a partir del string, o una fecha inválida si hay error.
 */
Fecha Fecha::fromString(const std::string& fechaStr) {
    int partes[3] = {0, 0, 0};  // [día, mes, año]
    int parteActual = 0;
    string subStrFec = "";

    string fechaConSlash = fechaStr + '/';  // Asegura el procesamiento del último valor

    for (char c : fechaConSlash) {
        if (c == '/') {
            if (subStrFec.empty()) {
                cerr << "Error: campo vacio en la fecha\n";
                return Fecha(0, 0, 0);  // Fecha inválida
            }
            try {
                if (parteActual < 3) {
                    partes[parteActual] = std::stoi(subStrFec);
                    parteActual++;
                } else {
                    std::cerr << "Error: más de tres partes en la fecha.\n";
                    return Fecha(0, 0, 0);
                }
            } catch (...) {
                std::cerr << "Error: conversión inválida en parte '" << subStrFec << "'\n";
                return Fecha(0, 0, 0);
            }
            subStrFec.clear();
        } else {
            subStrFec += c;
        }
    }

    // Verificar que las 3 partes se hayan llenado correctamente
    if (parteActual != 3) {
        std::cerr << "Error: numero incorrecto de partes en la fecha.\n";
        return Fecha(0, 0, 0);
    }

    // Crear y devolver el objeto Fecha con día, mes, año
    return Fecha(partes[0], partes[1], partes[2]);

}


/**
 * @brief Convierte la fecha actual a un string en formato "dd/mm/aaaa".
 *
 * @return std::string Representación en texto de la fecha o mensaje de error si es inválida.
 */
string Fecha::aString() const {
    // Asegúrate de validar antes de construir el string
    if (!esValida()) return "Fecha invalida\n";

    string diaStr = (dia < 10 ? "0" : "") + to_string(dia);
    string mesStr = (mes < 10 ? "0" : "") + to_string(mes);
    string anioStr = to_string(anio);

    return diaStr + "/" + mesStr + "/" + anioStr;
}
