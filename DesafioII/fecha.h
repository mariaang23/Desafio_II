#ifndef FECHA_H
#define FECHA_H

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
    int getDia() const;
    int getMes() const;
    int getAnio() const;
    bool esValida() const;

    bool operator<(const Fecha& otraFecha) const;
    bool operator==(const Fecha& otraFecha) const;
    Fecha operator+(int cantNoches) const;

    void imprimirConDia() const;
    void imprimir() const;

};

#endif // FECHA_H
