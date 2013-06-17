#ifndef PARAMETER_H
#define PARAMETER_H



#include <QtCore>
#include "constants.h"


class Parameter
{
public:
    Parameter(quint16 bitsCount = 0,
              ParameterType::Type parameterType = ParameterType::Common,
              qint32 extraParameter = -1);
    // Количество бит
    quint16 bitsCount();
    int setBitsCount(quint16 bitsCount);
    // Связанный параметр
    qint32 extraParameter();
    int setExtraParameter(qint32 extraParameter);
    // Тип параметра
    ParameterType::Type parameterType();
    int setParameterType(ParameterType::Type parameterType);
    // Функции сериализации и десериализации объекта
    friend QDataStream& operator>>(QDataStream& dataStream, Parameter& parameter);
    friend QDataStream& operator<<(QDataStream& dataStream, const Parameter& parameter);


private:
    ParameterType::Type mParameterType;
    qint32 mExtraParameter;
    quint16 mBitsCount;
};
Q_DECLARE_METATYPE(Parameter)



#endif // PARAMETER_H
