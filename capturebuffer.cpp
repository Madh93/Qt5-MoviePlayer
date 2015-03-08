#include "capturebuffer.hpp"

CaptureBuffer::CaptureBuffer(QObject* parent):
    QAbstractVideoSurface(parent) { }


CaptureBuffer::~CaptureBuffer() { }


QList<QVideoFrame::PixelFormat> CaptureBuffer::supportedPixelFormats(
        QAbstractVideoBuffer::HandleType handleType) const {

    // Devolver formatos compatibles con QImage
    if (handleType != QAbstractVideoBuffer::NoHandle) {
        return QList<QVideoFrame::PixelFormat>();
    }

    else {
        return QList<QVideoFrame::PixelFormat>()
                    << QVideoFrame::Format_ARGB32
                    << QVideoFrame::Format_ARGB32_Premultiplied
                    << QVideoFrame::Format_RGB32
                    << QVideoFrame::Format_RGB24
                    << QVideoFrame::Format_RGB565
                    << QVideoFrame::Format_RGB555;
    }
}


bool CaptureBuffer::present(const QVideoFrame &frame) {

    QList<QVideoFrame::PixelFormat> formatos = supportedPixelFormats();

    if (!formatos.contains(frame.pixelFormat()))
        return false;
    else {

        // Copia del frame
        QVideoFrame f(frame);
        // Permitir copiar del buffer
        f.map(QAbstractVideoBuffer::ReadOnly);
        // Obtener imagen a partir del frame
        QImage imagen = QImage(frame.bits(),
                               f.width(),
                               f.height(),
                               f.bytesPerLine(),
                               QVideoFrame::imageFormatFromPixelFormat(f.pixelFormat()));
        //imagen.copy();
        // Bloquear buffer
        f.unmap();
        // Emitir señal
        emit imagenChanged(imagen);

        return true;
    }
}
