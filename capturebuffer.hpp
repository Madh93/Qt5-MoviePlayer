#ifndef CAPTUREBUFFER_HPP
#define CAPTUREBUFFER_HPP

#include <QAbstractVideoSurface>

class CaptureBuffer : public QAbstractVideoSurface {

    Q_OBJECT

    public:

        CaptureBuffer(QObject *parent = 0);
        ~CaptureBuffer();

        bool present(const QVideoFrame &frame);
        QList<QVideoFrame::PixelFormat> supportedPixelFormats(
                QAbstractVideoBuffer::HandleType handleType =
                QAbstractVideoBuffer::NoHandle) const;
};

#endif // CAPTUREBUFFER_HPP
