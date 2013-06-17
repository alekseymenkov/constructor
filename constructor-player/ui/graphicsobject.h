#ifndef GRAPHICSOBJECT_H
#define GRAPHICSOBJECT_H



#include <QtGui>
#include <QGraphicsObject>
#include "../module/elementlogic.h"
#include "../module/elementinterface.h"



class GraphicsObject : public QGraphicsObject
{
    Q_OBJECT

public:
    GraphicsObject(QGraphicsItem* parent = 0);
    // Установка логики элемента
    int setElementLogic(QSharedPointer<ElementLogic> elementLogic);
    // Установка интерфейса элемента
    int setElementInterface(QSharedPointer<ElementInterface> elementInterface);
    // Установка состояния элемента (набор битов)
    int setState(QVector<bool> state, bool isDataFromServer);
    // Сброс состояния элемента
    int resetState();
    // Прорисовка элемента
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    // Размеры элемента
    QRectF boundingRect() const;
    // Функция смены флагов элемента
    void setFlag(GraphicsItemFlag flag, bool enabled = true);
    void setFlags(GraphicsItemFlags flags);

protected:
    // Отображение всплывающей подсказки
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);


private:
    // Запрет копирования объекта
    void operator=(const GraphicsObject& graphicsObject);
    GraphicsObject(const GraphicsObject& graphicsObject);

    // Основная процедура прорисовки
    int drawFigure(QPainter *painter, Figure& figure, QPen& pen,
                   QBrush& brush, QFont& font, QString& text);
    int drawStandartElement(QPainter* painter);
    int drawLabel(QPainter* painter);
    int drawSingleIndicator(QPainter* painter);
    int drawDoubleIndicator(QPainter* painter);
    int drawFlap(QPainter *painter);
    int drawBus(QPainter *painter);

    // Значение смещения для прорисовки боковых граней
    // Если для элемента установлена область conflictOffset, то
    // paintOffset = conflictOffset
    static const int mPaintOffset = 5;

    // Логика
    QSharedPointer<ElementLogic> mElementLogic;
    // Интерфейс
    QSharedPointer<ElementInterface> mElementInterface;

    // Максимальный размер элемента (для boundingRect)
    QSize mMaxSize;
    // Смещение для прорисовки боковых граней
    QSize mOffset;
    // Состояние элемента
    QVector<bool> mState;

    // Градиент для прорисовки конфликтных областей
    QRadialGradient gradient;

    // Счетчик частоты перерисовки
    static const int mRepaintTimerInterval = 350;
    QTimer* mRepaintTimer;
    // Параметры счетчика
    int mCurrentTimerValue;
    int mOldTimerValue;
    // Текущие прорисовываемые цвета (на деле - их индексы)
    QVector<int> mCurrentColors;
    // Набор флагов для исключения уменьшения загрузки процессора
    bool mIsRepaintNeeded;
    bool mIsDataFromServer;
    bool mIsConflict;
    // Флаг отображния всплывающей подсказки
    bool mIsToolTipShow;
    // Базовый угол поворота самого элемента
    qint16 mBaseAngle;
    // Шрифты
    QFont mMainFont;
    QFont mHeaderFont;
    // Значения параметров (высчитываются во время получения новых данных)
    // По строкам - параметры, по столбцам - биты для этих параметров
    QVector< QVector<bool> > mParametersValues;
    QVector<QString> mParametersString;
    // Флаг, запущена ли панель конструктора
    bool mIsConstructorShowed;
    // Текст всплывающей подсказки
    QString mToolTipText;  

    // Функция разбора числа с учетом коэффициента
    qreal parseNumber(QVector<bool> numberBits, qreal coeff, bool isSigned);
    // Функция расчета плотности топлива
    qreal calculateFuelDensity(qreal baseDensity, qreal temperature);


public slots:
    // Слот - таймер перерисовки
    int slotRepaintTimer();
    // Слот - сокрытие всплывающей подсказки
    int slotToolTipHide();


signals:
    // Сигнал - отправка данных для генерации всплывающей подсказки
    int signalSendDataToToolTip(QVector<QString> parametersString,
                                bool isConflict);
    // Сигнал - графический объект выбран
    int signalGraphicsObjectSelect();
    // Сигнал - объект перемещен
    int signalGraphicsObjectMoved();
};



#endif // GRAPHICSOBJECT_H
