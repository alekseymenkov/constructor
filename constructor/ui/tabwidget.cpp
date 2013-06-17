#include "tabwidget.h"



TabWidget::TabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    TabBar* tabBar = new TabBar(this);
    this->setTabBar(tabBar);
    this->setTabsClosable(true);

    // Добавление вкладки со значком плюсика
    this->addTab(new QWidget(), QIcon(":/images/add.png"), "");

    // Сокрытие кнопки закрытия вкладки
    tabBar->tabButton(this->count(), QTabBar::RightSide)->hide();

    // Всплывающая подсказка для кнопки закрытия вкладки
    mToolTipString = "Закрыть вкладку";
}



int TabWidget::addTab(QWidget *widget, const QString &label)
{
    return QTabWidget::insertTab(this->count(), widget, label);
}



int TabWidget::addTab(QWidget *widget, const QIcon &icon, const QString &label)
{
    return QTabWidget::insertTab(this->count(), widget, icon, label);
}



int TabWidget::insertTab(int index, QWidget *widget, const QString &label)
{
    if (this->count() == index)
        index--;
    return QTabWidget::insertTab(this->count(), widget, label);
}



int TabWidget::insertTab(int index, QWidget *widget, const QIcon &icon, const QString &label)
{
    if (this->count() == index)
        index--;
    return QTabWidget::insertTab(this->count(), widget, icon, label);
}



int TabWidget::count() const
{
    return QTabWidget::count() - 1;
}
