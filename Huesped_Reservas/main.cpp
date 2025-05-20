#include "reservas.h"
#include "huesped.h"

using namespace std;

int main()
{
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
