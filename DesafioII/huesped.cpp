#include "memoria.h"
#include "huesped.h"
#include "reservas.h"
#include "utilidades.h"
#include "alojamiento.h"
#include "anfitrion.h"
#include "fecha.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

/**
 * @brief Constructor de la clase Huesped.
 *
 * Inicializa un objeto Huesped con los datos personales y la cadena de códigos de reservas asociadas.
 * Este constructor no asigna memoria ni inicializa el arreglo de reservas directamente;
 * dicha asociación se realiza posteriormente mediante el método asociarReservas().
 *
 * @param _cedula Cédula del huésped (identificación única).
 * @param _clave Clave de acceso del huésped.
 * @param _antiguedad Antigüedad del huésped en el sistema (puede representar años o fechas).
 * @param _puntuacion Valor de calificación asignado al huésped.
 * @param _codigosReservas Cadena con los códigos de reserva asociados al huésped, separados por comas.
 */
Huesped::Huesped(const string& _cedula, const string& _clave, const string& _antiguedad,
                 const string& _puntuacion, const string& _codigosReservas)
    : cedula(_cedula), clave(_clave), antiguedad(_antiguedad), puntuacion(_puntuacion), codigosReservas(_codigosReservas){}

/**
 * @brief Destructor de la clase Huesped.
 *
 * Libera la memoria dinámica asignada al arreglo de punteros a reservas (`reservasHuesped`).
 * No elimina los objetos `Reservas` apuntados, ya que son gestionados globalmente por el sistema.
 * Esta limpieza asegura que no haya fugas de memoria asociadas al arreglo propio del huésped.
 */
Huesped::~Huesped() {
    delete[] reservasHuesped;  // liberar solo el arreglo de punteros
    liberarMemoria<Reservas*>(cantidadReservas);
    reservasHuesped = nullptr;
    mostrarUsoMemoria();
}

/**
 * @brief Obtiene la cédula del huésped.
 * @return cédula del huésped como string.
 */
const string& Huesped::getCedulaHuesped() const {
    return cedula;
}

/**
 * @brief Obtiene la clave del huésped.
 * @return clave del huésped como string.
 */
const string& Huesped::getClaveHuesped() const{
    return clave;
}

/**
 * @brief Obtiene la antiguedad del huésped.
 * @return antiguedad del huésped como string.
 */
const string& Huesped::getAntiguedad() const{
    return antiguedad;
}

/**
 * @brief Obtiene la puntuacion del huésped.
 * @return puntuacion del huésped como string.
 */
const string& Huesped::getPuntuacion() const{
    return puntuacion;
}

/**
 * @brief Obtiene los codigos de reservas del huésped.
 * @return codigosReservas del huésped como string.
 */
const string& Huesped::getCodigosReservas() const{
    return codigosReservas;
}

/**
 * @brief Carga la información de los huéspedes desde un archivo de texto.
 *
 * Lee el archivo "huespedes.txt" y crea dinamicamente un arreglo de punteros a objetos Huesped
 * por cada linea del archivo
 *
 * @param huespedes Referencia al puntero doble donde se almacenaran los objetos huespedes leidos.
 * @param totalHuespedes Referencia al entero que se actualizara con la cantidad total de huespedes.
 */
void Huesped::cargarHuespedes(Huesped**& huespedes, int& totalHuespedes){
    ifstream archivo("huespedes.txt");
    if (!archivo.is_open()) {
        cout << "No se pudo abrir el archivo de huespedes.txt\n";
        return;
    }

    string linea;
    int total = 0;

    while (getline(archivo, linea)) total++;
    archivo.clear(); archivo.seekg(0);

    huespedes = new Huesped*[total];
    registrarMemoria<Huesped*>(total);
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
        registrarMemoria<Huesped>(1);
        huespedes[i++] = nuevoHuesped;
        incrementarIteraciones();
    }
    archivo.close();

}

/**
 * @brief Asocia las reservas correspondientes a un huesped segun sus codigos.
 *
 * Recorre la cadena de códigos de reservas del huésped (separados por comas) y enlaza cada código
 * con su respectiva instancia de Reservas.
 *
 * @param listaReservas Arreglo general de punteros a objetos Reservas.
 * @param totalReservas Cantidad total de reservas disponibles en el sistema.
 */
void Huesped::asociarReservas(Reservas** listaReservas, int totalReservas){
    int numReservas = 0;
    for (char c : codigosReservas){
        if (c == ',') {
            numReservas++; // Contar la cantidad de reservas que tiene cada huesped
        }
    }

    reservasHuesped = new Reservas*[numReservas]; // Arreglo para almacenar reservas que le corresponden a cada huesped
    registrarMemoria<Reservas*>(numReservas);
    cantidadReservas = 0;

    // Asociar los códigos de reservas con las instancias de Reservas
    size_t start = 0;
    while (start < codigosReservas.length()){
        incrementarIteraciones<>();
        size_t end = codigosReservas.find(',', start);
        if (end == string::npos) {
            end = codigosReservas.length();
        }
        string codReserva = codigosReservas.substr(start, end - start); //Obtener cada codigo de reserva que le pertenece al huesped
        if (codReserva.empty()) {
            break; // o continue, si quieres seguir procesando
        }

        //Buscar la reserva correspondiente en la lista de reservas
        if (!codReserva.empty()){
            for (int i = 0; i < totalReservas; i++){
                if (listaReservas[i]->getCodigoReserva() == codReserva){
                    reservasHuesped[cantidadReservas++] = listaReservas[i]; // Asignar la reserva al arreglo de reservas del Huesped (enlazar al huesped con sus reservas)
                    incrementarIteraciones<>();
                    break;
                }   
            }
        }
        start = end + 1;
    }
}

/**
 * @brief Anula una reservación asociada al huesped y la elimina del sistema.
 *
 * Este metodo realiza un proceso completo para anular una reserva:
 * - Actualiza la cadena interna codigosReservas del huesped, eliminando el codigo de la reserva anulada.
 * - Busca la reserva dentro del arreglo reservasHuesped y la elimina de dicho arreglo.
 * - Busca la reserva tambien dentro del arreglo global reservaciones, la elimina y reorganiza el arreglo
 * para mantenerlo compacto.
 * - Libera la memoria ocupada por la reserva anulada.
 *
 * Si la reserva no se encuentra en alguna de las estructuras, se muestra un mensaje indicativo.
 *
 * @param _codigoReserva Codigo de la reserva que se desea anular.
 * @param reservaciones Referencia al arreglo global de punteros a objetos Reservas.
 * @param totalReservas Referencia a la cantidad total de reservas registradas en el sistema (se decrementa
 * si se anula con exito).
 */
void Huesped::anularReservacion(const string& _codigoReserva, Reservas**& reservaciones, int& totalReservas){

    string codigoActual = "", nuevoCodigoReservas = "";

    // Actualizar la cadena codigosReservas eliminando el codigo anulado
    for (int i = 0; codigosReservas[i] != '\0'; i++) {
        incrementarIteraciones();
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
        incrementarIteraciones();
        if(reservasHuesped[i]->getCodigoReserva() == _codigoReserva){
            indexHuesped = i;
            break;
        }
    }

    if (indexHuesped == -1) {
        cout << "\nReserva con codigo " << _codigoReserva << " no encontrada.\n";
        return;
    }

    // Buscar y borrar la reserva del arreglo global de reservaciones
    int indexGlobal = -1;
    for (int i = 0; i < totalReservas; i++){
        incrementarIteraciones();
        if (reservaciones[i] != nullptr && reservaciones[i]->getCodigoReserva() == _codigoReserva) {
            indexGlobal = i;
            delete reservaciones[i];
            liberarMemoria<Reservas>(1);
            for (int j = i; j < totalReservas; j++){
                incrementarIteraciones();
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

    //actualizar codigos de reservas del huesped
    for (int j = indexHuesped; j < cantidadReservas - 1; j++) {
        incrementarIteraciones();
        reservasHuesped[j] = reservasHuesped[j + 1];
    }
    cantidadReservas--;
    reservasHuesped[cantidadReservas] = nullptr;

    cout << "\nReserva " << _codigoReserva << " anulada correctamente\n";

    mostrarUsoMemoria();
}

/**
 * @brief Libera el arreglo de reservas del huesped y elimina la referencia a un codigo de reserva especifico.
 *
 * Este metodo realiza dos acciones:
 * 1. Anula todos los punteros del arreglo reservasHuesped y lo pone en nullptr.
 *    **Nota:** No se libera memoria ni se destruyen las instancias de Reservas; solo se eliminan las
 *    referencias del huésped.
 * 2. Actualiza la cadena codigosReservas, eliminando el codigo especificado.
 * Se usa para la funcion del anfitrion de anular reserva.
 *
 * @param _codigoReserva Codigo de la reserva que se desea eliminar de la cadena de codigos.
 */
void Huesped::liberarReservasHuesped(const string& _codigoReserva) {
    for (int i = 0; i < cantidadReservas; ++i) {
        incrementarIteraciones();
        reservasHuesped[i] = nullptr;
    }
    reservasHuesped = nullptr;

    string codigoActual = "", nuevoCodigoReservas = "";

    // Actualizar la cadena codigosReservas eliminando el codigo anulado
    for (int i = 0; codigosReservas[i] != '\0'; i++) {
        incrementarIteraciones();
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

    mostrarUsoMemoria();
}



/**
 * @brief Permite al huésped realizar una reserva de alojamiento mediante filtros o por codigo.
 *
 * El metodo guia al usuario por un flujo interactivo para reservar un alojamiento.
 * Existen dos modos de busqueda:
 * 1. Mediante filtros como municipio, costo máximo por noche y puntuacion minima del anfitrion.
 * 2. Ingresando directamente el código del alojamiento deseado.
 *
 * Si se encuentra un alojamiento disponible, se solicita al huesped la opción de agregar notas y seleccionar
 * un metodo de pago.
 * Finalmente, se llama a procesarReserva() para registrar la reserva, actualizar la informacion del huesped
 * y del sistema global.
 *
 * @param alojamientos Arreglo de punteros a todos los alojamientos disponibles.
 * @param totalAlojamientos Cantidad total de alojamientos en el sistema.
 * @param anfitriones Arreglo de punteros a anfitriones registrados en el sistema.
 * @param totalAnfitriones Cantidad total de anfitriones.
 * @param reservas Referencia al arreglo global de reservas del sistema.
 * @param totalReservas Referencia al total de reservas existentes (se incrementa si la reserva es exitosa).
 */
void Huesped::reservarAlojamiento(Alojamiento** alojamientos,int totalAlojamientos,
                                  Anfitrion** anfitriones, int totalAnfitriones, Reservas **& reservas, int& totalReservas) {

    cout << "\n----------Metodo de Busqueda----------\n";
    cout << "1. Buscar por costo maximo por noche o por puntuacion minima anfitrion o ambos\n";
    cout << "2. Buscar por codigo de alojamiento\n";

    int opcionBusqueda = intValidation(1,2);

    if(opcionBusqueda == 1){

        string fechaStr, municipio;
        int cantNoches;

        cout << "Ingrese la fecha de entrada (dd/mm/aaaa): ";
        cin >> fechaStr;
        Fecha fechaEntrada = Fecha::fromString(fechaStr);

        if (!fechaEntrada.esValida()) {
            cout << "Fecha invalida.\n Vuelva a intentarlo. /n";
            return;
        }

        cout << "Ingrese la cantidad de noches: ";
        cin >> cantNoches;

        cout << "\nIngrese el municipio destino: ";
        cin.ignore();
        getline(cin,municipio);

        // Filtro opcional
        cout << "\n---Seleccione una opcion segun desee---\n";
        cout << "1. Costo maximo\n2. Puntuacion minima del anfitrion\n3. Ambos\n4. Ninguno\nOpcion: ";
        int opcionFiltro;
        cin >> opcionFiltro;

        float costoMax = 0;
        float puntuacionMin = 0;

        if (opcionFiltro == 1 || opcionFiltro == 3) {
            cout << "Ingrese el costo maximo por noche: ";
            cin >> costoMax;
        }

        if (opcionFiltro == 2 || opcionFiltro == 3) {
            cout << "Ingrese la puntuacion minima del anfitrion (1.0 a 5.0): ";
            cin >> puntuacionMin;
        }

        // Mostrar alojamientos disponibles con filtros
        cout << "\n--- Alojamientos Disponibles ---\n";
        int indexAloj =0;
        for (int i = 0; i < totalAlojamientos; ++i) {
            incrementarIteraciones();
            indexAloj = i;
            Alojamiento* aloj = alojamientos[i];
            string municipioAloj = aloj->getMunicipio();

            if (municipioAloj != municipio) {
                // Si el municipio no coincide, no se muestra este alojamiento
                continue;//se sale y va a iterar sobre el siguiente
            }

            string fechaEntradaStr = fechaEntrada.aString();
            int precio = aloj->getPrecio();
            //buscar el anfitrion
            string codigoAloj = aloj->getCodigoAlojamiento();
            Anfitrion* anfitrionAloj = nullptr;
            for (int j = 0; j < totalAnfitriones; ++j) {
                incrementarIteraciones();
                if (anfitriones[j]->poseeAlojamiento(codigoAloj)) {
                    if (!alojamientos[i]->estaDisponible(fechaEntradaStr, cantNoches, reservas, totalReservas)){
                        cout << "\nEl alojamiento no esta disponible para la fecha y noches indicadas\n";
                        return;
                    }
                    aloj = alojamientos[i];
                    anfitrionAloj = anfitriones[j];
                    break;
                }
            }

            bool pasaFiltroCosto = true;
            bool pasaFiltroPuntuacion = true;

            //si el huesped pidio que se hiciera el filtro por medio del
            //costo y por medio de la puntuacion
            if (opcionFiltro == 1 || opcionFiltro == 3) {
                //si el precio del alojamiento es menor al costo ingresado max
                pasaFiltroCosto = (precio <= costoMax);
            }

            if (opcionFiltro == 2 || opcionFiltro == 3) {
                //si la puntuacion del anfitrionAloj es mayor a la ingresada
                pasaFiltroPuntuacion = (stof(anfitrionAloj->getPuntuacion()) >= puntuacionMin);
            }

            if (pasaFiltroCosto && pasaFiltroPuntuacion) {
                // Mostrar alojamiento, cumple todos los filtros
                cout <<"   "<< indexAloj<<". Nombre: " << aloj->getNombre() << "\n     Codigo Alojamiento: " << aloj->getCodigoAlojamiento() << "\n     Precio por noche: $"
                     << precio << "\n     Puntuacion del anfitrion encargado: " << anfitrionAloj->getPuntuacion() << "\n";

                continue;
            }

            cout <<"   "<< indexAloj<<". Nombre: " << aloj->getNombre() << "\n      Codigo Alojamiento: " << aloj->getCodigoAlojamiento() << "\n      Precio por noche: $"
                 << precio << "\n     Puntuacion del anfitrion encargado: " << anfitrionAloj->getPuntuacion() << "\n";
        }

        //escoger alojamiento
        int opcion = 0,opcionNotas;
        string notasHuesped = "";
        cout <<"\nSeleccione el alojamiento que desee segun su indice: ";
        cin >> opcion;

        cout << "--- Notas --- \n"
                "1. Agregar anotaciones. \n"
                "2. Realizar la reserva. \n"
                "Seleccione una opcion: ";
        cin >> opcionNotas;

        //para anotaciones
        bool notasExitoso = false;
        while (opcionNotas == 1 && !notasExitoso) {
            incrementarIteraciones();
            cout << "\n(Limite de 1000 caracteres)"
                    "\nIngrese anotaciones: " << endl;
            cin.ignore();
            getline(cin, notasHuesped);

            if (notasHuesped.size() > 1000) {
                cout << "Excede el limite de caracteres.\n Intente nuevamente.\n";
            } else {
                notasExitoso = true;
            }
        }

        // obtener metodo de pago
        int opMetodo;
        string metodoPago = "";
        bool metodoExitoso = false;

        while (!metodoExitoso) {
            incrementarIteraciones();
            cout << "\n--- Metodo de pago ---\n"
                    "1. PSE\n"
                    "2. Tarjeta de Credito\n"
                    "Seleccione una opcion: ";
            cin >> opMetodo;

            if (opMetodo == 1) {
                metodoPago = "PSE";
                metodoExitoso = true;
            } else if (opMetodo == 2) {
                metodoPago = "Tarjeta de Credito";
                metodoExitoso = true;
            } else {
                cout << "Opcion invalida.\nIntente nuevamente.\n";
            }
        }

        Alojamiento* alojamientoSeleccionado = alojamientos[opcion];

        // Generar codigo automatico de la reserva
        string codigoNuevaReserva = "R" + to_string(totalReservas + 1);

        //datos para crear la nueva reserva
        string codigoAlojamiento = alojamientoSeleccionado->getCodigoAlojamiento();
        int precioPorNoche = alojamientoSeleccionado->getPrecio();
        int montoCalculado = precioPorNoche * cantNoches;
        string montoStr = to_string(montoCalculado);
        string cedulaHuesped = getCedulaHuesped();
        string fechaEntradaStr = fechaEntrada.aString();
        Fecha fechaPago = fechaEntrada + cantNoches;
        string fechaPagoStr = fechaPago.aString();


        Reservas* nuevaReserva = new Reservas(codigoNuevaReserva, fechaEntradaStr, cantNoches,codigoAlojamiento,
                                              cedulaHuesped,metodoPago,fechaPagoStr,montoStr,notasHuesped);
        registrarMemoria<Reservas>(1);

        //modificar codigos del huesped
        string codigoTotal = "";
        codigoTotal = codigosReservas +","+ codigoNuevaReserva;
        codigosReservas = codigoTotal;

        //asociar esta reserva con el huesped
        int nuevaCantidadReservas = cantidadReservas+1;
        Reservas** nuevasReservasHuesped = new Reservas*[nuevaCantidadReservas];
        registrarMemoria<Reservas*>(nuevaCantidadReservas);
        for (int i = 0; i < cantidadReservas; ++i) {
            incrementarIteraciones();
            nuevasReservasHuesped[i] = reservasHuesped[i];
        }
        nuevasReservasHuesped[nuevaCantidadReservas]=nuevaReserva;
        delete[] reservasHuesped;
        liberarMemoria<Reservas*>(cantidadReservas);
        reservasHuesped = nuevasReservasHuesped;
        cantidadReservas++;


        nuevaReserva->setAlojamientoPtr(alojamientoSeleccionado);
        nuevaReserva->asociarFechasReservadas();

        // Agregar las reservas al arreglo
        Reservas** nuevasReservas = new Reservas*[totalReservas + 1];
        registrarMemoria<Reservas*>(totalReservas + 1);
        for (int i = 0; i < totalReservas; i++) {
            incrementarIteraciones();
            nuevasReservas[i] = reservas[i];
        }
        nuevasReservas[totalReservas] = nuevaReserva;
        delete[] reservas;
        liberarMemoria<Reservas*>(totalReservas);
        reservas = nuevasReservas;
        totalReservas++;

        cout <<"\nReserva creada exitosamente.\n";

        nuevaReserva->mostrarComprobante();


    }else{

        //Ingreso por codigo
        string codigoAloj = "";
        cout << "\nIngrese codigo de alojamiento: ";
        cin.ignore();
        getline(cin, codigoAloj);

        //pedir fecha y noches
        string fechaStr;
        int cantNoches;

        cout << "\nIngrese la fecha de entrada (dd/mm/aaaa): ";
        cin.ignore();
        getline(cin, fechaStr);
        Fecha fechaEntrada = Fecha::fromString(fechaStr);

        if (!fechaEntrada.esValida()) {
            cout << "Fecha invalida.\n Vuelva a intentarlo. \n";
            return;
        }

        cout << "\nIngrese la cantidad de noches: ";
        cin >> cantNoches;

        string fechaEntradaStr = fechaEntrada.aString();
        //seleccionar alojamiento
        Alojamiento* alojamientoSeleccionado = nullptr;
        for (int i = 0; i < totalAlojamientos; i++){
            incrementarIteraciones();
            if (alojamientos[i]->getCodigoAlojamiento() == codigoAloj){
                if (!alojamientos[i]->estaDisponible(fechaEntradaStr, cantNoches, reservas, totalReservas)){
                    cout << "\nEl alojamiento no esta disponible para la fecha y noches indicadas.\n";
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


        //si desea agregar notas
        int opcionNotas;
        string notasHuesped = "";

        cout << "\n--- Notas --- \n"
                "1. Agregar anotaciones. \n"
                "2. Realizar la reserva. \n"
                "Seleccione una opcion: ";
        cin >> opcionNotas;

        bool notasExitoso = false;
        while (opcionNotas == 1 && !notasExitoso) {
            incrementarIteraciones();
            cout << "\n(Limite de 1000 caracteres)\nIngrese anotaciones: " << endl;
            cin.ignore();
            getline(cin, notasHuesped);

            if (notasHuesped.size() > 1000) {
                cout << "Excede el limite de caracteres.\n Intente nuevamente.\n";
            } else {
                notasExitoso = true;
            }
        }

        // obtener metodo de pago
        int opMetodo;
        string metodoPago = "";
        bool metodoExitoso = false;

        while (!metodoExitoso) {
            incrementarIteraciones();
            cout << "\n--- Metodo de pago ---\n"
                    "1. PSE\n"
                    "2. Tarjeta de Credito\n"
                    "Seleccione una opcion: ";
            cin >> opMetodo;

            if (opMetodo == 1) {
                metodoPago = "PSE";
                metodoExitoso = true;
            } else if (opMetodo == 2) {
                metodoPago = "Tarjeta de Credito";
                metodoExitoso = true;
            } else {
                cout << "Opcion invalida.\nIntente nuevamente.\n";
            }
        }


        // Generar codigo automatico de la reserva
        string codigoNuevaReserva = "R" + to_string(totalReservas + 1);

        //datos para crear la nueva reserva
        string codigoAlojamiento = alojamientoSeleccionado->getCodigoAlojamiento();
        int precioPorNoche = alojamientoSeleccionado->getPrecio();
        int montoCalculado = precioPorNoche * cantNoches;
        string montoStr = to_string(montoCalculado);
        string cedulaHuesped = getCedulaHuesped();
        Fecha fechaPago = fechaEntrada + cantNoches;
        string fechaPagoStr = fechaPago.aString();


        Reservas* nuevaReserva = new Reservas(codigoNuevaReserva, fechaEntradaStr, cantNoches,codigoAlojamiento,
                                              cedulaHuesped,metodoPago,fechaPagoStr,montoStr,notasHuesped);

        registrarMemoria<Reservas>(1);

        //modificar codigos del huesped
        string codigoTotal = "";
        codigoTotal = codigosReservas +","+ codigoNuevaReserva;
        codigosReservas = codigoTotal;

        //asociar esta reserva con el huesped
        int nuevaCantidadReservas = cantidadReservas+1;
        Reservas** nuevasReservasHuesped = new Reservas*[nuevaCantidadReservas];
        registrarMemoria<Reservas*>(nuevaCantidadReservas);
        for (int i = 0; i < cantidadReservas; ++i) {
            incrementarIteraciones();
            nuevasReservasHuesped[i] = reservasHuesped[i];
        }
        nuevasReservasHuesped[nuevaCantidadReservas]=nuevaReserva;
        delete[] reservasHuesped;
        liberarMemoria<Reservas*>(cantidadReservas);
        reservasHuesped = nuevasReservasHuesped;
        cantidadReservas++;

        nuevaReserva->setAlojamientoPtr(alojamientoSeleccionado);
        nuevaReserva->asociarFechasReservadas();

        // Agregar las reservas al arreglo
        Reservas** nuevasReservas = new Reservas*[totalReservas + 1];
        registrarMemoria<Reservas*>(totalReservas + 1);
        for (int i = 0; i < totalReservas; i++) {
            incrementarIteraciones();
            nuevasReservas[i] = reservas[i];
        }
        nuevasReservas[totalReservas] = nuevaReserva;
        delete[] reservas;
        liberarMemoria<Reservas*>(totalReservas);
        reservas = nuevasReservas;
        totalReservas++;

        cout <<"\nReserva creada exitosamente.\n";

        nuevaReserva->mostrarComprobante();
    }
    mostrarUsoMemoria();
}

/**
 * @brief Elimina una reserva del historial del huesped.
 *
 * Este metodo realiza dos acciones principales:
 * 1. Actualiza la cadena codigosReservas, eliminando el codigo de la reserva especificada.
 * 2. Elimina el puntero correspondiente del arreglo reservasHuesped, desplazando los elementos posteriores
 *    para mantener el arreglo compacto y consistente.
 *
 * No libera memoria, ya que este método solo elimina la referencia interna del huésped y se asume que
 * la memoria de la reserva es gestionada globalmente.
 *
 * @param codigoReserva Codigo de la reserva que se desea eliminar del historial del huesped.
 */

void Huesped::eliminarReservaHistorico(const string& codigoReserva) {
    // Eliminar el código de codigosReservas
    string nuevaLista = "";
    string codigoActual = "";

    for (size_t i = 0; i <= codigosReservas.length(); ++i) {
        incrementarIteraciones();
        if (i == codigosReservas.length() || codigosReservas[i] == ',') {
            if (!codigoActual.empty() && codigoActual != codigoReserva) {
                if (!nuevaLista.empty()) {
                    nuevaLista += ",";
                }
                nuevaLista += codigoActual;
            }
            codigoActual = "";
        } else {
            codigoActual += codigosReservas[i];
        }
    }
    codigosReservas = nuevaLista;

    //Eliminar del arreglo reservasHuesped
    for (int i = 0; i < cantidadReservas; ++i) {
        incrementarIteraciones();
        if (reservasHuesped[i] != nullptr && reservasHuesped[i]->getCodigoReserva() == codigoReserva) {
            for (int j = i; j < cantidadReservas - 1; ++j) {
                reservasHuesped[j] = reservasHuesped[j + 1];
            }
            cantidadReservas--;
            reservasHuesped[cantidadReservas] = nullptr;
            break;
        }
    }
    mostrarUsoMemoria();
}

