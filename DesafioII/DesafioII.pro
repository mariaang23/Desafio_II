TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        alojamiento.cpp \
        anfitrion.cpp \
        fecha.cpp \
        huesped.cpp \
        main.cpp \
        reservas.cpp \
        utilidades.cpp

HEADERS += \
    alojamiento.h \
    anfitrion.h \
    fecha.h \
    huesped.h \
    liberar_memoria.h \
    memoria.h \
    reservas.h \
    utilidades.h
