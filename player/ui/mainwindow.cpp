#include "mainwindow.h"



#define VERSION 1.07



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Левая часть окна (окно просмотра + панель инструментов)
    wgtMain = new QWidget(this);

    // Панель инструментов
    wgtTools = new QWidget(wgtMain);

    // Таймер индикатора подключения
    indicatorTimer = new QTimer(wgtMain);
    indicatorTimer->setInterval(indicatorTimerInterval);

    // Кнопка - "Подключение к серверу"
    btnClientState = new QPushButton("&Подключить", wgtMain);
    btnClientState->setFixedSize(125, 25);
    btnClientState->setFocusPolicy(Qt::NoFocus);

    // Кнопка - "Вписать в окно"
    btnFitInView = new QPushButton(wgtMain);
    btnFitInView->setFixedSize(25, 25);
    btnFitInView->setIcon(QIcon(":/images/fitinview.png"));
    btnFitInView->setFocusPolicy(Qt::NoFocus);

    // Слайдер - "Масштаб"
    sldrZoom = new QSlider(wgtMain);
    sldrZoom->setFixedHeight(25);
    sldrZoom->setMinimumWidth(250);
    sldrZoom->setOrientation(Qt::Horizontal);
    sldrZoom->setRange(-25, 25);
    sldrZoom->setSingleStep(1);
    sldrZoom->setPageStep(1);
    sldrZoom->setFocusPolicy(Qt::NoFocus);

    // Индикатор подключения
    leIndicator = new QLineEdit(wgtMain);
    leIndicator->setFixedSize(23, 23);
    leIndicator->setContextMenuPolicy(Qt::NoContextMenu);
    leIndicator->setReadOnly(true);
    leIndicator->setFocusPolicy(Qt::NoFocus);
    leIndicator->setStyleSheet("QLineEdit {background-color: rgb(255, 93, 93);}");

    // Меню
    mbMain = new QMenuBar(this);
    mFile = new QMenu("&Файл", mbMain);
    mTools = new QMenu("&Инструменты", mbMain);
    mHelp = new QMenu("&Справка", mbMain);
    mbMain->addMenu(mFile);
    mbMain->addMenu(mTools);
    mbMain->addMenu(mHelp);
    // ---
    acOpenProject = new QAction("&Открыть", mFile);
    acOpenProject->setShortcut(QKeySequence("Ctrl+O"));
    acCloseProject = new QAction("&Закрыть", mFile);
    acSaveWindowParameter = new QAction("&Сохранить информацию об окне", mFile);
    acClearWindowParameter = new QAction("&Удалить информацию об окнах", mFile);
    acConnect = new QAction("&Подключить", mFile);
    acConnect->setShortcut(QKeySequence("Ctrl+R"));
    acQuit = new QAction("В&ыход", mFile);
    acQuit->setShortcut(QKeySequence("Ctrl+Q"));
    acSettings = new QAction("Настройки", mTools);
    acSettings->setShortcut(QKeySequence("Ctrl+P"));
    acSettings->setEnabled(false);
    acPanel = new QAction("Инструментальная панель", mTools);
    acPanel->setShortcut(QKeySequence("Ctrl+I"));
    acPanel->setCheckable(true);
    acHelp = new QAction("&Помощь", mHelp);
    acHelp->setShortcut(QKeySequence("F1"));
    acAbout = new QAction("&О программе", mHelp);
    // ---
    mFile->addAction(acOpenProject);
    mFile->addAction(acCloseProject);
    mFile->addSeparator();
    mFile->addAction(acSaveWindowParameter);
    mFile->addAction(acClearWindowParameter);
    mFile->addSeparator();
    mFile->addAction(acConnect);
    mFile->addSeparator();
    mFile->addAction(acQuit);
    // ---
    mTools->addAction(acSettings);
    mTools->addSeparator();
    mTools->addAction(acPanel);
    // ---
    mHelp->addAction(acHelp);
    mHelp->addSeparator();
    mHelp->addAction(acAbout);

    // Настройки лейаутов
    ltTools = new QHBoxLayout(wgtTools);
    ltTools->setSpacing(10);
    ltMain = new QVBoxLayout(wgtMain);
    ltMain->setSpacing(0);
    ltMain->setContentsMargins(0, 0, 0, 0);

    // Виджет с вкладками
    tabMain = new QTabWidget(wgtMain);
    tabMain->setTabsClosable(false);

    // Формирование главного лейаута
    ltMain->addWidget(wgtTools, 0);
    ltMain->addWidget(tabMain, 1);

    // Формирование лейаута с инструментами
    ltTools->addWidget(btnClientState, 0);
    ltTools->addWidget(leIndicator, 0);
    ltTools->addWidget(sldrZoom, 1);
    ltTools->addWidget(btnFitInView, 0);

    // Всплывающая подсказка
    toolTip = new ToolTip(this);

    // Количество запущенных программ
    int programInstanceRunned = this->registerInstance();

    // Загрузка основных настроек программы
    mProgramSettings = QPointer<PlayerSettingsLoader>(new PlayerSettingsLoader(programInstanceRunned - 1, this));
    mProgramSettings.data()->load();

    int programInstanceNedeed = mProgramSettings.data()->programsCount();
    // Если запущена только 1 копия программы - инициализируем остальные
    if (programInstanceRunned == 1)
    {
        for (int i = 0; i < (programInstanceNedeed - programInstanceRunned); i++)
            QProcess::startDetached(QFileInfo(QCoreApplication::applicationFilePath()).fileName());
    }

    // Флаг блокировки сигналов от QTabWidget
    mIsBlockRemoveTabSignals = false;

    // Создание класса, отвечающего за работу с проектами
    mProject = new Project(this);
    this->openProject(mProgramSettings.data()->lastOpenedProject());

    // Инициализация открытого проекта
    this->initProject();

    // Настройки главного окна
    this->updateWindowTitle();
    this->setWindowIcon(QIcon(":/images/icon.png"));
    this->setCentralWidget(wgtMain);
    this->setMenuBar(mbMain);

    // Общие связи
    connect(acSettings, SIGNAL(triggered()),
            this, SLOT(slotOpenConfigDialog()));

    connect(acPanel, SIGNAL(changed()),
            this, SLOT(slotChangePanelState()));

    connect(acQuit, SIGNAL(triggered()),
            this, SLOT(slotCloseApp()));

    connect(acConnect, SIGNAL(triggered()),
            this, SLOT(slotChangeClientState()));

    connect(acOpenProject, SIGNAL(triggered()),
            this, SLOT(slotOpenProject()));

    connect(acCloseProject, SIGNAL(triggered()),
            this, SLOT(slotCloseProject()));

    connect(acSaveWindowParameter, SIGNAL(triggered()),
            this, SLOT(slotSaveWindowParameter()));

    connect(acClearWindowParameter, SIGNAL(triggered()),
            this, SLOT(slotClearWindowParameter()));

    connect(btnClientState, SIGNAL(clicked()),
            this, SLOT(slotChangeClientState()));

    connect(indicatorTimer, SIGNAL(timeout()),
            this, SLOT(slotIndicatorTimer()));

    connect(tabMain, SIGNAL(currentChanged(int)),
            this, SLOT(slotCurrentTabChanged(int)));

    // Состояние панели инструментов
    if (mProgramSettings.data()->isPanelShow())
        acPanel->setChecked(true);
    else
        acPanel->setChecked(false);
    this->slotChangePanelState();

    // Отображение окна
    if (mProgramSettings.data()->isMaximized())
    {
        mOldWindowSize = mProgramSettings.data()->mainWindowSize();
        this->resize(mOldWindowSize);
        this->move(mProgramSettings.data()->mainWindowPosition());
        this->showMaximized();
    }
    else
    {
        this->resize(mProgramSettings.data()->mainWindowSize());
        this->move(mProgramSettings.data()->mainWindowPosition());
        this->show();
    }
}



MainWindow::~MainWindow()
{
    // Отключение общих связей
    disconnect(acQuit, SIGNAL(triggered()),
               this, SLOT(slotCloseApp()));

    disconnect(acConnect, SIGNAL(triggered()),
               this, SLOT(slotChangeClientState()));

    disconnect(acOpenProject, SIGNAL(triggered()),
               this, SLOT(slotOpenProject()));

    disconnect(acCloseProject, SIGNAL(triggered()),
               this, SLOT(slotCloseProject()));

    disconnect(acSaveWindowParameter, SIGNAL(triggered()),
               this, SLOT(slotSaveWindowParameter()));

    disconnect(acClearWindowParameter, SIGNAL(triggered()),
               this, SLOT(slotClearWindowParameter()));

    disconnect(btnClientState, SIGNAL(clicked()),
               this, SLOT(slotChangeClientState()));

    disconnect(indicatorTimer, SIGNAL(timeout()),
               this, SLOT(slotIndicatorTimer()));

    disconnect(tabMain, SIGNAL(currentChanged(int)),
               this, SLOT(slotCurrentTabChanged(int)));

    // Закрытие проекта
    this->closeProject();
}



// Событие "Изменение размера"
void MainWindow::resizeEvent(QResizeEvent* event)
{
    // Сохранение размера окна до разворота на весь экран
    if (this->isMaximized())
        mOldWindowSize = event->oldSize();

    return QMainWindow::resizeEvent(event);
}



bool MainWindow::saveProgramSettings()
{
    // Если проект сохранен на диск && требуется сохранение информации о проекте
    // в config.xml (параметр lastOpenedProject) - сохраняем данные о проекте
    mProgramSettings.data()->setLastOpenedProject(mProject.data()->projectDirectory());
    mProgramSettings.data()->setTabNumber(tabMain->currentIndex());

    // Сохранение настроек программы
    if (this->isMaximized())
        mProgramSettings.data()->setMainWindowParameters(mOldWindowSize,
                                                         this->pos(),
                                                         this->isMaximized(),
                                                         acPanel->isChecked());
    else
        mProgramSettings.data()->setMainWindowParameters(this->size(),
                                                         this->pos(),
                                                         this->isMaximized(),
                                                         acPanel->isChecked());

    return mProgramSettings.data()->save();
}



int MainWindow::registerInstance()
{
    // Инициализация общей памяти
    const QString memoryAreaName("constructor");
    memory = new QSharedMemory(memoryAreaName);

    // Количество экземпляров
    qint16 instancesCount = 0;

    const int size = sizeof(quint16);
    // Создание/Подключение к участку памяти
    if (!memory->create(size, QSharedMemory::ReadWrite))
        memory->attach(QSharedMemory::ReadWrite);

    // Блокировка
    memory->lock();
    // Изменение значения
    memcpy(&instancesCount, memory->data(), size);
    instancesCount++;
    memcpy(memory->data(), &instancesCount, size);
    // Разблокировка
    memory->unlock();

    return instancesCount;
}



int MainWindow::unregisterInstance()
{
    // Количество экземпляров
    qint16 instancesCount = 0;

    const int size = sizeof(quint16);
    // Создание/Подключение к участку памяти
    //    if (!memory->create(size, QSharedMemory::ReadWrite))
    //        memory->attach(QSharedMemory::ReadWrite);

    // Блокировка
    memory->lock();
    // Изменение значения
    memcpy(&instancesCount, memory->data(), size);
    instancesCount--;
    memcpy(memory->data(), &instancesCount, size);
    // Разблокировка
    memory->unlock();

    if (memory->detach())
        memory->deleteLater();

    return 0;
}



bool MainWindow::changeClientState(int index)
{
    bool result = false;
    if (mProject.data()->network(index).data()->isConnected())
        result = mProject.data()->network(index).data()->disconnectFromServer();
    else
        result = mProject.data()->network(index).data()->connectToServer();

    return result;
}



// Событие "Закрытие окна"
void MainWindow::closeEvent(QCloseEvent* event)
{
    if (mProject.isNull())
        return QMainWindow::closeEvent(event);

    this->unregisterInstance();

    return QMainWindow::closeEvent(event);
}



// Обработчик события "Открытие конфигурационного диалога"
int MainWindow::slotOpenConfigDialog()
{
    if (mProject.isNull())
        return -1;

    const bool isBlockMode = true;
    ConfigDialog dialog(mProject.data()->network(mCurrentTab).data()->isConnected(), isBlockMode);
    dialog.setProjectName(mProject.data()->projectName());
    dialog.setPort(mProject.data()->sceneSettings(mCurrentTab).data()->port());
    dialog.setServerAddress(mProject.data()->sceneSettings(mCurrentTab).data()->serverAddress());
    dialog.setIsAutoConnect(mProject.data()->sceneSettings(mCurrentTab).data()->isAutoConnect());
    dialog.setSceneName(mProject.data()->sceneSettings(mCurrentTab).data()->sceneName());
    dialog.setMMFName(mProject.data()->sceneSettings(mCurrentTab).data()->mmfName());

    if (dialog.exec() == QDialog::Accepted)
    {
        // Сохранение модифицированных настроек
        mProject.data()->sceneSettings(mCurrentTab).data()->setNetParameters(dialog.serverAddress(),
                                                                             dialog.port(),
                                                                             dialog.isAutoConnect());
        mProject.data()->sceneSettings(mCurrentTab).data()->setMMFName(dialog.mmfName());
        mProject.data()->controller(mCurrentTab).data()->setMMFName(dialog.mmfName());
        // Сохранение файла с настройками сцены
        mProject.data()->sceneSettings(mCurrentTab).data()->save();

        // Установка параметров для сервера
        mProject.data()->network(mCurrentTab).data()->setParameters(mProject.data()->sceneSettings(mCurrentTab).data()->serverAddress(),
                                                                    mProject.data()->sceneSettings(mCurrentTab).data()->port());
    }

    return 0;
}



// Обработчик события "Подключение/Отключение клиента"
int MainWindow::slotChangeClientState()
{
    if (mProject.isNull() || !mProject.data()->isProjectOpened())
        return -1;

    bool exp1 = mProject.data()->network(mCurrentTab).data()->isConnected();
    bool exp2 = mProject.data()->network(mCurrentTab).data()->isParametersCorrect();
    if (!exp1 && !exp2)
    {
        NetworkParametersDialog dialog(mProject.data()->network(mCurrentTab).data()->serverAddress(),
                                       mProject.data()->network(mCurrentTab).data()->port(),
                                       this);

        if (dialog.exec() == QDialog::Accepted)
        {
            mProject.data()->sceneSettings(mCurrentTab).data()->setNetParameters(dialog.serverAddress(),
                                                                                 dialog.port(),
                                                                                 false);
            mProject.data()->network(mCurrentTab).data()->setParameters(dialog.serverAddress(),
                                                                        dialog.port());
            mProject.data()->sceneSettings(mCurrentTab).data()->save();
        }
        else
        {
            return false;
        }
    }

    return this->changeClientState(mCurrentTab);
}



// Обработчик события "Результат соединения"
int MainWindow::slotConnectionResult(bool result)
{
    // Успешное подключение
    if (result)
    {
        // Модификация UI
        acConnect->setText("&Отключить");
        btnClientState->setText("Отключить");
        leIndicator->setStyleSheet("QLineEdit {background-color: rgb(93, 255, 93);}");
    }
    else
    {
        // Модификация UI
        acConnect->setText("&Подключить");
        btnClientState->setText("Подключить");
        leIndicator->setStyleSheet("QLineEdit {background-color: rgb(255, 93, 93);}");

        if (!mProject.data()->isProjectOpened())
            return 0;

        // Мигание индикатора при двух неудачных попытках подключения
        if (mProject.data()->network(mCurrentTab).data()->oldConnectionResult() == false)
        {
            indicatorPulsesCount = 5;
            indicatorTimer->start();
        }
    }

    return 0;
}



int MainWindow::slotChangePanelState()
{
    // Установка минимальных размеров главного окна
    if (acPanel->isChecked())
    {
        const int minimumWidth = 480;
        const int minimumHeight = 420;
        this->setMinimumSize(minimumWidth, minimumHeight);
        wgtTools->show();
    }
    else
    {
        const int minimumWidth = 300;
        const int minimumHeight = 250;
        this->setMinimumSize(minimumWidth, minimumHeight);
        wgtTools->hide();
    }

    return 0;
}



int MainWindow::slotSaveWindowParameter()
{
    bool result = this->saveProgramSettings();

    if (result)
        QMessageBox::information(this, "Сохранение информации", "Информация об окнах успешно сохранена!");
    else
        QMessageBox::warning(this, "Сохранение информации", "Во время сохранения информации возникли ошибки. Информация не сохранена!");

    return 0;
}



int MainWindow::slotClearWindowParameter()
{
    bool result = mProgramSettings.data()->clear();

    if (result)
        QMessageBox::information(this, "Удаление информации", "Информация об окнах успешно удалена!");
    else
        QMessageBox::warning(this, "Удаление информации", "Во время удаления информации возникли ошибки. Информация не удалена!");

    return 0;
}



// Обработчик события "Таймер индикатора"
int MainWindow::slotIndicatorTimer()
{
    if (indicatorPulsesCount == 0)
        indicatorTimer->stop();

    if (indicatorPulsesCount % 2 == 1)
        leIndicator->setStyleSheet("QLineEdit {background-color: rgb(255, 255, 255);}");
    else
        leIndicator->setStyleSheet("QLineEdit {background-color: rgb(255, 93, 93);}");

    indicatorPulsesCount--;

    return 0;
}



// Изменение номера вкладки
// Обработка щелчка по кнопке "Добавить вкладку"
int MainWindow::slotCurrentTabChanged(int index)
{
    if (mIsBlockRemoveTabSignals)
        return 0;

    // Отключение старого сигнала масштабирования для слайдера
    disconnect(sldrZoom, SIGNAL(valueChanged(int)),
               mProject.data()->view(mCurrentTab).data(), SLOT(slotSetScaleFactor(int)));
    // Отключение старого синала масштабирования для кнопки "Вписать в окно"
    disconnect(btnFitInView, SIGNAL(clicked()),
               mProject.data()->view(mCurrentTab).data(), SLOT(slotFitInView()));
    // Подключение сигнала сброса отображения всплывающей подсказки при масштабировании
    disconnect(mProject.data()->view(mCurrentTab).data(), SIGNAL(signalWheelEvent()),
               toolTip, SIGNAL(signalToolTipHide()));

    mCurrentTab = index;

    // Подключение нового сигнала масштабирования для слайдера
    connect(sldrZoom, SIGNAL(valueChanged(int)),
            mProject.data()->view(mCurrentTab).data(), SLOT(slotSetScaleFactor(int)));
    // Подключение нового сигнала масштабирования для кнопки "Вписать в окно"
    connect(btnFitInView, SIGNAL(clicked()),
            mProject.data()->view(mCurrentTab).data(), SLOT(slotFitInView()));
    // Подключение сигнала сброса отображения всплывающей подсказки при масштабировании
    connect(mProject.data()->view(mCurrentTab).data(), SIGNAL(signalWheelEvent()),
            toolTip, SIGNAL(signalToolTipHide()));

    // Восстановление положения масштаба
    sldrZoom->setValue(mProject.data()->view(mCurrentTab).data()->scaleFactor());

    // Изменение UI
    if (mProject.data()->network(mCurrentTab).data()->isConnected())
    {
        acConnect->setText("&Отключить");
        btnClientState->setText("Отключить");
        leIndicator->setStyleSheet("QLineEdit {background-color: rgb(93, 255, 93);}");
    }
    else
    {
        acConnect->setText("&Подключить");
        btnClientState->setText("Подключить");
        leIndicator->setStyleSheet("QLineEdit {background-color: rgb(255, 93, 93);}");
    }

    // Автоподлкючение открытой вкладки
    if (mProject.data()->sceneSettings(mCurrentTab).data()->isAutoConnect() &&
            !mProject.data()->network(mCurrentTab).data()->isConnected())
        this->changeClientState(mCurrentTab);

    return 0;
}



// Добавление графических элементов на сцену (для сцены sceneIndex)
// Установка минимальных размеров сцены
int MainWindow::showGraphicsItems(int sceneIndex)
{
    QSharedPointer<ElementInterface> elementInterface;
    QSharedPointer<GraphicsObject> graphicsItem;

    int graphicsItemsCount = mProject.data()->controller(sceneIndex).data()->elementsCount();
    for (int i = 0; i < graphicsItemsCount; i++)
    {
        elementInterface = mProject.data()->controller(sceneIndex).data()->elementInterface(i);
        // Элементы с критической ошибкой - игнорируем
        if (elementInterface.data()->moduleState() == ModuleState::Critical)
            continue;

        // Добавляем элемент по нужным координатам
        graphicsItem = mProject.data()->controller(sceneIndex).data()->graphicsItem(i);
        graphicsItem.data()->setX(elementInterface.data()->x());
        graphicsItem.data()->setY(elementInterface.data()->y());
        mProject.data()->scene(sceneIndex).data()->addItem(graphicsItem.data());

        // Настройка взаимодействия между элементами и всплывающими подсказками
        connect(mProject.data()->controller(sceneIndex).data()->elementInterface(i).data(),
                SIGNAL(signalSendToopTipInfo(QString,int,QVector<QString>,QVector<QString>,QVector<QString>,QPoint)),
                toolTip,
                SLOT(slotReceiveToopTipInfo(QString,int,QVector<QString>,QVector<QString>,QVector<QString>,QPoint)));
        connect(mProject.data()->controller(sceneIndex).data()->elementInterface(i).data(),
                SIGNAL(signalSendToolTipInfo(QString,int,QVector<QString>)),
                toolTip,
                SLOT(slotReceiveToolTipInfo(QString,int,QVector<QString>)));
        connect(toolTip,
                SIGNAL(signalToolTipHide()),
                elementInterface.data(),
                SLOT(slotToolTipHide()));
        connect(toolTip,
                SIGNAL(signalToolTipHide()),
                graphicsItem.data(),
                SLOT(slotToolTipHide()));
    }

    // Установка минимального размера сцены
    mProject.data()->scene(sceneIndex).data()->
            setSceneRect(mProject.data()->scene(sceneIndex).data()->itemsBoundingRect());

    return 0;
}



// Удаление графических элементов со сцены
int MainWindow::hideGraphicsItems(int sceneIndex)
{
    // Удаление элементов со цвены
    int graphicsItemsCount = mProject.data()->controller(sceneIndex).data()->elementsCount();
    for (int i = 0; i < graphicsItemsCount; i++)
    {
        QSharedPointer<ElementInterface> elementInterface =
                mProject.data()->controller(sceneIndex).data()->elementInterface(i);

        // Элементы с критической ошибкой - игнорируем (их не должно быть на сцене)
        if (elementInterface.data()->moduleState() == ModuleState::Critical)
            continue;

        // Удаляем элемент со сцены
        mProject.data()->scene(sceneIndex).data()->
                removeItem(mProject.data()->controller(sceneIndex).data()->graphicsItem(i).data());

        // Настройка взаимодействия между элементами и всплывающими подсказками
        disconnect(mProject.data()->controller(sceneIndex).data()->elementInterface(i).data(),
                   SIGNAL(signalSendToopTipInfo(QString,int,QVector<QString>,QVector<QString>,QVector<QString>,QPoint)),
                   toolTip,
                   SLOT(slotReceiveToopTipInfo(QString,int,QVector<QString>,QVector<QString>,QVector<QString>,QPoint)));
        disconnect(mProject.data()->controller(sceneIndex).data()->elementInterface(i).data(),
                   SIGNAL(signalSendToolTipInfo(QString,int,QVector<QString>)),
                   toolTip,
                   SLOT(slotReceiveToolTipInfo(QString,int,QVector<QString>)));
        disconnect(toolTip,
                   SIGNAL(signalToolTipHide()),
                   mProject.data()->controller(sceneIndex).data()->elementInterface(i).data(),
                   SLOT(slotToolTipHide()));
        disconnect(toolTip,
                   SIGNAL(signalToolTipHide()),
                   mProject.data()->controller(sceneIndex).data()->graphicsItem(i).data(),
                   SLOT(slotToolTipHide()));
    }

    return 0;
}



// Открытие нового проекта
bool MainWindow::openProject(QString path)
{
    if (path.isEmpty())
        return false;

    if (mProject.isNull())
    {
        this->showMessage(QString("Ошибка при открытии проекта!"), QMessageBox::Critical);
        return false;
    }

    bool result = mProject.data()->openProject(path);
    if (result)
        acSettings->setEnabled(true);
    else
        acSettings->setEnabled(false);

    //    if (result)                                                                                                   // КОД, ОТВЕЧАЮЩИЙ ЗА АВТОПОДКЛЮЧЕНИЕ
    //    {
    //        for (int i = 0; i < mProject.data()->unitsCount(); i++)
    //        {
    //            if (mProject.data()->sceneSettings(i).data()->isAutoConnect() &&
    //                    !mProject.data()->network(i).data()->isConnected())
    //                this->changeClientState(i);
    //        }
    //    }

    return result;
}



// Слот - "Открытие нового проекта"
// Слот выполняет вызов диалога выбора директории с проектом
int MainWindow::slotOpenProject()
{
    FileDialog openProjectDialog(this);
    openProjectDialog.setWindowTitle("Открытие проекта");
    openProjectDialog.setDirectory(qApp->applicationDirPath());
    openProjectDialog.setFileMode(QFileDialog::Directory);
    openProjectDialog.setOption(QFileDialog::ShowDirsOnly, true);
    openProjectDialog.setOption(QFileDialog::DontResolveSymlinks, true);
    openProjectDialog.setLabelText(QFileDialog::Accept, "&Открыть проект");
    openProjectDialog.exec();
    QString path = openProjectDialog.selectedFiles().first();

    if (path.isEmpty())
        return -1;

    // Закрытие проекта, при необходимости
    if (this->isProjectOpened())
        this->closeProject();

    // Попытка открытия проекта по пути path
    this->openProject(path);

    // Инициализация проекта
    this->initProject();

    // Установка заголовка окна
    this->updateWindowTitle();


    return 0;
}



// Закрытие проекта
bool MainWindow::closeProject()
{
    // Флаг блокировки сигналов от QTabWidget, связанных с закрытием вкладок
    mIsBlockRemoveTabSignals = true;

    // Отключение уникальных связей, чистка памяти
    int unitsCount = mProject.data()->unitsCount();
    for (int i = 0; i < unitsCount; i++)
    {
        // Отключение связей между проектом и UI
        disconnect(mProject.data()->network(i).data(), SIGNAL(signalConnectionResult(bool)),
                   this, SLOT(slotConnectionResult(bool)));

        disconnect(btnFitInView, SIGNAL(clicked()),
                   mProject.data()->view(i).data(), SLOT(slotFitInView()));

        disconnect(sldrZoom, SIGNAL(valueChanged(int)),
                   mProject.data()->view(i).data(), SLOT(slotSetScaleFactor(int)));

        disconnect(mProject.data()->view(i).data(), SIGNAL(signalSetScaleFactor(int)),
                   sldrZoom, SLOT(setValue(int)));

        // Сокрытие элементов со сцены
        this->hideGraphicsItems(i);

        // Удаление вкладки
        tabMain->removeTab(0);
    }

    // Очистка памяти, занимаемой проектом
    mProject.data()->closeProject();

    // Отключение блокировки сигналов
    mIsBlockRemoveTabSignals = false;

    return true;
}



// Слот - "Закрытие проекта без сохранения"
int MainWindow::slotCloseProject()
{
    bool result = true;
    // Закрытие проекта, в случае необходимости
    if (this->isProjectOpened())
        result = this->closeProject();

    if (result)
        acSettings->setEnabled(false);
    else
        acSettings->setEnabled(true);

    // Установка заголовка окна
    this->updateWindowTitle();

    // Сброс интерфейса
    this->slotConnectionResult(false);

    return 0;
}



// Слот - "Закрытие приложения"
int MainWindow::slotCloseApp()
{
    this->unregisterInstance();

    qApp->quit();

    return 0;
}



// Инициализация проекта. Настройка связи между GUI и логикой
bool MainWindow::initProject()
{
    if (mProject.isNull())
    {
        this->showMessage(QString("Ошибка при инициализации проекта!"), QMessageBox::Critical);
        return false;
    }

    // Текущая открытая вкладка
    mCurrentTab = 0;
    tabMain->setCurrentIndex(mCurrentTab);

    if (!mProject.data()->isProjectOpened())
        return true;

    // Основная инициализация ресурсов
    for (int i = 0; i < mProject.data()->unitsCount(); i++)
    {
        // Добавление вкладки
        tabMain->addTab(mProject.data()->view(i).data(),
                        mProject.data()->sceneSettings(i).data()->sceneName());

        // Связи, уникальные для каждой вкладки
        connect(mProject.data()->network(i).data(), SIGNAL(signalConnectionResult(bool)),
                this, SLOT(slotConnectionResult(bool)));

        connect(mProject.data()->view(i).data(), SIGNAL(signalSetScaleFactor(int)),
                sldrZoom, SLOT(setValue(int)));

        // Добавление на сцену элементов
        // Установка минимального размера сцены
        this->showGraphicsItems(i);
    }

    // Установка текущей вкладки
    if (mProgramSettings.data()->tabNumber() >= 0)
    {
        mCurrentTab = mProgramSettings.data()->tabNumber();
        tabMain->setCurrentIndex(mCurrentTab);
    }

    // Подключение нового сигнала масштабирования для слайдера
    connect(sldrZoom, SIGNAL(valueChanged(int)),
            mProject.data()->view(mCurrentTab).data(), SLOT(slotSetScaleFactor(int)));
    // Подключение нового сигнала масштабирования для кнопки "Вписать в окно"
    connect(btnFitInView, SIGNAL(clicked()),
            mProject.data()->view(mCurrentTab).data(), SLOT(slotFitInView()));
    // Подключение сигнала сброса отображения всплывающей подсказки при масштабировании
    connect(mProject.data()->view(mCurrentTab).data(), SIGNAL(signalWheelEvent()),
            toolTip, SIGNAL(signalToolTipHide()));


    // Автоподлкючение открытой вкладки
    if (mProject.data()->sceneSettings(mCurrentTab).data()->isAutoConnect() &&
            !mProject.data()->network(mCurrentTab).data()->isConnected())
        this->changeClientState(mCurrentTab);

    return true;
}



// Флаг - открыт ли какой-нибудь проект?
bool MainWindow::isProjectOpened()
{
    if (mProject.isNull())
        return false;
    else
        return mProject.data()->isProjectOpened();
}



// Обновление заголовка окна
// Установка марки, в случае необходимости
bool MainWindow::updateWindowTitle()
{
    if (mProject.data() == 0)
        return false;

    const QString defaultTitle = QString("Визуализатор v.%1").arg(QString::number(VERSION, 'f', 2));
    const QString delimiter = " - ";
    if (mProject.data()->projectName().isEmpty())
        this->setWindowTitle(defaultTitle + mProject.data()->projectName());
    else
        this->setWindowTitle(defaultTitle + delimiter + mProject.data()->projectName());

    return true;
}



// Стандартное сообщение
int MainWindow::showMessage(const QString message, QMessageBox::Icon iconType)
{
    const QString questionCaption("Вопрос");
    const QString informationCaption("Информация");
    const QString warningCaption("Предупреждение");
    const QString criticalCaption("Ошибка");

    switch (iconType)
    {
    case QMessageBox::Question:
        QMessageBox::question(this, questionCaption, message);
        return 0;
    case QMessageBox::NoIcon:
    case QMessageBox::Information:
        QMessageBox::information(this, informationCaption, message);
        return 0;
    case QMessageBox::Warning:
        QMessageBox::warning(this, warningCaption, message);
        return 0;
    case QMessageBox::Critical:
        QMessageBox::critical(this, criticalCaption, message);
        return 0;
    }

    return 0;
}
