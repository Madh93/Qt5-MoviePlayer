#include "slider.hpp"

Slider::Slider(QWidget *parent) : QSlider(parent) {

    setEnabled(false);
    setMaximum(100);
    setPageStep(1);
    setOrientation(Qt::Horizontal);
    setTickPosition(Slider::TicksBelow);
}


Slider::~Slider() {}


void Slider::mousePressEvent(QMouseEvent *e) {

    if (e->button() == Qt::LeftButton) {

        if (orientation() == Qt::Vertical)
            setValue(minimum() + ((maximum()-minimum()) * (height()-e->y())) / height());
        else
            setValue(minimum() + ((maximum()-minimum()) * e->x()) / width());

        e->accept();
    }

    QSlider::mousePressEvent(e);
}
