#include "dispositivos.hpp"
#include "ui_dispositivos.h"

Dispositivos::Dispositivos(QString current) :
    QDialog(0),
    ui(new Ui::Dispositivos) {

        ui->setupUi(this);

        actual = current;
        dispositivos = QCameraInfo::availableCameras();
        ui->comboBox->setCurrentIndex(0);
        setDispositivos();
}


Dispositivos::~Dispositivos() {
    delete ui;
}


void Dispositivos::setDispositivos() {

    //Añadir dispositivos
    ui->comboBox->addItem("Predeterminado");
    if (dispositivos.size() > 0)
        foreach (const QCameraInfo &camara, dispositivos)
            ui->comboBox->addItem(camara.description());

    //Seleccionar por defecto
    if (actual.isEmpty())
        ui->comboBox->setCurrentIndex(0);
    else
        for (int i=0; i<dispositivos.size(); i++)
            if (dispositivos[i].deviceName() == actual) {
                ui->comboBox->setCurrentIndex(i+1);
                break;
            }
}


QString Dispositivos::getDispositivo() {

    int index = ui->comboBox->currentIndex();

    if (index == 0)
        return "";
    else
        return dispositivos[index-1].deviceName();
}
