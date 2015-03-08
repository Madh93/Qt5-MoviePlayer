#ifndef CAPTUREBUFFER_HPP
#define CAPTUREBUFFER_HPP

#include <QAbstractVideoSurface>
#include <QLabel>

class CaptureBuffer : public QAbstractVideoSurface {

    Q_OBJECT

    private:

        QWidget *widget;
        QLabel *mylabel;

    public:

        CaptureBuffer(QWidget *widget = 0, QObject *parent = 0);
        ~CaptureBuffer();

        bool present(const QVideoFrame &frame);
        QList<QVideoFrame::PixelFormat> supportedPixelFormats(
                QAbstractVideoBuffer::HandleType handleType =
                QAbstractVideoBuffer::NoHandle) const;

        QWidget * getWidget() { return widget; }
        QLabel * getLabel() { return mylabel; }
};

#endif // CAPTUREBUFFER_HPP
