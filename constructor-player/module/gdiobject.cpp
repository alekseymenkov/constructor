#include "gdiobject.h"



GDIObject::GDIObject() :
    mGDIType(GDIObjectType::Undefined),
    mColor1(Qt::transparent),
    mColor2(Qt::transparent),
    mIsBlinked(false),
    mPenWidth(0),
    mPenStyle(Qt::SolidLine),
    mSizeModificator(1),
    mFontType(FontType::TextType),
    mTextAlign(Qt::AlignCenter)
{
}



// Установка настроек цвета объекта
int GDIObject::setColorParameters(QColor color1, QColor color2, bool isBlinked)
{
    mColor1 = color1;
    mColor2 = color2;
    mIsBlinked = isBlinked;
    return 0;
}



// Получение цвета объекта
QColor GDIObject::color(int colorNumber)
{
    switch (colorNumber)
    {
    case 0:
        return mColor1;
    case 1:
        return mColor2;
    default:
        return Qt::transparent;
    }
}



// Получение значения флага, отвечающего за смену цвета элемента
bool GDIObject::isBlinked()
{
    return mIsBlinked;
}



// Установка типа объекта
int GDIObject::setObjectType(GDIObjectType::Type type)
{
    mGDIType = type;
    return 0;
}



// Получение типа объекта
GDIObjectType::Type GDIObject::objectType()
{
    return mGDIType;
}



// Установка параметров карандаша
int GDIObject::setPenParameters(qreal penWidth, Qt::PenStyle penStyle)
{
    mPenWidth = penWidth;
    mPenStyle = penStyle;
    return 0;
}



// Получение толщины карандаша
qreal GDIObject::penWidth()
{
    return mPenWidth;
}



// Получение стиля карандаша
Qt::PenStyle GDIObject::penStyle()
{
    return mPenStyle;
}



// Установка параметров шрифта
int GDIObject::setFontParameters(QString text, FontType::Type fontType,
                                 qreal sizeModificator, Qt::Alignment textAlign)
{
    mText = text;
    mFontType = fontType;
    mSizeModificator = sizeModificator;
    mTextAlign = textAlign;
    return 0;
}



// Получение параметров шрифта
FontType::Type GDIObject::fontType()
{
    return mFontType;
}



qreal GDIObject::sizeModificator()
{
    return mSizeModificator;
}



// Получение отображаемого текста
QString GDIObject::text()
{
    return mText;
}



Qt::Alignment GDIObject::textAlign()
{
    return mTextAlign;
}



// Десериализация объекта
QDataStream& operator>>(QDataStream& dataStream, GDIObject& object)
{
    quint8 type = 0;

    // Загрузка общих параметров объекта
    dataStream>>type;
    object.mGDIType = static_cast<GDIObjectType::Type>(type);
    dataStream>>object.mColor1;
    dataStream>>object.mColor2;
    dataStream>>object.mIsBlinked;

    // Загрузка дополнительных параметров, в зависимости от типа объекта
    switch (object.mGDIType)
    {
    // Карандаш. Дополнительные параметры - стиль и толщина
    case GDIObjectType::Pen:
        dataStream>>object.mPenWidth;
        dataStream>>type;
        object.mPenStyle = static_cast<Qt::PenStyle>(type);
        break;
        // Текст. Дополнительные параметры - тип шрифта
    case GDIObjectType::Text:
        dataStream>>object.mText;
        dataStream>>object.mSizeModificator;
        dataStream>>type;
        object.mFontType = static_cast<FontType::Type>(type);
        dataStream>>type;
        object.mTextAlign = static_cast<Qt::Alignment>(type);
        break;
    case GDIObjectType::Brush:
        break;
    case GDIObjectType::Undefined:
        break;
    }

    return dataStream;
}



// Сериализация объекта
QDataStream& operator<<(QDataStream& dataStream, const GDIObject& object)
{
    // Выгрузка общих параметров объекта
    dataStream<<static_cast<quint8>(object.mGDIType);
    dataStream<<object.mColor1;
    dataStream<<object.mColor2;
    dataStream<<object.mIsBlinked;

    // Выгрузка дополнительных параметров, в зависимости от типа объекта
    switch (object.mGDIType)
    {
    // Карандаш. Дополнительные параметры - стиль и толщина
    case GDIObjectType::Pen:
        dataStream<<object.mPenWidth;
        dataStream<<static_cast<quint8>(object.mPenStyle);
        break;
        // Текст. Дополнительные параметры - тип шрифта
    case GDIObjectType::Text:
        dataStream<<object.mText;
        dataStream<<object.mSizeModificator;
        dataStream<<static_cast<quint8>(object.mFontType);
        dataStream<<static_cast<quint8>(object.mTextAlign);
        break;
    case GDIObjectType::Brush:
    case GDIObjectType::Undefined:
        break;
    }

    return dataStream;
}
