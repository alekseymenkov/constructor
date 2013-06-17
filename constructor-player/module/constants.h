#ifndef CONSTANTS_H
#define CONSTANTS_H



#include <QFont>
#include <QColor>

// Ширина линий
class LineWidth
{
public:
    static const qreal selectionLine = 5.5;
    static const qreal contourLine = 4.75;
    static const qreal outerLine = 2.75;
    static const qreal innerLine = 1.5;
};



class Colors
{
public:
    static QColor lightGray;
    static QColor gray;
    static QColor lightGreen;
    static QColor green;
    static QColor lightRed;
    static QColor red;
    static QColor lightBlue;
    static QColor blue;
    static QColor white;
    static QColor black;
    static QColor yellow;
};

struct ElementLogicType
{
    enum Type
    {
        Undefined,
        SingleIndicator,
        DoubleIndicator,
        Common,
        FuelDensity,
        SingleIndicatorDark,
        DoubleIndicatorDark,
        Flap,
        FlapDark,
        Bus,
        BusDark
    };
};


// Работоспособность модуля
struct ModuleState
{
    enum State
    {
        Success,
        Warning,
        Critical
    };
};



// Тип работы с элементом - добавление нового, копирование,
// редактирование, удаление
struct ElementEditDialogType
{
    enum Type
    {
        Add,
        Copy,
        Edit,
        Remove
    };
};



// Тип шрифта
struct FontType
{
    enum Type
    {
        TextType = 1,
        HeaderType = 2
    };
};



// Тип графического примитива
struct FigureType
{
    enum Type
    {
        Undefined,
        Line,       // 4 параметра (x1, y1) - (x2, y2)
        Ellipse,    // 4 параметра (x1, y1, width, height)
        Pie,        // 6 параметров (x1, y1, width, height, startAngle, angle)
        Chord,      // 6 параметров (x1, y1, width, height, startAngle, angle)
        Rect,       // 4 параметра (x1, y1, width, height)
        Polygon,    // 2 * n параметров (x1, y1) - ... - (xn, yn)
        Text,       // 4 параметра (x1, y1, width, height)
        Arc,        // 6 параметров (x1, y1, width, height, startAngle, angle)
        RoundedRect // 5 параметров (x1, y1, width, height, roundValue)
    };
};



// Тип дополнительного параметра модуля
struct ExtraParameterType
{
    enum Type
    {
        Undefined = 0,
        String = 1,
        Integer = 2,
        Real = 3,
        Bool = 4,
        Color = 5
    };
};



// Тип дополнительного элемента UI
struct ExtraUIElementType
{
    enum Type
    {
        Undefined = 0,
        LineEdit = 1,
        ColorDialog = 2,
        CheckBox = 3,
        SpinBox = 4
    };
};



// Тип параметра
struct ParameterType
{
    enum Type
    {
        // Обычные данные
        Common = 0,
        // Числовые данные
        Number = 1
    };
};



// Тип объекта GDI
struct GDIObjectType
{
    enum Type
    {
        Pen = 0,
        Brush = 1,
        Text = 2,
        Undefined = 3
    };
};



// Шрифты
class Font
{
public:
    // Размер обычного текста
    static const int mainFontSize = 19;
    static const int mainFontBold = 100;
    // Размер заголовков
    static const qint8 headerFontSize = 25;
    static const int headerFontBold = 100;
    // Размер заголовков
    static const qint8 letterSpace = 110;
    // Семейство шрифтов
    static QString family;
};



#endif // CONSTANTS_H
