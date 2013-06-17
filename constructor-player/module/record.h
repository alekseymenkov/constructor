#ifndef RECORD_H
#define RECORD_H



#include <QtCore>
#include "constants.h"
#include "gdiobject.h"
#include "parameter.h"



class Record
{
public:
    Record();
    // Параметры записи
    int setParameterID(qint32 parameterID);
    int setState(QVector<bool> state);
    int setAngle(qint16 angle);
    int setRecord(qint32 figureID, QVector<GDIObject> gdiObjects);
    // Идентификатор параметра
    qint32 parameterID();
    // Состояние параметра
    QVector<bool> state();
    // Угол поворота фигуры
    qint16 angle();
    qint32 figureID();
    // Параметры фигур для прорисоки
    QVector<GDIObject> GDIObjects();
    // Функции для сериализации и десериализации
    friend QDataStream& operator>>(QDataStream& dataStream, Record& record);
    friend QDataStream& operator<<(QDataStream& dataStream, const Record& record);


private:
    // Номер параметра
    qint32 mParameterID;
    // Состояние параметра
    QVector<bool> mState;
    // Номера фигур
    qint32 mFigureID;
    // Состояние фигур
    QVector<GDIObject> mGDIObjects;
    // Угол поворота фигур
    qint16 mAngle;

};
Q_DECLARE_METATYPE(Record)



#endif // RECORD_H
