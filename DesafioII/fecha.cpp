#include <iostream>
#include "fecha.h"
using namespace std;

// Función interna para verificar si el año es bisiesto
bool Fecha::esBisiesto() const {
    if (anio % 400 == 0) return true;
    if (anio % 100 == 0) return false;
    return anio % 4 == 0;
}

// Función interna para calcular el día de la semana
// Retorna un entero 1=Lunes, 2=Martes, ..., 7=Domingo
int Fecha::calcularDiaSemana() const {
    int diaCalc = dia;
    int mesCalc = mes;
    int anioCalc = anio;

    // Ajuste para enero y febrero según algoritmo de Zeller
    if (mesCalc < 3) {
        mesCalc += 12;
        anioCalc -= 1;
    }

    int K = anioCalc % 100;           // Año dentro del siglo
    int J = anioCalc / 100;           // Siglo

    int h = (diaCalc + (13 * (mesCalc + 1)) / 5 + K + (K / 4) + (J / 4) + 5 * J) % 7;

    // h = 0 -> sábado, 1 -> domingo, 2 -> lunes, ..., 6 -> viernes
    // Queremos: 1=Lunes,...,7=Domingo
    if (h == 0) return 6;      // Sábado
    else if (h == 1) return 7; // Domingo
    else return h - 1;         // Lunes a Viernes
}

// Devuelve el número de días que tiene un mes, considerando si es bisiesto
int Fecha::diasDelMes(int mes, int anio) const {
    if (mes == 2) {
        return ( (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0) ) ? 29 : 28;
    }
    else if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        return 30;
    }
    return 31;
}

// Constructor con valores por defecto
Fecha::Fecha(int diaInicial, int mesInicial, int anioInicial)
    : dia(diaInicial), mes(mesInicial), anio(anioInicial){}

// Métodos getters
int Fecha::getDia() const { return dia; }
int Fecha::getMes() const { return mes; }
int Fecha::getAnio() const { return anio; }

// Método para validar la fecha
bool Fecha::esValida() const {
    if (anio < 1900 || mes < 1 || mes > 12 || dia < 1) {
        return false;
    }
    if (dia > diasDelMes(mes, anio)) {
        return false;
    }
    return true;
}

// Sobrecarga operador <
bool Fecha::operator<(const Fecha& otraFecha) const {
    if (anio != otraFecha.anio) return anio < otraFecha.anio;
    if (mes != otraFecha.mes) return mes < otraFecha.mes;
    return dia < otraFecha.dia;
}

bool Fecha::operator==(const Fecha& otraFecha) const {
    return (dia == otraFecha.dia && mes == otraFecha.mes && anio == otraFecha.anio);
}

// Sobrecarga operador +
Fecha Fecha::operator+(int diasASumar) const {
    int d = dia;
    int m = mes;
    int a = anio;

    // Sumar los días
    d += diasASumar;

    // Ajustar la fecha si el día excede el número de días en el mes
    while (d > diasDelMes(m, a)) {
        d -= diasDelMes(m, a);
        m++;
        if (m > 12) {
            m = 1;
            a++;  // Incrementar el año si se pasa de diciembre
        }
    }

    // Retornar la nueva fecha con los días sumados
    return Fecha(d, m, a);
}



// Imprimir fecha con formato: "Lunes, 15 de Abril del 2025"
void Fecha::imprimirConDia() const {
    const char* nombresDias[7] = {"Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado", "Domingo"};
    const char* nombresMeses[12] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
                                    "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};

    if (!esValida()) {
        std::cout << "\nFecha inválida\n";
        return;
    }

    int indiceDia = calcularDiaSemana() - 1;  // Ajuste índice para array (0-based)

    std::cout << nombresDias[indiceDia] << ", " << dia << " de " << nombresMeses[mes - 1] << " del " << anio << std::endl;
}


// Imprimir fecha simple: "15 de Abril del 2025"
void Fecha::imprimir() const {
    const char* nombresMeses[12] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
                                    "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};

    if (!esValida()) {
        std::cout << "\nFecha inválida\n";
        return;
    }

    std::cout << dia << " de " << nombresMeses[mes - 1] << " del " << anio <<std::endl;
}

Fecha Fecha::fromString(const std::string& fechaStr) {
    int partes[3] = {0, 0, 0};  // dd, mm, aaaa
    int parteActual = 0;
    string subStrFec = "";

    string fechaConSlash = fechaStr + '/';  // Para procesar la última parte igual que las otras

    for (char c : fechaConSlash) {
        if (c == '/') {
            if (subStrFec.empty()) {
                cerr << "Error: subStrFec está vacío antes de la conversión a int.\n";
                return Fecha(0, 0, 0);  // Valor por defecto en caso de error
            }
            partes[parteActual++] = stoi(subStrFec);
            subStrFec = "";
            if (parteActual >= 3) break;
        } else {
            subStrFec += c;
        }
    }

    return Fecha(partes[0], partes[1], partes[2]);
}
