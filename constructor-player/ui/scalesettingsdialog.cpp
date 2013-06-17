#include "scalesettingsdialog.h"



ScaleSettingsDialog::ScaleSettingsDialog(QWidget *parent) :
    QDialog(parent),
    mIsDataSave(false)

{
    // Минимальные размеры для GroupBox
    const int minGbWidth = 366;
    const int minGbAppearanceHeight = 0;

    // Лейауты
    ltMain = new QVBoxLayout();
    ltButtons = new QHBoxLayout();

    // GroupBox с настройками внешнего вида
    gbBackground = new QGroupBox(this);
    gbBackground->setTitle("Внешний вид");
    gbBackground->setMinimumSize(minGbWidth, minGbAppearanceHeight);
    ltBackground = new QGridLayout(gbBackground);

    // GroupBox с настройками внешнего вида
    gbScaleSettings = new QGroupBox(this);
    gbScaleSettings->setTitle("Масштабирование");
    gbScaleSettings->setMinimumSize(minGbWidth, minGbAppearanceHeight);
    ltScaleSettings = new QGridLayout(gbScaleSettings);
    gbScaleSettings->setEnabled(false);

    // Уведомления
    lblImageNotification = new QLabel(gbBackground);

    // Фон, выбор изображения
    lblImage = new QLabel(gbBackground);
    lblImage->setText("Фоновое изображение:");

    // Фон (поле для ввода)
    leImage = new QLineEdit(gbBackground);
    leImage->setFocus();

    // Фон (кнопка)
    btnBrowseImage = new QPushButton(gbBackground);
    btnBrowseImage->setText("Обзор");

    // Масштаб, метка
    lblScale = new QLabel(gbScaleSettings);
    lblScale->setText("Масштаб (процент от исходных ширины и высоты):");

    // Ползунок машстаба
    slScale = new QSlider(gbScaleSettings);
    slScale->setOrientation(Qt::Horizontal);
    slScale->setRange(0, 1000);
    slScale->setValue(100);
    slScale->setFixedHeight(25);

    // SpinBox для масштабирования
    spbScale = new QSpinBox(gbScaleSettings);
    spbScale->setRange(1, 1000);
    spbScale->setValue(100);

    // Метки-уведомления о размерах
    lblSourceSizeText = new QLabel(gbScaleSettings);
    lblSourceSizeText->setText("Исходный размер:");
    lblNewSizeText = new QLabel(gbScaleSettings);
    lblNewSizeText->setText("Новый размер:");

    // Значения размеров
    lblSourceSizeValue = new QLabel(gbScaleSettings);
    lblSourceSizeValue->setText("неопределено");
    lblNewSizeValue = new QLabel(gbScaleSettings);
    lblNewSizeValue->setText("неопределено");

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

    // Настройка лейаута (Фон)
    ltBackground->addWidget(lblImage, 0, 0, 1, 3);
    ltBackground->addWidget(leImage, 1, 0, 1, 2);
    ltBackground->addWidget(btnBrowseImage, 1, 2, 1, 1);
    ltBackground->setRowStretch(2, 1);
    ltBackground->setColumnStretch(1, 1);
    ltBackground->addWidget(lblImageNotification, 3, 0, 1, 3);

    // Настройка лейаута (масштаб)
    ltScaleSettings->addWidget(lblScale, 0, 0, 1, 2);
    ltScaleSettings->addWidget(spbScale, 0, 2, 1, 1);
    ltScaleSettings->addWidget(slScale, 1, 0, 1, 3);
    ltScaleSettings->addWidget(lblSourceSizeText, 2, 0, 1, 1);
    ltScaleSettings->addWidget(lblSourceSizeValue, 2, 1, 1, 1);
    ltScaleSettings->addWidget(lblNewSizeText, 3, 0, 1, 1);
    ltScaleSettings->addWidget(lblNewSizeValue, 3, 1, 1, 1);
    ltScaleSettings->setColumnStretch(1, 1);
    ltScaleSettings->setColumnStretch(2, 1);

    // Настройка лейаута (управляющие кнопки)
    ltButtons->addStretch();
    ltButtons->addWidget(btnOk);
    ltButtons->addWidget(btnCancel);
    ltButtons->addWidget(btnApply);

    // Настройка главного лейаута
    ltMain->addWidget(gbBackground);
    ltMain->addWidget(gbScaleSettings);
    ltMain->addLayout(ltButtons);

    // Параметры диалогового окна
    this->setLayout(ltMain);
    this->setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred));
    this->setWindowTitle("Настройки");
    this->setFocusPolicy(Qt::NoFocus);

    // Расстановка tab-индексов
    QWidget::setTabOrder(leImage, btnBrowseImage);
    QWidget::setTabOrder(btnBrowseImage, spbScale);
    QWidget::setTabOrder(spbScale, slScale);
    QWidget::setTabOrder(slScale, btnOk);
    QWidget::setTabOrder(btnOk, btnCancel);
    QWidget::setTabOrder(btnCancel, btnApply);
    QWidget::setTabOrder(btnApply, leImage);

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

    connect(btnBrowseImage,
            SIGNAL(clicked()),
            this,
            SLOT(slotBtnBrowseImageClick()));

    connect(leImage,
            SIGNAL(textEdited(QString)),
            this,
            SLOT(slotImagePathChanged(QString)));

    connect(slScale,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(slotPBValueChanged(int)));

    connect(spbScale,
            SIGNAL(valueChanged(int)),
            this,
            SLOT(slotSPBValueChanged(int)));
}



ScaleSettingsDialog::~ScaleSettingsDialog()
{
    ltMain->deleteLater();
    ltButtons->deleteLater();
}



// Установка директории с проектом.
// Расположение проекта необходимо для получения относительного пути при установке
// фонового изображения
int ScaleSettingsDialog::setProjectDirectory(const QString projectDirectory)
{
    if (!projectDirectory.isEmpty())
        mProjectDirectory = projectDirectory;
    else
        mProjectDirectory = qApp->applicationDirPath();

    return 0;
}



// Слот - щелчок по кнопке "Применить"
int ScaleSettingsDialog::slotBtnApplyClick()
{
    mScaleFactor = spbScale->value();

    // Получение относительного пути для файла фонового рисунка
    QDir currentDir(mProjectDirectory);
    mImage = currentDir.relativeFilePath(leImage->text());

    btnApply->setEnabled(false);
    btnOk->setFocus();

    mIsDataSave = true;

    return 0;
}



int ScaleSettingsDialog::slotBtnOkClick()
{
    mScaleFactor = spbScale->value();

    // Получение относительного пути для файла фонового рисунка
    QDir currentDir(mProjectDirectory);
    mImage = currentDir.relativeFilePath(leImage->text());

    this->accept();

    return 0;
}



int ScaleSettingsDialog::slotBtnCancelClick()
{
    if (mIsDataSave)
        this->accept();
    else
        this->reject();

    return 0;
}



// Слот - щелчок по кнопке "Обзор" (фоновое изображение)
int ScaleSettingsDialog::slotBtnBrowseImageClick()
{
    QFileDialog dialog(this);
    dialog.setWindowTitle(QString("Выбрать фоновое изображение"));
    dialog.setDirectory(mProjectDirectory);
    dialog.setNameFilter(QString("SVG-изображение (*.svg)"));
    dialog.setLabelText(QFileDialog::Accept, "&Выбрать изображение");

    if (dialog.exec() != QDialog::Accepted)
        return 0;

    QString dialogResult = dialog.selectedFiles().first();

    if (!dialogResult.isEmpty())
    {
        // Установка пути до фонового изображения
        leImage->setText(dialogResult);

        // Очистка поля для уведомлений
        lblImageNotification->setText("");

        // Разблокировка кнопки "Применить"
        btnApply->setEnabled(true);

        // Установка модификаторов масштаба в 100%
        spbScale->setValue(100);
        slScale->setValue(100);

        // Получение информации об изображении
        mImageSize = QSvgRenderer(dialogResult).viewBox().size();

        lblSourceSizeValue->setText(QString("%1x%2 пкс")
                                    .arg(mImageSize.width())
                                    .arg(mImageSize.height()));
        lblNewSizeValue->setText(QString("%1x%2 пкс")
                                 .arg(mImageSize.width() * spbScale->value() / 100)
                                 .arg(mImageSize.height() * spbScale->value() / 100));
        // Установка возможности модификации масштаба изображения
        gbScaleSettings->setEnabled(true);
    }

    return 0;
}



// Изменение ползунка в ProgressBar
int ScaleSettingsDialog::slotPBValueChanged(int)
{
    btnApply->setEnabled(true);
    spbScale->setValue(slScale->value());
    lblSourceSizeValue->setText(QString("%1x%2 пкс").arg(mImageSize.width()).arg(mImageSize.height()));
    lblNewSizeValue->setText(QString("%1x%2 пкс")
                             .arg(mImageSize.width() * spbScale->value() / 100)
                             .arg(mImageSize.height() * spbScale->value() / 100));
    return 0;
}



// Изменение значения в SpinBox
int ScaleSettingsDialog::slotSPBValueChanged(int)
{
    btnApply->setEnabled(true);
    slScale->setValue(spbScale->value());
    lblSourceSizeValue->setText(QString("%1x%2 пкс").arg(mImageSize.width()).arg(mImageSize.height()));
    lblNewSizeValue->setText(QString("%1x%2 пкс")
                             .arg(mImageSize.width() * spbScale->value() / 100)
                             .arg(mImageSize.height() * spbScale->value() / 100));
    return 0;
}



// Слот - изменение расположения фонового изображения
int ScaleSettingsDialog::slotImagePathChanged(QString path)
{
    QFileInfo fileInfo(path);
    bool isExist = fileInfo.exists();
    bool isCorrectSuffix = (fileInfo.suffix() == "svg");
    if (isExist && isCorrectSuffix)
    {
        lblImageNotification->setText("");

        // Получение информации об изображении
        mImageSize = QSvgRenderer(path).viewBox().size();
        lblSourceSizeValue->setText(QString("%1x%2 пкс").arg(mImageSize.width()).arg(mImageSize.height()));
        lblNewSizeValue->setText(QString("%1x%2 пкс")
                                 .arg(mImageSize.width() * spbScale->value() / 100)
                                 .arg(mImageSize.height() * spbScale->value() / 100));
        // Установка возможности модификации масштаба изображения
        gbScaleSettings->setEnabled(true);
    }
    else
    {
        lblImageNotification->setText("<span style='color:red;'>Изображение не найдено!</span>");
        gbScaleSettings->setEnabled(false);
    }

    if (leImage->focusWidget())
        btnApply->setEnabled(true);

    return 0;
}



// Слот - изменение имени проекта
int ScaleSettingsDialog::slotTextChanged(QString)
{
    btnApply->setEnabled(true);
    return 0;
}



// Установка пути к изображению
int ScaleSettingsDialog::setImage(const QString image,
                                  const int scaleFactor)
{
    if (!image.isEmpty())
    {
        QDir currentDir(image);
        currentDir.makeAbsolute();
        mImage = currentDir.path();
    }
    else
    {
        mImage = image;
    }

    leImage->setText(mImage);
    if (!QFile(mImage).exists())
    {
        lblImageNotification->setText("<span style='color:red;'>Изображение не найдено!</span>");
        gbScaleSettings->setEnabled(false);
    }
    else
    {
        lblImageNotification->setText("");

        // Установка модификаторов масштаба в 100%
        spbScale->setValue(scaleFactor);
        slScale->setValue(scaleFactor);
        // Получение информации об изображении
        mImageSize = QSvgRenderer(image).viewBox().size();
        lblSourceSizeValue->setText(QString("%1x%2 пкс").arg(mImageSize.width()).arg(mImageSize.height()));
        lblNewSizeValue->setText(QString("%1x%2 пкс")
                                 .arg(mImageSize.width() * spbScale->value() / 100)
                                 .arg(mImageSize.height() * spbScale->value() / 100));
        // Установка возможности модификации масштаба изображения
        gbScaleSettings->setEnabled(true);
    }

    return 0;
}



QString ScaleSettingsDialog::image()
{
    return mImage;
}



int ScaleSettingsDialog::scaleFactor()
{
    return mScaleFactor;
}
