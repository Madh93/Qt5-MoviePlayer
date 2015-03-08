#ifndef CAPTUREBUFFER_HPP
#define CAPTUREBUFFER_HPP

#include <QAbstractVideoSurface>

class CaptureBuffer : public QAbstractVideoSurface {

    Q_OBJECT

    public:
        CaptureBuffer(QObject *parent = 0);
        ~CaptureBuffer();
};

#endif // CAPTUREBUFFER_HPP
