#ifndef MOVIEPLAYER_HPP
#define MOVIEPLAYER_HPP

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>

#include "base.hpp"
#include "movie.hpp"

namespace Ui {
    class MoviePlayer;
}

class MoviePlayer : public QMainWindow {

    Q_OBJECT

    private:

        Ui::MoviePlayer *ui;
        Movie *movie;

        void limpiarMovie();
        void activarFuncionalidades(bool cond);

    private slots:

        void pausar();

        //Archivo
        void on_actionAbrir_triggered();
        void on_actionAbrirRecientes_triggered();
        void on_actionGuardarComo_triggered();
        void on_actionCerrar_triggered();
        void on_actionSalir_triggered();

    public:

        explicit MoviePlayer(QWidget *parent = 0);
        ~MoviePlayer();
};

#endif // MOVIEPLAYER_HPP
