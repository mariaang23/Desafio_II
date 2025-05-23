#ifndef ALOJAMIENTO_H
#define ALOJAMIENTO_H

#include <string>

using namespace std;


class Alojamiento {
private:
    string codigo;
    string nombre;
    string departamento;
    string municipio;
    string tipo;
    string direccion;
    string precio;
    string amenidades;

public:
    Alojamiento(const string& _codigo, const string& _nombre,
                const string& _departamento, const string& _municipio, const string& _tipo,
                const string& _direccion, const string& _precioStr, const string& _amenidades);

    const string& getCodigoAlojamiento() const;

    const string& getNombre()const;

    const string& getNombreAnfitrion()const;

    const string& getDepartamento() const;

    const string& getMunicipio() const;

    const string& getTipo() const;

    const string& getDireccion()const;

    const string& getPrecio()const;

    const string& getAmenidades()const;

    void mostrarAlojamientos() const;

    static void cargarAlojamientos(Alojamiento**& alojamientos, int& totalAlojamientos);

    const string& getPrecio();
};

#endif // ALOJAMIENTO_H
