#include "colorlabel.h"

ColorLabel::ColorLabel(QWidget* parent, Qt::WindowFlags f) :
    QLabel(parent, f),
    mColor(Qt::white)
{
    this->setAutoFillBackground(true);
}



QColor ColorLabel::getColor()
{
    return mColor;
}



int ColorLabel::setColor(QColor color)
{
    mColor = color;
    this->setPalette(QPalette(mColor));
    return 0;
}



void ColorLabel::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    mColor = QColorDialog::getColor(mColor, this);

    if (mColor.isValid())
        this->setPalette(QPalette(mColor));

    return;
}



void ColorLabel::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::lightGray));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(0, 0, this->width() - 1, this->height() - 1);

    return QLabel::paintEvent(event);
}
