#include "movieplayer.hpp"
#include "ui_movieplayer.h"

MoviePlayer::MoviePlayer(QWidget *parent) : QMainWindow(parent), ui(new Ui::MoviePlayer) {

    ui->setupUi(this);

    speed = 100;
    movie = new Movie;
    camara = NULL;
    viewfinder = NULL;
    slider = new Slider;
    velocidad = new QLabel;
    tiempo = new QLabel;

    ui->toolBarInferior->addWidget(slider);
    ui->statusBar->addWidget(velocidad);
    ui->statusBar->addPermanentWidget(tiempo);

    //Preferencias
    if (preferencias.value("auto-reproduccion").toBool())
        ui->actionAutoReproducir->setChecked(true);

    //Añadir iconos
    ui->labelMovie->setBackgroundRole(QPalette::Dark);
    ui->actionAbrir->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    ui->actionCapturarVideo->setIcon(style()->standardIcon(QStyle::SP_DesktopIcon));
    ui->actionCerrar->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    ui->actionReproducir->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    ui->actionPausar->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
    ui->actionDetener->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    ui->actionRetroceder->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
    ui->actionAvanzar->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));

    //Conectar movie, slider y label
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(setFrameSlider(int)));
    connect(movie, SIGNAL(frameChanged(int)), this, SLOT(updateFrameSlider()));
    connect(movie, SIGNAL(updated(const QRect&)), this, SLOT(showFrame()));
}


MoviePlayer::~MoviePlayer() {

    delete ui;

    if (movie) {
        delete movie;
        movie = NULL;
    }

    if (camara) {
        delete camara;
        camara = NULL;
    }

    if (viewfinder) {
        delete viewfinder;
        viewfinder = NULL;
    }

    if (slider) {
        delete slider;
        slider = NULL;
    }

    if (velocidad) {
        delete velocidad;
        velocidad = NULL;
    }

    if (tiempo) {
        delete tiempo;
        tiempo = NULL;
    }

    speed = 0;
}


/***************************
 MÉTODOS PRIVADOS
**************************/

void MoviePlayer::limpiarMovie() {

    speed = 100;
    movie->stop();
    slider->setValue(0);
    tiempo->setText("");
    this->setWindowTitle(WINDOW_TITLE);
    activarFuncionalidades(false);
}


void MoviePlayer::activarFuncionalidades(bool cond) {

    ui->actionCerrar->setEnabled(cond);
    ui->actionAbrirCon->setEnabled(cond);
    ui->actionAbrirDirectorio->setEnabled(cond);
    ui->actionAbrirTerminal->setEnabled(cond);
    ui->actionRenombrar->setEnabled(cond);
    ui->actionEliminar->setEnabled(cond);
    ui->actionReproducir->setEnabled(cond);
    ui->actionPausar->setEnabled(cond);
    ui->actionDetener->setEnabled(cond);
    ui->actionAvanzar->setEnabled(cond);
    ui->actionRetroceder->setEnabled(cond);
    ui->actionSiguienteFotograma->setEnabled(cond);
    ui->actionAnteriorFotograma->setEnabled(cond);
    ui->menuVelocidad->setEnabled(cond);
    ui->actionCapturarPantalla->setEnabled(cond);
    ui->actionAjustarVentana->setEnabled(cond);
        ui->actionAjustarVentana->setChecked(false);
    slider->setEnabled(cond);
}


/***************************
 SLOTS
**************************/

void MoviePlayer::setFrameSlider(int frame) { movie->jumpToFrame(frame); }


void MoviePlayer::updateFrameSlider() {

    //Comprobar que no es una imagen estática (MJPEG no admite esta propiedad)
    if (movie->currentFrameNumber() >= 0) {
        if (movie->frameCount() > 0)
            slider->setMaximum(movie->frameCount() - 1);
        slider->setValue(movie->currentFrameNumber());
    }

    //Actualizar tiempo
    int total = movie->frameCount() * movie->nextFrameDelay() / 1000;
    int actual = movie->currentFrameNumber() * movie->nextFrameDelay() / 1000;
    tiempo->setText(QString::number(actual) + " / " + QString::number(total)+ "s");
}


void MoviePlayer::showFrame() {

    QPixmap pixmap = movie->currentPixmap();
    ui->labelMovie->setPixmap(pixmap);
}


void MoviePlayer::updateVelocidad() {

    movie->setSpeed(speed);
    velocidad->setText("Velocidad: " + QString::number(speed) + "%");
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

        if (!movie->isValid()) {
            QMessageBox::critical(this, WINDOW_CRITICAL, "El formato es inválido");
            return;
        }

        //Auto-reproducir
        if (ui->actionAutoReproducir->isChecked())
            movie->start();

        //Ajustes
        this->setWindowTitle(movie->name() + WINDOW_TITLE_OPENED);
        updateVelocidad();
        activarFuncionalidades(true);
        on_actionActivarCache_toggled(movie->size() <= MAX_SIZE_CACHED);
    }
}


void MoviePlayer::on_actionCapturarVideo_triggered() {

    qDebug() << "ENTRA";
    // Borrar camara anterior
    if (camara) {
        delete camara;
        camara = NULL;
    }

    qDebug() << "ENTRA2";
    if (viewfinder) {
        delete viewfinder;
        viewfinder = NULL;
    }

    // Abrir camara por defecto o guardad en preferencias
    QString ruta = preferencias.value("dispositivo").toString();

    if (ruta.isEmpty()) {
        camara = new QCamera;
        preferencias.setValue("dispositivo", "");
    }

    else {
        QList<QCameraInfo> dispositivos = QCameraInfo::availableCameras();
        foreach (const QCameraInfo &disp, dispositivos)
                if (disp.deviceName() == ruta) {
                    camara = new QCamera(disp);
                    break;
                }
        // Si la camara ya no existe
        if (camara == NULL) {
            camara = new QCamera;
            preferencias.setValue("dispositivo", "");
        }
    }

    // Si no hay camara (COMPROBAR)
    if (camara == NULL)
        return;

    viewfinder = new QCameraViewfinder;
    viewfinder->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    setCentralWidget(viewfinder);
    camara->setViewfinder(viewfinder);
    camara->start();
}


void MoviePlayer::on_actionCerrar_triggered() { limpiarMovie(); }


void MoviePlayer::on_actionSalir_triggered() { qApp->quit(); }


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

void MoviePlayer::on_actionPausar_triggered() { movie->setPaused(movie->state() == QMovie::Running); }

void MoviePlayer::on_actionDetener_triggered() { movie->stop(); }

void MoviePlayer::on_actionAvanzar_triggered() {

    if (movie->frameCount() > 0)
        movie->jumpToFrame(movie->currentFrameNumber() + (1000/movie->nextFrameDelay()));
}

void MoviePlayer::on_actionRetroceder_triggered() {

    if (movie->frameCount() > 0)
        movie->jumpToFrame(movie->currentFrameNumber() - (1000/movie->nextFrameDelay()));
}

void MoviePlayer::on_actionSiguienteFotograma_triggered() { movie->jumpToNextFrame(); }

void MoviePlayer::on_actionAnteriorFotograma_triggered() {

    movie->jumpToFrame(movie->currentFrameNumber()-1);
}

void MoviePlayer::on_actionNormal_triggered() { speed=100; updateVelocidad(); }

void MoviePlayer::on_actionIncrementar_triggered() { speed+=10; updateVelocidad(); }

void MoviePlayer::on_actionReducir_triggered() { speed-=10; updateVelocidad(); }

void MoviePlayer::on_actionDoblar_triggered() { speed*=2; updateVelocidad(); }

void MoviePlayer::on_actionMitad_triggered() { speed/=2; updateVelocidad(); }


/***************************
 HERRAMIENTAS
**************************/

void MoviePlayer::on_actionCapturarPantalla_triggered() {

    QPixmap captura = movie->currentPixmap();

    //Seleccionar ruta nueva
    QString ruta = QFileDialog::getSaveFileName(this, tr("Guardar captura"), QString(),
            tr("Todos los archivos (*);;Imagen BPM (*.bpm);;Imagen GIF (*.gif);;Imagen JPG (*.jpg);;Imagen JPEG (*.jpeg);;Imagen PNG (*.png);;Imagen PPM (*.ppm);;Imagen XBM (*.xbm);;Imagen XPM (*.xpm);;"));

    if (!ruta.isEmpty()) {

        QFile file(ruta);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, WINDOW_CRITICAL, "No se puede guardar la captura.");
            return;
        }

        //Guardar captura en la ruta indicada
        captura.save(ruta,0,60);
    }
}


void MoviePlayer::on_actionAjustarVentana_toggled(bool cond) {

     ui->labelMovie->setScaledContents(cond);
}


void MoviePlayer::on_actionActivarCache_toggled(bool cond) {

    if (cond)
        movie->setCacheMode(QMovie::CacheAll);
    else
        movie->setCacheMode(QMovie::CacheNone);

    ui->actionActivarCache->setChecked(cond);
}


/***************************
 PREFERENCIAS
**************************/

void MoviePlayer::on_actionDispositivos_triggered() {

    Dispositivos w(preferencias.value("dispositivo").toString());

    if (w.exec() == QDialog::Accepted) {
        preferencias.setValue("dispositivo", w.getDispositivo());
    }
}

void MoviePlayer::on_actionAutoReproducir_toggled(bool cond) {

    preferencias.setValue("auto-reproduccion", cond);
}


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

void MoviePlayer::on_actionAcercaDeQt_triggered() { qApp->aboutQt(); }
