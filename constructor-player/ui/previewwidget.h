#ifndef PREVIEWWIDGET_H
#define PREVIEWWIDGET_H

#include <QtGui>

class PreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PreviewWidget(QWidget *parent = 0);
    int setPreviewImagePath(QString previewImagePath);

protected:
    virtual void paintEvent(QPaintEvent*event);

private:
    QString mPreviewImagePath;
    QImage mPreviewImage;
    
};

#endif // PREVIEWWIDGET_H
