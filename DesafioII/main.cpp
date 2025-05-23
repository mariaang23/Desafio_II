#include "reservas.h"
#include "huesped.h"
#include "alojamiento.h"
#include "anfitrion.h"
#include "fecha.h"
#include "utilidades.h"
#include <iostream>
#include <string>

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

    /*
    //Mostrar alojamientos
    for (int i = 0; i < totalAnfitriones; i++){
        anfitriones[i]->mostrarAlojamientosAnfitrion();
    }
    */

    int totalReservas = 0, totalHuespedes = 0;
    Reservas** reservaciones = nullptr;
    Huesped** huespedes = nullptr;
    Reservas::cargarReservas(reservaciones, totalReservas);
    Huesped::cargarHuespedes(huespedes, totalHuespedes);

    //Asociar las reservas a los huespedes
    for (int i = 0; i < totalHuespedes; i++){
        huespedes[i]->asociarReservas(reservaciones, totalReservas);
    }

    /*
    // Mostrar reservas de cada huesped
    for (int i = 0; i < totHuespedes; i++){
        huespedes[i]->mostrarReservasHuesped();
    }
    */


    //Enlace de reservas a alojamientos
    for (int i = 0; i < totalReservas; ++i) {
        reservaciones[i]->enlazarAlojamiento(alojamientos, totalAlojamientos);
    }

    //mostrar todas las reservas de los alojamientos existentes solo para prueba:
    //mostrarReservasPorAlojamiento(alojamientos, totalAlojamientos, reservaciones, totalReservas);


    //Login funciona perfecto
    string id, key;
    cout << "Ingrese su documento :" << endl;
    cin >> id;
    cout << "Ingrese su clave :" << endl;
    cin >> key;

    for (int i = 0; i < totalAnfitriones; i++) {
        if ((anfitriones[i]->getCedulaAnfitrion() == id) && (anfitriones[i]->getClaveAnfitrion() == key)) {
            Anfitrion* anfitrionActual = anfitriones[i];
            mostrarMenuAnfitrion(anfitrionActual, reservaciones, totalReservas);
            break;
        }
    }

    for (int i = 0; i < totalHuespedes; i++){
        if ((huespedes[i]->getCedulaHuesped() == id) && (huespedes[i]->getClaveHuesped() == key)){
            mostrarMenuHuesped();
        }
    }
    return 0;
}
