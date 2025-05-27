#ifndef FECHA_H
#define FECHA_H
#include <string>
using namespace std;

class Fecha
{
private:
    int dia;
    int mes;
    int anio;
    bool esBisiesto() const;
    int calcularDiaSemana() const;
    int diasDelMes(int mes, int anio) const;

public:
    Fecha(int diaInicial = 1, int mesInicial = 1, int anioInicial = 2000);
    bool esValida() const;

    bool operator<(const Fecha& otraFecha) const;
    bool operator==(const Fecha& otraFecha) const;
    Fecha operator+(int cantNoches) const;

    void imprimirConDia() const;
    static Fecha fromString(const std::string& fechaStr);
    string aString() const;

};

#endif // FECHA_H
