#ifndef ACERCA_HPP
#define ACERCA_HPP

#include <QDialog>

namespace Ui {
    class Acerca;
}

class Acerca : public QDialog {

    Q_OBJECT


    private:
        Ui::Acerca *ui;

    public:
        explicit Acerca(QWidget *parent = 0);
        ~Acerca();
};

#endif // ACERCA_HPP
