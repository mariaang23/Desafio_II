#ifndef ALOJAMIENTO_H
#define ALOJAMIENTO_H

#include <string>
using namespace std;

class Reservas;

class Alojamiento {
private:
    string codigo;
    string nombre;
    string nombreAnf;
    string departamento;
    string municipio;
    string tipo;
    string direccion;
    int precio;
    string amenidades;

public:
    Alojamiento(const string& _codigo, const string& _nombre, const string& _nombreAnf,
                const string& _departamento, const string& _municipio, const string& _tipo,
                const string& _direccion, const int& _precioStr, const string& _amenidades);

    const string& getCodigoAlojamiento() const;
    const int &getPrecio() const;

    const string& getNombre() const;
    const string& getMunicipio() const;

    void mostrarAlojamientos() const;
    bool estaDisponible(const string& fechaEntrada, int cantNoches, Reservas** reservas, int totalReservas);

    static void cargarAlojamientos(Alojamiento**& alojamientos, int& totalAlojamientos);
};

#endif // ALOJAMIENTO_H
