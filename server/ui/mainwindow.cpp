#include "mainwindow.h"

#define VERSION 1.03

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Центральный виджет
    mainWidget = new QWidget(this);

    // Главный лейаут
    mainLayout = new QVBoxLayout(mainWidget);
    mainWidget->setLayout(mainLayout);

    // Журнал
    lblLog = new QLabel(mainWidget);
    lblLog->setText("<b>Журнал событий:</b>");
    mainLayout->addWidget(lblLog);

    teLog = new QTextEdit(mainWidget);
    teLog->setReadOnly(true);
    mainLayout->addWidget(teLog);

    // Меню
    menuBar = new QMenuBar(this);

    // Меню "Файл"
    mFileMenu = new QMenu("&Файл", menuBar);
    menuBar->addMenu(mFileMenu);
    acServerState = new QAction("&Запустить сервер", mFileMenu);
    acServerState->setShortcut(QKeySequence("Ctrl+R"));
    acQuit = new QAction("В&ыход", mFileMenu);
    acQuit->setShortcut(QKeySequence("Ctrl+Q"));
    mFileMenu->addAction(acServerState);
    mFileMenu->addSeparator();
    mFileMenu->addAction(acQuit);

    // Меню "Инструменты"
    mTools = new QMenu("&Инструменты", menuBar);
    menuBar->addMenu(mTools);
    acSettings = new QAction("&Настройки", mTools);
    acSettings->setShortcut(QKeySequence("Ctrl+P"));
    mTools->addAction(acSettings);

    // Параметры главного окна
    const qint16 size = 520;
    this->setWindowTitle(QString("Серверное приложение v.") + QString::number(VERSION, 'f', 2));
    this->setWindowIcon(QIcon(":/images/icon.png"));
    this->setMinimumSize(size * 1.35, size);
    this->setMenuBar(menuBar);
    this->setCentralWidget(mainWidget);

    // Настройки сети по умолчанию
    settings = new XMLSettings(this);
    settings->load();

    // Серверная часть приложения
    server = new Server();
    server->setParameters(settings->getNetInterface(),
                          settings->getPort(),
                          settings->getMaxClients());

    database = new Database(this);
    // Установка параметров для базы данных
    database->setDatabaseName(settings->getDBPath());

    // Запуск/остановка сервера
    connect(acServerState,
            SIGNAL(triggered()),
            this,
            SLOT(slotChangeServerState()));

    // Выход из программы
    connect(acQuit,
            SIGNAL(triggered()),
            qApp,
            SLOT(quit()));

    // Вызов окна настроек
    connect(acSettings,
            SIGNAL(triggered()),
            this,
            SLOT(slotOpenConfigDialog()));

    // Сообщения от сервера
    connect(server,
            SIGNAL(signalSendTextToLog(QString)),
            this,
            SLOT(slotAddTextToLog(QString)));

    // Сообщения от сервера
    connect(database,
            SIGNAL(signalSendTextToLog(QString)),
            this,
            SLOT(slotAddTextToLog(QString)));

    connect(server,
            SIGNAL(signalDBCoefficientRequest(quint16,quint16,quint16,quint16)),
            database,
            SLOT(slotDBCoefficientRequest(quint16,quint16,quint16,quint16)));

    connect(database,
            SIGNAL(signalDBCoefficientResponse(quint16,quint16,float)),
            server,
            SLOT(slotDBCoefficientResponse(quint16,quint16,float)));

    connect(server,
            SIGNAL(signalDBDescriptionRequest(quint16,quint16,quint16,quint16)),
            database,
            SLOT(slotDBDescriptionRequest(quint16,quint16,quint16,quint16)));

    connect(database,
            SIGNAL(signalDBDescriptionResponse(quint16,quint16,QString)),
            server,
            SLOT(slotDBDescriptionResponse(quint16,quint16,QString)));

    // Автозапуск сервера
    if (settings->getServerAutorun())
        slotChangeServerState();
}



// Открытие диалогового окна с настройками
qint8 MainWindow::slotOpenConfigDialog()
{
    QStringList netInterfacesList = server->getNetInterfaces();

    ConfigDialog dialog(server->isRunning(), this);
    dialog.setNetInterface(netInterfacesList, settings->getNetInterface());
    dialog.setPort(settings->getPort());
    dialog.setServerAutorun(settings->getServerAutorun());
    dialog.setDBPath(settings->getDBPath());

    if (dialog.exec() == QDialog::Accepted)
    {
        // Сохранение модифицированных настроек
        settings->setNetParameters(dialog.getPort(),
                                   dialog.getNetInterface(),
                                   dialog.getServerAutorun());
        settings->setDBParameters(dialog.getDBPath());
        settings->save();

        // Установка параметров для сервера
        server->setParameters(settings->getNetInterface(),
                              settings->getPort(),
                              settings->getMaxClients());

        // Установка параметров для базы данных
        database->setDatabaseName(settings->getDBPath());
    }

    return 0;
}



// Запуск/Остановка сервера
qint8 MainWindow::slotChangeServerState()
{
    qint8 result = 0;
    if (server->isRunning())
    {
        result = server->stop();
        if (result == 0)
            acServerState->setText("Запустить сервер");
    }
    else
    {
        result = server->start();
        if (result == 0)
            acServerState->setText("Остановить сервер");
    }

    return 0;
}



// Запись информации в журнал
qint8 MainWindow::slotAddTextToLog(const QString text)
{
    teLog->append(QDate::currentDate().toString("dd.MM.yyyy") +
                  " " +
                  QTime::currentTime().toString() + " - " + text);

    QCoreApplication::processEvents(QEventLoop::ExcludeSocketNotifiers);

    return 0;
}
