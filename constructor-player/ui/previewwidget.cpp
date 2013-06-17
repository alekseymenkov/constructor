#include "previewwidget.h"



PreviewWidget::PreviewWidget(QWidget *parent) :
    QWidget(parent)
{
}



int PreviewWidget::setPreviewImagePath(QString previewImagePath)
{
    mPreviewImagePath = previewImagePath;

    QImage tempImage = QImage(mPreviewImagePath);

    if (tempImage.isNull())
        return -1;

    if (tempImage.width() == 0 || tempImage.height() == 0)
        return -1;

    this->setFixedHeight(this->width() * 2);

    if (tempImage.height() / tempImage.width() > 2)
        tempImage = tempImage.copy(0, 0, tempImage.width(), tempImage.width() * 2);

    mPreviewImage = tempImage.scaled(this->size(),
                                     Qt::KeepAspectRatio,
                                     Qt::SmoothTransformation);

    this->setFixedHeight(mPreviewImage.height());

    return 0;
}



void PreviewWidget::paintEvent(QPaintEvent* event)
{
    if (mPreviewImage.isNull())
        return QWidget::paintEvent(event);

    QPainter painter(this);

    painter.drawImage(0, 0, mPreviewImage);

    painter.setPen(QPen(Qt::lightGray, 2));
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(1, 1, mPreviewImage.width() - 2, mPreviewImage.height() - 2);

    return QWidget::paintEvent(event);
}
