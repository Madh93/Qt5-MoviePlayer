#ifndef ACERCA_HPP
#define ACERCA_HPP

#include <QDialog>

namespace Ui {
class Acerca;
}

class Acerca : public QDialog
{
        Q_OBJECT

    public:
        explicit Acerca(QWidget *parent = 0);
        ~Acerca();

    private:
        Ui::Acerca *ui;
};

#endif // ACERCA_HPP
