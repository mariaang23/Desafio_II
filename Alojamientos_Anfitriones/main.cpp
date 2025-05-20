#include "alojamiento.h"
#include "anfitrion.h"

int main() {
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

    for (int i = 0; i < totalAlojamientos; i++) delete alojamientos[i];
    delete[] alojamientos;
    for (int i = 0; i < totalAnfitriones; i++) delete anfitriones[i];
    delete[] anfitriones;

    return 0;
}
