#ifndef HUESPED_H
#define HUESPED_H
#include "reservas.h"
#include "anfitrion.h"
#include <string>

class Anfitrion;
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

    void anularReservacion(const string& _codigoReserva, Reservas**& reservaciones, int& totalReservas);

    static void cargarHuespedes(Huesped**& huespedes, int& totalHuespedes);

    void asociarReservas(Reservas** listaReservas, int totalReservas);

    void liberarReservasHuesped(const string& _codigoReserva);

    void mostrarReservasHuesped();

    void reservarAlojamiento(Alojamiento** alojamientos, int totalAlojamientos,
                             Anfitrion** anfitriones, int totalAnfitriones, Reservas** &reservas, int &totalReservas);

    void eliminarReservaHistorico(const string& codigoReserva);
};

#endif // HUESPED_H
