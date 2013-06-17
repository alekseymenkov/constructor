#include "elementeditdialog.h"

//

ElementEditDialog::ElementEditDialog(QWidget *parent) :
    QDialog(parent)
{
    // Константы
    const int fixedButtonHeight = 25;
    const int fixedGroupBoxWidth = 225;
    const int max16BitValue = 65535;

    this->setWindowTitle("Параметры элемента");

    // Главный лейаут
    ltMain = new QGridLayout(this);
    this->setLayout(ltMain);

    // Создание окна предварительного просмотра
    previewWidget = new PreviewWidget(this);
    previewWidget->setFixedWidth(fixedGroupBoxWidth);

    // Панель с настройками (положение, размер, поворот)
    wgtSettings = new QWidget(this);
    ltSettings = new QVBoxLayout(wgtSettings);
    ltSettings->setMargin(0);

    // Панель с основными настройками элемента (таблица)
    gbModuleSettings = new QGroupBox(this);
    gbModuleSettings->setTitle("Параметры элемента");
    ltModule = new QGridLayout(gbModuleSettings);

    // Настройки положения
    gbPositionSettings = new QGroupBox(this);
    gbPositionSettings->setTitle("Расположение");
    gbPositionSettings->setFixedWidth(fixedGroupBoxWidth);
    ltPosition = new QGridLayout(gbPositionSettings);

    // Настройки размера
    gbSizeSettings = new QGroupBox(this);
    gbSizeSettings->setTitle("Размер");
    gbSizeSettings->setFixedWidth(fixedGroupBoxWidth);
    ltSize = new QGridLayout(gbSizeSettings);

    // Прочие настройки
    gbOtherSettings = new QGroupBox(this);
    gbOtherSettings->setTitle("Прочие параметры");
    gbOtherSettings->setFixedWidth(fixedGroupBoxWidth);
    ltOther = new QGridLayout(gbOtherSettings);

    gbExtraSettings = new QGroupBox(this);
    gbExtraSettings->setTitle("Динамические настройки");
    gbExtraSettings->setFixedWidth(fixedGroupBoxWidth);
    gbExtraSettings->setVisible(false);
    ltExtra = new QGridLayout(gbExtraSettings);

    // Модуль с логикой работы элемента
    // Метка
    lblModuleName = new QLabel(gbModuleSettings);
    lblModuleName->setText("Модуль:");
    // Выпадающий список
    cbModuleName = new QComboBox(gbModuleSettings);
    cbModuleName->setCurrentIndex(-1);

    // Название элемента
    // Метка
    lblElementName = new QLabel(gbModuleSettings);
    lblElementName->setText("Название элемента:");
    // Поле для ввода
    leElementName = new QLineEdit(gbModuleSettings);

    // Параметры элемента
    // Метка
    lblElementParts = new QLabel(gbModuleSettings);
    lblElementParts->setText("Список частей элемента:");
    // Таблица
    tblElementParts = new QTableWidget(gbModuleSettings);
    tblElementParts->setColumnCount(6);
    tblElementParts->setSelectionBehavior(QAbstractItemView::SelectRows);
    tblElementParts->setSelectionMode(QAbstractItemView::SingleSelection);
    tblElementParts->verticalHeader()->setDefaultSectionSize(25);
    tblElementParts->verticalHeader()->setVisible(false);
    tblElementParts->horizontalHeader()->setClickable(false);
    tblElementParts->setFocusPolicy(Qt::NoFocus);
    tblElementParts->setStyleSheet("QTableView::item:selected {background: #ece9d8; color: blue;}");
    tblElementParts->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    tblElementParts->setHorizontalHeaderItem(0, new QTableWidgetItem("№"));
    tblElementParts->horizontalHeader()->setResizeMode(0, QHeaderView::ResizeToContents);
    tblElementParts->setItemDelegateForColumn(0, new LineEditDelegate(LineEditDelegate::DelegateType::NoEdit, tblElementParts));
    tblElementParts->setHorizontalHeaderItem(1, new QTableWidgetItem("Описание"));
    tblElementParts->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
    tblElementParts->setItemDelegateForColumn(1, new LineEditDelegate(LineEditDelegate::DelegateType::NoValidator, tblElementParts));
    tblElementParts->setHorizontalHeaderItem(2, new QTableWidgetItem("№ байта"));
    tblElementParts->horizontalHeader()->setResizeMode(2, QHeaderView::ResizeToContents);
    tblElementParts->setItemDelegateForColumn(2, new LineEditDelegate(LineEditDelegate::DelegateType::Validator16Bit, tblElementParts));
    tblElementParts->setHorizontalHeaderItem(3, new QTableWidgetItem("№ бита"));
    tblElementParts->horizontalHeader()->setResizeMode(3, QHeaderView::ResizeToContents);
    tblElementParts->setItemDelegateForColumn(3, new LineEditDelegate(LineEditDelegate::DelegateType::Validator8Bit, tblElementParts));
    tblElementParts->setHorizontalHeaderItem(4, new QTableWidgetItem("Кол-во бит"));
    tblElementParts->horizontalHeader()->setResizeMode(4, QHeaderView::ResizeToContents);
    tblElementParts->setItemDelegateForColumn(4, new LineEditDelegate(LineEditDelegate::DelegateType::Validator8Bit, tblElementParts));
    tblElementParts->setHorizontalHeaderItem(5, new QTableWidgetItem(""));
    tblElementParts->horizontalHeader()->setResizeMode(5, QHeaderView::Fixed);
    tblElementParts->horizontalHeader()->resizeSection(5, 20);
    tblElementParts->setItemDelegateForColumn(5, new LineEditDelegate(LineEditDelegate::DelegateType::NoValidator, tblElementParts));

    // Настройка лейаута ltModule
    ltModule->addWidget(lblModuleName, 0, 0, 1, 1);
    ltModule->addWidget(cbModuleName, 0, 1, 1, 1);
    ltModule->addWidget(lblElementName, 1, 0, 1, 1);
    ltModule->addWidget(leElementName, 1, 1, 1, 1);
    ltModule->addWidget(lblElementParts, 2, 0, 1, 1);
    ltModule->addWidget(tblElementParts, 3, 0, 1, 2);

    // Модификатор размера
    // Метка
    lblSize = new QLabel(gbSizeSettings);
    lblSize->setText("Модификатор:");
    // SpinBox
    spbSize = new QDoubleSpinBox(gbSizeSettings);
    spbSize->setLocale(QLocale::c());
    spbSize->setSingleStep(0.01);
    spbSize->setMinimum(0);
    spbSize->setMaximum(10);

    // Ширина элемента
    // Метка
    lblWidth = new QLabel(gbSizeSettings);
    lblWidth->setText("Ширина, пкс:");
    // Поле для ввода
    leWidth = new QLineEdit(gbSizeSettings);
    leWidth->setValidator(new QIntValidator(0, max16BitValue, leWidth));
    leWidth->setReadOnly(true);

    // Высота элемента
    // Метка
    lblHeight = new QLabel(gbSizeSettings);
    lblHeight->setText("Высота, пкс:");
    // Поле для ввода
    leHeight = new QLineEdit(gbSizeSettings);
    leHeight->setValidator(new QIntValidator(0, max16BitValue, leHeight));
    leHeight->setReadOnly(true);

    // Настройка лейаута ltSize
    ltSize->addWidget(lblSize, 0, 0, 1, 1);
    ltSize->addWidget(lblWidth, 1, 0, 1, 1);
    ltSize->addWidget(lblHeight, 2, 0, 1, 1);
    ltSize->addWidget(spbSize, 0, 1, 1, 1);
    ltSize->addWidget(leWidth, 1, 1, 1, 1);
    ltSize->addWidget(leHeight, 2, 1, 1, 1);

    // Х-координата элемента
    // Метка
    lblXCoordinate = new QLabel(gbPositionSettings);
    lblXCoordinate->setText("Координата X, пкс:");
    // Поле для ввода
    leXCoordinate = new QLineEdit(gbPositionSettings);
    leXCoordinate->setValidator(new QIntValidator(0, max16BitValue, leXCoordinate));

    // Y-координата элемента
    // Метка
    lblYCoordinate = new QLabel(gbPositionSettings);
    lblYCoordinate->setText("Координата Y, пкс:");
    // Поле для ввода
    leYCoordinate = new QLineEdit(gbPositionSettings);
    leYCoordinate->setValidator(new QIntValidator(0, max16BitValue, leYCoordinate));

    // Настройка лейаута ltPosition
    ltPosition->addWidget(lblXCoordinate, 0, 0, 1, 1);
    ltPosition->addWidget(leXCoordinate, 0, 1, 1, 1);
    ltPosition->addWidget(lblYCoordinate, 1, 0, 1, 1);
    ltPosition->addWidget(leYCoordinate, 1, 1, 1, 1);

    // Стартовый угол поворота элемента
    // Метка
    lblAngle = new QLabel(gbOtherSettings);
    lblAngle->setText("Поворот элемента, град.:");
    // SpinBox
    spbAngle = new QSpinBox(gbOtherSettings);
    spbAngle->setRange(0, 360);
    spbAngle->setSingleStep(1);

    // Настройки лейаута ltOther
    ltOther->addWidget(lblAngle, 0, 0, 1, 1);
    ltOther->addWidget(spbAngle, 0, 1, 1, 1);

    // Управляющая кнопка - Ок
    btnOK = new QPushButton(this);
    btnOK->setFixedHeight(fixedButtonHeight);
    btnOK->setText("ОК");
    btnOK->setDefault(true);
    btnOK->setEnabled(false);

    // Управляющая кнопка - Отмена
    btnCancel = new QPushButton(this);
    btnCancel->setFixedHeight(fixedButtonHeight);
    btnCancel->setText("Отмена");

    // Управляющая кнопка - вернуть значения по умолчанию
    btnReset = new QPushButton(this);
    btnReset->setFixedHeight(fixedButtonHeight);
    btnReset->setText(" По умолчанию ");

    // Настройка панели с управляющими кнопками
    dbbButtons = new QDialogButtonBox(Qt::Horizontal);
    dbbButtons->addButton(btnOK, QDialogButtonBox::ActionRole);
    dbbButtons->addButton(btnCancel, QDialogButtonBox::ActionRole);
    dbbButtons->addButton(btnReset, QDialogButtonBox::ResetRole);

    // Настройка панели с параметрами
    ltSettings->addWidget(gbPositionSettings);
    ltSettings->addWidget(gbSizeSettings);
    ltSettings->addWidget(gbOtherSettings);
    ltSettings->addWidget(gbExtraSettings);
    ltSettings->addWidget(previewWidget, 1);
    ltSettings->addStretch(1);

    // Настройка расположения элементов в главном окне
    ltMain->addWidget(gbModuleSettings, 0, 0, 1, 1);
    ltMain->addWidget(wgtSettings, 0, 1, 1, 1);
    ltMain->addWidget(dbbButtons, 1, 0, 1, 2);
    ltMain->setColumnStretch(0, 1);
    ltMain->setRowStretch(0, 1);

    // Предыдущий индекс
    mPreviousIndex = -1;

    connect(cbModuleName,
            SIGNAL(activated(int)),
            this,
            SLOT(slotChangeModule(int)));

    connect(spbSize,
            SIGNAL(valueChanged(double)),
            this,
            SLOT(slotSizeModificatorChanged(double)));

    connect(btnReset,
            SIGNAL(clicked()),
            this,
            SLOT(slotResetSettings()));

    connect(btnOK,
            SIGNAL(clicked()),
            this,
            SLOT(slotBtnOkClicked()));

    connect(btnCancel,
            SIGNAL(clicked()),
            this,
            SLOT(slotBtnCancelClicked()));

    connect(tblElementParts,
            SIGNAL(itemClicked(QTableWidgetItem*)),
            this,
            SLOT(slotParameterStateChanged(QTableWidgetItem*)));
}



// Установка интерфейсного модуля элемента
int ElementEditDialog::setElementInterface(QSharedPointer<ElementInterface> elementInterface)
{
    mElementInterface = elementInterface;

    for (int i = 0; i < mExtraLabels.size(); i++)
    {
        delete mExtraLabels[i];
        delete mExtraControls[i];
    }
    mExtraLabels.clear();
    mExtraControls.clear();

    // Интерфейсный элемент не установлен
    if (mElementInterface.isNull())
        return -1;

    // Установка изображения для окна просмотра (просмотр в указанной директории)
    int result = previewWidget->setPreviewImagePath(mElementInterface.data()->previewImagePath());
    if (result < 0)
    {
        // Просмотр изображения в директории по умолчанию
        QString moduleName = mElementInterface.data()->moduleName();
        previewWidget->setPreviewImagePath("images/" + moduleName.mid(0, moduleName.length() - 4));
    }

    // Имя элемента
    leElementName->setText(mElementInterface.data()->elementName());
    // Расположение элемента на сцене
    leXCoordinate->setText(QString("%1").arg(mElementInterface.data()->x()));
    leYCoordinate->setText(QString("%1").arg(mElementInterface.data()->y()));
    // Размер элемента
    if (mElementInterface.data()->elementLogicType() == ElementLogicType::Common ||
            mElementInterface.data()->elementLogicType() == ElementLogicType::FuelDensity)
    {
        spbSize->setReadOnly(false);
        spbSize->setEnabled(true);
        spbSize->setValue(mElementInterface.data()->sizeModificator());
        leWidth->setText(QString::number(mElementInterface.data()->size().width() *
                                         mElementInterface.data()->sizeModificator()));
        leWidth->setReadOnly(true);
        leWidth->setEnabled(false);
        leHeight->setText(QString::number(mElementInterface.data()->size().height() *
                                          mElementInterface.data()->sizeModificator()));
        leHeight->setReadOnly(true);
        leHeight->setEnabled(false);
    }
    else if (mElementInterface.data()->elementLogicType() == ElementLogicType::SingleIndicator ||
             mElementInterface.data()->elementLogicType() == ElementLogicType::DoubleIndicator ||
             mElementInterface.data()->elementLogicType() == ElementLogicType::SingleIndicatorDark ||
             mElementInterface.data()->elementLogicType() == ElementLogicType::DoubleIndicatorDark ||
             mElementInterface.data()->elementLogicType() == ElementLogicType::Flap ||
             mElementInterface.data()->elementLogicType() == ElementLogicType::FlapDark ||
             mElementInterface.data()->elementLogicType() == ElementLogicType::Bus ||
             mElementInterface.data()->elementLogicType() == ElementLogicType::BusDark)
    {
        spbSize->setReadOnly(true);
        spbSize->setEnabled(false);
        spbSize->setValue(mElementInterface.data()->sizeModificator());
        leWidth->setText(QString::number(mElementInterface.data()->size().width()));
        leWidth->setReadOnly(false);
        leWidth->setEnabled(true);
        leHeight->setText(QString::number(mElementInterface.data()->size().height()));
        leHeight->setReadOnly(false);
        leHeight->setEnabled(true);
    }

    // Угол поворота
    spbAngle->setValue(mElementInterface.data()->startAngle());

    // Основные параметры элемента
    tblElementParts->clearContents();
    tblElementParts->setRowCount(mElementInterface.data()->parametersCount());
    QTableWidgetItem* item = 0;
    QColor textColor;
    bool isParameterUsed = true;
    for (int i = 0; i < mElementInterface.data()->parametersCount(); i++)
    {
        isParameterUsed = mElementInterface.data()->isParameterUsed(i);
        if (isParameterUsed)
            textColor = Qt::black;
        else
            textColor = Qt::lightGray;

        // Номер
        item = new QTableWidgetItem(QString::number(i));
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        tblElementParts->setItem(i, 0, item);
        tblElementParts->item(i, 0)->setTextAlignment(Qt::AlignCenter);
        tblElementParts->item(i, 0)->setTextColor(textColor);

        // Описание
        if (isParameterUsed)
            tblElementParts->setItem(i, 1, new QTableWidgetItem(mElementInterface.data()->description(i)));
        else
            tblElementParts->setItem(i, 1, new QTableWidgetItem(QString()));
        tblElementParts->item(i, 1)->setTextAlignment(Qt::AlignCenter);
        tblElementParts->item(i, 1)->setTextColor(textColor);

        // Номер байта
        if (isParameterUsed)
            tblElementParts->setItem(i, 2, new QTableWidgetItem(QString::number(mElementInterface.data()->startByte(i))));
        else
            tblElementParts->setItem(i, 2, new QTableWidgetItem(QString("0")));
        tblElementParts->item(i, 2)->setTextAlignment(Qt::AlignCenter);
        tblElementParts->item(i, 2)->setTextColor(textColor);

        // Номер бита
        if (isParameterUsed)
            tblElementParts->setItem(i, 3, new QTableWidgetItem(QString::number(mElementInterface.data()->startBit(i))));
        else
            tblElementParts->setItem(i, 3, new QTableWidgetItem(QString("0")));
        tblElementParts->item(i, 3)->setTextAlignment(Qt::AlignCenter);
        tblElementParts->item(i, 3)->setTextColor(textColor);

        // Количество бит
        item = new QTableWidgetItem(QString::number(mElementInterface.data()->bitsCount(i)));
        tblElementParts->setItem(i, 4, item);
        tblElementParts->item(i, 4)->setTextAlignment(Qt::AlignCenter);
        tblElementParts->item(i, 4)->setTextColor(textColor);
        // Возможность редактирования количества бит, если параметр - число
        if (mElementInterface.data()->parameterType(i) == ParameterType::Number)
            item->setFlags(item->flags() | Qt::ItemIsEditable);
        else
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);

        // Флаг - используется ли параметр?
        item = new QTableWidgetItem();
        if (isParameterUsed)
            item->setCheckState(Qt::Checked);
        else
            item->setCheckState(Qt::Unchecked);
        tblElementParts->setItem(i, 5, item);

        // Запрет редактирования строки
        if (!isParameterUsed)
            tblElementParts->setItemDelegateForRow(i, new LineEditDelegate(LineEditDelegate::DelegateType::NoEdit, tblElementParts));
    }

    // Добавление динамических элементов
    qint32 parametersCount = mElementInterface.data()->extraParametersCount();

    for (int i = 0; i < parametersCount; i++)
    {
        QLabel* label = new QLabel(gbExtraSettings);
        label->setText(mElementInterface.data()->extraParameterName(i));
        ltExtra->addWidget(label, i, 0, 1, 1);
        mExtraLabels.append(label);

        QLineEdit* lineEdit = 0;
        ColorLabel* colorLabel = 0;
        QCheckBox* checkBox = 0;
        QSpinBox* spinBox = 0;
        QWidget* widget = 0;
        QVariant value;
        switch(mElementInterface.data()->extraUIElementType(i))
        {
        case ExtraUIElementType::LineEdit:
            lineEdit = new QLineEdit(gbExtraSettings);
            widget = qobject_cast<QWidget*>(lineEdit);
            // Валидаторы на поле ввода, в зависимости от типа
            if (mElementInterface.data()->extraParameterType(i) == ExtraParameterType::Integer)
                lineEdit->setValidator(new QIntValidator(lineEdit));
            else if (mElementInterface.data()->extraParameterType(i) == ExtraParameterType::Real)
                lineEdit->setValidator(new QDoubleValidator(lineEdit));

            lineEdit->setText(mElementInterface.data()->extraParameter(i).toString());
            break;

        case ExtraUIElementType::ColorDialog:
            colorLabel = new ColorLabel(gbExtraSettings);
            colorLabel->setFixedHeight(18);
            widget = qobject_cast<QWidget*>(colorLabel);
            value = mElementInterface.data()->extraParameter(i);
            colorLabel->setColor(value.value<QColor>());
            break;

        case ExtraUIElementType::CheckBox:
            checkBox = new QCheckBox(gbExtraSettings);
            widget = qobject_cast<QWidget*>(checkBox);
            value = mElementInterface.data()->extraParameter(i);
            checkBox->setChecked(value.toBool());
            break;

        case ExtraUIElementType::SpinBox:
            spinBox = new QSpinBox(gbExtraSettings);
            widget = qobject_cast<QWidget*>(spinBox);
            // Ограничения на SpinBox
            spinBox->setRange(0, 6);
            // Текущее значение
            if (mElementInterface.data()->extraParameter(i).toString().isEmpty())
                value = mElementInterface.data()->defaultExtraParameter(i);
            else
                value = mElementInterface.data()->extraParameter(i);
            spinBox->setValue(value.toInt());
            break;

        case ExtraUIElementType::Undefined:
            break;
        }

        ltExtra->addWidget(widget, i, 1, 1, 1);
        mExtraControls.append(widget);
    }

    // Управление внешним видом динамических параметров
    displayExtraParameters();

    // Индекс модуля по умолчанию (в combobox)
    int index = cbModuleName->findText(mElementInterface.data()->moduleName());
    if (index < 0)
    {
        cbModuleName->setCurrentIndex(-1);
        btnOK->setEnabled(false);
    }
    else
    {
        cbModuleName->setCurrentIndex(index);
        btnOK->setEnabled(true);
    }



    return 0;
}



// Получение модифицированного интерфейсного модуля элемента
QSharedPointer<ElementInterface> ElementEditDialog::getElementInterface()
{
    return mElementInterface;
}



// Загрузка списка доступных модулей
int ElementEditDialog::setModulesList(QStringList modulesList)
{
    mModulesList = modulesList;

    cbModuleName->clear();
    cbModuleName->addItems(mModulesList);

    if (mElementInterface.isNull())
    {
        cbModuleName->setCurrentIndex(-1);
        return -1;
    }

    int index = cbModuleName->findText(mElementInterface.data()->moduleName());
    if (index < 0)
        cbModuleName->setCurrentIndex(-1);
    else
        cbModuleName->setCurrentIndex(index);

    return 0;
}



// Управление отображением динамических полей
int ElementEditDialog::displayExtraParameters()
{
    int showItems = 0;
    int hideItems = 0;
    int count = 0;
    for (int i = 0; i < mElementInterface.data()->parametersCount(); i++)
    {
        bool exp1 = mElementInterface.data()->parameterType(i) == ParameterType::Number;
        // Колонка с Checkbox == 5
        bool exp2 = tblElementParts->item(i, 5)->checkState();
        // Если параметр - числовой и используется
        if (exp1 && exp2)
        {
            mExtraControls[count * 2]->show();
            mExtraControls[count * 2 + 1]->show();
            mExtraLabels[count * 2]->show();
            mExtraLabels[count * 2 + 1]->show();
            showItems += 2;
        }
        // Если параметр - числовой и не используется
        else if (exp1 && !exp2)
        {
            mExtraControls[count * 2]->hide();
            mExtraControls[count * 2 + 1]->hide();
            mExtraLabels[count * 2]->hide();
            mExtraLabels[count * 2 + 1]->hide();
            hideItems += 2;
        }

        if (exp1)
            count++;
    }

    if (mElementInterface.data()->extraParametersCount() == hideItems)
        gbExtraSettings->hide();
    else
        gbExtraSettings->show();

    return 0;
}



// Слот - изменение базового модуля элемента
int ElementEditDialog::slotChangeModule(int index)
{
    if (mPreviousIndex != index)
    {
        emit signalChangeModule(cbModuleName->currentText());
        mPreviousIndex = index;
    }

    return 0;
}



// Слот - изменение модификатора размера элемента
int ElementEditDialog::slotSizeModificatorChanged(double value)
{
    // Интерфейсный элемент не установлен
    if (mElementInterface.isNull())
        return -1;

    leWidth->setText(QString::number(mElementInterface.data()->size().width() * value));
    leHeight->setText(QString::number(mElementInterface.data()->size().height() * value));

    return 0;
}



// Установка настроек элемента по умолчанию
int ElementEditDialog::slotResetSettings()
{
    // Интерфейсный элемент не установлен
    if (mElementInterface.isNull())
        return -1;

    emit signalChangeModule(cbModuleName->currentText());

    return 0;
}



// Сохранение результатов редактирования элемента
int ElementEditDialog::slotBtnOkClicked()
{
    mElementInterface.data()->setElementName(leElementName->text());
    mElementInterface.data()->setModuleName(cbModuleName->currentText());
    mElementInterface.data()->setStartAngle(spbAngle->value());
    mElementInterface.data()->setSizeModificator(spbSize->value());
    mElementInterface.data()->setSize(QSize(leWidth->text().toInt(), leHeight->text().toInt()));
    mElementInterface.data()->setX(leXCoordinate->text().toInt());
    mElementInterface.data()->setY(leYCoordinate->text().toInt());
    mElementInterface.data()->setStartAngle(spbAngle->value());

    int startByte = 0;
    int startBit = 0;
    int bitsCount = 0;
    // 0 - Номер; 1 - Описание; 2 - Номер байта;
    // 3 - Номер бита; 4 - Количество бит
    for (quint16 i = 0; i < tblElementParts->rowCount(); i++)
    {
        startByte = tblElementParts->item(i, 2)->text().toInt();
        startBit = tblElementParts->item(i, 3)->text().toInt();
        bitsCount = tblElementParts->item(i, 4)->text().toInt();

        // Защита от дурака (количество бит == 0)
        if (bitsCount == 0)
            bitsCount = mElementInterface.data()->bitsCount(i);

        mElementInterface.data()->changeParameter(i, startByte, startBit, bitsCount,
                                                  tblElementParts->item(i, 1)->text(),
                                                  tblElementParts->item(i, 5)->checkState());
    }

    // Добавление динамических элементов
    qint32 parametersCount = mElementInterface.data()->extraParametersCount();

    QVariant variant;
    for (int i = 0; i < parametersCount; i++)
    {
        switch(mElementInterface.data()->extraUIElementType(i))
        {
        case ExtraUIElementType::LineEdit:
            variant = qobject_cast<QLineEdit*>(mExtraControls[i])->text();
            break;
        case ExtraUIElementType::ColorDialog:
            variant = qobject_cast<ColorLabel*>(mExtraControls[i])->getColor();
            break;
        case ExtraUIElementType::CheckBox:
            variant = qobject_cast<QCheckBox*>(mExtraControls[i])->isChecked();
            break;
        case ExtraUIElementType::SpinBox:
            variant = qobject_cast<QSpinBox*>(mExtraControls[i])->value();
            break;
        case ExtraUIElementType::Undefined:
            break;
        }

        mElementInterface.data()->setExtraParameter(i, variant);
    }

    this->accept();

    return 0;
}



// Отклонение результатов редактирования элемента
int ElementEditDialog::slotBtnCancelClicked()
{
    this->reject();
    return 0;
}



// Слот - обработка щелчка по CheckBox
int ElementEditDialog::slotParameterStateChanged(QTableWidgetItem* changedItem)
{
    // Столбец с CheckBox = 5
    if (changedItem->column() != 5)
        return -1;

    // Изменение выделения при щелчке на строку
    tblElementParts->selectRow(changedItem->row());

    QTableWidgetItem* item;
    if (changedItem->checkState())
    {
        for (int i = 0; i < tblElementParts->columnCount(); i++)
        {
            item = tblElementParts->item(changedItem->row(), i);
            item->setTextColor(Qt::black);
        }

        tblElementParts->setItemDelegateForRow(changedItem->row(),
                                               new LineEditDelegate(LineEditDelegate::DelegateType::NoValidator, tblElementParts));
    }
    else
    {
        for (int i = 0; i < tblElementParts->columnCount(); i++)
        {
            item = tblElementParts->item(changedItem->row(), i);
            item->setTextColor(Qt::lightGray);
        }

        // Удаление описания к параметру
        tblElementParts->item(changedItem->row(), 1)->setText(QString());
        tblElementParts->item(changedItem->row(), 2)->setText("0");
        tblElementParts->item(changedItem->row(), 3)->setText("0");
        // Запрет редактирования строки
        tblElementParts->setItemDelegateForRow(changedItem->row(),
                                               new LineEditDelegate(LineEditDelegate::DelegateType::NoEdit, tblElementParts));
    }

    // Управление внешним видом динамических параметров
    displayExtraParameters();

    return 0;
}
