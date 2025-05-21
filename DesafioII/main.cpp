#include "reservas.h"
#include "huesped.h"
#include "alojamiento.h"
#include "anfitrion.h"

using namespace std;

int main()
{
    int totalAlojamientos =0, totalAnfitriones=0;
    Alojamiento** alojamientos = nullptr;
    Anfitrion** anfitriones = nullptr;

    Alojamiento::cargarAlojamientos(alojamientos,totalAlojamientos);
    Anfitrion::cargarAnfitriones(anfitriones, totalAnfitriones);

    for (int i = 0; i < totalAnfitriones; i++){
        anfitriones[i]->asociarAlojamientos(alojamientos, totalAlojamientos);
    }

    for (int i = 0; i < totalAnfitriones; i++){
        anfitriones[i]->mostrarAlojamientosAnfitrion();
    }

    int totReservas = 0, totHuespedes = 0;
    Reservas** reservaciones = nullptr;
    Huesped** huespedes = nullptr;
    Reservas::cargarReservas(reservaciones, totReservas);
    Huesped::cargarHuespedes(huespedes, totHuespedes);

    //Asociar las reservas a los huespedes
    for (int i = 0; i < totHuespedes; i++){
        huespedes[i]->asociarReservas(reservaciones, totReservas);
    }

    // Mostrar reservas de cada huesped
    for (int i = 0; i < totHuespedes; i++){
        huespedes[i]->mostrarReservasHuesped();
    }
}
