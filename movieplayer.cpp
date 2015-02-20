#include "movieplayer.hpp"
#include "ui_movieplayer.h"

MoviePlayer::MoviePlayer(QWidget *parent) : QMainWindow(parent), ui(new Ui::MoviePlayer) {

    ui->setupUi(this);

    movie = new Movie;

    //Señales y slots de los botones
    connect(ui->buttonAbrir, SIGNAL(clicked()), this, SLOT(on_actionAbrir_triggered()));
    connect(ui->buttonCerrar, SIGNAL(clicked()), this, SLOT(on_actionCerrar_triggered()));
    connect(ui->buttonPlay, SIGNAL(clicked()), movie, SLOT(start()));
    connect(ui->buttonPausa, SIGNAL(clicked()), this, SLOT(pausar()));
    connect(ui->buttonStop, SIGNAL(clicked()), movie, SLOT(stop()));
}


MoviePlayer::~MoviePlayer() {

    delete ui;

    if (movie) {
        delete movie;
        movie = NULL;
    }
}


/***************************
 MÉTODOS PRIVADOS
**************************/

void MoviePlayer::limpiarMovie() {
/*
    if (movie) {
        delete movie;
        movie = new Movie;
    }
*/
    movie->stop();
    this->setWindowTitle(WINDOW_TITLE);
    activarFuncionalidades(false);
}


void MoviePlayer::activarFuncionalidades(bool cond) {

    //ui->actionGuardarComo->setEnabled(cond);
    ui->actionCerrar->setEnabled(cond);
    ui->actionAbrirCon->setEnabled(cond);
    ui->actionAbrirDirectorio->setEnabled(cond);
    ui->actionAbrirTerminal->setEnabled(cond);
    ui->actionRenombrar->setEnabled(cond);
    ui->actionEliminar->setEnabled(cond);
    ui->buttonCerrar->setEnabled(cond);
    ui->buttonPlay->setEnabled(cond);
    ui->buttonPausa->setEnabled(cond);
    ui->buttonStop->setEnabled(cond);
}


/***************************
 SLOTS
**************************/

void MoviePlayer::pausar() {

    movie->setPaused(movie->state() == QMovie::Running);
}


/***************************
 ARCHIVO
**************************/

void MoviePlayer::on_actionAbrir_triggered() {

    QString ruta = QFileDialog::getOpenFileName(this, "Abrir archivo", QString(),
                                                "Todos los archivos (*);;Imagen GIF (*.gif);;Imagen MNG (*.mng);;Vídeo MJPEG (*.mjpeg);;");

    if (!ruta.isEmpty()) {

        QFile file(ruta);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, WINDOW_CRITICAL,"No se puede abrir el archivo.");
            return;
        }

        //Borrar movie anterior
        limpiarMovie();

        //Cargar movie
        movie->setFileName(ruta);
        ui->labelMovie->setMovie(movie);

        if (!movie->isValid()) {
            QMessageBox::critical(this, WINDOW_CRITICAL, "El formato es inválido");
            return;
        }

        //movie->start();

        //Ajustes
        this->setWindowTitle(movie->name() + WINDOW_TITLE_OPENED);
        activarFuncionalidades(true);
    }

}


void MoviePlayer::on_actionAbrirRecientes_triggered() { }


void MoviePlayer::on_actionGuardarComo_triggered() {

    //Seleccionar ruta nueva
    QString rutaNueva = QFileDialog::getSaveFileName(this, "Guardar archivo", QString(),
            "Todos los archivos (*);;Imagen GIF (*.gif);;Imagen MNG (*.mng);;Vídeo MJPEG (*.mjpeg);;");

    if (!rutaNueva.isEmpty()) {

        QFile file(rutaNueva);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, WINDOW_CRITICAL, "No se puede guardar el archivo.");
            return;
        }

        //Guardar movie en la nueva ruta
        //imagen->save(rutaNueva);
    }
}


void MoviePlayer::on_actionCerrar_triggered() {

    limpiarMovie();
}


void MoviePlayer::on_actionSalir_triggered() {

    qApp->quit();
}


/***************************
 EDITAR
**************************/

void MoviePlayer::on_actionAbrirCon_triggered() {

    QInputDialog inputAbrir;
    QString aplicacion, rutaAplicacion;
    bool okButton;

    aplicacion = inputAbrir.getText(NULL ,"Abrir archivo con...","Introduzca el nombre de la aplicación:",
                                          QLineEdit::Normal,"Ej: gimp", &okButton);

    rutaAplicacion = "/usr/bin/" + aplicacion;

    //Comprobar si existe la aplicación y ejecutar
    if (okButton && !aplicacion.isEmpty()){

        if (QFile::exists(rutaAplicacion) || QFile::exists(aplicacion)) {
            QProcess *proceso = new QProcess(this);
            proceso->start(aplicacion, QStringList() << movie->path());
        }
        else
            QMessageBox::warning(this, WINDOW_WARNING, "No se encuentra la aplicación.");
    }
}


void MoviePlayer::on_actionAbrirDirectorio_triggered() {

    QProcess *proceso = new QProcess(this);
    proceso->start("xdg-open", QStringList() << movie->directory());
}


void MoviePlayer::on_actionAbrirTerminal_triggered() {

    QProcess *proceso = new QProcess(this);
    proceso->setWorkingDirectory(movie->directory());
    proceso->start("x-terminal-emulator", QStringList() << movie->directory());
}


void MoviePlayer::on_actionRenombrar_triggered() {

    QInputDialog inputRenombrar;
    QString nuevoNombre, nuevaRuta;
    bool okButton;

    nuevoNombre = inputRenombrar.getText(NULL ,"Renombrar imagen","Introduzca un nuevo nombre:",
                                          QLineEdit::Normal,movie->baseName(), &okButton);

    if (okButton && !nuevoNombre.isEmpty()){

        nuevaRuta = movie->directory() + nuevoNombre + movie->extension();

        //Si no existe un archivo con el mismo nombre sobreescribir
        if (!QFile::exists(nuevaRuta)) {

            QFile::rename(movie->path(),nuevaRuta);
            movie->setFileName(nuevaRuta);
            this->setWindowTitle(movie->name() +  WINDOW_TITLE_OPENED);
        }

        else {

            AvisoExistente *aviso = new AvisoExistente;

            //Si se desea sobreescribir un archivo con el mismo nombre
            if (aviso->exec() == QMessageBox::Save) {

                QFile fileNuevo(nuevaRuta);
                if (!fileNuevo.open(QIODevice::WriteOnly)) {
                    QMessageBox::critical(this, WINDOW_CRITICAL, "No se puede renombrar el archivo.");
                    return;
                }

                QFile::remove(nuevaRuta);
                QFile::rename(movie->path(),nuevaRuta);
                movie->setFileName(nuevaRuta);
                this->setWindowTitle(movie->name() + WINDOW_TITLE_OPENED);
            }

            delete aviso;
            aviso = NULL;
        }
    }
}


void MoviePlayer::on_actionEliminar_triggered() {

    AvisoEliminar *aviso = new AvisoEliminar;

    //Eliminar del disco si se da el visto bueno
    if (aviso->exec() == QMessageBox::Save) {

        QFile::remove(movie->path());
        limpiarMovie();
    }

    delete aviso;
    aviso = NULL;
}


/***************************
 REPRODUCCIÓN
**************************/

void MoviePlayer::on_actionReproducir_triggered() { movie->start(); }

void MoviePlayer::on_actionPausar_triggered() { pausar(); }

void MoviePlayer::on_actionDetener_triggered() { movie->stop(); }


/***************************
 HERRAMIENTAS
**************************/



/***************************
 PREFERENCIAS
**************************/

void MoviePlayer::on_actionPantallaCompleta_toggled(bool cond) {

    if (cond)
        this->showFullScreen();
    else
        this->showMaximized();
}


/***************************
 AYUDA
**************************/

void MoviePlayer::on_actionAyuda_triggered() {

    QMessageBox::warning(this, WINDOW_WARNING, "La documentación no está disponible actualmente.");
}

void MoviePlayer::on_actionAcercaDe_triggered() {

   QMessageBox::about(this, WINDOW_ABOUT, tr("\t       Movie Player"
                                                 "\n\n             Sistemas Operativos Avanzados"
                                                 "\n          Práctica 2. Estado del visor de MJPEG"
                                                 "\n\nElaborado por: Miguel Ángel Delgado Hernández"));
}

void MoviePlayer::on_actionAcercaDeQt_triggered() {

    qApp->aboutQt();
}
