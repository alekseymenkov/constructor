#include "elementlogic.h"



ElementLogic::ElementLogic()
{
    mParameters.clear();
    mFigures.clear();
    mRecords.clear();
    mDefaultRecords.clear();
    mModuleHash.clear();
    mModuleName.clear();
    mPreviewImagePath.clear();
    mStartAngle = 0;
    mSize = QSize(0, 0);
    mWarningSize = QSize(0, 0);
    mExtraParametersName.clear();
    mExtraParametersType.clear();
    mElementLogicType = ElementLogicType::Common;
}



int ElementLogic::addParameter(Parameter parameter)
{
    mParameters.append(parameter);
    return 0;
}



int ElementLogic::addFigure(Figure figure)
{
    mFigures.append(figure);
    return 0;
}



int ElementLogic::addRecord(Record record)
{
    mRecords.append(record);
    return 0;
}



int ElementLogic::addDefaultRecord(Record defaultRecord)
{
    mDefaultRecords.append(defaultRecord);
    return 0;
}



int ElementLogic::addExtraParameter(QString parameterName,
                                    ExtraParameterType::Type parameterType,
                                    ExtraUIElementType::Type uiType, QVariant defaultValue)
{
    mExtraParametersName.append(parameterName);
    mExtraParametersType.append(parameterType);
    mExtraUIElementType.append(uiType);
    mDefaultExtraValues.append(defaultValue);

    return 0;
}



Parameter ElementLogic::parameter(qint32 parameterID)
{
    if (parameterID < mParameters.size())
        return mParameters[parameterID];
    else
        return Parameter();
}



Figure ElementLogic::figure(qint32 figureID)
{
    if (figureID < mFigures.size())
        return mFigures[figureID];
    else
        return Figure();
}



Record ElementLogic::record(qint32 recordID)
{
    if (recordID < mRecords.size())
        return mRecords[recordID];
    else
        return Record();
}



Record ElementLogic::defaultRecord(qint32 defaultRecordID)
{
    if (defaultRecordID < mDefaultRecords.size())
        return mDefaultRecords[defaultRecordID];
    else
        return Record();
}



QString ElementLogic::extraParameterName(qint32 id)
{
    if (id < mExtraParametersName.size())
        return mExtraParametersName[id];
    else
        return QString();
}



QVariant ElementLogic::extraParameterValue(qint32 id)
{
    if (id < mDefaultExtraValues.size())
        return mDefaultExtraValues[id];
    else
        return QVariant();
}



ExtraParameterType::Type ElementLogic::extraParameterType(qint32 id)
{
    if (id < mExtraParametersType.size())
        return mExtraParametersType[id];
    else
        return ExtraParameterType::Undefined;
}



ExtraUIElementType::Type ElementLogic::extraUIElementType(qint32 id)
{
    if (id < mExtraUIElementType.size())
        return mExtraUIElementType[id];
    else
        return ExtraUIElementType::Undefined;
}



qint32 ElementLogic::parametersCount()
{
    return mParameters.size();
}



qint32 ElementLogic::figuresCount()
{
    return mFigures.size();
}



qint32 ElementLogic::recordsCount()
{
    return mRecords.size();
}



qint32 ElementLogic::defaultRecordsCount()
{
    return mDefaultRecords.size();
}



qint32 ElementLogic::extraParametersCount()
{
    return mExtraParametersName.size();
}



int ElementLogic::setElementLogicType(ElementLogicType::Type type)
{
    mElementLogicType = type;
    return 0;
}



ElementLogicType::Type ElementLogic::elementLogicType()
{
    return mElementLogicType;
}



int ElementLogic::setStartAngle(quint16 startAngle)
{
    mStartAngle = startAngle;
    return 0;
}



quint16 ElementLogic::startAngle()
{
    return mStartAngle;
}



int ElementLogic::setSize(QSize size)
{
    mSize = size;
    return 0;
}



QSize ElementLogic::size()
{
    return mSize;
}



int ElementLogic::setWarningSize(QSize warningSize)
{
    mWarningSize = warningSize;
    return 0;
}



QSize ElementLogic::warningSize()
{
    return mWarningSize;
}



int ElementLogic::setModuleHash(QString moduleHash)
{
    mModuleHash = moduleHash;
    return 0;
}



QString ElementLogic::moduleHash()
{
    return mModuleHash;
}



int ElementLogic::setModuleName(QString moduleName)
{
    mModuleName = moduleName;
    return 0;
}



QString ElementLogic::moduleName()
{
    return mModuleName;
}



int ElementLogic::setPreviewImagePath(QString previewImagePath)
{
    mPreviewImagePath = previewImagePath;
    return 0;
}



QString ElementLogic::previewImagePath()
{
    return mPreviewImagePath;
}



QDataStream& operator>>(QDataStream& dataStream, ElementLogic& element)
{
    quint8 elementLogicType = 0;
    QVector<quint8> extraParameterType;
    QVector<quint8> extraUIElementType;

    dataStream>>element.mParameters;
    dataStream>>element.mFigures;
    dataStream>>element.mRecords;
    dataStream>>element.mDefaultRecords;
    dataStream>>element.mModuleName;
    dataStream>>element.mPreviewImagePath;
    dataStream>>element.mStartAngle;
    dataStream>>element.mSize;
    dataStream>>element.mWarningSize;
    dataStream>>element.mExtraParametersName;
    dataStream>>extraParameterType;
    dataStream>>extraUIElementType;
    dataStream>>element.mDefaultExtraValues;

    element.mExtraParametersType = QVector<ExtraParameterType::Type>(extraParameterType.size(), ExtraParameterType::Undefined);
    for (int i = 0; i < extraParameterType.size(); i++)
        element.mExtraParametersType[i] = static_cast<ExtraParameterType::Type>(extraParameterType[i]);

    element.mExtraUIElementType = QVector<ExtraUIElementType::Type>(extraUIElementType.size(), ExtraUIElementType::Undefined);
    for (int i = 0; i < extraUIElementType.size(); i++)
        element.mExtraUIElementType[i] = static_cast<ExtraUIElementType::Type>(extraUIElementType[i]);

    dataStream>>elementLogicType;
    element.mElementLogicType = static_cast<ElementLogicType::Type>(elementLogicType);

    return dataStream;
}



QDataStream& operator<<(QDataStream& dataStream, const ElementLogic& element)
{
    QVector<quint8> extraUIElementType(element.mExtraUIElementType.size(), 0);
    for (int i = 0; i < element.mExtraUIElementType.size(); i++)
        extraUIElementType[i] = static_cast<quint8>(element.mExtraUIElementType[i]);

    QVector<quint8> extraParameterType(element.mExtraParametersType.size(), 0);
    for (int i = 0; i < element.mExtraParametersType.size(); i++)
        extraParameterType[i] = static_cast<quint8>(element.mExtraParametersType[i]);

    dataStream<<element.mParameters;
    dataStream<<element.mFigures;
    dataStream<<element.mRecords;
    dataStream<<element.mDefaultRecords;
    dataStream<<element.mModuleName;
    dataStream<<element.mPreviewImagePath;
    dataStream<<element.mStartAngle;
    dataStream<<element.mSize;
    dataStream<<element.mWarningSize;
    dataStream<<element.mExtraParametersName;
    dataStream<<extraParameterType;
    dataStream<<extraUIElementType;
    dataStream<<element.mDefaultExtraValues;
    dataStream<<static_cast<quint8>(element.mElementLogicType);

    return dataStream;
}



int ElementLogic::clear()
{
    mParameters.clear();
    mFigures.clear();
    mRecords.clear();
    mDefaultRecords.clear();
    mModuleHash.clear();
    mModuleName.clear();
    mPreviewImagePath.clear();
    mStartAngle = 0;
    mSize = QSize(0, 0);
    mWarningSize = QSize(0, 0);
    mExtraParametersName.clear();
    mExtraParametersType.clear();
    mExtraUIElementType.clear();
    mDefaultExtraValues.clear();
    mElementLogicType = ElementLogicType::Common;

    return 0;
}
