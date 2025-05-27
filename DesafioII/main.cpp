#include "alojamiento.h"
#include "reservas.h"
#include "huesped.h"
#include "anfitrion.h"
#include "utilidades.h"
#include <iostream>

using namespace std;

int main()
{
    int totalAlojamientos = 0, totalAnfitriones = 0;
    Alojamiento** alojamientos = nullptr;
    Anfitrion** anfitriones = nullptr;

    Alojamiento::cargarAlojamientos(alojamientos, totalAlojamientos);
    Anfitrion::cargarAnfitriones(anfitriones, totalAnfitriones);

    for (int i = 0; i < totalAnfitriones; i++) {
        anfitriones[i]->asociarAlojamientos(alojamientos, totalAlojamientos);
    }

    int totalReservas = 0, totalHuespedes = 0;
    Reservas** reservaciones = nullptr;
    Huesped** huespedes = nullptr;

    Reservas::cargarReservas(reservaciones, totalReservas);
    Huesped::cargarHuespedes(huespedes, totalHuespedes);

    // Asociar las reservas a los huéspedes
    for (int i = 0; i < totalHuespedes; i++) {
        huespedes[i]->asociarReservas(reservaciones, totalReservas);
    }

    // Asociar fechas reservadas a cada reserva
    for (int i = 0; i < totalReservas; i++) {
        reservaciones[i]->asociarFechasReservadas();
    }

    // Enlazar reservas con alojamientos
    for (int i = 0; i < totalReservas; ++i) {
        reservaciones[i]->enlazarAlojamiento(alojamientos, totalAlojamientos);
    }

    // Login
    bool exitPpal = false;
    while (!exitPpal) {
        cout << "---------------------------------------------" << endl;
        cout << "BIENVENIDO AL SISTEMA DE RESERVAS" << endl;
        cout << "---------------------------------------------" << endl;

        string id, key;
        cout << "Ingrese su documento: " << endl;
        cin >> id;
        cout << "Ingrese su clave: " << endl;
        cin >> key;

        bool loginExitoso = false;

        // Verificar login de anfitrión
        for (int i = 0; i < totalAnfitriones; i++) {
            if ((anfitriones[i]->getCedulaAnfitrion() == id) && (anfitriones[i]->getClaveAnfitrion() == key)) {
                Anfitrion* anfitrionActual = anfitriones[i];
                mostrarMenuAnfitrion(anfitrionActual, reservaciones, totalReservas, huespedes, totalHuespedes);
                loginExitoso = true;
                break;
            }
        }

        // Verificar login de huésped
        if (!loginExitoso) {
            for (int i = 0; i < totalHuespedes; i++) {
                if ((huespedes[i]->getCedulaHuesped() == id) && (huespedes[i]->getClaveHuesped() == key)) {
                    Huesped* huespedActual = huespedes[i];
                    mostrarMenuHuesped(huespedActual, huespedes, totalHuespedes, reservaciones, totalReservas, alojamientos, totalAlojamientos,anfitriones,totalAnfitriones);
                    loginExitoso = true;
                    break;
                }
            }
        }

        if (!loginExitoso) {
            cout << "\nUsuario o clave incorrecta." << endl;
        }
        cout << "Cantidad reservas: "<<totalReservas;
        string mensaje = "\n---- Seleccione la opcion que desee --- "
                         "\nS. Salir"
                         "\nV. Volver al inicio de sesion\n";
        char opcion = charValidation(mensaje);

        if (opcion == 's' || opcion == 'S') {
            guardarHuespedesArchivo(huespedes, totalHuespedes, "huespedes.txt");
            guardarReservasActivasArchivo(reservaciones, totalReservas, "ReservasActivas.txt");
            cout << "\nHa salido del sistema exitosamente." << endl;
            exitPpal = true;
        }
    }
/*
    // Liberar memoria
    cout << "Liberando anfitriones..." << endl;
    liberarArregloDePunteros(anfitriones, totalAnfitriones);
    cout << "Liberando alojamientos..." << endl;
    liberarArregloDePunteros(alojamientos, totalAlojamientos);
    cout << "Liberando huespedes..." << endl;
    liberarArregloDePunteros(huespedes, totalHuespedes);
    cout << "Liberando reservaciones..." << endl;
    liberarArregloDePunteros(reservaciones, totalReservas);
    cout << "Liberacion terminada." << endl;
*/
    return 0;
}
