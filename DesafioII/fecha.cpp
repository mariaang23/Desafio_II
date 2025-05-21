#include <iostream>
#include "fecha.h"

// Función interna para verificar si el año es bisiesto
bool Fecha::esBisiesto() const {
    if (anio % 400 == 0) {
        return true;
    }
    if (anio % 100 == 0) {
        return false;
    }
    if (anio % 4 == 0) {
        return true;
    }
    return false;
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
    int diaSemana;
    if (h == 0) {
        diaSemana = 6;  // sábado
    } else if (h == 1) {
        diaSemana = 7;  // domingo
    } else {
        diaSemana = h - 1; // lunes a viernes
    }
    return diaSemana;
}

// Constructor con valores por defecto
Fecha::Fecha(int diaInicial, int mesInicial, int anioInicial): dia(diaInicial), mes(mesInicial), anio(anioInicial){
}

// Métodos getters
int Fecha::getDia() const { return dia; }
int Fecha::getMes() const { return mes; }
int Fecha::getAnio() const { return anio; }

// Método para validar la fecha
bool Fecha::esValida() const {
    if (anio < 1900) {
        return false;  // Año mínimo permitido
    }

    if (mes < 1 || mes > 12) {
        return false;  // Mes inválido
    }

    if (dia < 1) {
        return false;  // Día inválido
    }

    // Días por mes
    int diasPorMes[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

    // Ajustar febrero si es bisiesto
    if (esBisiesto()) {
        diasPorMes[1] = 29;
    }

    if (dia > diasPorMes[mes - 1]) {
        return false;  // Día excede el número máximo del mes
    }

    return true;
}

// Sobrecarga operador <
bool Fecha::operator<(const Fecha& otraFecha) const {
    if (anio < otraFecha.anio) {
        return true;
    } else if (anio > otraFecha.anio) {
        return false;
    }

    if (mes < otraFecha.mes) {
        return true;
    } else if (mes > otraFecha.mes) {
        return false;
    }

    if (dia < otraFecha.dia) {
        return true;
    } else {
        return false;
    }
}

// Sobrecarga operador ==
bool Fecha::operator==(const Fecha& otraFecha) const {
    return (dia == otraFecha.dia && mes == otraFecha.mes && anio == otraFecha.anio);
}

// Imprimir fecha con formato: "Lunes, 15 de Abril del 2025"
void Fecha::imprimirConDia() const {
    const char* nombresDias[7] = {"Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado", "Domingo"};
    const char* nombresMeses[12] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
                                    "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};

    if (!esValida()) {
        std::cout << "Fecha inválida";
        return;
    }

    int indiceDia = calcularDiaSemana() - 1;  // Ajuste índice para array (0-based)

    std::cout << nombresDias[indiceDia] << ", " << dia << " de " << nombresMeses[mes - 1] << " del " << anio;
}

// Imprimir fecha simple: "15 de Abril del 2025"
void Fecha::imprimir() const {
    const char* nombresMeses[12] = {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
                                    "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"};

    if (!esValida()) {
        std::cout << "Fecha inválida";
        return;
    }

    std::cout << dia << " de " << nombresMeses[mes - 1] << " del " << anio;
}
