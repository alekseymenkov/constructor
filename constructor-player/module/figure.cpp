#include "figure.h"



Figure::Figure() :
    mFigureType(FigureType::Undefined),
    mZIndex(-1),
    mIsStatic(false)
{
    mPoints.clear();
}



qint8 Figure::setFigureParameters(FigureType::Type figureType,
                                  QVector<qint32> points,
                                  bool isStatic,
                                  qint8 zIndex)
{
    mFigureType = figureType;
    mZIndex = zIndex;
    mIsStatic = isStatic;
    mPoints = points;
    return 0;
}



FigureType::Type Figure::figureType()
{
    return mFigureType;
}



QVector<qint32> Figure::points()
{
    return mPoints;
}



qint8 Figure::zIndex()
{
    return mZIndex;
}



bool Figure::isStatic()
{
    return mIsStatic;
}



QDataStream& operator>>(QDataStream& dataStream, Figure& figure)
{
    quint8 figureType = 0;
    dataStream>>figureType;
    figure.mFigureType = static_cast<FigureType::Type>(figureType);
    dataStream>>figure.mZIndex;
    dataStream>>figure.mIsStatic;
    dataStream>>figure.mPoints;

    return dataStream;
}



QDataStream& operator<<(QDataStream& dataStream, const Figure& figure)
{
    dataStream<<static_cast<quint8>(figure.mFigureType);
    dataStream<<figure.mZIndex;
    dataStream<<figure.mIsStatic;
    dataStream<<figure.mPoints;

    return dataStream;
}
