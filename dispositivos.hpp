#ifndef DISPOSITIVOS_HPP
#define DISPOSITIVOS_HPP

#include <QDialog>
#include <QCamera>
#include <QCameraViewfinder>
#include <QCameraInfo>

namespace Ui {
    class Dispositivos;
}

class Dispositivos : public QDialog {

    Q_OBJECT

    private:

        Ui::Dispositivos *ui;
        QList<QCameraInfo> dispositivos;

        void setDispositivos();

    public:

        explicit Dispositivos(QWidget *parent = 0);
        ~Dispositivos();

        QString getDispositivo();
};

#endif // DISPOSITIVOS_HPP
