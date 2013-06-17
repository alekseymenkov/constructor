#include "mainwindow.h"



#define VERSION 1.07



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Окно редактирования элемента
    elementEditDialog = 0;

    // Главный виджет - разделитель
    splitterMain = new QSplitter(this);

    // Левая часть окна (окно просмотра + панель инструментов)
    // Минимальный размер - 480 пикселей
    const int minimumWgtMainWidth = 480;
    wgtMain = new QWidget(splitterMain);
    wgtMain->setMinimumWidth(minimumWgtMainWidth);

    // Правая часть (конструктор)
    // Минимальный размер - 493 пикселя
    const int minimumWgtConstructorWidth = 493;
    wgtConstructor = new QWidget(splitterMain);
    wgtConstructor->setMinimumWidth(minimumWgtConstructorWidth);
    wgtConstructor->setEnabled(false);
    wgtConstructor->setVisible(false);

    // Панель инструментов
    wgtTools = new QWidget(wgtMain);

    // Таймер индикатора подключения
    indicatorTimer = new QTimer(wgtMain);
    indicatorTimer->setInterval(indicatorTimerInterval);

    // Кнопка - "Подключение к серверу"
    btnClientState = new QPushButton("Запрос &данных", wgtMain);
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
    acCreateProject = new QAction("&Создать", mFile);
    acCreateProject->setShortcut(QKeySequence("Ctrl+N"));
    acOpenProject = new QAction("&Открыть", mFile);
    acOpenProject->setShortcut(QKeySequence("Ctrl+O"));
    acSaveProject = new QAction("&Сохранить", mFile);
    acSaveProject->setShortcut(QKeySequence("Ctrl+S"));
    acSaveAsProject = new QAction("&Сохранить как...", mFile);
    acSaveAsProject->setShortcut(QKeySequence("Ctrl+Alt+S"));
    acCloseProject = new QAction("&Закрыть", mFile);
    acRequestData = new QAction("&Запрос данных", mFile);
    acRequestData->setShortcut(QKeySequence("Ctrl+R"));
    acQuit = new QAction("В&ыход", mFile);
    acQuit->setShortcut(QKeySequence("Ctrl+Q"));
    acConstructor = new QAction("Панель &конструктора", mTools);
    acConstructor->setShortcut(QKeySequence("Ctrl+D"));
    acConstructor->setCheckable(true);
    acSettings = new QAction("Настройки", mTools);
    acSettings->setShortcut(QKeySequence("Ctrl+P"));
    acScale = new QAction("Внешний вид", mTools);
    acScale->setShortcut(QKeySequence("Ctrl+M"));
    acPanel = new QAction("Инструментальная панель", mTools);
    acPanel->setShortcut(QKeySequence("Ctrl+I"));
    acPanel->setCheckable(true);
    acHelp = new QAction("&Помощь", mHelp);
    acHelp->setShortcut(QKeySequence("F1"));
    acAbout = new QAction("&О программе", mHelp);
    // ---
    mFile->addAction(acCreateProject);
    mFile->addAction(acOpenProject);
    mFile->addAction(acSaveProject);
    mFile->addAction(acSaveAsProject);
    mFile->addAction(acCloseProject);
    mFile->addSeparator();
    mFile->addAction(acRequestData);
    mFile->addSeparator();
    mFile->addAction(acQuit);
    // ---
    mTools->addAction(acScale);
    mTools->addAction(acSettings);
    mTools->addSeparator();
    mTools->addAction(acConstructor);
    mTools->addAction(acPanel);
    // ---
    mHelp->addAction(acHelp);
    mHelp->addSeparator();
    mHelp->addAction(acAbout);

    // Настройка элементов
    gbElementsTable = new QGroupBox(wgtConstructor);
    gbElementsTable->setTitle("Список элементов");
    ltElementsTable = new QGridLayout(gbElementsTable);
    ltElementsTable->setSpacing(10);

    // Таблица - список элементов
    tblElementsList = new QTableWidget(wgtConstructor);
    tblElementsList->setGeometry(10, 64, 280, 400);
    tblElementsList->setColumnCount(6);
    tblElementsList->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblElementsList->setSelectionMode(QAbstractItemView::SingleSelection);
    tblElementsList->verticalHeader()->setDefaultSectionSize(25);
    tblElementsList->verticalHeader()->setVisible(false);
    tblElementsList->horizontalHeader()->setClickable(false);
    tblElementsList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tblElementsList->setFocusPolicy(Qt::NoFocus);
    tblElementsList->setStyleSheet("QTableView::item:selected {background: #ece9d8; color: blue;}");
    tblElementsList->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    tblElementsList->setHorizontalHeaderItem(0, new QTableWidgetItem(""));
    tblElementsList->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
    tblElementsList->horizontalHeader()->resizeSection(0, 22);
    tblElementsList->setHorizontalHeaderItem(1, new QTableWidgetItem("№"));
    tblElementsList->horizontalHeader()->setResizeMode(1, QHeaderView::ResizeToContents);
    tblElementsList->setHorizontalHeaderItem(2, new QTableWidgetItem("Модуль"));
    tblElementsList->horizontalHeader()->setResizeMode(2, QHeaderView::Stretch);
    tblElementsList->setHorizontalHeaderItem(3, new QTableWidgetItem("Название"));
    tblElementsList->horizontalHeader()->setResizeMode(3, QHeaderView::ResizeToContents);
    tblElementsList->setHorizontalHeaderItem(4, new QTableWidgetItem("Ст. байт"));
    tblElementsList->horizontalHeader()->setResizeMode(4, QHeaderView::ResizeToContents);
    tblElementsList->setHorizontalHeaderItem(5, new QTableWidgetItem("Кол-во байт"));
    tblElementsList->horizontalHeader()->setResizeMode(5, QHeaderView::ResizeToContents);

    // Кнопка - "Добавить элемент"
    btnAddElement = new QPushButton(wgtConstructor);
    btnAddElement->setText("Добавить");
    btnAddElement->setFixedSize(125, 25);

    // Кнопка - "Копировать элемент"
    btnCopyElement = new QPushButton(wgtConstructor);
    btnCopyElement->setText("Копировать");
    btnCopyElement->setFixedSize(125, 25);
    btnCopyElement->setEnabled(false);

    // Кнопка - "Редактировать элемент"
    btnEditElement = new QPushButton(wgtConstructor);
    btnEditElement->setText("Редактировать");
    btnEditElement->setFixedSize(125, 25);
    btnEditElement->setEnabled(false);

    // Кнопка - "Удалить элемент"
    btnRemoveElement = new QPushButton(wgtConstructor);
    btnRemoveElement->setText("Удалить");
    btnRemoveElement->setFixedSize(125, 25);
    btnRemoveElement->setEnabled(false);

    // Метка - предупреждение о невозможности редактирования элементов
    lblEditDisabled = new QLabel(wgtConstructor);
    lblEditDisabled->setText("<span style='color:red;'>Редактирование<br>запрещено!</span>");
    lblEditDisabled->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    lblEditDisabled->setFixedSize(125, 50);
    lblEditDisabled->setVisible(false);

    // Лейаут "Настройка элементов"
    ltElementsTable->addWidget(tblElementsList, 0, 0, 6, 1);
    ltElementsTable->addWidget(btnAddElement, 0, 1, 1, 1);
    ltElementsTable->addWidget(btnCopyElement, 1, 1, 1, 1);
    ltElementsTable->addWidget(btnEditElement, 2, 1, 1, 1);
    ltElementsTable->addWidget(btnRemoveElement, 3, 1, 1, 1);
    ltElementsTable->addWidget(lblEditDisabled, 4, 1, 1, 1);

    // Настройки лейаутов
    ltTools = new QHBoxLayout(wgtTools);
    ltTools->setSpacing(10);
    int margin = ltTools->margin();
    ltMain = new QVBoxLayout(wgtMain);
    ltMain->setSpacing(0);
    ltMain->setContentsMargins(0, 0, 0, 0);
    ltConstructor = new QVBoxLayout(wgtConstructor);
    ltConstructor->setContentsMargins(margin / 2, margin, margin, margin);

    // Настройка разделителя
    splitterMain->addWidget(wgtMain);
    splitterMain->addWidget(wgtConstructor);
    splitterMain->setCollapsible(0, false);
    splitterMain->setCollapsible(1, false);
    splitterMain->setStretchFactor(0, 1);

    // Виджет с вкладками
    tabMain = new TabWidget(wgtMain);

    // Формирование главного лейаута
    ltMain->addWidget(wgtTools, 0);
    ltMain->addWidget(tabMain, 1);

    // Формирование лейаута с инструментами
    ltTools->addWidget(btnClientState, 0);
    ltTools->addWidget(leIndicator, 0);
    ltTools->addWidget(sldrZoom, 1);
    ltTools->addWidget(btnFitInView, 0);

    // Формирование лейаута с конструтором
    ltConstructor->addWidget(gbElementsTable);

    // Всплывающая подсказка
    toolTip = new ToolTip(this);

    // Загрузка основных настроек программы
    mProgramSettings = QPointer<ContructorSettingsLoader>(new ContructorSettingsLoader(this));
    mProgramSettings.data()->load();

    mIsBlockRemoveTabSignals = false;

    // Создание класса, отвечающего за работу с проектами
    mProject = new Project(this);
    bool result = this->openProject(mProgramSettings.data()->lastOpenedProject());
    if (!result)
        this->createProject();

    // Инициализация созданного/открытого проекта
    this->initProject();

    // Настройки главного окна
    this->updateWindowTitle();
    this->setWindowIcon(QIcon(":/images/icon.png"));
    this->setCentralWidget(splitterMain);
    this->setMenuBar(mbMain);

    // Общие связи
    connect(tblElementsList, SIGNAL(itemSelectionChanged()),
            this, SLOT(slotTableSelectionChanged()));

    connect(acSettings, SIGNAL(triggered()),
            this, SLOT(slotOpenConfigDialog()));

    connect(acScale, SIGNAL(triggered()),
            this, SLOT(slotOpenScaleSettingsDialog()));

    connect(acQuit, SIGNAL(triggered()),
            this, SLOT(slotCloseApp()));

    connect(acRequestData, SIGNAL(triggered()),
            this, SLOT(slotChangeClientState()));

    connect(acPanel, SIGNAL(changed()),
            this, SLOT(slotChangePanelState()));

    connect(acConstructor, SIGNAL(triggered()),
            this, SLOT(slotChangeConstructorState()));

    connect(acCreateProject, SIGNAL(triggered()),
            this, SLOT(slotCreateProject()));

    connect(acOpenProject, SIGNAL(triggered()),
            this, SLOT(slotOpenProject()));

    connect(acSaveProject, SIGNAL(triggered()),
            this, SLOT(slotSaveProject()));

    connect(acSaveAsProject, SIGNAL(triggered()),
            this, SLOT(slotSaveAsProject()));

    connect(acCloseProject, SIGNAL(triggered()),
            this, SLOT(slotCloseProject()));

    connect(btnClientState, SIGNAL(clicked()),
            this, SLOT(slotChangeClientState()));

    connect(indicatorTimer, SIGNAL(timeout()),
            this, SLOT(slotIndicatorTimer()));

    connect(btnAddElement, SIGNAL(clicked()),
            this, SLOT(slotAddElement()));

    connect(btnCopyElement, SIGNAL(clicked()),
            this, SLOT(slotCopyElement()));

    connect(btnEditElement, SIGNAL(clicked()),
            this, SLOT(slotEditElement()));

    connect(btnRemoveElement, SIGNAL(clicked()),
            this, SLOT(slotRemoveElement()));

    connect(tabMain, SIGNAL(currentChanged(int)),
            this, SLOT(slotCurrentTabChanged(int)));

    connect(tabMain, SIGNAL(tabCloseRequested(int)),
            this, SLOT(slotRemoveTab(int)));

    connect(this, SIGNAL(signalProjectChanged()),
            mProject.data(), SLOT(slotProjectChanged()));

    // Отображение конструктора
    if (mProgramSettings.data()->isConstructorShow())
        acConstructor->setChecked(true);
    else
        acConstructor->setChecked(false);
    this->slotChangeConstructorState();

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
        this->showMaximized();
    }
    else
    {
        this->resize(mProgramSettings.data()->mainWindowSize());
        this->show();
    }
}



MainWindow::~MainWindow()
{
    // Отключение общих связей
    disconnect(tblElementsList, SIGNAL(itemSelectionChanged()),
               this, SLOT(slotTableSelectionChanged()));

    disconnect(acQuit, SIGNAL(triggered()),
               this, SLOT(slotCloseApp()));

    disconnect(acSettings, SIGNAL(triggered()),
               this, SLOT(slotOpenConfigDialog()));

    disconnect(acScale, SIGNAL(triggered()),
               this, SLOT(slotOpenScaleSettingsDialog()));

    disconnect(acRequestData, SIGNAL(triggered()),
               this, SLOT(slotChangeClientState()));

    disconnect(acPanel, SIGNAL(changed()),
               this, SLOT(slotChangePanelState()));

    disconnect(acConstructor, SIGNAL(triggered()),
               this, SLOT(slotChangeConstructorState()));

    disconnect(acCreateProject, SIGNAL(triggered()),
               this, SLOT(slotCreateProject()));

    disconnect(acOpenProject, SIGNAL(triggered()),
               this, SLOT(slotOpenProject()));

    disconnect(acSaveProject, SIGNAL(triggered()),
               this, SLOT(slotSaveProject()));

    disconnect(acSaveAsProject, SIGNAL(triggered()),
               this, SLOT(slotSaveAsProject()));

    disconnect(acCloseProject, SIGNAL(triggered()),
               this, SLOT(slotCloseProject()));

    disconnect(btnClientState, SIGNAL(clicked()),
               this, SLOT(slotChangeClientState()));

    disconnect(indicatorTimer, SIGNAL(timeout()),
               this, SLOT(slotIndicatorTimer()));

    disconnect(btnAddElement, SIGNAL(clicked()),
               this, SLOT(slotAddElement()));

    disconnect(btnCopyElement, SIGNAL(clicked()),
               this, SLOT(slotCopyElement()));

    disconnect(btnEditElement, SIGNAL(clicked()),
               this, SLOT(slotEditElement()));

    disconnect(btnRemoveElement, SIGNAL(clicked()),
               this, SLOT(slotRemoveElement()));

    disconnect(tabMain, SIGNAL(currentChanged(int)),
               this, SLOT(slotCurrentTabChanged(int)));

    disconnect(tabMain, SIGNAL(tabCloseRequested(int)),
               this, SLOT(slotRemoveTab(int)));

    disconnect(this, SIGNAL(signalProjectChanged()),
               mProject.data(), SLOT(slotProjectChanged()));

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


// Событие "Закрытие окна"
void MainWindow::closeEvent(QCloseEvent* event)
{
    if (mProject.isNull())
        return QMainWindow::closeEvent(event);

    // Сохранение настроек программы
    bool result = this->saveProgramSettings();

    if (!result)
        return event->ignore();

    return QMainWindow::closeEvent(event);
}



// Сохранение настроек программы
bool MainWindow::saveProgramSettings()
{
    if (!mProject.data()->isSavedToDisc() && mProject.data()->isProjectModified())
    {
        // Предупреждение о закрытии проекта без сохранения
        QMessageBox msgBox;
        msgBox.setText("Сохранить изменения в проекте?");
        msgBox.setWindowTitle("Закрытие проекта");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.setIcon(QMessageBox::Question);

        switch (msgBox.exec())
        {
        case QMessageBox::Yes:
            this->slotSaveProject();
            break;
        case QMessageBox::No:
            break;
        case QMessageBox::Cancel:
            return false;
        }
    }
    else if (mProject.data()->isSavedToDisc() && mProject.data()->isProjectModified())
    {
        this->slotSaveProject();
    }

    // Если проект сохранен на диск && требуется сохранение информации о проекте
    // в config.xml (параметр lastOpenedProject) - сохраняем данные о проекте
    mProgramSettings.data()->setLastOpenedProject(mProject.data()->projectDirectory());
    // Сохранение информации о состоянии инструментальной панели
    mProgramSettings.data()->setPanelState(acPanel->isChecked());
    // Сохранение информации о состоянии панели конструктора
    mProgramSettings.data()->setConstructorParameters(wgtConstructor->size(),
                                                      acConstructor->isChecked());

    // Сохранение настроек программы
    if (this->isMaximized())
        mProgramSettings.data()->setMainWindowParameters(mOldWindowSize,
                                                         this->isMaximized());
    else
        mProgramSettings.data()->setMainWindowParameters(this->size(),
                                                         this->isMaximized());

    mProgramSettings.data()->save();

    return true;
}



// Обработчик события "Открытие конфигурационного диалога"
int MainWindow::slotOpenConfigDialog()
{
    if (mProject.isNull())
        return -1;

    ConfigDialog dialog(mProject.data()->network(mCurrentTab).data()->isConnected());
    dialog.setProjectName(mProject.data()->projectName());
    dialog.setPort(mProject.data()->sceneSettings(mCurrentTab).data()->port());
    dialog.setServerAddress(mProject.data()->sceneSettings(mCurrentTab).data()->serverAddress());
    dialog.setSceneName(mProject.data()->sceneSettings(mCurrentTab).data()->sceneName());
    dialog.setIsAutoDBRequest(mProject.data()->sceneSettings(mCurrentTab).data()->isAutoDBRequest());
    dialog.setIsForceDBRequest(mProject.data()->sceneSettings(mCurrentTab).data()->isForceDBRequest());
    dialog.setMMFName(mProject.data()->sceneSettings(mCurrentTab).data()->mmfName());

    if (dialog.exec() == QDialog::Accepted)
    {
        // Сохранение модифицированных настроек
        mProject.data()->setProjectName(dialog.projectName());
        mProject.data()->controller(mCurrentTab).data()->setMMFName(dialog.mmfName());
        mProject.data()->sceneSettings(mCurrentTab).data()->setNetParameters(dialog.serverAddress(),
                                                                             dialog.port(),
                                                                             dialog.isAutoConnect());
        mProject.data()->sceneSettings(mCurrentTab).data()->setSceneParameters(dialog.sceneName());
        mProject.data()->sceneSettings(mCurrentTab).data()->setMMFName(dialog.mmfName());
        mProject.data()->sceneSettings(mCurrentTab).data()->setUpdateParameters(dialog.isAutoDBRequest(),
                                                                                dialog.isForceDBRequest());
        if (mProject.data()->isSavedToDisc())
        {
            // Сохранение файла с настройками сцены
            mProject.data()->sceneSettings(mCurrentTab).data()->save();
            // Сохранение файла с настройками проекта
            mProject.data()->save();
        }

        // Обновление заголовка окна
        this->updateWindowTitle();

        // Загрузка настроек программы (фон)
        mProject.data()->view(mCurrentTab).data()->loadImage(mProject.data()->sceneSettings(mCurrentTab).data()->image());
        // Установка параметров для сервера
        mProject.data()->network(mCurrentTab).data()->setParameters(mProject.data()->sceneSettings(mCurrentTab).data()->serverAddress(),
                                                                    mProject.data()->sceneSettings(mCurrentTab).data()->port());
        // Установка параметров обновления для контроллера
        mProject.data()->controller(mCurrentTab)->setUpdateParameters(mProject.data()->sceneSettings(mCurrentTab).data()->isAutoDBRequest(),
                                                                      mProject.data()->sceneSettings(mCurrentTab).data()->isForceDBRequest());
        // Смена названия вкладки
        tabMain->setTabText(mCurrentTab, mProject.data()->sceneSettings(mCurrentTab).data()->sceneName());

        // Отправка сигнала - "Проект изменен"
        emit signalProjectChanged();
    }

    return 0;
}



// Обработчик события "Открытие диалога с настройками масштаба"
int MainWindow::slotOpenScaleSettingsDialog()
{
    if (mProject.isNull())
        return -1;

    ScaleSettingsDialog dialog(this);
    dialog.setProjectDirectory(mProject.data()->projectDirectory());
    dialog.setImage(mProject.data()->sceneSettings(mCurrentTab).data()->image(),
                    mProject.data()->sceneSettings(mCurrentTab).data()->scaleFactor());

    if (dialog.exec() == QDialog::Accepted)
    {
        // Сохранение модифицированных настроек
        mProject.data()->sceneSettings(mCurrentTab).data()->setImageParameters(dialog.image(), dialog.scaleFactor());

        if (mProject.data()->isSavedToDisc())
        {
            // Сохранение файла с настройками сцены
            mProject.data()->sceneSettings(mCurrentTab).data()->save();
            // Сохранение файла с настройками проекта
            mProject.data()->save();
        }

        // Обновление заголовка окна
        this->updateWindowTitle();

        // Загрузка настроек программы (фон)
        mProject.data()->view(mCurrentTab).data()->setPermanentScaleFactor(mProject.data()->sceneSettings(mCurrentTab).data()->scaleFactor());
        mProject.data()->view(mCurrentTab).data()->loadImage(mProject.data()->sceneSettings(mCurrentTab).data()->image());

        // Отправка сигнала - "Проект изменен"
        emit signalProjectChanged();
    }

    return 0;
}



// Обработчик события "Подключение/Отключение клиента"
int MainWindow::slotChangeClientState()
{
    if (mProject.isNull())
        return -1;

    int result = 0;

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
                                                                                 false);                    // ПОПРАВИТЬ БЛЯ!
            mProject.data()->network(mCurrentTab).data()->setParameters(dialog.serverAddress(),
                                                                        dialog.port());
            if (mProject.data()->isSavedToDisc())
                mProject.data()->sceneSettings(mCurrentTab).data()->save();
        }
        else
        {
            return result;
        }
    }

    if (mProject.data()->network(mCurrentTab).data()->isConnected())
        result = mProject.data()->network(mCurrentTab).data()->disconnectFromServer();
    else
        result = mProject.data()->network(mCurrentTab).data()->connectToServer();

    return result;
}



int MainWindow::slotChangePanelState()
{
    // Установка минимальных размеров главного окна
    if (acPanel->isChecked())
        wgtTools->show();
    else
        wgtTools->hide();

    return 0;
}



// Обработчик события "Результат соединения"
int MainWindow::slotConnectionResult(bool result)
{
    // Успешное подключение
    if (result)
    {
        // Модификация UI
        acRequestData->setText("&Отключить");
        btnClientState->setText("Отключить");
        leIndicator->setStyleSheet("QLineEdit {background-color: rgb(93, 255, 93);}");
        btnAddElement->setEnabled(false);
        btnCopyElement->setEnabled(false);
        btnEditElement->setEnabled(false);
        btnRemoveElement->setEnabled(false);
        lblEditDisabled->setVisible(true);
    }
    else
    {
        // Модификация UI
        acRequestData->setText("&Запрос данных");
        btnClientState->setText("Запрос данных");
        leIndicator->setStyleSheet("QLineEdit {background-color: rgb(255, 93, 93);}");
        btnAddElement->setEnabled(true);
        // Кнопки активизируются только в том случае, если в таблице выделен элемент
        if (tblElementsList->selectedItems().size() != 0)
        {
            btnCopyElement->setEnabled(true);
            btnEditElement->setEnabled(true);
            btnRemoveElement->setEnabled(true);
        }
        lblEditDisabled->setVisible(false);

        if (!mProject.data()->isSavedToDisc() && !mProject.data()->isProjectModified())
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



// Изменение интерфейсного элемента elementID в контроллере ID
int MainWindow::slotElementInterfaceChanged(int id, int elementID)
{
    if (mProject.isNull())
        return -1;

    QSharedPointer<ElementInterface> elementInterface =
            mProject.data()->controller(id).data()->elementInterface(elementID);
    QSharedPointer<GraphicsObject> graphicsItem =
            mProject.data()->controller(id).data()->graphicsItem(elementID);

    elementInterface.data()->checkCorrect();

    // Если ID контроллера совпадает с номером открытой вкладки, то
    // меняем запись в таблице
    if (id == mCurrentTab)
    {
        this->changeElementInTable(elementID,
                                   elementInterface.data()->fullModuleName(),
                                   elementInterface.data()->elementName(),
                                   elementInterface.data()->startByte(),
                                   elementInterface.data()->bytesCount(),
                                   elementInterface.data()->moduleState());
    }

    // Удаление элемента со сцены, в случае критической ошибки
    if (elementInterface.data()->moduleState() == ModuleState::Critical)
    {
        if (graphicsItem.data()->scene() != 0)
            mProject.data()->scene(id).data()->removeItem(graphicsItem.data());

        // Настройка взаимодействия между элементами и всплывающими подсказками
        disconnect(elementInterface.data(),
                   SIGNAL(signalSendToopTipInfo(QString,int,QVector<QString>,QVector<QString>,QVector<QString>,QPoint)),
                   toolTip,
                   SLOT(slotReceiveToopTipInfo(QString,int,QVector<QString>,QVector<QString>,QVector<QString>,QPoint)));
        disconnect(elementInterface.data(),
                   SIGNAL(signalSendToolTipInfo(QString,int,QVector<QString>)),
                   toolTip,
                   SLOT(slotReceiveToolTipInfo(QString,int,QVector<QString>)));
        disconnect(toolTip,
                   SIGNAL(signalToolTipHide()),
                   elementInterface.data(),
                   SLOT(slotToolTipHide()));
        disconnect(toolTip,
                   SIGNAL(signalToolTipHide()),
                   graphicsItem.data(),
                   SLOT(slotToolTipHide()));
    }

    return 0;
}



// Обработчик события "Выбор графического элемента"
int MainWindow::slotGraphicsObjectSelected(int elementNum)
{
    QModelIndex row = tblElementsList->model()->index(elementNum, 0);
    tblElementsList->setCurrentIndex(row);

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



// Запуск/Отключение режима конструктора
int MainWindow::slotChangeConstructorState()
{
    // Сохранение настроек интерфейсных модулей
    QSharedPointer<ElementInterface> elementInterface;
    QSharedPointer<GraphicsObject> graphicsItem;

    // Размер панели конструктора
    int constructorWidth = 0;
    if (!acConstructor->isChecked())
    {
        wgtConstructor->width();
        ltMain->setContentsMargins(0, 0, 0, 0);
        wgtConstructor->setEnabled(false);
        wgtConstructor->setVisible(false);

        // Удаление строк из таблицы при скрытии панели конструктора
        tblElementsList->clearContents();
        tblElementsList->setRowCount(0);

        for (int i = 0; i < mProject.data()->controller(mCurrentTab).data()->elementsCount(); i++)
        {
            elementInterface = mProject.data()->controller(mCurrentTab).data()->elementInterface(i);
            graphicsItem = mProject.data()->controller(mCurrentTab).data()->graphicsItem(i);
            elementInterface.data()->setX(graphicsItem.data()->x());
            elementInterface.data()->setY(graphicsItem.data()->y());
            // Запрет перемещения элементов
            graphicsItem.data()->setFlag(QGraphicsItem::ItemIsMovable, false);
        }

        constructorWidth = 0;
    }
    else
    {
        // Заполнение таблицы с элементами
        this->generateTable(mCurrentTab);

        // Разрешение перемещения элементов
        int elementsCount = mProject.data()->controller(mCurrentTab).data()->elementsCount();

        for (int i = 0; i < elementsCount; i++)
        {
            // Разрешение перемещения элементов
            graphicsItem = mProject.data()->controller(mCurrentTab).data()->graphicsItem(i);
            graphicsItem.data()->setFlag(QGraphicsItem::ItemIsMovable, true);
        }

        ltMain->setContentsMargins(0, 0, ltTools->margin(), 0);
        wgtConstructor->resize(mProgramSettings.data()->constructorSize());
        wgtConstructor->setEnabled(true);
        wgtConstructor->setVisible(true);

        constructorWidth = wgtConstructor->minimumWidth();
    }

    // Корректирующее значение для ширины сплиттера и главного окна
    const int midLineWidth = 5;

    // Установка минимальной ширины сплиттера
    splitterMain->setMinimumWidth(wgtMain->minimumWidth() +
                                  midLineWidth +
                                  constructorWidth);

    // Установка минимальной ширины панели инструментов
    wgtTools->setMinimumWidth(wgtMain->minimumWidth());

    // Установка минимальных размеров главного окна
    const int minimumMainWindowHeight = 420;
    this->setMinimumWidth(wgtMain->minimumWidth() +
                          midLineWidth +
                          constructorWidth);
    this->setMinimumHeight(minimumMainWindowHeight);

    QResizeEvent resizeEvent(QSize(this->width(), this->height()),
                             QSize(this->width(), this->height()));
    QCoreApplication::sendEvent(this, &resizeEvent);

    return 0;

}



// Обработчик события - "Изменение выделения строки в таблице"
int MainWindow::slotTableSelectionChanged()
{
    QList<QTableWidgetItem*> itemsList = tblElementsList->selectedItems();

    // Модификация UI
    if (itemsList.count() != 0 && !mProject.data()->network(mCurrentTab).data()->isConnected())
    {
        btnCopyElement->setEnabled(true);
        btnEditElement->setEnabled(true);
        btnRemoveElement->setEnabled(true);
    }
    else if (!mProject.data()->network(mCurrentTab).data()->isConnected())
    {
        btnCopyElement->setEnabled(false);
        btnEditElement->setEnabled(false);
        btnRemoveElement->setEnabled(false);
    }

    return 0;
}



// Работа с диалоговым окном конструктора
int MainWindow::showElementEditDialog(ElementEditDialogType::Type type)
{
    // Отправка сигнала - "Проект изменен"
    emit signalProjectChanged();

    // Порядковый номер элемента в таблице (совпадает с номером в массиве)
    quint16 elementNum = tblElementsList->currentIndex().row();

    QSharedPointer<ElementInterface> elementInterface;
    QSharedPointer<GraphicsObject> graphicsItem;

    // Создание начальных данных для диалога
    switch (type)
    {
    case ElementEditDialogType::Add:
        break;
        // Копирование элемента интерфейса
    case ElementEditDialogType::Copy:
        // Получаем образец элемента интерфейса
        elementInterface = mProject.data()->controller(mCurrentTab).data()->elementInterface(elementNum);
        // Создаем новый элемент интерфейса с помощью копирующего конструктора
        elementInterface = QSharedPointer<ElementInterface>(new ElementInterface(*elementInterface.data()));
        // Получаем образец графического элемента
        graphicsItem = mProject.data()->controller(mCurrentTab).data()->graphicsItem(elementNum);
        // Сохраняем координаты графического элемента (на случай перемещения элемента)
        elementInterface.data()->setY(graphicsItem.data()->y());
        elementInterface.data()->setY(graphicsItem.data()->y());
        break;
    case ElementEditDialogType::Edit:
        // Получаем элемент интерфейса и графики
        elementInterface = mProject.data()->controller(mCurrentTab).data()->elementInterface(elementNum);
        graphicsItem = mProject.data()->controller(mCurrentTab).data()->graphicsItem(elementNum);
        // Сохраняем измененные координаты
        elementInterface.data()->setX(graphicsItem.data()->x());
        elementInterface.data()->setY(graphicsItem.data()->y());
        break;
    case ElementEditDialogType::Remove:
        // Удаление GUI-модуля со сцены
        graphicsItem = mProject.data()->controller(mCurrentTab).data()->graphicsItem(elementNum);
        elementInterface = mProject.data()->controller(mCurrentTab).data()->elementInterface(elementNum);
        // Настройка взаимодействия между элементами и всплывающими подсказками
        disconnect(elementInterface.data(),
                   SIGNAL(signalSendToopTipInfo(QString,int,QVector<QString>,QVector<QString>,QVector<QString>,QPoint)),
                   toolTip,
                   SLOT(slotReceiveToopTipInfo(QString,int,QVector<QString>,QVector<QString>,QVector<QString>,QPoint)));
        disconnect(elementInterface.data(),
                   SIGNAL(signalSendToolTipInfo(QString,int,QVector<QString>)),
                   toolTip,
                   SLOT(slotReceiveToolTipInfo(QString,int,QVector<QString>)));
        disconnect(toolTip,
                   SIGNAL(signalToolTipHide()),
                   elementInterface.data(),
                   SLOT(slotToolTipHide()));
        disconnect(toolTip,
                   SIGNAL(signalToolTipHide()),
                   graphicsItem.data(),
                   SLOT(slotToolTipHide()));
        if (graphicsItem.data()->scene() != 0)
            mProject.data()->scene(mCurrentTab).data()->removeItem(graphicsItem.data());
        // Удаление интерфейсного модуля
        mProject.data()->controller(mCurrentTab).data()->removeElementInterface(elementNum);
        // Удаление строки в таблице
        tblElementsList->removeRow(elementNum);
        // Сохранение обновленной информации
        if (mProject.data()->isSavedToDisc())
            mProject.data()->controller(mCurrentTab).data()->save();

        return 0;
    }

    // Работа с диалогом конструктора
    elementEditDialog = new ElementEditDialog(this);
    elementEditDialog->setModulesList(mProject.data()->controller(mCurrentTab).data()->modulesList());
    elementEditDialog->setElementInterface(elementInterface);
    elementEditDialog->resize(mProgramSettings.data()->elementEditDialogSize());

    connect(elementEditDialog, SIGNAL(signalChangeModule(QString)),
            this, SLOT(slotRequestModule(QString)));

    qint32 dialogResult = elementEditDialog->exec();

    disconnect(elementEditDialog, SIGNAL(signalChangeModule(QString)),
               this, SLOT(slotRequestModule(QString)));

    // Модификация и сохранение размера диалогового окна
    if (mProgramSettings.data()->elementEditDialogSize() != elementEditDialog->size())
    {
        mProgramSettings.data()->setElementEditDialogSize(elementEditDialog->size());
        mProgramSettings.data()->save();
    }

    elementInterface = elementEditDialog->getElementInterface();

    elementEditDialog->deleteLater();
    elementEditDialog = 0;

    if (dialogResult != QDialog::Accepted)
        return 0;

    // Проверка корректности измененного/созданного элемента
    elementInterface.data()->checkCorrect();
    // Сброс начального состояния
    elementInterface.data()->resetState();

    // Обработка результата
    switch (type)
    {
    case ElementEditDialogType::Add:
    case ElementEditDialogType::Copy:
        mProject.data()->controller(mCurrentTab).data()->addElementInterface(elementInterface);
        this->addElementInTable(elementInterface.data()->fullModuleName(),
                                elementInterface.data()->elementName(),
                                elementInterface.data()->startByte(),
                                elementInterface.data()->bytesCount(),
                                elementInterface.data()->moduleState());
        if (elementInterface.data()->moduleState() != ModuleState::Critical)
        {
            // Получение созданного GUI-элемента
            graphicsItem = mProject.data()->controller(mCurrentTab).data()->graphicsItem(mProject.data()->controller(mCurrentTab).data()->elementsCount() - 1);
            // Изменения в графическом представлении элемента
            graphicsItem.data()->setX(elementInterface.data()->x());
            graphicsItem.data()->setY(elementInterface.data()->y());
            // Добавление созданного GUI-элемента на сцену
            mProject.data()->scene(mCurrentTab).data()->addItem(graphicsItem.data());
            // Настройка взаимодействия между элементами и всплывающими подсказками
            connect(elementInterface.data(),
                    SIGNAL(signalSendToopTipInfo(QString,int,QVector<QString>,QVector<QString>,QVector<QString>,QPoint)),
                    toolTip,
                    SLOT(slotReceiveToopTipInfo(QString,int,QVector<QString>,QVector<QString>,QVector<QString>,QPoint)));
            connect(elementInterface.data(),
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
        break;
    case ElementEditDialogType::Edit:
        mProject.data()->controller(mCurrentTab).data()->changeElementInterface(elementNum, elementInterface);

        if (elementInterface.data()->moduleState() != ModuleState::Critical)
        {
            // Получение созданного GUI-элемента
            graphicsItem = mProject.data()->controller(mCurrentTab).data()->graphicsItem(elementNum);
            // Изменения в графическом представлении элемента
            graphicsItem.data()->setX(elementInterface.data()->x());
            graphicsItem.data()->setY(elementInterface.data()->y());
            if (graphicsItem.data()->scene() == 0)
            {
                // Добавление созданного GUI-элемента на сцену
                mProject.data()->scene(mCurrentTab).data()->addItem(graphicsItem.data());
                // Настройка взаимодействия между элементами и всплывающими подсказками
                connect(elementInterface.data(),
                        SIGNAL(signalSendToopTipInfo(QString,int,QVector<QString>,QVector<QString>,QVector<QString>,QPoint)),
                        toolTip,
                        SLOT(slotReceiveToopTipInfo(QString,int,QVector<QString>,QVector<QString>,QVector<QString>,QPoint)));
                connect(elementInterface.data(),
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
        }

        // Изменения в интерфейсе элемента
        this->changeElementInTable(elementNum,
                                   elementInterface.data()->fullModuleName(),
                                   elementInterface.data()->elementName(),
                                   elementInterface.data()->startByte(),
                                   elementInterface.data()->bytesCount(),
                                   elementInterface.data()->moduleState());
        break;
    case ElementEditDialogType::Remove:
        break;
    }

    if (mProject.data()->isSavedToDisc())
        mProject.data()->controller(mCurrentTab).data()->save();

    return 0;
}



// Добавление элемента в таблицу
int MainWindow::addElementInTable(QString moduleName, QString elementName,
                                  quint16 minByte, quint16 bytesCount,
                                  ModuleState::State state)
{
    // Номер добавляемого элемента
    int index = tblElementsList->rowCount();
    // Добавляем строку в таблицу
    tblElementsList->insertRow(index);

    QIcon icon;
    switch (state)
    {
    case ModuleState::Success:
        icon = QIcon(":/images/module_success.png");
        break;
    case ModuleState::Warning:
        icon = QIcon(":/images/module_warning.png");
        break;
    case ModuleState::Critical:
        icon = QIcon(":/images/module_critical.png");
        break;
    }

    // Иконка
    tblElementsList->setItem(index, 0, new QTableWidgetItem(icon, ""));
    tblElementsList->item(index, 0)->setTextAlignment(Qt::AlignCenter);
    // Номер
    tblElementsList->setItem(index, 1, new QTableWidgetItem(QString::number(index)));
    tblElementsList->item(index, 1)->setTextAlignment(Qt::AlignCenter);
    // Модуль
    tblElementsList->setItem(index, 2, new QTableWidgetItem(moduleName));
    tblElementsList->item(index, 2)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    // Название элемента
    tblElementsList->setItem(index, 3, new QTableWidgetItem(elementName));
    tblElementsList->item(index, 3)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    // Стартовый байт
    tblElementsList->setItem(index, 4, new QTableWidgetItem(QString::number(minByte)));
    tblElementsList->item(index, 4)->setTextAlignment(Qt::AlignCenter);
    // Количество байт
    tblElementsList->setItem(index, 5, new QTableWidgetItem(QString::number(bytesCount)));
    tblElementsList->item(index, 5)->setTextAlignment(Qt::AlignCenter);

    return 0;
}



// Изменение элемента в таблице
int MainWindow::changeElementInTable(qint16 elementNum, QString moduleName,
                                     QString elementName, quint16 minByte,
                                     quint16 bytesCount, ModuleState::State state)
{
    if (elementNum >= tblElementsList->rowCount())
        return -1;

    QIcon icon;
    switch (state)
    {
    case ModuleState::Success:
        icon = QIcon(":/images/module_success.png");
        break;
    case ModuleState::Warning:
        icon = QIcon(":/images/module_warning.png");
        break;
    case ModuleState::Critical:
        icon = QIcon(":/images/module_critical.png");
        break;
    }

    // Иконка
    tblElementsList->item(elementNum, 0)->setIcon(icon);
    // Имя модуля
    tblElementsList->item(elementNum, 2)->setText(moduleName);
    // Название элемента
    tblElementsList->item(elementNum, 3)->setText(elementName);
    // Минимальный байт
    tblElementsList->item(elementNum, 4)->setText(QString::number(minByte));
    // Количество байт
    tblElementsList->item(elementNum, 5)->setText(QString::number(bytesCount));

    return 0;
}



// Заполнение таблицы с элементами (вызов при смене вкладки)
int MainWindow::generateTable(int currentTab)
{
    QSharedPointer<ElementInterface> elementInterface;

    // Сброс содержимого таблицы
    tblElementsList->clearContents();
    // Установка стартовой строки в таблице
    tblElementsList->setRowCount(0);

    // Заполнение таблицы с зарегистрированными элементами
    int elementsCount = mProject.data()->controller(currentTab).data()->elementsCount();
    for (int i = 0; i < elementsCount; i++)
    {
        elementInterface = mProject.data()->controller(currentTab).data()->elementInterface(i);
        addElementInTable(elementInterface.data()->fullModuleName(),
                          elementInterface.data()->elementName(),
                          elementInterface.data()->startByte(),
                          elementInterface.data()->bytesCount(),
                          elementInterface.data()->moduleState());
    }

    return 0;
}



// Изменение номера вкладки
// Обработка щелчка по кнопке "Добавить вкладку"
int MainWindow::slotCurrentTabChanged(int index)
{
    if (mIsBlockRemoveTabSignals)
        return 0;

    // Условие - щелчок произведен на кнопку "Добавить вкладку"
    bool exp1 = index == tabMain->count();
    // Условие - количество открытых вкладок менее mMaxTabsCount (mMaxTabsCount - лимит)
    bool exp2 = tabMain->count() < mMaxTabsCount;
    // Добавление новой вкладки
    if (exp1 && exp2)
    {
        // Добавление новой вкладки
        this->slotAddTab();
        // Переключение на созданную вкладку
        tabMain->setCurrentIndex(index);
        // Центрирование
        mProject.data()->view(mCurrentTab).data()->centerView();
        return 0;
    }
    // Добавление новой вкладки невозможно
    else if (exp1)
    {
        tabMain->setCurrentIndex(mCurrentTab);
        return -1;
    }

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

    // Установка возможности перемещения элементов сцены
    // (в зависимости от панели конструктора)
    int elementsCount = mProject.data()->controller(mCurrentTab).data()->elementsCount();
    QSharedPointer<GraphicsObject> graphicsItem;
    for (int i = 0; i < elementsCount; i++)
    {
        // Разрешение перемещения элементов
        graphicsItem = mProject.data()->controller(mCurrentTab).data()->graphicsItem(i);
        graphicsItem.data()->setFlag(QGraphicsItem::ItemIsMovable,
                                     wgtConstructor->isEnabled());
    }

    // Восстановление положения масштаба
    sldrZoom->setValue(mProject.data()->view(mCurrentTab).data()->scaleFactor());

    // Формирование таблицы
    this->generateTable(mCurrentTab);

    // Изменение UI
    if (mProject.data()->network(mCurrentTab).data()->isConnected())
    {
        acRequestData->setText("&Отключить");
        btnClientState->setText("Отключить");
        leIndicator->setStyleSheet("QLineEdit {background-color: rgb(93, 255, 93);}");
        // Модификация UI
        btnAddElement->setEnabled(false);
        btnCopyElement->setEnabled(false);
        btnEditElement->setEnabled(false);
        btnRemoveElement->setEnabled(false);
        lblEditDisabled->setVisible(true);
    }
    else
    {
        acRequestData->setText("&Запрос данных");
        btnClientState->setText("Запрос данных");
        leIndicator->setStyleSheet("QLineEdit {background-color: rgb(255, 93, 93);}");
        // Модификация UI
        btnAddElement->setEnabled(true);
        btnCopyElement->setEnabled(false);
        btnEditElement->setEnabled(false);
        btnRemoveElement->setEnabled(false);
        lblEditDisabled->setVisible(false);
    }

    return 0;
}



// Добавление новой вкладки
int MainWindow::slotAddTab()
{
    // Отправка сигнала - "Проект изменен"
    emit signalProjectChanged();

    // Получение нового идентификатора вкладки
    int index = mProject.data()->addUnit();

    // Добавление вкладки
    tabMain->addTab(mProject.data()->view(index).data(),
                    mProject.data()->sceneSettings(index).data()->sceneName());

    connect(mProject.data()->network(index), SIGNAL(signalConnectionResult(bool)),
            this, SLOT(slotConnectionResult(bool)));

    connect(mProject.data()->controller(index).data(), SIGNAL(signalModulesListChanged(int, QStringList)),
            this, SLOT(slotModulesListChanged(int, QStringList)));

    connect(mProject.data()->controller(index), SIGNAL(signalElementInterfaceChanged(int, int)),
            this, SLOT(slotElementInterfaceChanged(int, int)));

    connect(mProject.data()->controller(index), SIGNAL(signalGraphicsObjectSelect(int)),
            this, SLOT(slotGraphicsObjectSelected(int)));

    connect(mProject.data()->view(index), SIGNAL(signalSetScaleFactor(int)),
            sldrZoom, SLOT(setValue(int)));

    // Сохранение проекта
    mProject.data()->save();

    return 0;
}



int MainWindow::slotRemoveTab(int index)
{
    // Предупреждение об удалении вкладки
    QMessageBox msgBox;
    msgBox.setText("Вы действительно хотите удалить вкладку?");
    msgBox.setWindowTitle("Удаление вкладки");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setIcon(QMessageBox::Question);

    switch (msgBox.exec())
    {
    case QMessageBox::Yes:
        break;
    case QMessageBox::No:
        return 0;
    }

    // Отправка сигнала - "Проект изменен"
    emit signalProjectChanged();

    // Если закрываем последнюю вкладку - открываем пустую новую
    if (tabMain->count() == 1)
        this->slotAddTab();

    // При удалении текущей вкладки - открытие предыдущей вкладки
    bool exp1 = mCurrentTab == index;
    bool exp2 = index != 0;
    // Удаляется не первая вкладка. Фокус перебрасывается на предыдущую
    if (exp1 && exp2)
    {
        mCurrentTab--;
        tabMain->setCurrentIndex(mCurrentTab);
    }
    //    // Удаляется первая вкладка. Фокус перебрасывается на следующую
    //    else if (exp1)
    //    {
    //        tabMain->setCurrentIndex(mCurrentTab);
    //    }

    // Отключение логики от GUI
    disconnect(mProject.data()->network(index).data(), SIGNAL(signalConnectionResult(bool)),
               this, SLOT(slotConnectionResult(bool)));

    disconnect(mProject.data()->controller(index).data(), SIGNAL(signalModulesListChanged(int, QStringList)),
               this, SLOT(slotModulesListChanged(int, QStringList)));

    disconnect(mProject.data()->controller(index).data(), SIGNAL(signalElementInterfaceChanged(int, int)),
               this, SLOT(slotElementInterfaceChanged(int, int)));

    disconnect(mProject.data()->controller(index).data(), SIGNAL(signalGraphicsObjectSelect(int)),
               this, SLOT(slotGraphicsObjectSelected(int)));

    disconnect(btnFitInView, SIGNAL(clicked()),
               mProject.data()->view(index).data(), SLOT(slotFitInView()));

    disconnect(sldrZoom, SIGNAL(valueChanged(int)),
               mProject.data()->view(index).data(), SLOT(slotSetScaleFactor(int)));

    disconnect(mProject.data()->view(index).data(), SIGNAL(signalSetScaleFactor(int)),
               sldrZoom, SLOT(setValue(int)));

    // Удаление графических элементов со сцены
    this->hideGraphicsItems(index);

    // Удаление вкладки
    tabMain->removeTab(index);

    // Удаление логики
    mProject.data()->removeUnit(index);

    // Повторное заполнение таблицы элементов
    this->generateTable(mCurrentTab);

    // Сохранений проекта
    mProject.data()->save();

    return 0;
}



// Обработчик события "Запрос интерфейса из окна конструктора"
int MainWindow::slotRequestModule(QString moduleName)
{
    if (elementEditDialog != 0)
        elementEditDialog->setElementInterface(mProject.data()->controller(mCurrentTab).data()->elementInterface(moduleName));
    return 0;
}



// Обработчик события "Список модулей изменился"
int MainWindow::slotModulesListChanged(int id, QStringList modulesList)
{
    bool exp1 = elementEditDialog != 0;
    bool exp2 = id == mCurrentTab;
    if (exp1 && exp2)
        elementEditDialog->setModulesList(modulesList);

    return 0;
}



// Добавить элемент (конструктор)
int MainWindow::slotAddElement()
{
    showElementEditDialog(ElementEditDialogType::Add);
    return 0;
}



// Копировать выделенный элемент (конструктор)
int MainWindow::slotCopyElement()
{
    showElementEditDialog(ElementEditDialogType::Copy);
    return 0;
}



// Редактировать выделенный элемент (конструктор)
int MainWindow::slotEditElement()
{
    showElementEditDialog(ElementEditDialogType::Edit);
    return 0;
}



// Удалить элемент из списка (конструктор)
int MainWindow::slotRemoveElement()
{
    showElementEditDialog(ElementEditDialogType::Remove);
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

    return mProject.data()->openProject(path);
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
    int result = openProjectDialog.exec();

    if (result == QDialog::Rejected)
        return -1;

    QString path = openProjectDialog.selectedFiles().first();

    // Закрытие проекта, при необходимости
    if (this->isProjectOpened())
        this->closeProject();
    // Попытка открытия проекта по пути path
    if (!this->openProject(path))
        this->createProject();
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

        disconnect(mProject.data()->controller(i).data(), SIGNAL(signalModulesListChanged(int, QStringList)),
                   this, SLOT(slotModulesListChanged(int, QStringList)));

        disconnect(mProject.data()->controller(i).data(), SIGNAL(signalElementInterfaceChanged(int, int)),
                   this, SLOT(slotElementInterfaceChanged(int, int)));

        disconnect(mProject.data()->controller(i).data(), SIGNAL(signalGraphicsObjectSelect(int)),
                   this, SLOT(slotGraphicsObjectSelected(int)));

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

    return 0;
}



// Сохранение проекта
bool MainWindow::saveProject(const QString projectPath)
{
    bool result = mProject.data()->save(projectPath);

    // Сохранение обновленного проекта
    if (result)
    {
        mProgramSettings.data()->setLastOpenedProject(mProject.data()->projectDirectory());
        mProgramSettings.data()->save();

        // Обновление заголовка
        this->updateWindowTitle();
    }

    return result;
}



// Слот - "Закрытие проекта без сохранения"
int MainWindow::slotCloseProject()
{
    if (!mProject.data()->isSavedToDisc() && mProject.data()->isProjectModified())
    {
        // Предупреждение о закрытии проекта без сохранения
        QMessageBox msgBox;
        msgBox.setText("Сохранить изменения в проекте?");
        msgBox.setWindowTitle("Закрытие проекта");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        msgBox.setIcon(QMessageBox::Question);

        switch (msgBox.exec())
        {
        case QMessageBox::Yes:
            this->slotSaveProject();
            break;
        case QMessageBox::No:
            break;
        case QMessageBox::Cancel:
            return 0;
        }
    }
    else if (mProject.data()->isSavedToDisc() && mProject.data()->isProjectModified())
    {
        this->slotSaveProject();
    }

    // Закрытие проекта, в случае необходимости
    if (this->isProjectOpened())
        this->closeProject();
    // Создание нового проекта
    this->createProject();
    // Инициализация проекта
    this->initProject();

    // Установка заголовка окна
    this->updateWindowTitle();

    // Сброс интерфейса
    this->slotConnectionResult(false);

    return 0;
}



// Процедура, выполняющая создание нового проекта
bool MainWindow::createProject()
{
    if (mProject.isNull())
    {
        this->showMessage(QString("Ошибка при создании проекта!"), QMessageBox::Critical);
        return false;
    }

    mProject.data()->createProject();

    return 0;
}



// Слот - "Создание проекта"
int MainWindow::slotCreateProject()
{
    // Закрытие проекта, в случае необходимости
    if (this->isProjectOpened())
        this->closeProject();
    // Создание проекта
    this->createProject();
    // Инициализация проекта
    this->initProject();

    // Установка заголовка окна
    this->updateWindowTitle();

    return 0;
}



int MainWindow::slotSaveAsProject()
{
    FileDialog saveProjectDialog(this);
    saveProjectDialog.setWindowTitle("Сохранение проекта");
    saveProjectDialog.setDirectory(qApp->applicationDirPath());
    saveProjectDialog.setFileMode(QFileDialog::Directory);
    saveProjectDialog.setOption(QFileDialog::ShowDirsOnly, true);
    saveProjectDialog.setOption(QFileDialog::DontResolveSymlinks, true);
    saveProjectDialog.setAcceptMode(QFileDialog::AcceptSave);
    saveProjectDialog.setLabelText(QFileDialog::Accept, "&Сохранить проект");
    int result = saveProjectDialog.exec();

    if (result == QDialog::Rejected)
        return false;

    QString path = saveProjectDialog.selectedFiles().first();

    if (mProject.isNull())
    {
        this->showMessage("Ошибка при сохранении проекта!", QMessageBox::Critical);
        return false;
    }

    return this->saveProject(path);
}



// Сохранение проекта
// Если проект не был сохранен ранее - вызов диалогового окна
int MainWindow::slotSaveProject()
{
    if (mProject.isNull())
    {
        this->showMessage(QString("Ошибка при сохранении проекта!"), QMessageBox::Warning);
        return false;
    }


    if (mProject.data()->isSavedToDisc())
        return this->saveProject(mProject.data()->projectDirectory());
    else
        return this->slotSaveAsProject();
}



// Инициализация проекта. Настройка связи между GUI и логикой
bool MainWindow::initProject()
{
    if (mProject.isNull())
    {
        this->showMessage(QString("Ошибка при инициализации проекта!"), QMessageBox::Critical);
        return false;
    }

    // Основная инициализация ресурсов
    for (int i = 0; i < mProject.data()->unitsCount(); i++)
    {
        // Добавление вкладки
        tabMain->addTab(mProject.data()->view(i).data(),
                        mProject.data()->sceneSettings(i).data()->sceneName());

        // Связи, уникальные для каждой вкладки
        connect(mProject.data()->network(i).data(), SIGNAL(signalConnectionResult(bool)),
                this, SLOT(slotConnectionResult(bool)));

        connect(mProject.data()->controller(i).data(), SIGNAL(signalModulesListChanged(int, QStringList)),
                this, SLOT(slotModulesListChanged(int, QStringList)));

        connect(mProject.data()->controller(i).data(), SIGNAL(signalElementInterfaceChanged(int, int)),
                this, SLOT(slotElementInterfaceChanged(int, int)));

        connect(mProject.data()->controller(i).data(), SIGNAL(signalGraphicsObjectSelect(int)),
                this, SLOT(slotGraphicsObjectSelected(int)));

        connect(mProject.data()->view(i).data(), SIGNAL(signalSetScaleFactor(int)),
                sldrZoom, SLOT(setValue(int)));

        // Добавление на сцену элементов
        // Установка минимального размера сцены
        this->showGraphicsItems(i);
    }

    // Текущая открытая вкладка
    mCurrentTab = 0;
    tabMain->setCurrentIndex(mCurrentTab);

    // Подключение нового сигнала масштабирования для слайдера
    connect(sldrZoom, SIGNAL(valueChanged(int)),
            mProject.data()->view(mCurrentTab).data(), SLOT(slotSetScaleFactor(int)));
    // Подключение нового сигнала масштабирования для кнопки "Вписать в окно"
    connect(btnFitInView, SIGNAL(clicked()),
            mProject.data()->view(mCurrentTab).data(), SLOT(slotFitInView()));
    // Подключение сигнала сброса отображения всплывающей подсказки при масштабировании
    connect(mProject.data()->view(mCurrentTab).data(), SIGNAL(signalWheelEvent()),
            toolTip, SIGNAL(signalToolTipHide()));

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
// Установка марки "[Проект] Не соханен", в случае необходимости
bool MainWindow::updateWindowTitle()
{
    if (mProject.data() == 0)
        return false;

    const QString defaultTitle = QString("Конструктор элементов v.%1 - ").arg(QString::number(VERSION, 'f', 2));
    const QString unsavedProject(" (Не сохранен)");

    if (mProject.data()->isSavedToDisc())
        this->setWindowTitle(defaultTitle + mProject.data()->projectName());
    else if (!mProject.data()->isSavedToDisc())
        this->setWindowTitle(defaultTitle + mProject.data()->projectName() + unsavedProject);

    return true;
}



// Слот - "Закрытие приложения"
int MainWindow::slotCloseApp()
{
    bool result = this->saveProgramSettings();

    if (result)
        qApp->quit();

    return 0;
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
