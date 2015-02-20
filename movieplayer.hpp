#ifndef MOVIEPLAYER_HPP
#define MOVIEPLAYER_HPP

#include <QMainWindow>

namespace Ui {
    class MoviePlayer;
}

class MoviePlayer : public QMainWindow {

    Q_OBJECT

    private:

        Ui::MoviePlayer *ui;

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
