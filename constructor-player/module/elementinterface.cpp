#include "elementinterface.h"



ElementInterface::ElementInterface(QObject *object) :
    QObject(object),
    mMinByte(-1),
    mMaxByte(0),
    mX(0),
    mY(0),
    mSize(0, 0),
    mParametersCount(0),
    mSizeModificator(1.0),
    mAngle(0),
    mModuleState(ModuleState::Critical),
    mIsToolTipShow(false)
{
    mStartBytes.clear();
    mStartBits.clear();
    mBitsCount.clear();
    mDescriptions.clear();
    mIsParameterCorrect.clear();
    mIsParameterUsed.clear();

    mState.clear();

    mModuleName.clear();
    mModuleHash.clear();
    mElementName.clear();

    mExtraParameters.clear();
}



ElementInterface::ElementInterface(QSharedPointer<ElementLogic> elementLogic, QObject *object) :
    QObject(object),
    mX(0),
    mY(0),
    mSize(0, 0),
    mParametersCount(0),
    mSizeModificator(1.0),
    mAngle(0),
    mModuleState(ModuleState::Critical),
    mIsToolTipShow(false)
{
    mElementLogic = elementLogic;

    int extraParametersCount = 0;
    if (!mElementLogic.isNull())
    {
        mParametersCount = mElementLogic.data()->parametersCount();
        mAngle = mElementLogic.data()->startAngle();
        extraParametersCount = mElementLogic.data()->extraParametersCount();

        if (mSize.isEmpty())
            mSize = elementLogic.data()->size();
    }

    mStartBytes = QVector<quint16>(mParametersCount);
    mStartBits = QVector<quint8>(mParametersCount);
    mBitsCount = QVector<quint16>(mParametersCount);
    mDescriptions = QVector<QString>(mParametersCount);
    mIsParameterCorrect = QVector<bool>(mParametersCount);
    mIsParameterUsed = QVector<bool>(mParametersCount, true);
    mExtraParameters = QVector<QVariant>(extraParametersCount);

    for (int i = 0; i < mExtraParameters.size(); i++)
    {
        QVariant extraParameterValue = mElementLogic.data()->extraParameterValue(i);
        switch(mElementLogic.data()->extraParameterType(i))
        {
        case ExtraParameterType::Integer:
            mExtraParameters[i] = extraParameterValue.isNull() ? QVariant(1) : extraParameterValue;
            break;
        case ExtraParameterType::Real:
            mExtraParameters[i] = extraParameterValue.isNull() ? QVariant(1.0) : extraParameterValue;
            break;
        case ExtraParameterType::String:
            mExtraParameters[i] = extraParameterValue.isNull() ? QVariant("") : extraParameterValue;
            break;
        case ExtraParameterType::Bool:
            mExtraParameters[i] = extraParameterValue.isNull() ? QVariant(true) : extraParameterValue;
            break;
        case ExtraParameterType::Color:
            mExtraParameters[i] = QVariant(Qt::white);
            break;
        case ExtraParameterType::Undefined:
            break;
        }
    }

    mModuleName.clear();
    mElementName.clear();
    mModuleHash = elementLogic.data()->moduleHash();

    // Пересчет запрашиваемого диапазона данных
    checkRange();
}



ElementInterface::ElementInterface(const ElementInterface& elementInterface)
{
    mStartBytes = elementInterface.mStartBytes;
    mStartBits = elementInterface.mStartBits;
    mBitsCount = elementInterface.mBitsCount;
    mDescriptions = elementInterface.mDescriptions;
    mIsParameterCorrect = elementInterface.mIsParameterCorrect;
    mIsParameterUsed = elementInterface.mIsParameterUsed;
    mMinByte = elementInterface.mMinByte;
    mMaxByte = elementInterface.mMaxByte;
    mState = elementInterface.mState;
    mX = elementInterface.mX;
    mY = elementInterface.mY;
    mSize = elementInterface.mSize;
    mElementLogic = elementInterface.mElementLogic;
    mParametersCount = elementInterface.mParametersCount;
    mSizeModificator = elementInterface.mSizeModificator;
    mModuleName = elementInterface.mModuleName;
    mModuleHash = elementInterface.mModuleHash;
    mElementName = elementInterface.mElementName;
    mAngle = elementInterface.mAngle;
    mModuleState = elementInterface.mModuleState;
    mExtraParameters = elementInterface.mExtraParameters;
    mIsToolTipShow = elementInterface.mIsToolTipShow;
}



bool ElementInterface::addParameter(quint16 startByte, quint8 startBit, quint16 bitsCount,
                                    QString description, bool isUsed)
{
    mStartBytes.append(startByte);
    mStartBits.append(startBit);
    mBitsCount.append(bitsCount);
    mDescriptions.append(description);
    mIsParameterUsed.append(isUsed);
    if (description.isEmpty() && isUsed)
        mIsParameterCorrect.append(false);
    else
        mIsParameterCorrect.append(true);
    mParametersCount++;

    // Пересчет запрашиваемого диапазона данных
    checkRange();

    return true;
}



// Изменение существующего параметра
bool ElementInterface::changeParameter(qint32 parameterID, quint16 startByte,
                                       quint8 startBit, quint16 bitsCount,
                                       QString description, bool isUsed)
{
    if (parameterID >= mParametersCount)
        return false;

    mStartBytes[parameterID] = startByte;
    mStartBits[parameterID] = startBit;
    mBitsCount[parameterID] = bitsCount;
    mDescriptions[parameterID] = description;
    mIsParameterUsed[parameterID] = isUsed;
    if (description.isEmpty() && isUsed)
        mIsParameterCorrect[parameterID] = false;
    else
        mIsParameterCorrect[parameterID] = true;

    // Пересчет запрашиваемого диапазона данных
    checkRange();

    return 0;
}



int ElementInterface::setSizeModificator(qreal sizeModificator)
{
    mSizeModificator = sizeModificator;
    return 0;
}



qreal ElementInterface::sizeModificator()
{
    return mSizeModificator;
}



QSize ElementInterface::size()
{
    if (mElementLogic.isNull())
        return QSize(0, 0);
    else
        return mSize;
}



int ElementInterface::setSize(QSize size)
{
    mSize = size;
    return 0;
}



int ElementInterface::setModuleName(QString moduleName)
{
    mModuleName = moduleName;
    return 0;
}



QString ElementInterface::moduleName()
{
    return mModuleName;
}



QString ElementInterface::fullModuleName()
{
    if (mElementLogic.isNull())
        return mModuleName;

    QString moduleName = mElementLogic.data()->moduleName();

    if (moduleName.isEmpty())
        return mModuleName;
    else
        return QString("%1 (%2)").arg(moduleName).arg(mModuleName);
}



int ElementInterface::setModuleHash(QString moduleHash)
{
    mModuleHash = moduleHash;
    return 0;
}



QString ElementInterface::moduleHash()
{
    return mModuleHash;
}



int ElementInterface::setStartAngle(qint16 angle)
{
    mAngle = angle;
    return 0;
}



qint16 ElementInterface::startAngle()
{
    return mAngle;
}



int ElementInterface::setElementName(QString elementName)
{
    mElementName = elementName;
    return 0;
}



QString ElementInterface::elementName()
{
    return mElementName;
}



quint16 ElementInterface::startByte(qint32 parameterNum)
{
    if (parameterNum < mParametersCount)
        return mStartBytes[parameterNum];
    else
        return 0;
}



quint8 ElementInterface::startBit(qint32 parameterNum)
{
    if (parameterNum < mParametersCount)
        return mStartBits[parameterNum];
    else
        return 0;
}



quint16 ElementInterface::bitsCount(qint32 parameterNum)
{
    quint16 bitsCount = 0;
    if (!mElementLogic.isNull() && parameterNum < mParametersCount)
    {
        if (mElementLogic.data()->parameter(parameterNum).parameterType() == ParameterType::Number &&
                mBitsCount[parameterNum] != 0)
            bitsCount = mBitsCount[parameterNum];
        else
            bitsCount = mElementLogic.data()->parameter(parameterNum).bitsCount();
    }

    return bitsCount;
}



QString ElementInterface::description(qint32 parameterNum)
{
    if (parameterNum < mParametersCount)
        return mDescriptions[parameterNum];
    else
        return QString();
}



ParameterType::Type ElementInterface::parameterType(qint32 parameterNum)
{
    ParameterType::Type type = ParameterType::Common;
    if (!mElementLogic.isNull() && parameterNum < mParametersCount)
        type = mElementLogic.data()->parameter(parameterNum).parameterType();
    return type;
}



bool ElementInterface::isParameterCorrect(qint32 parameterNum)
{
    if (parameterNum < mParametersCount)
        return mIsParameterCorrect[parameterNum];
    else
        return false;
}

bool ElementInterface::isParameterUsed(qint32 parameterNum)
{
    if (parameterNum < mParametersCount)
        return mIsParameterUsed[parameterNum];
    else
        return false;
}



int ElementInterface::checkRange()
{
    const int partOfByte = 7;
    const int byte = 8;

    mActualParametersCount = 0;

    mMinByte = -1;
    mMaxByte = 0;
    for (int i = 0; i < mParametersCount; i++)
    {
        if (!mIsParameterUsed[i])
            continue;

        if (mStartBytes[i] < mMinByte)
            mMinByte = mStartBytes[i];

        int bitsCount = 0;
        if (!mElementLogic.isNull() && mElementLogic.data()->parameter(i).parameterType() != ParameterType::Number)
            bitsCount = mElementLogic.data()->parameter(i).bitsCount();
        else if (!mElementLogic.isNull() && mElementLogic.data()->parameter(i).parameterType() == ParameterType::Number)
            bitsCount = mBitsCount[i];

        quint32 maxByte = mStartBytes[i] + (mStartBits[i] + bitsCount + partOfByte) / byte;
        if (maxByte > mMaxByte)
            mMaxByte = maxByte;

        mActualParametersCount++;
    }

    return 0;
}



qint32 ElementInterface::parametersCount()
{
    return mParametersCount;
}



qint32 ElementInterface::actualParametersCount()
{
    return mActualParametersCount;
}



int ElementInterface::setModuleState(ModuleState::State moduleState)
{
    mModuleState = moduleState;
    return 0;
}



ModuleState::State ElementInterface::moduleState()
{
    return mModuleState;
}



quint16 ElementInterface::startByte()
{
    // Проверка на случай отключенных параметров
    if (mMinByte == 65535)
        return 0;
    else
        return mMinByte;
}



quint16 ElementInterface::bytesCount()
{
    if (mMinByte == 65535)
        return 0;
    else
        return mMaxByte - mMinByte;
}



int ElementInterface::setX(qint32 x)
{
    mX = x;
    return 0;
}



qint32 ElementInterface::x()
{
    return mX;
}



int ElementInterface::setY(qint32 y)
{
    mY = y;
    return 0;
}



qint32 ElementInterface::y()
{
    return mY;
}



int ElementInterface::setState(QVector<bool> state)
{
    mState = state;
    return 0;
}



int ElementInterface::resetState()
{
    const int bitsInByte = 8;
    mState = QVector<bool>(this->bytesCount() * bitsInByte, false);
    return 0;
}



int ElementInterface::clear()
{
    mStartBytes = QVector<quint16>(mParametersCount, 0);
    mStartBits = QVector<quint8>(mParametersCount, 0);
    mBitsCount = QVector<quint16>(mParametersCount, 0);
    mDescriptions = QVector<QString>(mParametersCount);
    mIsParameterCorrect = QVector<bool>(mParametersCount, false);
    mIsParameterUsed = QVector<bool>(mParametersCount, true);

    mMinByte = 0;
    mMaxByte = 0;

    mSizeModificator = 1;

    mElementLogic.clear();
    mModuleState = ModuleState::Critical;

    mExtraParameters.clear();

    return 0;
}



// Проверка состояния элемента
int ElementInterface::checkCorrect()
{
    bool criticalState1 = true;
    bool criticalState2 = true;
    bool criticalState3 = true;

    // Для интерфейса не установлена логика (ElementLogic)
    criticalState1 = mElementLogic.isNull();
    // Хеш на время создания элемента не равен текущему
    if (!criticalState1)
        criticalState2 = mModuleHash != mElementLogic.data()->moduleHash();
    // Хеш для элемента не определен
    criticalState3 = mModuleHash.isEmpty();

    if (criticalState1 || criticalState2 || criticalState3)
    {
        mModuleState = ModuleState::Critical;
        return -2;
    }

    // Корректность задания всех параметров в элементе
    bool warningState1 = false;
    for (quint16 i = 0; i < mParametersCount; i++)
    {
        if (mIsParameterCorrect[i])
            continue;
        warningState1 = true;
        break;
    }

    // Имя элемента - пустое
    bool warningState2 = false;
    if (mElementName.isEmpty())
        warningState2 = true;

    if (warningState1 || warningState2)
    {
        mModuleState = ModuleState::Warning;
        return -1;
    }

    mModuleState = ModuleState::Success;

    return 0;
}



// Функция установки логики для уже сформированных интерфейсных элементов,
// загружаемых из файла elements.xml. Для создания интерфейсных элементов
// используется конструктор, принимающий в качестве параметра ElementLogic
int ElementInterface::setElementLogic(QSharedPointer<ElementLogic> elementLogic)
{
    mElementLogic = elementLogic;
    //    if (!mElementLogic.isNull() && mSize.isEmpty())
    //        mSize = elementLogic.data()->size();

    return 0;
}



int ElementInterface::addExtraParameter(QVariant parameter)
{
    mExtraParameters.append(parameter);
    return 0;
}



int ElementInterface::setExtraParameter(qint32 parameterID, QVariant parameter)
{
    if (parameterID < mExtraParameters.size())
        mExtraParameters[parameterID] = parameter;
    return 0;
}



QVariant ElementInterface::extraParameter(qint32 parameterID)
{
    if (parameterID < mExtraParameters.size())
        return mExtraParameters[parameterID];
    else
        return QVariant();
}



QVariant ElementInterface::defaultExtraParameter(qint32 parameterID)
{
    if (!mElementLogic.isNull())
        return mElementLogic.data()->extraParameterValue(parameterID);
    else
        return QVariant();
}



// Генерация всплывающей подсказки
// Если подсказка еще не отображена - высылаем виджету ToolTip название элемента,
// количество параметров, диапазон бит для каждого параметр, значения параметров,
// описания к параметрам и положение мыши, откуда была вызвана всплывающая подсказка
// В случае, если подсказка уже отображена - высылаем только значения параметров,
// а все остальные данные кешируются в виджете ToolTip
int ElementInterface::slotReceiveDataToToolTip(QVector<QString> parametersValues, bool isConflict)
{
    QString name;
    name.append("<h2>").append(mElementName).append(isConflict ? "<span style='color: rgb(255, 32, 32);'> (КОНФЛИКТ)</span></h2>" : " </h2>");

    if (!mIsToolTipShow)
        mMousePosition = QCursor::pos();

    QVector<int> parametersToRemove;
    for (int i = 0; i < mElementLogic.data()->parametersCount(); i++)
    {
        if (!mIsParameterUsed[i])
            parametersToRemove.append(i);
    }

    // Массив описаний к параметрам
    QVector<QString> descriptions;

    if (!mIsToolTipShow)
    {
        QVector<QString> bitsRange;
        for (int i = 0; i < mParametersCount; i++)
        {
            if (!mIsParameterUsed[i])
                continue;

            const int bitsInByte = 8;
            // Два формата вывода информации о запрашиваемых данных
            QString range;
            int bitsCount = 0;
            if (mElementLogic.data()->parameter(i).parameterType() == ParameterType::Number)
                bitsCount = mBitsCount[i];
            else
                bitsCount = mElementLogic.data()->parameter(i).bitsCount();
            int startBit = (mStartBytes[i] - mMinByte) * bitsInByte + mStartBits[i];
            if (this->bitsCount(i) == 1)
                range = QString("%1").arg(startBit);
            else
                range = QString("%1 - %2")
                        .arg(startBit)
                        .arg(startBit + bitsCount - 1);
            bitsRange.append(range);

            descriptions.append(mDescriptions[i]);
        }

        // Удаление неиспользуемых параметров
        for (int i = parametersToRemove.size() - 1; i >= 0; i--)
            parametersValues.remove(parametersToRemove[i]);

        emit signalSendToopTipInfo(name, this->actualParametersCount(),
                                   bitsRange, parametersValues,
                                   descriptions, mMousePosition);
        mIsToolTipShow = true;
    }
    else
    {
        // Удаление неиспользуемых параметров
        for (int i = parametersToRemove.size() - 1; i >= 0; i--)
            parametersValues.remove(parametersToRemove[i]);

        emit signalSendToolTipInfo(name, this->actualParametersCount(),
                                   parametersValues);
    }

    return 0;
}



int ElementInterface::slotToolTipHide()
{
    mIsToolTipShow = false;
    return 0;
}



qint32 ElementInterface::extraParametersCount()
{
    if (!mElementLogic.isNull())
        return mElementLogic.data()->extraParametersCount();
    else
        return 0;
}



QString ElementInterface::extraParameterName(qint32 id)
{
    if (!mElementLogic.isNull())
        return mElementLogic.data()->extraParameterName(id);
    else
        return QString();
}



ExtraParameterType::Type ElementInterface::extraParameterType(qint32 id)
{
    if (!mElementLogic.isNull())
        return mElementLogic.data()->extraParameterType(id);
    else
        return ExtraParameterType::Undefined;
}



ExtraUIElementType::Type ElementInterface::extraUIElementType(qint32 id)
{
    if (!mElementLogic.isNull())
        return mElementLogic.data()->extraUIElementType(id);
    else
        return ExtraUIElementType::Undefined;
}



QString ElementInterface::previewImagePath()
{
    if (!mElementLogic.isNull())
        return mElementLogic.data()->previewImagePath();
    else
        return QString();
}



ElementLogicType::Type ElementInterface::elementLogicType()
{
    if (!mElementLogic.isNull())
        return mElementLogic.data()->elementLogicType();
    else
        return ElementLogicType::Undefined;
}
