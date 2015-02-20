#include "movieplayer.hpp"
#include "ui_movieplayer.h"

MoviePlayer::MoviePlayer(QWidget *parent) : QMainWindow(parent), ui(new Ui::MoviePlayer) {

    ui->setupUi(this);

    video = new QMovie;

    //Señales y slots de los botones
    connect(ui->buttonAbrir, SIGNAL(clicked()), this, SLOT(on_actionAbrir_triggered()));
    connect(ui->buttonCerrar, SIGNAL(clicked()), this, SLOT(on_actionCerrar_triggered()));
    connect(ui->buttonPlay, SIGNAL(clicked()), video, SLOT(start()));
    connect(ui->buttonPausa, SIGNAL(clicked()), this, SLOT(pausar()));
    connect(ui->buttonStop, SIGNAL(clicked()), video, SLOT(stop()));
}


MoviePlayer::~MoviePlayer() {

    delete ui;

    if (video) {
        delete video;
        video = NULL;
    }
}


/***************************
 MÉTODOS PRIVADOS
**************************/

void MoviePlayer::limpiarVideo() {
/*
    if (video) {
        delete video;
        video = new QMovie;
    }
*/
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

    video->setPaused(video->state() == QMovie::Running);
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

        //Borrar video anterior
        limpiarVideo();

        //Cargar video
        video->setFileName(ruta);
        ui->labelVideo->setMovie(video);

        if (!video->isValid()) {
            QMessageBox::critical(this, WINDOW_CRITICAL, "El formato es inválido");
            return;
        }

        //video->start();

        //Ajustes
        //this->setWindowTitle(imagen->name() + " - Visor de Imágenes");
        activarFuncionalidades(true);
    }

}


void MoviePlayer::on_actionAbrirRecientes_triggered() { }


void MoviePlayer::on_actionGuardarComo_triggered() { }


void MoviePlayer::on_actionCerrar_triggered() {

    limpiarVideo();
}


void MoviePlayer::on_actionSalir_triggered() {

    qApp->quit();
}
