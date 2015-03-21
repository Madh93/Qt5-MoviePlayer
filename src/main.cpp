#include "movieplayer.hpp"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MoviePlayer w;
    w.show();

    return a.exec();
}
