#ifndef SLIDER_HPP
#define SLIDER_HPP

#include <QSlider>
#include <QMouseEvent>

class Slider : public QSlider {

    public:

        Slider(QWidget *parent = 0);
        ~Slider();

    protected:

        virtual void mousePressEvent(QMouseEvent *e);
};

#endif // SLIDER_HPP
