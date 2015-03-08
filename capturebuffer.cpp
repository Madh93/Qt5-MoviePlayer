#include "capturebuffer.hpp"

CaptureBuffer::CaptureBuffer(QWidget *widget, QObject* parent):
    //widget(widget),
    QAbstractVideoSurface(parent) {

        mylabel = new QLabel;
}


CaptureBuffer::~CaptureBuffer() {

    delete mylabel;

}


bool CaptureBuffer::present(const QVideoFrame &frame) {

    // Presents a video frame.
    // Returns true if the frame was presented, and false if an error occurred.

//    if (notMyFormat(frame.pixelFormat())) {
//        setError(IncorrectFormatError);
//        return false;
//    } else {

        QVideoFrame frametodraw(frame);

        if(!frametodraw.map(QAbstractVideoBuffer::ReadOnly))
        {
           setError(ResourceError);
           return false;
        }

         //this is a shallow operation. it just refer the frame buffer
         QImage image(
                frametodraw.bits(),
                frametodraw.width(),
                frametodraw.height(),
                frametodraw.bytesPerLine(),
                QImage::Format_RGB444);

        mylabel->resize(image.size());

        //QPixmap::fromImage create a new buffer for the pixmap
        mylabel->setPixmap(QPixmap::fromImage(image));

        //we can release the data
        frametodraw.unmap();

        mylabel->update();

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
