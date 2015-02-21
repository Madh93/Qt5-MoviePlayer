#ifndef MOVIEPLAYER_HPP
#define MOVIEPLAYER_HPP

#include <QMainWindow>
#include <QFileDialog>
#include <QInputDialog>
#include <QProcess>
#include <QPixmap>

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
        int speed;
        Movie *movie;
        QLabel *velocidad, *tiempo;

        void limpiarMovie();
        void activarFuncionalidades(bool cond);

    private slots:

        void pausar();
        void updateFrameSlider();
        void setFrameSlider(int frame);
        void showFrame();
        void updateVelocidad();

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
        void on_actionAvanzar_triggered();
        void on_actionRetroceder_triggered();
        void on_actionSiguienteFotograma_triggered();
        void on_actionAnteriorFotograma_triggered();
        void on_actionNormal_triggered();
        void on_actionIncrementar_triggered();
        void on_actionReducir_triggered();
        void on_actionDoblar_triggered();
        void on_actionMitad_triggered();

        //Herramientas
        void on_actionAjustarVentana_toggled(bool cond);
        void on_actionActivarCache_toggled(bool cond);

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
