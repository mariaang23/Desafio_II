#ifndef RESERVAS_H
#define RESERVAS_H

#include <string>
#include "fecha.h"
using namespace std;

class Reservas
{
private:
    string codigoReserva;
    string fechaEntrada;
    string cantNoches;
    string codigoAlojamiento;
    string cedulaHuesped;
    string metodoPago;
    string fechaPago;
    string monto;
    string anotaciones;
    Fecha** fechasReservadas;


public:
    Reservas(const string& _codigoReserva, const string& _fechaEntrada, const string& _cantNoches, const string& _codigoAlojamiento,
             const string& _cedulaHuesped, const string& _metodoPago, const string& _fechaPago, const string& _monto, const string& _anotaciones);

    ~Reservas();

    const string& getCodigoReserva() const;
    const string& getCodigoAlojamiento() const;
    const string& getCantNoches() const;

    void mostrarReservas() const;
    void mostrarFechasReservadas() const;

    void asociarFechasReservadas();

    static void cargarReservas(Reservas**& reservaciones, int& totalReservas);

};
#endif // RESERVAS_H

