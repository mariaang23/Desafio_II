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
    const string& getCedulaHuesped() const;
    const string& getFechaEntrada() const;
    const string& getCantNoches() const;
    const string& getMetodoPago() const;
    const string& getFechaPago() const;
    const string& getMonto() const;
    const string& getAnotaciones() const;
    Fecha** getFechasReservadas() const;

    void setFechasReservadas(Fecha** nuevasFechas);
    void setAlojamientoPtr(Alojamiento* nuevoPtr);

    Alojamiento* getAlojamiento() const;
    void mostrarReservas() const;
    void mostrarFechasReservadas() const;
    void mostrarReserva() const;

    void asociarFechasReservadas();

    static void cargarReservas(Reservas**& reservaciones, int& totalReservas);

    void enlazarAlojamiento(Alojamiento** alojamientos, int totalAlojamientos);

    float calcularMonto(Alojamiento* _alojamientoPtr, string& _cantNoches);

    static void guardarReservasActivasArchivo(Reservas **&reservas, int &totalReservas, const string& archivo);
};
#endif // RESERVAS_H

