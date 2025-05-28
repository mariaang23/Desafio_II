#ifndef LIBERAR_MEMORIA_H
#define LIBERAR_MEMORIA_H

#include "memoria.h"

// Plantilla genérica para liberar arreglos de punteros
template <typename T>
void liberarArregloDePunteros(T**& arreglo, int& total) {
    if (arreglo == nullptr) return;

    for (int i = 0; i < total; ++i) {
        if (arreglo[i] != nullptr) {
            delete arreglo[i];
            liberarMemoria<T>(1);
            arreglo[i] = nullptr;
        }
    }

    delete[] arreglo;
    liberarMemoria<T*>(total);
    arreglo = nullptr;
    total = 0;
}

#endif // LIBERAR_MEMORIA_H
