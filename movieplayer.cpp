#include "movieplayer.hpp"
#include "ui_movieplayer.h"

MoviePlayer::MoviePlayer(QWidget *parent) : QMainWindow(parent), ui(new Ui::MoviePlayer) {
    ui->setupUi(this);
}


MoviePlayer::~MoviePlayer() {
    delete ui;
}


// ARCHIVO

void MoviePlayer::on_actionAbrir_triggered() { }
void MoviePlayer::on_actionAbrirRecientes_triggered() { }
void MoviePlayer::on_actionGuardarComo_triggered() { }
void MoviePlayer::on_actionCerrar_triggered() { }
void MoviePlayer::on_actionSalir_triggered() { }
