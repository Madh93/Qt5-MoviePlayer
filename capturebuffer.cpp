#include "capturebuffer.hpp"

CaptureBuffer::CaptureBuffer(QObject* parent):
    QAbstractVideoSurface(parent) {

}


CaptureBuffer::~CaptureBuffer() {

}


bool CaptureBuffer::present(const QVideoFrame &frame) {

    // Presents a video frame.
    // Returns true if the frame was presented, and false if an error occurred.

    return true;
}


QList<QVideoFrame::PixelFormat> CaptureBuffer::supportedPixelFormats(
        QAbstractVideoBuffer::HandleType handleType) const {

    // Devolver formatos compatibles con QImage

    QList<QVideoFrame::PixelFormat> formatos;

    formatos << QVideoFrame::Format_ARGB32;
    formatos << QVideoFrame::Format_ARGB32_Premultiplied;
    formatos << QVideoFrame::Format_RGB32;
    formatos << QVideoFrame::Format_RGB24;
    formatos << QVideoFrame::Format_RGB565;
    formatos << QVideoFrame::Format_RGB555;

    return formatos;
}
