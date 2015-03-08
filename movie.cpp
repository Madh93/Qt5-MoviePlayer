#include "movie.hpp"

Movie::Movie(QObject *parent) :
    QMovie(parent),
    info(NULL) { }


Movie::~Movie() {

    if (info) {
        delete info;
        info = NULL;
    }
}


void Movie::setFileName(const QString &ruta){

    this->QMovie::setFileName(ruta);

    QFile file(ruta);
    info = new QFileInfo(file.fileName());
}


QString Movie::path() { return info->absoluteFilePath(); }

QString Movie::name() { return info->fileName(); }

QString Movie::baseName() { return info->baseName(); }

QString Movie::extension() { return "." + info->suffix(); }

QString Movie::directory() { return info->path() + "/"; }

qint64 Movie::size() { return info->size(); }
