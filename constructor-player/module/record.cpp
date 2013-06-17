#include "record.h"



Record::Record() :
    mParameterID(0),
    mFigureID(0),
    mAngle(-1)
{
    mState.clear();
    mGDIObjects.clear();
}



int Record::setParameterID(qint32 parameterID)
{
    mParameterID = parameterID;
    return 0;
}



int Record::setState(QVector<bool> state)
{
    mState = state;
    return 0;
}



int Record::setAngle(qint16 angle)
{
    mAngle = angle;
    return 0;
}



int Record::setRecord(qint32 figureID, QVector<GDIObject> gdiObjects)
{
    mFigureID = figureID;
    mGDIObjects = gdiObjects;
    return 0;
}



qint32 Record::parameterID()
{
    return mParameterID;
}



QVector<bool> Record::state()
{
    return mState;
}



qint16 Record::angle()
{
    return mAngle;
}



qint32 Record::figureID()
{
    return mFigureID;
}



QVector<GDIObject> Record::GDIObjects()
{
    return mGDIObjects;
}



QDataStream& operator>>(QDataStream& dataStream, Record& record)
{
    dataStream>>record.mParameterID;
    dataStream>>record.mState;
    dataStream>>record.mFigureID;
    dataStream>>record.mGDIObjects;
    dataStream>>record.mAngle;

    return dataStream;
}



QDataStream& operator<<(QDataStream& dataStream, const Record& object)
{
    dataStream<<object.mParameterID;
    dataStream<<object.mState;
    dataStream<<object.mFigureID;
    dataStream<<object.mGDIObjects;
    dataStream<<object.mAngle;

    return dataStream;
}
