#ifndef MOVIE_HPP
#define MOVIE_HPP

#include <QMovie>
#include <QFile>
#include <QFileInfo>


class Movie : public QMovie {

    private:

        QFileInfo *info;

    public:

        Movie(QObject* parent = 0);
        ~Movie();
};

#endif // MOVIE_HPP
