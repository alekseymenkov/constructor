#ifndef TOOLTIP_H
#define TOOLTIP_H



#include <QtGui>



class ToolTip : public QWidget
{
    Q_OBJECT

public:
    explicit ToolTip(QWidget* parent = 0, Qt::WindowFlags f = 0);


protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void paintEvent(QPaintEvent* event);


private:
    bool mIsShow;
    // Главный лейаут
    QVBoxLayout* ltMain;
    // Лейаут для таблицы меток
    QGridLayout* ltTable;
    // Виджет под таблицу
    QWidget* wgtTable;
    // Метки для вывода информации
    // i % 3 == 0 - Номер бита
    // i % 3 == 1 - Значение
    // i % 3 == 2 - Описание
    QVector< QPointer<QLabel> > lblCaption;
    QVector< QPointer<QLabel> > lbls;
    QPointer<QLabel> lblName;


public slots:
    // Первичное получение информации
    int slotReceiveToopTipInfo(QString name,
                               int count,
                               const QVector<QString> bits,
                               const QVector<QString> values,
                               const QVector<QString> description,
                               QPoint pos);
    // Повторное получение информации
    int slotReceiveToolTipInfo(QString name,
                               int count,
                               const QVector<QString> values);


signals:
    // Сигнал - скрытие всплывающей подсказки
    int signalToolTipHide();
};

#endif // TOOLTIP_H
