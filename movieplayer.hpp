#ifndef MOVIEPLAYER_HPP
#define MOVIEPLAYER_HPP

#include <QMainWindow>

namespace Ui {
class MoviePlayer;
}

class MoviePlayer : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MoviePlayer(QWidget *parent = 0);
        ~MoviePlayer();

    private:
        Ui::MoviePlayer *ui;
};

#endif // MOVIEPLAYER_HPP
