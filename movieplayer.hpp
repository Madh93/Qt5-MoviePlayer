#ifndef MOVIEPLAYER_HPP
#define MOVIEPLAYER_HPP

#define WINDOW_TITLE "Movie Player"
#define WINDOW_CRITICAL "Error - Movie Player"
#define WINDOW_WARNING "Aviso - Movie Player"
#define WINDOW_ABOUT "Acerca de - Movie Player"

#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QMovie>

namespace Ui {
    class MoviePlayer;
}

class MoviePlayer : public QMainWindow {

    Q_OBJECT

    private:

        Ui::MoviePlayer *ui;
        QMovie *video;

    private slots:

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
