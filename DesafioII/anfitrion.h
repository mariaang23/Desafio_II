#ifndef ANFITRION_H
#define ANFITRION_H

#include <string>
#include "alojamiento.h"

using namespace std;

class Anfitrion {
private:
    string cedula;
    string clave;
    string antiguedad;
    string puntuacion;
    string codigosAlojamientos;

    Alojamiento** alojamientosAnfitrion;
    int cantidadAlojamientos;

public:
    Anfitrion(const string& _cedula, const string& _clave, const string& _antiguedad, const string& _puntos, const string& _codigosAlojamientos);

    const string& getCedulaAnfitrion() const;

    const string& getClaveAnfitrion() const;

    static void cargarAnfitriones(Anfitrion**& anfitriones, int& totalAnfitriones);

    void asociarAlojamientos(Alojamiento** listaAlojamientos, int totalAlojamientos);

    void mostrarAlojamientosAnfitrion();
};

#endif // ANFITRION_H
