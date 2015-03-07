#ifndef DISPOSITIVOS_HPP
#define DISPOSITIVOS_HPP

#include <QDialog>

namespace Ui {
    class Dispositivos;
}

class Dispositivos : public QDialog {

    Q_OBJECT

    private:
        Ui::Dispositivos *ui;

    public:
        explicit Dispositivos(QWidget *parent = 0);
        ~Dispositivos();
};

#endif // DISPOSITIVOS_HPP
