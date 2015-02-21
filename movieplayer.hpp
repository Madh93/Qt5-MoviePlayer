#ifndef MOVIEPLAYER_HPP
#define MOVIEPLAYER_HPP

#include <QMainWindow>
#include <QFileDialog>
#include <QInputDialog>
#include <QProcess>

#include "base.hpp"
#include "movie.hpp"
#include "slider.hpp"

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
        void updateFrameSlider();
        void setFrameSlider(int frame);

        //Archivo
        void on_actionAbrir_triggered();
        void on_actionAbrirRecientes_triggered();   //PENDIENTE
        void on_actionGuardarComo_triggered();      //CORREGIR
        void on_actionCerrar_triggered();
        void on_actionSalir_triggered();

        //Editar
        void on_actionAbrirCon_triggered();
        void on_actionAbrirDirectorio_triggered();
        void on_actionAbrirTerminal_triggered();
        void on_actionRenombrar_triggered();
        void on_actionEliminar_triggered();

        //Reproducción
        void on_actionReproducir_triggered();
        void on_actionPausar_triggered();
        void on_actionDetener_triggered();

        //Herramientas

        //Preferencias
        void on_actionPantallaCompleta_toggled(bool cond);

        //Ayuda
        void on_actionAyuda_triggered();
        void on_actionAcercaDe_triggered();
        void on_actionAcercaDeQt_triggered();

    public:

        explicit MoviePlayer(QWidget *parent = 0);
        ~MoviePlayer();
};

#endif // MOVIEPLAYER_HPP
