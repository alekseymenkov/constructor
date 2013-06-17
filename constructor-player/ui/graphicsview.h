#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H



#include <QGraphicsView>
#include <QtSvg>



class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    GraphicsView(QGraphicsScene* scene, QWidget* parent = 0);
    // Загрузка фонового изображения
    int loadImage(QString imagePath);
    // Значение модификатора масштаба
    int scaleFactor();
    // Центрирование области просмотра
    int centerView();
    int setPermanentScaleFactor(int scaleFactor);


protected:
    virtual void wheelEvent(QWheelEvent *event);
    virtual void drawBackground(QPainter *painter, const QRectF &rect);
    virtual void resizeEvent(QResizeEvent *event);


private:
    // Константы
    static const int maxZoom = 25;
    static const int minZoom = -25;
    static const qreal zoomValue = 1.1;
    // Модуль, отвечающий за отрисовку SVG
    QSvgRenderer* mRenderer;
    // Модификатор масштаба
    int mScaleFactor;
    int mPermanentScaleFactor;
    // Кешируемые значения (размер области просмотра до изменения ее размера)
    QSize mViewportSize;


public slots:
    // Слот - "Вписать в окно"
    int slotFitInView();
    // Слот - "Установить модификатор масштаба"
    int slotSetScaleFactor(int newScaleFactor);


signals:
    // Сигнал - "Установить модификатор масштаба"
    int signalSetScaleFactor(int scaleFactor);
    // Сигнал - "Масштабирование колесом мыши"
    int signalWheelEvent();
};



#endif // GRAPHICSVIEW_H
