#ifndef HUESPED_H
#define HUESPED_H
#include "reservas.h"
#include <string>

class Huesped
{
private:
    string cedula;
    string clave;
    string antiguedad;
    string puntuacion;
    string codigosReservas;

    Reservas** reservasHuesped;
    int cantidadReservas;

public:
    Huesped(const string& _cedula, const string& _clave, const string& _antiguedad,
            const string& _puntuacion, const string& _reservas);

    ~Huesped();

    const string& getCedula() const;

    int reservarAlojamiento(Reservas* reserva); //Falta

    void anularReservacion(Reservas* reserva); //Falta

    static void cargarHuespedes(Huesped**& huespedes, int& totalHuespedes);

    void asociarReservas(Reservas** listaReservas, int totalReservas);

    void mostrarReservasHuesped();

};

#endif // HUESPED_H
