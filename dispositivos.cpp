#include "dispositivos.hpp"
#include "ui_dispositivos.h"

Dispositivos::Dispositivos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dispositivos) {
        ui->setupUi(this);

        dispositivos = QCameraInfo::availableCameras();
        setDispositivos();
}


Dispositivos::~Dispositivos() {
    delete ui;
}


void Dispositivos::setDispositivos() {

    ui->comboBox->addItem("Predeterminado");

    if (dispositivos.size() > 0)
        foreach (const QCameraInfo &camara, dispositivos)
            ui->comboBox->addItem(camara.description());
}


QString Dispositivos::getDispositivo() {

    int index = ui->comboBox->currentIndex();

    if (index == 0)
        index++;

   return dispositivos[index-1].deviceName();
}
