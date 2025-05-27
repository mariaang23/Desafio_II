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

Huesped::Huesped(const string& _cedula, const string& _clave, const string& _antiguedad,
                 const string& _puntuacion, const string& _codigosReservas)
    : cedula(_cedula), clave(_clave), antiguedad(_antiguedad), puntuacion(_puntuacion), codigosReservas(_codigosReservas){}


Huesped::~Huesped() {
    delete[] reservasHuesped;  // liberás solo el arreglo de punteros
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
        cout << "No se pudo abrir el archivo de huespedes.txt\n";
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
        cout << "\nReserva con codigo " << _codigoReserva << " no encontrada.\n";
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
}

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

        //modificar codigos del huesped
        string codigoTotal = "";
        codigoTotal = codigosReservas +","+ codigoNuevaReserva;
        codigosReservas = codigoTotal;

        //asociar esta reserva con el huesped
        int nuevaCantidadReservas = cantidadReservas+1;
        Reservas** nuevasReservasHuesped = new Reservas*[nuevaCantidadReservas];
        for (int i = 0; i < cantidadReservas; ++i) {
            nuevasReservasHuesped[i] = reservasHuesped[i];
        }
        nuevasReservasHuesped[nuevaCantidadReservas]=nuevaReserva;
        delete[] reservasHuesped;
        reservasHuesped = nuevasReservasHuesped;
        cantidadReservas++;


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

        //modificar codigos del huesped
        string codigoTotal = "";
        codigoTotal = codigosReservas +","+ codigoNuevaReserva;
        codigosReservas = codigoTotal;

        //asociar esta reserva con el huesped
        int nuevaCantidadReservas = cantidadReservas+1;
        Reservas** nuevasReservasHuesped = new Reservas*[nuevaCantidadReservas];
        for (int i = 0; i < cantidadReservas; ++i) {
            nuevasReservasHuesped[i] = reservasHuesped[i];
        }
        nuevasReservasHuesped[nuevaCantidadReservas]=nuevaReserva;
        delete[] reservasHuesped;
        reservasHuesped = nuevasReservasHuesped;
        cantidadReservas++;

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

        cout <<"\nReserva creada exitosamente.\n";

        nuevaReserva->mostrarComprobante();
    }
}

void Huesped::eliminarReservaHistorico(const string& codigoReserva) {
    // Eliminar el código de codigosReservas
    string nuevaLista = "";
    string codigoActual = "";

    for (size_t i = 0; i <= codigosReservas.length(); ++i) {
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
        if (reservasHuesped[i] != nullptr && reservasHuesped[i]->getCodigoReserva() == codigoReserva) {
            for (int j = i; j < cantidadReservas - 1; ++j) {
                reservasHuesped[j] = reservasHuesped[j + 1];
            }
            cantidadReservas--;
            reservasHuesped[cantidadReservas] = nullptr;
            break;
        }
    }
}

