#include "movieplayer.hpp"
#include "qstyle.h"
#include "ui_movieplayer.h"

MoviePlayer::MoviePlayer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MoviePlayer),
    speed(100),
    movie(NULL),
    camara(NULL),
    captureBuffer(NULL),
    label(NULL) {

        ui->setupUi(this);

        crearLabel();

        // Añadir widgets
        ui->toolBarInferior->addWidget(&slider);
        ui->statusBar->addWidget(&velocidad);
        ui->statusBar->addPermanentWidget(&tiempo);

        // Preferencias
        if (preferencias.value("auto-reproduccion").toBool())
            ui->actionAutoReproducir->setChecked(true);

        // Añadir iconos
        ui->actionAbrir->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
        ui->actionCapturarVideo->setIcon(style()->standardIcon(QStyle::SP_DesktopIcon));
        ui->actionCerrar->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
        ui->actionReproducir->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        ui->actionPausar->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        ui->actionDetener->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
        ui->actionRetroceder->setIcon(style()->standardIcon(QStyle::SP_MediaSeekBackward));
        ui->actionAvanzar->setIcon(style()->standardIcon(QStyle::SP_MediaSeekForward));
}


MoviePlayer::~MoviePlayer() {

    delete ui;
    delete label;

    if (movie) {
        delete movie;
        movie = NULL;
    }

    if (camara) {
        delete camara;
        camara = NULL;
    }

    if (captureBuffer) {
        delete captureBuffer;
        captureBuffer = NULL;
    }

    speed = 0;
}


/***************************
 MÉTODOS PRIVADOS
**************************/

void MoviePlayer::crearLabel() {

    if (label) {
        delete label;
        label = NULL;
    }

    label = new QLabel;
    label->setText("Abra un archivo o capture vídeo...");
    label->setAutoFillBackground(true);
    label->setAlignment(Qt::AlignCenter);
    label->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);
    QPalette paleta = this->palette();
    paleta.setColor(QPalette::Background, QColor(90,90,90));
    paleta.setColor(QPalette::WindowText, Qt::white);
    label->setPalette(paleta);
    ui->verticalLayoutPrincipal->addWidget(label);
}


void MoviePlayer::limpiarMovie() {

    if (movie) {
        disconnect(&slider, SIGNAL(valueChanged(int)), this, SLOT(setFrameSlider(int)));
        disconnect(movie, SIGNAL(frameChanged(int)), this, SLOT(updateFrameSlider()));
        disconnect(movie, SIGNAL(updated(const QRect&)), this, SLOT(showFrame()));

        movie->stop();
        delete movie;
        movie = NULL;

        speed = 100;
        slider.setValue(0);
        tiempo.setText("");
        velocidad.setText("");
        this->setWindowTitle(WINDOW_TITLE);
    }

    activarFuncionalidades(false);
    crearLabel();
}


void MoviePlayer::limpiarCamara() {

    if (camara) {
        disconnect(captureBuffer, SIGNAL(imagenChanged(QImage)), this, SLOT(updateImagen(QImage)));
        camara->stop();
        delete camara;
        camara = NULL;
    }

    if (captureBuffer) {
        delete captureBuffer;
        captureBuffer = NULL;
    }
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
    slider.setEnabled(cond);
}


void MoviePlayer::updateVelocidad() {

    movie->setSpeed(speed);
    velocidad.setText("Velocidad: " + QString::number(speed) + "%");
}


/***************************
 SLOTS
**************************/

void MoviePlayer::setFrameSlider(int frame) { movie->jumpToFrame(frame); }


void MoviePlayer::updateFrameSlider() {

    // Comprobar que no es una imagen estática (MJPEG no admite esta propiedad)
    if (movie->currentFrameNumber() >= 0) {
        if (movie->frameCount() > 0)
            slider.setMaximum(movie->frameCount() - 1);
        slider.setValue(movie->currentFrameNumber());
    }

    // Actualizar tiempo
    int total = movie->frameCount() * movie->nextFrameDelay() / 1000;
    int actual = movie->currentFrameNumber() * movie->nextFrameDelay() / 1000;
    tiempo.setText(QString::number(actual) + " / " + QString::number(total)+ "s");
}


void MoviePlayer::showFrame() {

    pixmap = movie->currentPixmap();
    label->setPixmap(pixmap);
}


void MoviePlayer::updateImagen(QImage imagen){

    pixmap = QPixmap(QPixmap::fromImage(imagen.scaled(label->size())));

    // Mostrar hora
    QPainter painter(&pixmap);
    painter.setPen(Qt::green);
    painter.setFont(QFont("",20));
    painter.drawText(pixmap.width()-120,
                     pixmap.height()-20,
                     QTime().currentTime().toString());

    label->setPixmap(pixmap);
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

        // Borrar movie anterior
        on_actionCerrar_triggered();

        // Cargar movie
        movie = new Movie;
        movie->setFileName(ruta);

        if (!movie->isValid()) {
            QMessageBox::critical(this, WINDOW_CRITICAL, "El formato es inválido");
            return;
        }

        // Auto-reproducir
        if (ui->actionAutoReproducir->isChecked())
            movie->start();

        // Ajustes
        this->setWindowTitle(movie->name() + WINDOW_TITLE_OPENED);
        label->setText("");
        updateVelocidad();
        activarFuncionalidades(true);
        on_actionActivarCache_toggled(movie->size() <= MAX_SIZE_CACHED);

        // Conectar movie, slider y label
        connect(&slider, SIGNAL(valueChanged(int)), this, SLOT(setFrameSlider(int)));
        connect(movie, SIGNAL(frameChanged(int)), this, SLOT(updateFrameSlider()));
        connect(movie, SIGNAL(updated(const QRect&)), this, SLOT(showFrame()));
    }
}


void MoviePlayer::on_actionCapturarVideo_triggered() {

    // Borrar camara anterior
    on_actionCerrar_triggered();


    // Abrir camara por defecto o guardada en preferencias
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
        if (!camara) {
            camara = new QCamera;
            preferencias.setValue("dispositivo", "");
        }
    }

    // Si no hay camara
    if (!camara->isAvailable()) {
        QMessageBox::critical(this, WINDOW_CRITICAL, "No existe ningún dispositivo o está ocupado.");
        return;
    }

    // Iniciar captura
    captureBuffer = new CaptureBuffer;
    camara->setViewfinder(captureBuffer);
    camara->start();

    // Ajustes
    ui->actionCerrar->setEnabled(true);
    ui->actionCapturarPantalla->setEnabled(true);
    connect(captureBuffer, SIGNAL(imagenChanged(QImage)), this, SLOT(updateImagen(QImage)));
}


void MoviePlayer::on_actionCerrar_triggered() { limpiarMovie(); limpiarCamara(); }


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

    // Comprobar si existe la aplicación y ejecutar
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

        // Si no existe un archivo con el mismo nombre sobreescribir
        if (!QFile::exists(nuevaRuta)) {

            QFile::rename(movie->path(),nuevaRuta);
            movie->setFileName(nuevaRuta);
            this->setWindowTitle(movie->name() +  WINDOW_TITLE_OPENED);
        }

        else {

            AvisoExistente aviso;

            // Si se desea sobreescribir un archivo con el mismo nombre
            if (aviso.exec() == QMessageBox::Save) {

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
        }
    }
}


void MoviePlayer::on_actionEliminar_triggered() {

    AvisoEliminar aviso;

    // Eliminar del disco si se da el visto bueno
    if (aviso.exec() == QMessageBox::Save) {

        QFile::remove(movie->path());
        limpiarMovie();
    }
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

    // Seleccionar ruta nueva
    QString ruta = QFileDialog::getSaveFileName(this, tr("Guardar captura"), QString(),
            tr("Todos los archivos (*);;Imagen BPM (*.bpm);;Imagen GIF (*.gif);;Imagen JPG (*.jpg);;Imagen JPEG (*.jpeg);;Imagen PNG (*.png);;Imagen PPM (*.ppm);;Imagen XBM (*.xbm);;Imagen XPM (*.xpm);;"));

    if (!ruta.isEmpty()) {

        QFile file(ruta);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, WINDOW_CRITICAL, "No se puede guardar la captura.");
            return;
        }

        // Guardar captura en la ruta indicada
        pixmap.save(ruta,0,60);
    }
}


void MoviePlayer::on_actionAjustarVentana_toggled(bool cond) {

     label->setScaledContents(cond);
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
        on_actionCapturarVideo_triggered();
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

void MoviePlayer::on_actionAcercaDe_triggered() { Acerca w; w.exec(); }

void MoviePlayer::on_actionAcercaDeQt_triggered() { qApp->aboutQt(); }
