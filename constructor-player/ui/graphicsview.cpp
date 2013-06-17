#include "graphicsview.h"



GraphicsView::GraphicsView(QGraphicsScene* scene, QWidget* parent) :
    QGraphicsView(scene, parent),
    mScaleFactor(0),
    mPermanentScaleFactor(100)
{
    // Антиалиасинг графики и текста
    this->setRenderHint(QPainter::Antialiasing, true);
    this->setRenderHint(QPainter::TextAntialiasing, true);
    // Параметры кеширования фонового изображения
    this->setCacheMode(QGraphicsView::CacheBackground);
    // Режим скроллинга
    this->setDragMode(QGraphicsView::ScrollHandDrag);
    // Параметры отображения скроллбаров
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    // Параметры индексирования
    this->setFocusPolicy(Qt::NoFocus);
    // Реднерер SVG-изображения
    mRenderer = new QSvgRenderer(this);
    // Установка размеров окна просмотра
    mViewportSize = QSize(scene->width(), scene->height());
}



// Масштабирование колесом мыши
void GraphicsView::wheelEvent(QWheelEvent* event)
{
    // Скрытие всплывающей подсказки
    emit signalWheelEvent();

    if (event->modifiers() != Qt::ControlModifier)
    {
        QGraphicsView::wheelEvent(event);
        return;
    }

    int numSteps = event->delta() / 8 / 15;
    if (numSteps > 0)
    {
        if (mScaleFactor < maxZoom)
        {
            mScaleFactor++;
            this->scale(zoomValue, zoomValue);
        }
    }
    else
    {
        if (mScaleFactor > minZoom)
        {
            mScaleFactor--;
            this->scale(1 / zoomValue, 1 / zoomValue);
        }
    }

    emit signalSetScaleFactor(mScaleFactor);

    return;
}



// Отрисовка фонового изображения
void GraphicsView::drawBackground(QPainter* painter, const QRectF& rect)
{
    Q_UNUSED(rect)

    if (mRenderer->isValid())
        mRenderer->render(painter, this->sceneRect());
    return;
}



// Процедура может входить в рекурсию, в случае появления ScrollBar
// Предположительно - из-за того погрешности чисел с плавающей точкой
// Решение - добавление условий exp2 и exp3. Условия блокируют вызов resizeEvent
// в случаях изменения размера из-за появления ScrollBar
// Если кто-нибудь будет поддерживать этот код - проблема в процедуре centerView
void GraphicsView::resizeEvent(QResizeEvent* event)
{
    bool exp1 = event->size() != event->oldSize();
    bool exp2 = qAbs(event->oldSize().width() - event->size().width()) != this->verticalScrollBar()->width();
    bool exp3 = qAbs(event->oldSize().height() - event->size().height()) != this->horizontalScrollBar()->height();
    if (exp1 && exp2 && exp3)
    {
        this->centerView();
        // Сохранение изменных размеров области просмотра
        mViewportSize = viewport()->size();
    }

    return;
}



// Слот - "Вписать в окно"
int GraphicsView::slotFitInView()
{
    this->fitInView(sceneRect(), Qt::KeepAspectRatio);
    this->fitInView(sceneRect(), Qt::KeepAspectRatio);

    mScaleFactor = 0;

    emit signalSetScaleFactor(mScaleFactor);

    return 0;
}



// Слот - "Установить модификатор масштаба"
int GraphicsView::slotSetScaleFactor(int newScaleFactor)
{
    if (mScaleFactor == newScaleFactor)
        return 0;
    // Масштабирование
    int scaleFactorDiff = newScaleFactor - mScaleFactor;
    qreal scaleValue = qPow(zoomValue, scaleFactorDiff);
    this->scale(scaleValue, scaleValue);

    mScaleFactor = newScaleFactor;

    return 0;
}



// Загрузка изображения
int GraphicsView::loadImage(QString imagePath)
{
    if (imagePath.isEmpty())
        return -1;

    mRenderer->load(imagePath);

    QRectF rect = mRenderer->viewBoxF();
    rect.setWidth(rect.width() * mPermanentScaleFactor / 100);
    rect.setHeight(rect.height() * mPermanentScaleFactor / 100);

    this->setSceneRect(rect);

    this->slotFitInView();

    return 0;
}



// Значение модификатор масштаба
int GraphicsView::scaleFactor()
{
    return mScaleFactor;
}



// Центрирование области просмотра
int GraphicsView::centerView()
{
    // Расчет центральной точки
    const int pointDiff = 1;
    qreal pointsPerPixel = (mapToScene(pointDiff, pointDiff) - mapToScene(0, 0)).x();
    QPointF centerPoint = (mapToScene(0, 0) + QPointF(mViewportSize.width(), mViewportSize.height()) * pointsPerPixel / 2);
    // Масштабирование под окно с новым размером
    this->fitInView(sceneRect(), Qt::KeepAspectRatio);
    // Зуминг
    qreal scaleValue = pow(1.1, mScaleFactor);
    this->scale(scaleValue, scaleValue);
    // Позиционирование
    this->centerOn(centerPoint);

    return 0;
}



int GraphicsView::setPermanentScaleFactor(int scaleFactor)
{
    mPermanentScaleFactor = scaleFactor;

    return 0;
}
