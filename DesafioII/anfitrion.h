#ifndef ANFITRION_H
#define ANFITRION_H

#include <string>
#include "alojamiento.h"
#include "reservas.h"
class Huesped;

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
    const string& getPuntuacion() const;

    static void cargarAnfitriones(Anfitrion**& anfitriones, int& totalAnfitriones);

    void asociarAlojamientos(Alojamiento** listaAlojamientos, int totalAlojamientos);

    void mostrarAlojamientosAnfitrion();

    void mostrarReservasDeSusAlojamientos(Reservas** reservaciones, int totalReservas);

    void anularReservacion(const string& _codigoReserva, Reservas**& reservas, int &totalReservas, Huesped** huespedes, int totalHuespedes);

    bool poseeAlojamiento(const string& codigoAloj) const;
};

#endif // ANFITRION_H
