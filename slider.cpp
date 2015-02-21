#include "slider.hpp"

Slider::Slider(QWidget *parent) : QSlider(parent) {}


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
