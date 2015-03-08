#ifndef CAPTUREBUFFER_HPP
#define CAPTUREBUFFER_HPP

#include <QAbstractVideoSurface>
#include <QDebug>

class CaptureBuffer : public QAbstractVideoSurface {

    Q_OBJECT

    signals:

        void imagenChanged(QImage);

    public:

        CaptureBuffer(QObject *parent = 0);
        ~CaptureBuffer();

        bool present(const QVideoFrame &frame);
        QList<QVideoFrame::PixelFormat> supportedPixelFormats(
                QAbstractVideoBuffer::HandleType handleType =
                QAbstractVideoBuffer::NoHandle) const;
};

#endif // CAPTUREBUFFER_HPP
