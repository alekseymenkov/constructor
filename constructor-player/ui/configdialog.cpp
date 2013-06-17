#include "configdialog.h"



ConfigDialog::ConfigDialog(bool isClientRunning, int isBlockMode, QWidget *parent) :
    QDialog(parent),
    mIsClientRunning(isClientRunning),
    mIsDataSave(false)

{
    // Минимальные размеры для GroupBox
    const int minGbWidth = 366;
    const int minGbRegConfigHeight = 124;
    const int minGbAppearanceHeight = 79;
    const int minGbUpdateHeight = 92;

    // Лейауты
    ltMain = new QVBoxLayout();
    ltButtons = new QHBoxLayout();

    // GroupBox с настройками сети
    gbRegConfig = new QGroupBox(this);
    gbRegConfig->setTitle("Регистрационные данные");
    gbRegConfig->setMinimumSize(minGbWidth, minGbRegConfigHeight);
    ltRegConfig = new QGridLayout(gbRegConfig);
    gbRegConfig->setLayout(ltRegConfig);

    // GroupBox с настройками внешнего вида
    gbAppearance = new QGroupBox(this);
    gbAppearance->setTitle("Общие настройки");
    gbAppearance->setMinimumSize(minGbWidth, minGbAppearanceHeight);
    ltAppearance = new QGridLayout(gbAppearance);
    gbAppearance->setLayout(ltAppearance);

    // GroupBox с прочими настройками
    gbUpdate = new QGroupBox(this);
    gbUpdate->setTitle("Прочие настройки");
    gbUpdate->setMinimumSize(minGbWidth, minGbUpdateHeight);
    ltUpdate = new QGridLayout(gbUpdate);
    gbUpdate->setLayout(ltUpdate);
    if (isBlockMode)
        gbUpdate->hide();

    // Название проекта
    lblProjectName = new QLabel(gbAppearance);
    lblProjectName->setText("Название проекта:");

    leProjectName = new QLineEdit(gbAppearance);
    leProjectName->setText(mProjectName);
    leProjectName->setFocus();
    if (isBlockMode)
        leProjectName->setEnabled(false);

    // Имя отображаемого файла
    lblMMFName = new QLabel(gbAppearance);
    lblMMFName->setText("Имя отображаемого файла:");

    leMMFName = new QLineEdit(gbAppearance);
    leMMFName->setText(mProjectName);

    // Адрес сервера
    lblServerAddress = new QLabel(gbRegConfig);
    lblServerAddress->setText("Адрес сервера:");

    leServerAddress = new QLineEdit(gbRegConfig);
    leServerAddress->setText(mServerAddress);

    // Получение данных с сервера
    cbAutoDBRequest = new QCheckBox(gbUpdate);
    cbAutoDBRequest->setText("Автоматическое получение описаний к параметрам");

    // Принудительное обновление данных с сервера
    cbForceDBRequest = new QCheckBox(gbUpdate);
    cbForceDBRequest->setText("Принудительное получение описаний к параметрам");

    // Порт
    lblPort = new QLabel(gbRegConfig);
    lblPort->setText("Порт:");

    lePort = new QLineEdit(gbRegConfig);
    lePort->setText(QString::number(mPort));
    lePort->setValidator(new QIntValidator(0, 65535, lePort));

    // Автоматическое подключение к серверу
    cbAutoConnect = new QCheckBox(gbRegConfig);
    cbAutoConnect->setText("Автоматическое подключение к серверу");

    // Уведомления
    lblNetNotification = new QLabel(gbRegConfig);
    lblUpdateNotification = new QLabel(gbUpdate);

    // Название вкладки
    lblSceneName = new QLabel(gbAppearance);
    lblSceneName->setText("Имя вкладки:");

    // Название вкладки (поле для ввода)
    leSceneName = new QLineEdit(gbAppearance);
    if (isBlockMode)
        leSceneName->setEnabled(false);

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
    ltRegConfig->addWidget(lblMMFName, 0, 0, 1, 1);
    ltRegConfig->addWidget(leMMFName, 0, 1, 1, 1);
    ltRegConfig->addWidget(lblServerAddress, 1, 0, 1, 1);
    ltRegConfig->addWidget(leServerAddress, 1, 1, 1, 1);
    ltRegConfig->addWidget(lblPort, 2, 0, 1, 1);
    ltRegConfig->addWidget(lePort, 2, 1, 1, 1);
    ltRegConfig->addWidget(cbAutoConnect, 3, 0, 1, 2);
    ltRegConfig->setRowStretch(4, 1);
    ltRegConfig->addWidget(lblNetNotification, 5, 0, 1, 2);

    // Настройка лейаута (Фон)
    ltAppearance->addWidget(lblProjectName, 0, 0, 1, 1);
    ltAppearance->addWidget(leProjectName, 0, 1, 1, 2);
    ltAppearance->addWidget(lblSceneName, 1, 0, 1, 1);
    ltAppearance->addWidget(leSceneName, 1, 1, 1, 2);
    ltAppearance->setRowStretch(2, 1);
    ltAppearance->setColumnStretch(1, 1);

    // Настройки лейаута (прочие настройки)
    ltUpdate->addWidget(cbAutoDBRequest, 0, 0, 1, 1);
    ltUpdate->addWidget(cbForceDBRequest, 1, 0, 1, 1);
    ltUpdate->setRowStretch(2, 1);
    ltUpdate->addWidget(lblUpdateNotification, 3, 0, 1, 2);

    // Настройка лейаута (управляющие кнопки)
    ltButtons->addStretch();
    ltButtons->addWidget(btnOk);
    ltButtons->addWidget(btnCancel);
    ltButtons->addWidget(btnApply);

    // Настройка главного лейаута
    ltMain->addWidget(gbAppearance);
    ltMain->addWidget(gbRegConfig);
    ltMain->addWidget(gbUpdate);
    ltMain->addLayout(ltButtons);

    // Параметры диалогового окна
    this->setLayout(ltMain);
    this->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    this->setWindowTitle("Настройки");
    this->setFocusPolicy(Qt::NoFocus);

    // Расстановка tab-индексов
    QWidget::setTabOrder(leProjectName, leSceneName);
    QWidget::setTabOrder(leSceneName, leMMFName);
    QWidget::setTabOrder(leMMFName, leServerAddress);
    QWidget::setTabOrder(leServerAddress, lePort);
    QWidget::setTabOrder(lePort, cbAutoDBRequest);
    QWidget::setTabOrder(cbAutoDBRequest, cbForceDBRequest);
    QWidget::setTabOrder(cbForceDBRequest, btnOk);
    QWidget::setTabOrder(btnOk, btnCancel);
    QWidget::setTabOrder(btnCancel, btnApply);
    QWidget::setTabOrder(btnApply, leProjectName);

    connect(leServerAddress,
            SIGNAL(textEdited(QString)),
            this,
            SLOT(slotNetConfigChanged(QString)));

    connect(lePort,
            SIGNAL(textEdited(QString)),
            this,
            SLOT(slotNetConfigChanged(QString)));

    connect(cbAutoConnect,
            SIGNAL(clicked()),
            this,
            SLOT(slotNetConfigChanged()));

    connect(btnApply,
            SIGNAL(clicked()),
            this,
            SLOT(slotBtnApplyClick()));

    connect(btnCancel,
            SIGNAL(clicked()),
            this,
            SLOT(slotBtnCancelClick()));

    connect(btnOk,
            SIGNAL(clicked()),
            this,
            SLOT(slotBtnOkClick()));

    connect(leSceneName,
            SIGNAL(textEdited(QString)),
            this,
            SLOT(slotSceneNameChanged(QString)));

    connect(leProjectName,
            SIGNAL(textEdited(QString)),
            this,
            SLOT(slotTextChanged(QString)));

    connect(leMMFName,
            SIGNAL(textEdited(QString)),
            this,
            SLOT(slotTextChanged(QString)));

    connect(cbAutoDBRequest,
            SIGNAL(clicked()),
            this,
            SLOT(slotUpdateConfigChanged()));

    connect(cbForceDBRequest,
            SIGNAL(clicked()),
            this,
            SLOT(slotUpdateConfigChanged()));
}



ConfigDialog::~ConfigDialog()
{
    ltMain->deleteLater();
    ltButtons->deleteLater();
}



// Слот - изменение настроек
int ConfigDialog::slotNetConfigChanged(QString)
{
    if (mIsClientRunning)
        lblNetNotification->setText("<span style='color:red;'>Изменения вступят в силу после перезапуска клиента.</span>");
    btnApply->setEnabled(true);
    return 0;
}



// Слот - изменение настроек
int ConfigDialog::slotNetConfigChanged()
{
    if (mIsClientRunning)
        lblNetNotification->setText("<span style='color:red;'>Изменения вступят в силу после перезапуска клиента.</span>");
    btnApply->setEnabled(true);
    return 0;
}



// Слот - щелчок по кнопке "Применить"
int ConfigDialog::slotBtnApplyClick()
{
    mPort = lePort->text().toInt();
    mServerAddress = leServerAddress->text();
    mIsAutoConnect = cbAutoConnect->isChecked();
    mSceneName = leSceneName->text();
    mProjectName = leProjectName->text();
    mMMFName = leMMFName->text();

    if (mSceneName.isEmpty())
        mSceneName = "Закладка";

    if (mProjectName.isEmpty())
        mProjectName = "Безымянный проект";

    mIsAutoDBRequest = cbAutoDBRequest->isChecked();
    mIsForceDBRequest = cbForceDBRequest->isChecked();

    btnApply->setEnabled(false);
    btnOk->setFocus();

    mIsDataSave = true;

    return 0;
}



int ConfigDialog::slotBtnOkClick()
{
    mPort = lePort->text().toInt();
    mServerAddress = leServerAddress->text();
    mIsAutoConnect = cbAutoConnect->isChecked();
    mSceneName = leSceneName->text();
    mProjectName = leProjectName->text();
    mMMFName = leMMFName->text();

    if (mSceneName.isEmpty())
        mSceneName = "Закладка";

    if (mProjectName.isEmpty())
        mProjectName = "Безымянный проект";

    mIsAutoDBRequest = cbAutoDBRequest->isChecked();
    mIsForceDBRequest = cbForceDBRequest->isChecked();

    this->accept();

    return 0;
}



int ConfigDialog::slotBtnCancelClick()
{
    if (mIsDataSave)
        this->accept();
    else
        this->reject();

    return 0;
}



// Слот - изменение настроек обновления
int ConfigDialog::slotUpdateConfigChanged()
{
    bool exp1 = this->sender() == ((QObject*)cbAutoDBRequest);
    bool exp2 = this->sender() == ((QObject*)cbForceDBRequest);

    if (!cbAutoDBRequest->isChecked() && exp1)
        cbForceDBRequest->setChecked(false);
    else if (cbForceDBRequest->isChecked() && exp2)
        cbAutoDBRequest->setChecked(true);

    if (cbAutoDBRequest->isChecked() && mIsClientRunning)
        lblUpdateNotification->setText("<span style='color:red;'>Данные будут обновлены при следующем подключении к серверу!</span>");
    else
        lblUpdateNotification->setText("");

    btnApply->setEnabled(true);

    return 0;
}



// Слот - изменение названия вкладки
int ConfigDialog::slotSceneNameChanged(QString)
{
    btnApply->setEnabled(true);
    return 0;
}


// Слот - изменение имени проекта
int ConfigDialog::slotTextChanged(QString)
{
    btnApply->setEnabled(true);
    return 0;
}



// Установка значения адреса сервера
int ConfigDialog::setServerAddress(const QString serverAddress)
{
    mServerAddress = serverAddress;
    leServerAddress->setText(mServerAddress);

    return 0;
}



// Получение информации об адресе сервера
QString ConfigDialog::serverAddress()
{
    return mServerAddress;
}



// Автоматическое подключение к серверу
int ConfigDialog::setIsAutoConnect(bool isAutoConnect)
{
    cbAutoConnect->setChecked(isAutoConnect);
    mIsAutoConnect = isAutoConnect;

    return 0;
}



bool ConfigDialog::isAutoConnect()
{
    return mIsAutoConnect;
}



int ConfigDialog::setMMFName(const QString mmfName)
{
    mMMFName = mmfName;
    leMMFName->setText(mMMFName);

    return 0;
}



int ConfigDialog::setSceneName(const QString sceneName)
{
    mSceneName = sceneName;
    leSceneName->setText(sceneName);

    return 0;
}



// Установка имени проекта
int ConfigDialog::setProjectName(const QString projectName)
{
    mProjectName = projectName;
    leProjectName->setText(mProjectName);

    return 0;
}



int ConfigDialog::setIsAutoDBRequest(bool isAutoDBRequest)
{
    mIsAutoDBRequest = isAutoDBRequest;

    cbAutoDBRequest->setChecked(mIsAutoDBRequest);

    return 0;
}



int ConfigDialog::setIsForceDBRequest(bool isForceDBRequest)
{
    mIsForceDBRequest = isForceDBRequest;

    cbForceDBRequest->setChecked(mIsForceDBRequest);

    return 0;
}



QString ConfigDialog::projectName()
{
    return mProjectName;
}



QString ConfigDialog::mmfName()
{
    return mMMFName;
}



QString ConfigDialog::sceneName()
{
    return mSceneName;
}



bool ConfigDialog::isAutoDBRequest()
{
    return mIsAutoDBRequest;
}



bool ConfigDialog::isForceDBRequest()
{
    return mIsForceDBRequest;
}



int ConfigDialog::setPort(quint16 port)
{
    mPort = port;
    lePort->setText(QString::number(port));
    lePort->installEventFilter(new LineEditEventFilter(mPort, lePort));
    return 0;
}



quint16 ConfigDialog::port()
{
    return mPort;
}
