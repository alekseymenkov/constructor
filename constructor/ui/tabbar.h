#ifndef TABBAR_H
#define TABBAR_H



#include <QtGui>



// Панель вкладок
class TabBar : public QTabBar
{
    Q_OBJECT

public:
    explicit TabBar(QWidget* parent = 0);

protected:
    void wheelEvent(QWheelEvent* event);

protected:
    QSize tabSizeHint(int index) const;
};



#endif // TABBAR_H
