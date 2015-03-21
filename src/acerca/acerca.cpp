#include "acerca.hpp"
#include "ui_acerca.h"

Acerca::Acerca(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Acerca) {

        ui->setupUi(this);
}


Acerca::~Acerca() {

    delete ui;
}
