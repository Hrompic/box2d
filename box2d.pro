TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lsfml-graphics -lsfml-window -lsfml-system -lBox2D
SOURCES += \
        main.cpp

DISTFILES += \
    images/crate_43.png \
    images/gorilla.png \
    images/slice01_01.png
