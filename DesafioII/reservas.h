#ifndef RESERVAS_H
#define RESERVAS_H

#include "alojamiento.h"
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
    Alojamiento* alojamientoPtr;
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

    const string& getCantidadNoches() const;

    const string& getFechaEntrada() const;

    void mostrarReservas() const;
    void mostrarFechasReservadas() const;

    void asociarFechasReservadas();

    void mostrarReserva() const;

    static void cargarReservas(Reservas**& reservaciones, int& totalReservas);

    Alojamiento* getAlojamiento() const;

    void enlazarAlojamiento(Alojamiento** alojamientos, int totalAlojamientos);

    float calcularMonto(Alojamiento* _alojamientoPtr, string& _cantNoches);

};
#endif // RESERVAS_H

