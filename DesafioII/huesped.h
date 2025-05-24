#ifndef HUESPED_H
#define HUESPED_H
#include "reservas.h"
#include <string>

using namespace std;

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

    const string& getCedulaHuesped() const;
    const string& getClaveHuesped() const;
    const string& getAntiguedad() const;
    const string& getPuntuacion() const;
    const string& getCodigosReservas() const;

    int reservarAlojamiento(Reservas* reserva); //Falta

    void anularReservacion(Huesped** huespedes,int totalHuespedes, const string& _codigoReserva);

    static void cargarHuespedes(Huesped**& huespedes, int& totalHuespedes);

    void asociarReservas(Reservas** listaReservas, int totalReservas);

    void mostrarReservasHuesped();

    static void guardarHuespedesArchivo(Huesped** huespedes, int totalHuespedes, const string &archivo);
};

#endif // HUESPED_H
