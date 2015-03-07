#include "dispositivos.hpp"
#include "ui_dispositivos.h"

Dispositivos::Dispositivos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dispositivos) {
        ui->setupUi(this);
}

Dispositivos::~Dispositivos() {
    delete ui;
}
