#include "configdialog.h"

ConfigDialog::ConfigDialog(bool isServerRunning, QWidget *parent) :
    QDialog(parent),
    mIsServerRunning(isServerRunning),
    mIsDataSave(false)
{
    // Лейауты
    ltMain = new QVBoxLayout();
    ltButtons = new QHBoxLayout();

    // Минимальные размеры для GroupBox
    const quint16 minGbWidth = 316;
    const quint16 minGbNetConfigHeight = 121;
    const quint16 minGbDatabaseHeight = 75;

    // GroupBox с настройками сети
    gbNetConfig = new QGroupBox(this);
    gbNetConfig->setTitle("Сеть");
    gbNetConfig->setMinimumSize(minGbWidth, minGbNetConfigHeight);
    ltNetConfig = new QGridLayout(gbNetConfig);
    gbNetConfig->setLayout(ltNetConfig);

    // GroupBox с настройками доступа к базе
    gbDatabase = new QGroupBox(this);
    gbDatabase->setTitle("База данных");
    gbDatabase->setMinimumSize(minGbWidth, minGbDatabaseHeight);
    ltDatabase = new QGridLayout(gbDatabase);
    gbDatabase->setLayout(ltDatabase);

    // Сетевой интерфейс (метка)
    lblNetInterface = new QLabel(gbNetConfig);
    lblNetInterface->setText("Сетевой интерфейс:");

    // Сетевой интерфейс (выпадающий список)
    cbNetInterface = new QComboBox(gbNetConfig);
    cbNetInterface->setFocus();

    // Порт (метка)
    lblPort = new QLabel(gbNetConfig);
    lblPort->setText("Порт:");

    // Порт (поле для ввода)
    lePort = new QLineEdit(gbNetConfig);
    lePort->setText(QString::number(mPort));
    lePort->setValidator(new QIntValidator(0, 65535, lePort));
    lePort->installEventFilter(new LineEditEventFilter(mPort, lePort));

    // Флажок - активация сервера при запуске программы
    chbStartServer = new QCheckBox(gbNetConfig);
    chbStartServer->setText("Автоматический запуск сервера");
    chbStartServer->setChecked(mIsServerAutorun);

    // Сеть, уведомления (метка)
    lblNetNotification = new QLabel(gbNetConfig);

    // БД, уведомления (метка)
    lblDBNotification = new QLabel(gbDatabase);

    // База данных (метка)
    lblDatabase = new QLabel(gbDatabase);
    lblDatabase->setText("Расположение:");

    // База данных (поле для ввода)
    leDatabase = new QLineEdit(gbDatabase);

    // База данных (кнопка)
    btnBrowseDB = new QPushButton(gbDatabase);
    btnBrowseDB->setText("Обзор");

    // Кнопка "Применить"
    btnApply = new QPushButton(this);
    btnApply->setText("Применить");
    btnApply->setEnabled(false);

    // Кнопка "Отмена"
    btnCancel = new QPushButton(this);
    btnCancel->setText("Отмена");

    // Кнопка "ОК"
    btnOk = new QPushButton(this);
    btnOk->setText("ОК");
    btnOk->setDefault(true);

    // Настройка лейаута (сеть)
    ltNetConfig->addWidget(lblNetInterface, 0, 0, 1, 1);
    ltNetConfig->addWidget(cbNetInterface, 0, 1, 1, 1);
    ltNetConfig->addWidget(lblPort, 1, 0, 1, 1);
    ltNetConfig->addWidget(lePort, 1, 1, 1, 1);
    ltNetConfig->addWidget(chbStartServer, 2, 0, 1, 2);
    ltNetConfig->setRowStretch(3, 1);
    ltNetConfig->addWidget(lblNetNotification, 4, 0, 1, 2);

    // Настройка лейаута (база данных)
    ltDatabase->addWidget(lblDatabase, 0, 0, 1, 2);
    ltDatabase->addWidget(leDatabase, 1, 0, 1, 1);
    ltDatabase->addWidget(btnBrowseDB, 1, 1, 1, 1);
    ltDatabase->setRowStretch(2, 1);
    ltDatabase->addWidget(lblDBNotification, 3, 0, 1, 2);

    // Настройка лейаута (управляющие кнопки)
    ltButtons->addStretch();
    ltButtons->addWidget(btnOk);
    ltButtons->addWidget(btnCancel);
    ltButtons->addWidget(btnApply);

    // Настройка главного лейаута
    ltMain->addWidget(gbNetConfig);
    ltMain->addWidget(gbDatabase);
    ltMain->addLayout(ltButtons);

    // Параметры диалогового окна
    this->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    this->setWindowTitle("Настройки");
    this->setLayout(ltMain);
    this->setFocusPolicy(Qt::NoFocus);

    // Расстановка tab-индексов
    QWidget::setTabOrder(cbNetInterface, lePort);
    QWidget::setTabOrder(lePort, chbStartServer);
    QWidget::setTabOrder(chbStartServer, leDatabase);
    QWidget::setTabOrder(leDatabase, btnBrowseDB);
    QWidget::setTabOrder(btnBrowseDB, btnOk);
    QWidget::setTabOrder(btnOk, btnCancel);
    QWidget::setTabOrder(btnCancel, btnApply);
    QWidget::setTabOrder(btnApply, cbNetInterface);

    // Отслеживание изменений в значениях выпадающего списка
    connect(cbNetInterface,
            SIGNAL(activated(int)),
            this,
            SLOT(slotNetConfigChanged(int)));

    // Отслеживание изменений в номере порта
    connect(lePort,
            SIGNAL(textEdited(QString)),
            this,
            SLOT(slotNetConfigChanged(QString)));

    // Отслеживание изменений во флажке
    connect(chbStartServer,
            SIGNAL(clicked(bool)),
            this,
            SLOT(slotNetConfigChanged (bool)));

    // Щелчок на кнопке "Применить"
    connect(btnApply,
            SIGNAL(clicked()),
            this,
            SLOT(slotBtnApplyClick()));

    // Щелчок на кнопке "Отмена"
    connect(btnCancel,
            SIGNAL(clicked()),
            this,
            SLOT(slotBtnCancelClick()));

    // Щелчок на кнопке "Ок"
    connect(btnOk,
            SIGNAL(clicked()),
            this,
            SLOT(slotBtnOkClick()));

    // Щелчок на кнопке "Обзор" (БД)
    connect(btnBrowseDB,
            SIGNAL(clicked()),
            this,
            SLOT(slotBtnBrowseDBClick()));

    connect(leDatabase,
            SIGNAL(textEdited(QString)),
            this,
            SLOT(slotDBPathChanged(QString)));
}



ConfigDialog::~ConfigDialog()
{
    ltMain->deleteLater();
    ltButtons->deleteLater();
}



// Слот - изменение настроек (тип 1)
qint8 ConfigDialog::slotNetConfigChanged (int)
{
    if (mIsServerRunning)
        lblNetNotification->setText("<span style='color:red;'>Изменения вступят в силу после перезапуска сервера.</span>");
    btnApply->setEnabled(true);
    return 0;
}



// Слот - изменение настроек (тип 2)
qint8 ConfigDialog::slotNetConfigChanged (QString)
{
    if (mIsServerRunning)
        lblNetNotification->setText("<span style='color:red;'>Изменения вступят в силу после перезапуска сервера.</span>");
    btnApply->setEnabled(true);
    return 0;
}



// Слот - изменение настроек (тип 3)
qint8 ConfigDialog::slotNetConfigChanged (bool)
{
    if (mIsServerRunning)
        lblNetNotification->setText("<span style='color:red;'>Изменения вступят в силу после перезапуска сервера.</span>");
    btnApply->setEnabled(true);
    return 0;
}



// Слот - щелчок по кнопке "Применить"
qint8 ConfigDialog::slotBtnApplyClick()
{
    mPort = lePort->text().toInt();
    mNetInterface = cbNetInterface->currentText();
    mIsServerAutorun = chbStartServer->isChecked();
    // Получение относительного пути для файла c БД
    QDir appDir(qApp->applicationDirPath());
    mDBPath = appDir.relativeFilePath(leDatabase->text());

    btnApply->setEnabled(false);
    btnOk->setFocus();

    mIsDataSave = true;

    return 0;
}



// Слот - щелчок по кнопке "ОК"
qint8 ConfigDialog::slotBtnOkClick()
{
    mPort = lePort->text().toInt();
    mNetInterface = cbNetInterface->currentText();
    mIsServerAutorun = chbStartServer->isChecked();
    // Получение относительного пути для файла c БД
    QDir appDir(qApp->applicationDirPath());
    mDBPath = appDir.relativeFilePath(leDatabase->text());

    this->accept();

    return 0;
}



// Слот - щелчок по кнопке "Отмена"
qint8 ConfigDialog::slotBtnCancelClick()
{
    if (mIsDataSave)
        this->accept();
    else
        this->reject();

    return 0;
}



// Слот - щелчок по кнопке "Обзор" (база данных)
qint8 ConfigDialog::slotBtnBrowseDBClick()
{
    QString dialogResult = QFileDialog::getOpenFileName(this,
                                                        QString("Открыть базу данных"),
                                                        QString(),
                                                        QString("База данных Access (*.mdb *.accdb)"));
    if (!dialogResult.isEmpty())
    {
        // Установка пути до выбранного файла
        leDatabase->setText(dialogResult);
        // Очистка поля для уведомлений
        lblDBNotification->setText("");
        // Разблокировка кнопки "Применить"
        btnApply->setEnabled(true);
    }

    return 0;
}



// Слот - изменение расположения БД
qint8 ConfigDialog::slotDBPathChanged(QString path)
{
    QFileInfo fileInfo(path);
    bool isExist = fileInfo.exists();
    bool isCorrectSuffix = (fileInfo.suffix() == "mdb") || (fileInfo.suffix() == "accdb");
    if (isExist && isCorrectSuffix)
        lblDBNotification->setText("");
    else
        lblDBNotification->setText("<span style='color:red;'>База данных не найдена!</span>");

    btnApply->setEnabled(true);

    return 0;
}


// Установка вариантов для выпадающего списка "Сетевой интерфейс"
qint8 ConfigDialog::setNetInterface(QStringList interfaces, QString interface)
{
    cbNetInterface->addItems(interfaces);
    qint8 index = cbNetInterface->findText(interface);
    if (index < 0)
        cbNetInterface->setCurrentIndex(0);
    else
        cbNetInterface->setCurrentIndex(index);

    return 0;
}



// Получение информации о выбранном сетевом интерфейсе
QString ConfigDialog::getNetInterface()
{
    return mNetInterface;
}



// Установка параметров автозапуска сервера
qint8 ConfigDialog::setServerAutorun(bool autorun)
{
    mIsServerAutorun = autorun;
    chbStartServer->setChecked(autorun);
    return 0;
}



// Получение параметров автозапуска сервера
bool ConfigDialog::getServerAutorun()
{
    return mIsServerAutorun;
}



// Установка пути к БД
qint8 ConfigDialog::setDBPath(QString dbPath)
{
    if (!dbPath.isEmpty())
    {
        QDir appDir(dbPath);
        appDir.makeAbsolute();
        mDBPath = appDir.path();
    }
    else
    {
        mDBPath = dbPath;
    }

    leDatabase->setText(mDBPath);
    if (!QFile(mDBPath).exists())
        lblDBNotification->setText("<span style='color:red;'>База данных не найдена!</span>");
    else
        lblDBNotification->setText("");

    return 0;
}



// Расположение БД
QString ConfigDialog::getDBPath()
{
    return mDBPath;
}



// Установка рабочего порта сервера
qint8 ConfigDialog::setPort(quint16 port)
{
    mPort = port;
    lePort->setText(QString::number(port));
    return 0;
}



// Получение информации о рабочем порте сервера
quint16 ConfigDialog::getPort()
{
    return mPort;
}
