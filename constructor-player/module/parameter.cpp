#include "parameter.h"



Parameter::Parameter(quint16 bitsCount,
                     ParameterType::Type parameterType,
                     qint32 extraParameter) :
    mParameterType(parameterType),
    mExtraParameter(extraParameter),
    mBitsCount(bitsCount)
{
}



quint16 Parameter::bitsCount()
{
    return mBitsCount;
}



int Parameter::setBitsCount(quint16 bitsCount)
{
    mBitsCount = bitsCount;
    return 0;
}



qint32 Parameter::extraParameter()
{
    return mExtraParameter;
}



int Parameter::setExtraParameter(qint32 extraParameter)
{
    mExtraParameter = extraParameter;
    return 0;
}



ParameterType::Type Parameter::parameterType()
{
    return mParameterType;
}



int Parameter::setParameterType(ParameterType::Type parameterType)
{
    mParameterType = parameterType;
    return 0;
}



QDataStream& operator>>(QDataStream& dataStream, Parameter& parameter)
{
    quint8 type = 0;
    // Загрузка общих параметров объекта
    dataStream>>type;
    parameter.mParameterType = static_cast<ParameterType::Type>(type);
    dataStream>>parameter.mExtraParameter;
    dataStream>>parameter.mBitsCount;

    return dataStream;
}



QDataStream& operator<<(QDataStream& dataStream, const Parameter& parameter)
{
    dataStream<<static_cast<quint8>(parameter.mParameterType);
    dataStream<<parameter.mExtraParameter;
    dataStream<<parameter.mBitsCount;

    return dataStream;
}
