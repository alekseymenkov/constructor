#include "tabbar.h"



TabBar::TabBar(QWidget *parent) :
    QTabBar(parent)
{
}



// Убираем возможность колесом мыши открыть новые вкладки
void TabBar::wheelEvent(QWheelEvent* event)
{
    // Открыта предпоследняя вкладка (последняя - кнопка создания новой вкладки)
    bool exp1 = this->currentIndex() == this->count() - 2;
    // Скролл колесом осуществляется вправо (к последней вкладке)
    bool exp2 =  event->delta() < 0;
    if (exp1 && exp2)
        return event->ignore();
    else
        return QTabBar::wheelEvent(event);
}



// Перегрузка функции, определяющей геометрию вкладки
// Корректировка размера вкладки "Добавить" на offset пикселей
QSize TabBar::tabSizeHint(int index) const
{
    const int offset = -24;
    if (index == this->count() - 1)
        return QSize(QTabBar::tabSizeHint(index).width() + offset,
                     QTabBar::tabSizeHint(index).height());
    else
        return QTabBar::tabSizeHint(index);
}
