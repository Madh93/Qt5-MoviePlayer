QT       += core gui \
    multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Movie
CONFIG+=ordered
TEMPLATE = app

INCLUDEPATH += $$PWD/acerca
INCLUDEPATH += $$PWD/capturebuffer
INCLUDEPATH += $$PWD/dispositivos
INCLUDEPATH += $$PWD/movie
INCLUDEPATH += $$PWD/movieplayer
INCLUDEPATH += $$PWD/slider

SOURCES += main.cpp\
    acerca/acerca.cpp \
    capturebuffer/capturebuffer.cpp \
    dispositivos/dispositivos.cpp \
    slider/slider.cpp \
    movie/movie.cpp \
    movieplayer/movieplayer.cpp

HEADERS  += base.hpp \
    acerca/acerca.hpp \
    capturebuffer/capturebuffer.hpp \
    dispositivos/dispositivos.hpp \
    slider/slider.hpp \
    movie/movie.hpp \
    movieplayer/movieplayer.hpp

FORMS    += acerca/acerca.ui \
    dispositivos/dispositivos.ui \
    movieplayer/movieplayer.ui
