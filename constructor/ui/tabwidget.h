#ifndef TABWIDGET_H
#define TABWIDGET_H



#include <QtGui>
#include "tabbar.h"



class TabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit TabWidget(QWidget *parent = 0);
    // Функции, переопределенные для добавления новых вкладок перед вкладкой со знаком "+"
    int addTab(QWidget *widget, const QString &label);
    int addTab(QWidget *widget, const QIcon &icon, const QString &label);
    int insertTab(int index, QWidget *widget, const QString &label);
    int insertTab(int index, QWidget *widget, const QIcon &icon, const QString &label);
    int count() const;

private:
    QString mToolTipString;
};



#endif // TABWIDGET_H
