#ifndef COLORLABEL_H
#define COLORLABEL_H

#include <QLabel>
#include <QColorDialog>
#include <QPainter>

class ColorLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ColorLabel(QWidget* parent = 0,
                        Qt::WindowFlags f = 0);
    QColor getColor();
    int setColor(QColor color);


protected:
    void mouseReleaseEvent(QMouseEvent* event);
    void paintEvent(QPaintEvent* event);

private:
    QColor mColor;
};

#endif // COLORLABEL_H
