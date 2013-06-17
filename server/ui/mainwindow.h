#ifndef MAINWINDOW_H
#define MAINWINDOW_H



#include <QtGui>
#include <QTime>
#include "configdialog.h"
#include "../logic/server.h"
#include "../logic/xmlsettings.h"
#include "../logic/database.h"


class MainWindow : public QMainWindow
{
	Q_OBJECT


public slots:
    qint8 slotAddTextToLog(const QString text);


private slots:
    qint8 slotOpenConfigDialog();
    qint8 slotChangeServerState();


public:
	MainWindow(QWidget *parent = 0);


private:
    // Сервер
    Server* server;
    QTimer* serverTimer;

    // База данных
    Database* database;

    // Настройки
    XMLSettings* settings;

    // Элементы UI
    // Центральный виджет
    QWidget* mainWidget;
    // Вертикальный лейаут
    QVBoxLayout* mainLayout;
    // Журнал
    QLabel* lblLog;
    QTextEdit* teLog;
    // Меню
    QMenuBar* menuBar;
    QMenu* mFileMenu;
    QMenu* mTools;
    QAction* acServerState;
    QAction* acQuit;
    QAction* acSettings;
};



#endif // MAINWINDOW_H
