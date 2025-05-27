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
    int cantNoches;
    string codigoAlojamiento;
    Alojamiento* alojamientoPtr;
    string cedulaHuesped;
    string metodoPago;
    string fechaPago;
    string monto;
    string anotaciones;
    Fecha** fechasReservadas;


public:
    Reservas(const string& _codigoReserva, const string& _fechaEntrada, int _cantNoches, const string& _codigoAlojamiento,
             const string& _cedulaHuesped, const string& _metodoPago, const string& _fechaPago, const string& _monto, const string& _anotaciones);

    ~Reservas();

    const string& getCodigoReserva() const; //SE USA
    const string& getCodigoAlojamiento() const; //SE USA
    const string& getCedulaHuesped() const; //SE USA
    const string& getFechaEntrada() const; //SE USA
    int getCantNoches() const;  //SE USA
    const string& getMetodoPago() const; //SE USA
    const string& getFechaPago() const; //SE USA
    const string &getMonto() const; //SE USA
    const string& getAnotaciones() const; //SE USA
    Fecha** getFechasReservadas() const; //SE USA

    void setFechasReservadas(Fecha** nuevasFechas);
    void setAlojamientoPtr(Alojamiento* nuevoPtr); //SE USA

    Alojamiento* getAlojamiento() const;
    void mostrarReserva() const; //SE USA

    void asociarFechasReservadas(); //SE USA

    static void cargarReservas(Reservas**& reservaciones, int& totalReservas); //SE USA

    void enlazarAlojamiento(Alojamiento** alojamientos, int totalAlojamientos); //SE USA

    void mostrarComprobante() const; //SE USA

};
#endif // RESERVAS_H

