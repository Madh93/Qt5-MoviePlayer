#ifndef BASE
#define BASE

#include <QDebug>
#include <QMessageBox>

#define MAX_SIZE_CACHED 5242880


// TÍTULOS DE VENTANA

#define WINDOW_TITLE "Movie Player"
#define WINDOW_TITLE_OPENED " - Movie Player"
#define WINDOW_CRITICAL "Error - Movie Player"
#define WINDOW_WARNING "Aviso - Movie Player"
#define WINDOW_ABOUT "Acerca de - Movie Player"


// AVISOS DE CONFIRMACIÓN

class AvisoEliminar : public QMessageBox {

    public:

        AvisoEliminar(QWidget *parent = 0) : QMessageBox(parent) {

            this->setWindowTitle(WINDOW_WARNING);
            this->setText("Está a punto de eliminar el archivo del disco.");
            this->setInformativeText("¿Desea eliminar el archivo?");
            this->setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
            this->setButtonText(QMessageBox::Save, "Eliminar");
            this->setButtonText(QMessageBox::Cancel, "Cancelar");
            this->setDefaultButton(QMessageBox::Cancel);
        }

        ~AvisoEliminar() {}
};


class AvisoExistente : public QMessageBox {

    public:

        AvisoExistente(QWidget *parent = 0) : QMessageBox(parent) {

            this->setWindowTitle(WINDOW_WARNING);
            this->setText("Ya existe un archivo con el mismo nombre.");
            this->setInformativeText("¿Desea sobreescribir el archivo?");
            this->setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
            this->setButtonText(QMessageBox::Save, "Renombrar");
            this->setButtonText(QMessageBox::Cancel, "Cancelar");
            this->setDefaultButton(QMessageBox::Cancel);
        }

        ~AvisoExistente() {}
};


#endif // BASE
