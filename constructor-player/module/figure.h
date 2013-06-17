#ifndef FIGURE_H
#define FIGURE_H



#include <QtCore>
#include "constants.h"



class Figure
{
public:
    Figure();
    // Установка параметров фигуры
    qint8 setFigureParameters(FigureType::Type figureType,
			      QVector<qint32> points,
			      bool isStatic = false,
			      qint8 zIndex = -1);
    FigureType::Type figureType();
    qint8 zIndex();
    bool isStatic();
    QVector<qint32> points();
    // Функции для сериализации и десериализации объекта
    friend QDataStream& operator>>(QDataStream& dataStream, Figure& figure);
    friend QDataStream& operator<<(QDataStream& dataStream, const Figure& figure);


private:
    FigureType::Type mFigureType;
    qint8 mZIndex;
    bool mIsStatic;
    QVector<qint32> mPoints;
};
Q_DECLARE_METATYPE(Figure)



#endif // FIGURE_H
