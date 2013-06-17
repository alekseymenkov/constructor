#ifndef GDIOBJECT_H
#define GDIOBJECT_H



#include <QtCore>
#include <QColor>
#include "constants.h"



class GDIObject
{
public:
    GDIObject();
    // Параметры цвета
    int setColorParameters(QColor color1, QColor color2, bool isBlinked);
    QColor color(int colorNumber);
    bool isBlinked();
    // Тип объекта
    int setObjectType(GDIObjectType::Type type);
    GDIObjectType::Type objectType();
    // Параметры карандаша
    int setPenParameters(qreal penWidth, Qt::PenStyle penStyle);
    qreal penWidth();
    Qt::PenStyle penStyle();
    // Параметры текста
    int setFontParameters(QString text, FontType::Type fontType,
                          qreal sizeModificator = 1,
                          Qt::Alignment textAlign = Qt::AlignCenter);
    FontType::Type fontType();
    qreal sizeModificator();
    QString text();
    Qt::Alignment textAlign();
    // Функции для сериализации и десериализации объекта
    friend QDataStream& operator>>(QDataStream& dataStream, GDIObject& object);
    friend QDataStream& operator<<(QDataStream& dataStream, const GDIObject& object);


private:
    // Тип объекта
    GDIObjectType::Type mGDIType;
    // Параметры цвета
    QColor mColor1;
    QColor mColor2;
    bool mIsBlinked;
    // Параметры карандаша
    qreal mPenWidth;
    Qt::PenStyle mPenStyle;
    // Параметры текста
    QString mText;
    qreal mSizeModificator;
    FontType::Type mFontType;
    Qt::Alignment mTextAlign;
};
Q_DECLARE_METATYPE(GDIObject)



#endif // GDIOBJECT_H
