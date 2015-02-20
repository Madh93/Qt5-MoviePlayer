#include "movieplayer.hpp"
#include "ui_movieplayer.h"

MoviePlayer::MoviePlayer(QWidget *parent) : QMainWindow(parent), ui(new Ui::MoviePlayer) {

    ui->setupUi(this);

    movie = new Movie;

    //Señales y slots de los botones
    connect(ui->buttonAbrir, SIGNAL(clicked()), this, SLOT(on_actionAbrir_triggered()));
    connect(ui->buttonCerrar, SIGNAL(clicked()), this, SLOT(on_actionCerrar_triggered()));
    connect(ui->buttonPlay, SIGNAL(clicked()), movie, SLOT(start()));
    connect(ui->buttonPausa, SIGNAL(clicked()), this, SLOT(pausar()));
    connect(ui->buttonStop, SIGNAL(clicked()), movie, SLOT(stop()));
}


MoviePlayer::~MoviePlayer() {

    delete ui;

    if (movie) {
        delete movie;
        movie = NULL;
    }
}


/***************************
 MÉTODOS PRIVADOS
**************************/

void MoviePlayer::limpiarMovie() {
/*
    if (movie) {
        delete movie;
        movie = new Movie;
    }
*/
    movie->stop();
    this->setWindowTitle(WINDOW_TITLE);
    activarFuncionalidades(false);
}


void MoviePlayer::activarFuncionalidades(bool cond) {

    ui->actionGuardarComo->setEnabled(cond);
    ui->actionCerrar->setEnabled(cond);
    ui->actionAbrirCon->setEnabled(cond);
    ui->actionAbrirDirectorio->setEnabled(cond);
    ui->actionAbrirTerminal->setEnabled(cond);
    ui->actionRenombrar->setEnabled(cond);
    ui->actionEliminar->setEnabled(cond);
    ui->buttonCerrar->setEnabled(cond);
    ui->buttonPlay->setEnabled(cond);
    ui->buttonPausa->setEnabled(cond);
    ui->buttonStop->setEnabled(cond);
}


/***************************
 SLOTS
**************************/

void MoviePlayer::pausar() {

    movie->setPaused(movie->state() == QMovie::Running);
}


/***************************
 ARCHIVO
**************************/


void MoviePlayer::on_actionAbrir_triggered() {

    QString ruta = QFileDialog::getOpenFileName(this, "Abrir archivo", QString(),
                                                "Todos los archivos (*);;Imagen GIF (*.gif);;Imagen MNG (*.mng);;Vídeo MJPEG (*.mjpeg);;");

    if (!ruta.isEmpty()) {

        QFile file(ruta);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, WINDOW_CRITICAL,"No se puede abrir el archivo.");
            return;
        }

        //Borrar movie anterior
        limpiarMovie();

        //Cargar movie
        movie->setFileName(ruta);
        ui->labelMovie->setMovie(movie);

        if (!movie->isValid()) {
            QMessageBox::critical(this, WINDOW_CRITICAL, "El formato es inválido");
            return;
        }

        //movie->start();

        //Ajustes
        //this->setWindowTitle(imagen->name() + " - Visor de Imágenes");
        activarFuncionalidades(true);
    }

}


void MoviePlayer::on_actionAbrirRecientes_triggered() { }


void MoviePlayer::on_actionGuardarComo_triggered() { }


void MoviePlayer::on_actionCerrar_triggered() {

    limpiarMovie();
}


void MoviePlayer::on_actionSalir_triggered() {

    qApp->quit();
}
