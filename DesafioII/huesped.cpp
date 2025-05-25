#include "huesped.h"
#include "reservas.h"
#include "utilidades.h"
#include "alojamiento.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Huesped::Huesped(const string& _cedula, const string& _clave, const string& _antiguedad,
                 const string& _puntuacion, const string& _codigosReservas)
    : cedula(_cedula), clave(_clave), antiguedad(_antiguedad), puntuacion(_puntuacion), codigosReservas(_codigosReservas){}


Huesped::~Huesped(){
    for (int i = 0; i < cantidadReservas; i++){
        delete reservasHuesped[i]; // Eliminar el arreglo de reservas
        reservasHuesped[i] = nullptr;
    }
    delete[] reservasHuesped;
    reservasHuesped = nullptr;
}

const string& Huesped::getCedulaHuesped() const {
    return cedula;
}

const string& Huesped::getClaveHuesped() const{
    return clave;
}

const string& Huesped::getAntiguedad() const{
    return antiguedad;
}

const string& Huesped::getPuntuacion() const{
    return puntuacion;
}

const string& Huesped::getCodigosReservas() const{
    return codigosReservas;
}

void Huesped::cargarHuespedes(Huesped**& huespedes, int& totalHuespedes){
    ifstream archivo("huespedes.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de huespedes\n";
        return;
    }

    string linea;
    int total = 0;

    while (getline(archivo, linea)) total++;
    archivo.clear(); archivo.seekg(0);

    huespedes = new Huesped*[total];
    totalHuespedes = total;
    int i = 0;

    while (getline(archivo, linea)) {
        stringstream frase(linea);
        string cedula_, clave_, antiguedad_, puntuacion_, codigosReservas_;
        getline(frase, cedula_, ';');
        getline(frase, clave_, ';');
        getline(frase, antiguedad_, ';');
        getline(frase, puntuacion_, ';');
        getline(frase, codigosReservas_, ';');
        Huesped* nuevoHuesped= new Huesped(cedula_, clave_, antiguedad_, puntuacion_, codigosReservas_);
        huespedes[i++] = nuevoHuesped;
    }
    archivo.close();
}

void Huesped::asociarReservas(Reservas** listaReservas, int totalReservas){
    int numReservas = 0;
    for (char c : codigosReservas){
        if (c == ',') numReservas++; // Contar la cantidad de reservas que tiene cada huesped
    }

    reservasHuesped = new Reservas*[numReservas]; // Arreglo para almacenar reservas que le corresponden a cada huesped
    cantidadReservas = 0;

    // Asociar los códigos de reservas con las instancias de Reservas
    size_t start = 0;
    while (start < codigosReservas.length()){
        size_t end = codigosReservas.find(',', start);
        if (end == string::npos) {
            end = codigosReservas.length();
        }
        string codReserva = codigosReservas.substr(start, end - start); //Obtener cada codigo de reserva que le pertenece al huesped
        if (codReserva.empty()) {
            break; // o continue, si quieres seguir procesando
        }

        //Buscar la reserva correspondiente en la lista de reservas
        if (!codReserva.empty()){    for (int i = 0; i < totalReservas; i++){
                if (listaReservas[i]->getCodigoReserva() == codReserva){
                    reservasHuesped[cantidadReservas++] = listaReservas[i]; // Asignar la reserva al arreglo de reservas del Huesped (enlazar al huesped con sus reservas)
                    break;
                }
            }
        }
        start = end + 1;
    }
}

void Huesped::mostrarReservasHuesped(){
    std::cout << "Reservas para huesped " << cedula << ":\n";
    for (int j = 0; j < cantidadReservas; j++){
        std::cout << "Reservas: " << cantidadReservas << std::endl;
        reservasHuesped[j]->mostrarReservas();
    }
}

void Huesped::guardarHuespedesArchivo(Huesped** huespedes, int totalHuespedes, const string &archivo){
    ofstream out(archivo, ios::trunc);
    if (!out.is_open()) {
        cerr << "No se pudo abrir el archivo: " << archivo << endl;
        return;
    }

    for (int i = 0; i < totalHuespedes; ++i) {
        out << huespedes[i]->getCedulaHuesped() << ";"
            << huespedes[i]->getClaveHuesped() << ";"
            << huespedes[i]->getAntiguedad() << ";"
            << huespedes[i]->getPuntuacion() << ";";

        string codigos = huespedes[i]->getCodigosReservas();
        if (!codigos.empty()){
            out << codigos;
        }
        out << endl;
    }

    out.close();
}

void Huesped::anularReservacion(const string& _codigoReserva, Reservas**& reservaciones, int& totalReservas){

    string codigoActual = "", nuevoCodigoReservas = "";

    // Actualizar la cadena codigosReservas eliminando el codigo anulado
    for (int i = 0; codigosReservas[i] != '\0'; i++) {
        if (codigosReservas[i] != ',') {
            codigoActual += codigosReservas[i];
        } else {
            if (codigoActual != _codigoReserva) {
                if (!nuevoCodigoReservas.empty()) {
                    nuevoCodigoReservas += ",";
                }
                nuevoCodigoReservas += codigoActual;
            }
            codigoActual = "";
        }
    }

    // Procesar el último código si el string no termina en coma
    if (!codigoActual.empty() && codigoActual != _codigoReserva) {
        if (!nuevoCodigoReservas.empty()) {
            nuevoCodigoReservas += ",";
        }
        nuevoCodigoReservas += codigoActual;
    }

    codigosReservas = nuevoCodigoReservas;

    // Buscar indice de la reserva que se quiere borrar en reservasHuesped
    int indexHuesped = -1;
    for (int i = 0; i < cantidadReservas; i++){
        if(reservasHuesped[i]->getCodigoReserva() == _codigoReserva){
            indexHuesped = i;
            break;
        }
    }

    if (indexHuesped == -1) {
        cout << "\nReserva con código " << _codigoReserva << " no encontrada.\n";
        return;
    }

    // Buscar y borrar la reserva del arreglo global de reservaciones
    int indexGlobal = -1;
    for (int i = 0; i < totalReservas; i++){
        if (reservaciones[i] != nullptr && reservaciones[i]->getCodigoReserva() == _codigoReserva) {
            indexGlobal = i;
            delete reservaciones[i];
            for (int j = i; j < totalReservas; j++){
                reservaciones[j] = reservaciones[j + 1];
            }
            totalReservas--;
            reservaciones[totalReservas] = nullptr;
            break;
        }
    }

    if (indexGlobal == -1){
        cout << "\nNo se encontro la reserva en el arreglo global\n";
        return;
    }

    for (int j = indexHuesped; j < cantidadReservas - 1; j++) {
        reservasHuesped[j] = reservasHuesped[j + 1];
    }
    cantidadReservas--;
    reservasHuesped[cantidadReservas] = nullptr;

    cout << "\nReserva " << _codigoReserva << " anulada correctamente\n";
}

void Huesped::liberarReservasHuesped(const string& _codigoReserva) {
    for (int i = 0; i < cantidadReservas; ++i) {
        reservasHuesped[i] = nullptr;
    }
    reservasHuesped = nullptr;

    // Actualizar codigos de las reservas correspondientes a huesped[i]
    string codigo = "", codigoTotal = "";
    for (size_t i = 0; i < codigosReservas.length(); i++){
        if (codigosReservas[i] == ','){
            codigo += codigosReservas[i];
        }
        else{
            if (codigo != _codigoReserva){
                codigoTotal += codigo + ',';
            }
            codigo = "";
        }
    }
    codigosReservas = codigoTotal;
}

void reservarAlojamiento(Reservas**& reservas, int& totalReservas,
                         Huesped* huespedActual,
                         Alojamiento** alojamientos, int totalAlojamientos,
                         const string& fechaEntrada, int cantNoches){

    cout << "\n----------Metodo de Busqueda----------\n";
    cout << "1. Buscar por costo maximo por noche o por puntuacion minima anfitrion o ambos\n";
    cout << "2. Buscar por codigo de alojamiento\n";

    int opcionBusqueda = intValidation(1,2);
    Alojamiento* alojamientoSeleccionado = nullptr;

    if (opcionBusqueda == 1){
        //Implementar codigo
    }
    else{
        //Busqueda por codigo de alojamiento
        string codigoAlojIngresado;
        cout << "Ingrese el codigo del alojamiento que desea reservar: " << endl;
        cin >> codigoAlojIngresado;

        //Busqueda del codigo de alojamiento encada objeto de la clase alojamiento
        for (int i = 0; i < totalAlojamientos; i++){
            if (alojamientos[i]->getCodigoAlojamiento() == codigoAlojIngresado){
                if (!alojamientos[i]->estaDisponible(fechaEntrada, cantNoches, reservas, totalReservas)){
                    cout << "\nEl alojamiento no está disponible para la fecha y noches indicadas\n";
                    return;
                }
                alojamientoSeleccionado = alojamientos[i];
                break;
            }
        }
        if (!alojamientoSeleccionado){
            cout << "\nNo se encontro un alojamiento con el codigo ingresado\n";
            return;
        }
    }

    // Generar codigo automatico de la reserva
    string codigoNuevaReserva = "R" + to_string(totalReservas + 1);

    float precioPorNoche = stof(alojamientoSeleccionado->getPrecio());
    float montoCalculado = precioPorNoche * cantNoches;

    Reservas* nuevaReserva = new Reservas(codigoNuevaReserva, fechaEntrada, cantNoches, alojamientoSeleccionado->getCodigoAlojamiento(),
                                                                                        huespedActual->getCedulaHuesped(), to_string(montoCalculado));

    nuevaReserva->setAlojamientoPtr(alojamientoSeleccionado);
    nuevaReserva->asociarFechasReservadas();

    // Agregar las reservas al arreglo
    Reservas** nuevasReservas = new Reservas*[totalReservas + 1];
    for (int i = 0; i < totalReservas; i++) {
        nuevasReservas[i] = reservas[i];
    }
    nuevasReservas[totalReservas] = nuevaReserva;
    delete[] reservas;
    reservas = nuevasReservas;
    totalReservas++;

    cout <<"\nReserva creada exitosamente\n";
    nuevaReserva->mostrarReserva();



}

