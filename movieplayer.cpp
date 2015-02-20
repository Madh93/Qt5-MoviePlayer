#include "movieplayer.hpp"
#include "ui_movieplayer.h"

MoviePlayer::MoviePlayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MoviePlayer)
{
    ui->setupUi(this);
}

MoviePlayer::~MoviePlayer()
{
    delete ui;
}
