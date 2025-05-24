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

    for (int i = 0; i < totalReservas; i++) {
        reservaciones[i]->asociarFechasReservadas();
    }

    for (int i = 0; i < totalReservas; i++) {
        reservaciones[i]->mostrarFechasReservadas();
    }

    // Enlazar reservas con alojamientos
    for (int i = 0; i < totalReservas; ++i) {
        reservaciones[i]->enlazarAlojamiento(alojamientos, totalAlojamientos);
    }

    // Login
    string id, key;
    cout << "Ingrese su documento: " << endl;
    cin >> id;
    cout << "Ingrese su clave: " << endl;
    cin >> key;

    for (int i = 0; i < totalAnfitriones; i++) {
        if ((anfitriones[i]->getCedulaAnfitrion() == id) && (anfitriones[i]->getClaveAnfitrion() == key)) {
            Anfitrion* anfitrionActual = anfitriones[i];
            mostrarMenuAnfitrion(anfitrionActual, reservaciones, totalReservas);
            break;
        }
    }

    for (int i = 0; i < totalHuespedes; i++) {
        if ((huespedes[i]->getCedulaHuesped() == id) && (huespedes[i]->getClaveHuesped() == key)) {
            mostrarMenuHuesped();
        }
    }

    return 0;
}
