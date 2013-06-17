#include "logicloader.h"



LogicLoader::LogicLoader(QObject* parent) :
    QObject(parent),
    mSlash("/"),
    mModulesFolder("/modules"),
    mBasePath(qApp->applicationDirPath() + mModulesFolder),
    mProjectPath(QString()),
    mOldProjectPath(QString())
{
    mModules.clear();

    // Проверка наличия папок, необходимых для работы программы
    // Папка программы с модулями
    mBaseDir = QDir(mBasePath);
    if (!mBaseDir.exists())
        QDir().mkdir(mBasePath);

    mDirectoryWatcher = new QFileSystemWatcher(this);
    mDirectoryWatcher->addPath(qApp->applicationDirPath());

    connect(mDirectoryWatcher,
            SIGNAL(directoryChanged(QString)),
            this,
            SLOT(slotDirectoryChanged(QString)));

    mFileWatcher = new QFileSystemWatcher(this);
    mFileWatcher->addPath(mBasePath);

    connect(mFileWatcher,
            SIGNAL(directoryChanged(QString)),
            this,
            SLOT(slotFilesChanged(QString)));

    // Получение списка модулей в папке
    this->refreshModulesList();

    // Серый очень толстый карандаш (LineWidth::selectionLine)
    GDIObject greySuperThickPen;
    greySuperThickPen.setObjectType(GDIObjectType::Pen);
    greySuperThickPen.setColorParameters(Colors::gray, Colors::gray, false);
    greySuperThickPen.setPenParameters(LineWidth::selectionLine * 3, Qt::SolidLine);

QVector<GDIObject> gdiObjects;
    gdiObjects.clear();
    gdiObjects<<greySuperThickPen;

    QFile file("C:\\wtf.bin");
    file.open(QIODevice::WriteOnly);
    QDataStream ds(&file);


    ds<<gdiObjects;

    // Зеленая кисть
    GDIObject greenBrush;
    greenBrush.setObjectType(GDIObjectType::Brush);
    greenBrush.setColorParameters(Colors::lightGreen, Colors::lightGreen, false);
    // Красная кисть
    GDIObject redBrush;
    redBrush.setObjectType(GDIObjectType::Brush);
    redBrush.setColorParameters(Colors::lightRed, Colors::lightRed, false);
    // Синяя кисть
    GDIObject blueBrush;
    blueBrush.setObjectType(GDIObjectType::Brush);
    blueBrush.setColorParameters(Colors::lightBlue, Colors::lightBlue, false);
    // Без кисти
    GDIObject noBrush;
    noBrush.setObjectType(GDIObjectType::Brush);
    noBrush.setColorParameters(Qt::transparent, Qt::transparent, false);
    // Белая кисть
    GDIObject whiteBrush;
    whiteBrush.setObjectType(GDIObjectType::Brush);
    whiteBrush.setColorParameters(Colors::white, Colors::white, false);
    // Серая кисть
    GDIObject greyBrush;
    greyBrush.setObjectType(GDIObjectType::Brush);
    greyBrush.setColorParameters(Colors::gray, Colors::gray, false);
    // Желтая кисть
    GDIObject yellowBrush;
    yellowBrush.setObjectType(GDIObjectType::Brush);
    yellowBrush.setColorParameters(Colors::yellow, Colors::yellow, false);
    // Красно-белая кисть
    GDIObject redWhiteBrush;
    redWhiteBrush.setObjectType(GDIObjectType::Brush);
    redWhiteBrush.setColorParameters(Colors::lightRed, Colors::white, true);
    // Сине-белая кисть
    GDIObject blueWhiteBrush;
    blueWhiteBrush.setObjectType(GDIObjectType::Brush);
    blueWhiteBrush.setColorParameters(Colors::lightBlue, Colors::white, true);
    // Зелено-белая кисть
    GDIObject greenWhiteBrush;
    greenWhiteBrush.setObjectType(GDIObjectType::Brush);
    greenWhiteBrush.setColorParameters(Colors::lightGreen, Colors::white, true);
    // Черная кисть
    GDIObject blackBrush;
    blackBrush.setObjectType(GDIObjectType::Brush);
    blackBrush.setColorParameters(Colors::black, Colors::black, false);
    // Черный карандаш (LineWidth::innerLine)
    GDIObject blackThinPen;
    blackThinPen.setObjectType(GDIObjectType::Pen);
    blackThinPen.setColorParameters(Colors::black, Colors::black, false);
    blackThinPen.setPenParameters(LineWidth::innerLine, Qt::SolidLine);
    // Черный карандаш (LineWidth::outerLine)
    GDIObject blackMediumPen;
    blackMediumPen.setObjectType(GDIObjectType::Pen);
    blackMediumPen.setColorParameters(Colors::black, Colors::black, false);
    blackMediumPen.setPenParameters(LineWidth::outerLine, Qt::SolidLine);
    // Черный мигающий карандаш (LineWidth::contourLine)
    GDIObject blackTransContourPen;
    blackTransContourPen.setObjectType(GDIObjectType::Pen);
    blackTransContourPen.setColorParameters(Colors::black, Qt::transparent, true);
    blackTransContourPen.setPenParameters(LineWidth::contourLine, Qt::SolidLine);
    // БЕлый мигающий карандаш (LineWidth::contourLine)
    GDIObject whiteTransContourPen;
    whiteTransContourPen.setObjectType(GDIObjectType::Pen);
    whiteTransContourPen.setColorParameters(Colors::white, Qt::transparent, true);
    whiteTransContourPen.setPenParameters(LineWidth::contourLine, Qt::SolidLine);
    // Красный мигающий карандаш (LineWidth::contourLine)
    GDIObject redTransContourPen;
    redTransContourPen.setObjectType(GDIObjectType::Pen);
    redTransContourPen.setColorParameters(Colors::red, Qt::transparent, true);
    redTransContourPen.setPenParameters(LineWidth::contourLine, Qt::SolidLine);
    // Черный карандаш (LineWidth::contourLine)
    GDIObject blackContourPen;
    blackContourPen.setObjectType(GDIObjectType::Pen);
    blackContourPen.setColorParameters(Colors::black, Colors::black, false);
    blackContourPen.setPenParameters(LineWidth::contourLine, Qt::SolidLine);
    // Черный карандаш (LineWidth::contourLine)
    GDIObject whiteContourPen;
    whiteContourPen.setObjectType(GDIObjectType::Pen);
    whiteContourPen.setColorParameters(Colors::white, Colors::white, false);
    whiteContourPen.setPenParameters(LineWidth::contourLine, Qt::SolidLine);
    // Зеленый карандаш (LineWidth::contourLine)
    GDIObject greenContourPen;
    greenContourPen.setObjectType(GDIObjectType::Pen);
    greenContourPen.setColorParameters(Colors::green, Colors::green, false);
    greenContourPen.setPenParameters(LineWidth::contourLine, Qt::SolidLine);
    // Красный карандаш (LineWidth::contourLine)
    GDIObject redContourPen;
    redContourPen.setObjectType(GDIObjectType::Pen);
    redContourPen.setColorParameters(Colors::red, Colors::red, false);
    redContourPen.setPenParameters(LineWidth::contourLine, Qt::SolidLine);
    // Без карандаша
    GDIObject noPen;
    noPen.setObjectType(GDIObjectType::Pen);
    noPen.setColorParameters(Qt::transparent, Qt::transparent, false);
    noPen.setPenParameters(0, Qt::SolidLine);
    // Черно-красный карандаш (LineWidth::outerLine)
    GDIObject blackRedPen;
    blackRedPen.setObjectType(GDIObjectType::Pen);
    blackRedPen.setColorParameters(Colors::black, Colors::red, true);
    blackRedPen.setPenParameters(1, Qt::SolidLine);
    // Красный карандаш (LineWidth::outerLine)
    GDIObject redMediumPen;
    redMediumPen.setObjectType(GDIObjectType::Pen);
    redMediumPen.setColorParameters(Colors::red, Colors::red, false);
    redMediumPen.setPenParameters(LineWidth::outerLine, Qt::SolidLine);
    // Зеленый карандаш (LineWidth::outerLine)
    GDIObject greenMediumPen;
    greenMediumPen.setObjectType(GDIObjectType::Pen);
    greenMediumPen.setColorParameters(Colors::green, Colors::green, false);
    greenMediumPen.setPenParameters(LineWidth::outerLine, Qt::SolidLine);
    // Белый карандаш (LineWidth::outerLine)
    GDIObject whiteMediumPen;
    whiteMediumPen.setObjectType(GDIObjectType::Pen);
    whiteMediumPen.setColorParameters(Colors::white, Colors::white, false);
    whiteMediumPen.setPenParameters(LineWidth::outerLine, Qt::SolidLine);
    // Зеленый карандаш (LineWidth::selectionLine)
    GDIObject greenThickPen;
    greenThickPen.setObjectType(GDIObjectType::Pen);
    greenThickPen.setColorParameters(Colors::green, Colors::green, false);
    greenThickPen.setPenParameters(LineWidth::selectionLine, Qt::SolidLine);
    // Синий карандаш (LineWidth::selectionLine)
    GDIObject blueThickPen;
    blueThickPen.setObjectType(GDIObjectType::Pen);
    blueThickPen.setColorParameters(Colors::blue, Colors::blue, false);
    blueThickPen.setPenParameters(LineWidth::selectionLine, Qt::SolidLine);
    // Красный карандаш (LineWidth::selectionLine)
    GDIObject redThickPen;
    redThickPen.setObjectType(GDIObjectType::Pen);
    redThickPen.setColorParameters(Colors::red, Colors::red, false);
    redThickPen.setPenParameters(LineWidth::selectionLine, Qt::SolidLine);
    // Черный карандаш (LineWidth::selectionLine)
    GDIObject blackThickPen;
    blackThickPen.setObjectType(GDIObjectType::Pen);
    blackThickPen.setColorParameters(Colors::black, Colors::black, false);
    blackThickPen.setPenParameters(LineWidth::selectionLine, Qt::SolidLine);
    // Текстовый элемент
    GDIObject text;
    text.setObjectType(GDIObjectType::Text);
    text.setFontParameters("", FontType::HeaderType);
    // Текстовый элемент, по умолчанию "0.00"
    GDIObject defaultText;
    defaultText.setObjectType(GDIObjectType::Text);
    defaultText.setFontParameters("0.00", FontType::HeaderType);


    bool _COMPILE_MODULES_ = false;


    // ===============================================================
    // ===============================================================
    // Элемент Bus (шина) ========================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int w = 100;
        const int h = 25;

        // Рамка (коэфф. скругления углов)
        pointsArray.clear();
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);

        // Параметр 1
        parameter.setBitsCount(1);
        parameter.setParameterType(ParameterType::Common);
        parameter.setExtraParameter(-1);
        parameters.append(parameter);

        // Заливка при неактивном бите
        gdiObjects.clear();
        gdiObjects<<greyBrush;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, false));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Заливка при активном бите
        gdiObjects.clear();
        gdiObjects<<greenBrush;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Рамка по умолчанию
        gdiObjects.clear();
        gdiObjects<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        // Размер элемента
        logic.setSize(QSize(w, h));
        // Стартовый угол
        logic.setStartAngle(0);
        // Дополнительные параметры
        logic.setModuleName("Шина");
        logic.setElementLogicType(ElementLogicType::Bus);

        logic.setPreviewImagePath("images/bus.png");

        QFile file(mBasePath + "/Bus.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }


    // ===============================================================
    // ===============================================================
    // Элемент BusDark (шина) ====================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int w = 100;
        const int h = 25;

        // Рамка (коэфф. скругления углов)
        pointsArray.clear();
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);

        // Параметр 1
        parameter.setBitsCount(1);
        parameter.setParameterType(ParameterType::Common);
        parameter.setExtraParameter(-1);
        parameters.append(parameter);

        // Заливка при неактивном бите
        gdiObjects.clear();
        gdiObjects<<greyBrush;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, false));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Заливка при активном бите
        gdiObjects.clear();
        gdiObjects<<greenBrush;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Рамка по умолчанию
        gdiObjects.clear();
        gdiObjects<<whiteContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        // Размер элемента
        logic.setSize(QSize(w, h));
        // Стартовый угол
        logic.setStartAngle(0);
        // Дополнительные параметры
        logic.setModuleName("Шина");
        logic.setElementLogicType(ElementLogicType::BusDark);

        logic.setPreviewImagePath("images/bus.png");

        QFile file(mBasePath + "/BusDark.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }


    // ===============================================================
    // ===============================================================
    // Элемент Flap (створка) ========================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int w = 100;
        const int h = 25;

        // Рамка (коэфф. скругления углов)
        pointsArray.clear();
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);

        // Параметр 1
        parameter.setBitsCount(1);
        parameter.setParameterType(ParameterType::Common);
        parameter.setExtraParameter(-1);
        parameters.append(parameter);

        // Рамка по умолчанию
        gdiObjects.clear();
        gdiObjects<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        // Размер элемента
        logic.setSize(QSize(w, h));
        // Стартовый угол
        logic.setStartAngle(0);
        // Дополнительные параметры
        logic.addExtraParameter("Точка поворота:", ExtraParameterType::Integer, ExtraUIElementType::LineEdit, QVariant(1));
        logic.addExtraParameter("Угол поворота:", ExtraParameterType::Integer, ExtraUIElementType::LineEdit, QVariant(0));
        logic.addExtraParameter("Цвет при значении 0:", ExtraParameterType::Color, ExtraUIElementType::ColorDialog);
        logic.addExtraParameter("Цвет при значении 1:", ExtraParameterType::Color, ExtraUIElementType::ColorDialog);
        logic.setModuleName("Створка");
        logic.setElementLogicType(ElementLogicType::Flap);

        logic.setPreviewImagePath("images/flap.png");

        QFile file(mBasePath + "/Flap.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }


    // ===============================================================
    // ===============================================================
    // Элемент FlapDark (створка) ====================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int w = 100;
        const int h = 25;

        // Рамка (коэфф. скругления углов)
        pointsArray.clear();
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);

        // Параметр 1
        parameter.setBitsCount(1);
        parameter.setParameterType(ParameterType::Common);
        parameter.setExtraParameter(-1);
        parameters.append(parameter);

        // Рамка по умолчанию
        gdiObjects.clear();
        gdiObjects<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        // Размер элемента
        logic.setSize(QSize(w, h));
        // Стартовый угол
        logic.setStartAngle(0);
        // Дополнительные параметры
        logic.addExtraParameter("Точка поворота:", ExtraParameterType::Integer, ExtraUIElementType::LineEdit, QVariant(1));
        logic.addExtraParameter("Угол поворота:", ExtraParameterType::Integer, ExtraUIElementType::LineEdit, QVariant(0));
        logic.addExtraParameter("Цвет при значении 0:", ExtraParameterType::Color, ExtraUIElementType::ColorDialog);
        logic.addExtraParameter("Цвет при значении 1:", ExtraParameterType::Color, ExtraUIElementType::ColorDialog);
        logic.setModuleName("Створка");
        logic.setElementLogicType(ElementLogicType::FlapDark);

        logic.setPreviewImagePath("images/flap.png");

        QFile file(mBasePath + "/FlapDark.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }



    // ===============================================================
    // ===============================================================
    // Элемент Density ===============================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int w = 200;
        const int h = 50;

        // Рамка (коэфф. скругления углов)
        pointsArray.clear();
        pointsArray<<0<<0<<w<<h<<w/25<<w/25;
        figure.setFigureParameters(FigureType::RoundedRect, pointsArray);
        figures.append(figure);
        // Текстовая область
        pointsArray.clear();
        pointsArray<<0<<0<<w<<h;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);

        // Параметр 1
        parameter.setBitsCount(1);
        parameter.setParameterType(ParameterType::Common);
        parameter.setExtraParameter(-1);
        parameters.append(parameter);
        // Параметр 2
        parameter.setBitsCount(15);
        parameter.setParameterType(ParameterType::Number);
        parameter.setExtraParameter(0);
        parameters.append(parameter);

        // Параметр 1, истина
        // Рамка зеленая
        gdiObjects.clear();
        gdiObjects<<greenContourPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Рамка красная
        gdiObjects.clear();
        gdiObjects<<redContourPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, false));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Текст зеленый
        gdiObjects.clear();
        gdiObjects<<greenMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(1, gdiObjects);
        records.append(record);
        // Текст красный
        gdiObjects.clear();
        gdiObjects<<redMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, false));
        record.setRecord(1, gdiObjects);
        records.append(record);
        // Собственно текст
        gdiObjects.clear();
        gdiObjects<<text;
        record.setParameterID(1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        records.append(record);

        // Рамка по умолчанию
        gdiObjects.clear();
        gdiObjects<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);
        // Текст по умолчанию
        gdiObjects.clear();
        gdiObjects<<defaultText<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        // Размер элемента
        logic.setSize(QSize(w, h));
        // Стартовый угол
        logic.setStartAngle(0);
        // Дополнительные параметры
        logic.addExtraParameter("Коэффициент:", ExtraParameterType::Real, ExtraUIElementType::LineEdit);
        logic.addExtraParameter("Постфикс:", ExtraParameterType::String, ExtraUIElementType::LineEdit);
        logic.addExtraParameter("Точность:", ExtraParameterType::Integer, ExtraUIElementType::SpinBox, QVariant(6));
        logic.addExtraParameter("Учитывать знак:", ExtraParameterType::Bool, ExtraUIElementType::CheckBox, QVariant(true));
        logic.addExtraParameter("Плотность (г/см^3):", ExtraParameterType::Real, ExtraUIElementType::LineEdit);
        logic.setModuleName("Датчик плотности");
        logic.setElementLogicType(ElementLogicType::FuelDensity);

        logic.setPreviewImagePath("images/Text.png");

        QFile file(mBasePath + "/FuelDensity.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }


    // ===============================================================
    // ===============================================================
    // Элемент Density (темная) ======================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int w = 200;
        const int h = 50;

        // Рамка (коэфф. скругления углов)
        pointsArray.clear();
        pointsArray<<0<<0<<w<<h<<w/25<<w/25;
        figure.setFigureParameters(FigureType::RoundedRect, pointsArray);
        figures.append(figure);
        // Текстовая область
        pointsArray.clear();
        pointsArray<<0<<0<<w<<h;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);

        // Параметр 1
        parameter.setBitsCount(1);
        parameter.setParameterType(ParameterType::Common);
        parameter.setExtraParameter(-1);
        parameters.append(parameter);
        // Параметр 2
        parameter.setBitsCount(15);
        parameter.setParameterType(ParameterType::Number);
        parameter.setExtraParameter(0);
        parameters.append(parameter);

        // Параметр 1, истина
        // Рамка зеленая
        gdiObjects.clear();
        gdiObjects<<greenContourPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Рамка красная
        gdiObjects.clear();
        gdiObjects<<redContourPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, false));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Текст зеленый
        gdiObjects.clear();
        gdiObjects<<greenMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(1, gdiObjects);
        records.append(record);
        // Текст красный
        gdiObjects.clear();
        gdiObjects<<redMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, false));
        record.setRecord(1, gdiObjects);
        records.append(record);
        // Собственно текст
        gdiObjects.clear();
        gdiObjects<<text;
        record.setParameterID(1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        records.append(record);

        // Рамка по умолчанию
        gdiObjects.clear();
        gdiObjects<<whiteContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);
        // Текст по умолчанию
        gdiObjects.clear();
        gdiObjects<<defaultText<<whiteMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        // Размер элемента
        logic.setSize(QSize(w, h));
        // Стартовый угол
        logic.setStartAngle(0);
        // Дополнительные параметры
        logic.addExtraParameter("Коэффициент:", ExtraParameterType::Real, ExtraUIElementType::LineEdit);
        logic.addExtraParameter("Постфикс:", ExtraParameterType::String, ExtraUIElementType::LineEdit);
        logic.addExtraParameter("Точность:", ExtraParameterType::Integer, ExtraUIElementType::SpinBox, QVariant(6));
        logic.addExtraParameter("Учитывать знак:", ExtraParameterType::Bool, ExtraUIElementType::CheckBox, QVariant(true));
        logic.addExtraParameter("Плотность (г/см^3):", ExtraParameterType::Real, ExtraUIElementType::LineEdit);
        logic.setModuleName("Датчик плотности");
        logic.setElementLogicType(ElementLogicType::FuelDensity);

        logic.setPreviewImagePath("images/Text.png");

        QFile file(mBasePath + "/FuelDensity_Dark.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }



    // ===============================================================
    // ===============================================================
    // Штанга  =======================================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        // Базовый размер
        const int w = 260;
        const int h = 125;

        // Треугольник штанги (фигура по умолчанию) - 0
        pointsArray.clear();
        pointsArray<<179<<44<<259<<124<<99<<124;
        figure.setFigureParameters(FigureType::Polygon, pointsArray);
        figures.append(figure);
        // Хвост у треугольника штанги (фигура по умолчанию) - 1
        pointsArray.clear();
        pointsArray<<216<<80<<249<<80<<265<<95<<248<<112;
        figure.setFigureParameters(FigureType::Polygon, pointsArray);
        figures.append(figure);
        // Штанга - 2
        pointsArray.clear();
        pointsArray<<56<<0<<79<<23<<96<<23<<148<<75<<96<<23<<79<<23<<56<<46;
        figure.setFigureParameters(FigureType::Polygon, pointsArray);
        figures.append(figure);
        // Стрелка вправо, заливка - 3
        pointsArray.clear();
        pointsArray<<0<<16<<27<<16<<27<<3<<47<<23<<27<<43<<27<<30<<0<<30;
        figure.setFigureParameters(FigureType::Polygon, pointsArray);
        figures.append(figure);
        // Стрелка вправо, контур - 4
        pointsArray.clear();
        pointsArray<<0<<16<<27<<16<<27<<3<<47<<23<<27<<43<<27<<30<<0<<30;
        figure.setFigureParameters(FigureType::Polygon, pointsArray);
        figures.append(figure);
        // Стрелка вниз, заливка - 5
        pointsArray.clear();
        pointsArray<<172<<68<<187<<68<<187<<95<<200<<95<<179<<115<<159<<95<<172<<95;
        figure.setFigureParameters(FigureType::Polygon, pointsArray);
        figures.append(figure);
        // Стрелка вниз, контур - 6
        pointsArray.clear();
        pointsArray<<172<<68<<187<<68<<187<<95<<200<<95<<179<<115<<159<<95<<172<<95;
        figure.setFigureParameters(FigureType::Polygon, pointsArray);
        figures.append(figure);
        // Стрелка вправо вниз (диагональная) - 7
        pointsArray.clear();
        pointsArray<<100<<38<<110<<27<<130<<46<<139<<37<<139<<66<<110<<66<<119<<57;
        figure.setFigureParameters(FigureType::Polygon, pointsArray);
        figures.append(figure);
        // Стрелка влево вверх (диагональная) - 8
        pointsArray.clear();
        pointsArray<<105<<32<<134<<32<<125<<41<<144<<60<<134<<71<<115<<52<<105<<61;
        figure.setFigureParameters(FigureType::Polygon, pointsArray);
        figures.append(figure);

        // Количество параметров
        const int parametersCount = 9;
        parameter.setBitsCount(1);
        parameter.setParameterType(ParameterType::Common);
        parameter.setExtraParameter(-1);
        for (int i = 0; i < parametersCount; i++)
            parameters.append(parameter);

        // Мигающая штанга
        gdiObjects.clear();
        gdiObjects<<whiteTransContourPen<<noBrush;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        records.append(record);
        // Синяя стрелка влево вверх
        gdiObjects.clear();
        gdiObjects<<whiteMediumPen<<blueBrush;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(8, gdiObjects);
        records.append(record);
        // Штанга
        gdiObjects.clear();
        gdiObjects<<whiteContourPen<<blackBrush;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        records.append(record);
        // Мигающая штанга
        gdiObjects.clear();
        gdiObjects<<whiteTransContourPen<<noBrush;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        records.append(record);
        // Синяя стрелка вправо вниз
        gdiObjects.clear();
        gdiObjects<<whiteMediumPen<<blueBrush;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, true));
        record.setRecord(7, gdiObjects);
        records.append(record);
        // Мигающая штанга
        gdiObjects.clear();
        gdiObjects<<whiteTransContourPen<<noBrush;
        record.setParameterID(4);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        records.append(record);
        // Красная стрелка влево вверх
        gdiObjects.clear();
        gdiObjects<<whiteMediumPen<<redBrush;
        record.setParameterID(4);
        record.setState(QVector<bool>(1, true));
        record.setRecord(8, gdiObjects);
        records.append(record);
        // Черная пустая стрелка влево
        gdiObjects.clear();
        gdiObjects<<whiteMediumPen<<noBrush;
        record.setParameterID(5);
        record.setState(QVector<bool>(1, true));
        record.setRecord(4, gdiObjects);
        records.append(record);
        // Зеленая стрелка влево
        gdiObjects.clear();
        gdiObjects<<greenBrush<<noPen;
        record.setParameterID(6);
        record.setState(QVector<bool>(1, true));
        record.setRecord(3, gdiObjects);
        records.append(record);
        // Черная пустая стрелка влево
        gdiObjects.clear();
        gdiObjects<<whiteMediumPen<<noBrush;
        record.setParameterID(7);
        record.setState(QVector<bool>(1, true));
        record.setRecord(6, gdiObjects);
        records.append(record);
        // Зеленая стрелка влево
        gdiObjects.clear();
        gdiObjects<<greenBrush<<noPen;
        record.setParameterID(8);
        record.setState(QVector<bool>(1, true));
        record.setRecord(5, gdiObjects);
        records.append(record);

        // Треугольник штанги, прорисовка по умолчанию
        gdiObjects.clear();
        gdiObjects<<whiteContourPen<<blackBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);
        // Хвост штанги, прорисовка по умолчанию
        gdiObjects.clear();
        gdiObjects<<whiteContourPen<<blackBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 10;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        // Размер элемента
        logic.setSize(QSize(w, h));
        // Стартовый угол
        logic.setStartAngle(0);
        // Тип логического элемента
        logic.setElementLogicType(ElementLogicType::Common);
        // Название модуля
        logic.setModuleName("Штанга");
        // Сохранение полученного модуля
        logic.setPreviewImagePath("images/Штанга.png");

        QFile file(mBasePath + "/Штанга.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }



    // ===============================================================
    // ===============================================================
    // Лампочка (1 бит) ==============================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        // Базовый размер
        const int w = 50;
        const int h = 50;

        // Рамка
        pointsArray.clear();
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);
        pointsArray.clear();
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);

        // Параметр 1 (Обычный, 1 бит)
        parameter.setBitsCount(1);
        parameter.setParameterType(ParameterType::Common);
        parameter.setExtraParameter(-1);
        parameters.append(parameter);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        // Размер элемента
        logic.setSize(QSize(w, h));
        // Стартовый угол
        logic.setStartAngle(0);
        // Базовое значение ExtraParameter
        QVariant variant = Qt::transparent;
        // Дополнительные параметры
        logic.addExtraParameter("Название индикатора:", ExtraParameterType::String, ExtraUIElementType::LineEdit);
        logic.addExtraParameter("Цвет при значении 0:", ExtraParameterType::Color, ExtraUIElementType::ColorDialog,
                                variant);
        logic.addExtraParameter("Цвет при значении 1:", ExtraParameterType::Color, ExtraUIElementType::ColorDialog,
                                variant);
        // Тип логического элемента
        logic.setElementLogicType(ElementLogicType::SingleIndicator);
        // Название модуля
        logic.setModuleName("Индикатор, 1 бит");
        // Сохранение полученного модуля
        logic.setPreviewImagePath("images/Lamp1.png");

        QFile file(mBasePath + "/Lamp1.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }

    // ===============================================================
    // ===============================================================
    // Лампочка (1 бит) ==============================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        // Базовый размер
        const int w = 50;
        const int h = 50;

        // Рамка
        pointsArray.clear();
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);
        pointsArray.clear();
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);

        // Параметр 1 (Обычный, 1 бит)
        parameter.setBitsCount(1);
        parameter.setParameterType(ParameterType::Common);
        parameter.setExtraParameter(-1);
        parameters.append(parameter);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        // Размер элемента
        logic.setSize(QSize(w, h));
        // Стартовый угол
        logic.setStartAngle(0);
        // Базовое значение ExtraParameter
        QVariant variant = Qt::transparent;
        // Дополнительные параметры
        logic.addExtraParameter("Название индикатора:", ExtraParameterType::String, ExtraUIElementType::LineEdit);
        logic.addExtraParameter("Цвет при значении 0:", ExtraParameterType::Color, ExtraUIElementType::ColorDialog,
                                variant);
        logic.addExtraParameter("Цвет при значении 1:", ExtraParameterType::Color, ExtraUIElementType::ColorDialog,
                                variant);
        // Тип логического элемента
        logic.setElementLogicType(ElementLogicType::SingleIndicatorDark);
        // Название модуля
        logic.setModuleName("Индикатор, 1 бит");
        // Сохранение полученного модуля
        logic.setPreviewImagePath("images/Lamp1.png");

        QFile file(mBasePath + "/Lamp1_Dark.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }



    // ===============================================================
    // ===============================================================
    // Лампочка (2 бита) ==============================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        // Базовый размер
        const int w = 50;
        const int h = 50;

        // Рамка
        pointsArray.clear();
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);
        pointsArray.clear();
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);

        // Параметр 1 (Обычный, 1 бит)
        parameter.setBitsCount(1);
        parameter.setParameterType(ParameterType::Common);
        parameter.setExtraParameter(-1);
        parameters.append(parameter);
        parameter.setBitsCount(1);
        parameter.setParameterType(ParameterType::Common);
        parameter.setExtraParameter(-1);
        parameters.append(parameter);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        // Размер элемента
        logic.setSize(QSize(w, h));
        // Стартовый угол
        logic.setStartAngle(0);
        // Базовое значение ExtraParameter
        QVariant variant = Qt::transparent;
        // Дополнительные параметры
        logic.addExtraParameter("Название индикатора:", ExtraParameterType::String, ExtraUIElementType::LineEdit);
        logic.addExtraParameter("Цвет при значении 00:", ExtraParameterType::Color, ExtraUIElementType::ColorDialog,
                                variant);
        logic.addExtraParameter("Цвет при значении 01:", ExtraParameterType::Color, ExtraUIElementType::ColorDialog,
                                variant);
        logic.addExtraParameter("Цвет при значении 10:", ExtraParameterType::Color, ExtraUIElementType::ColorDialog,
                                variant);
        logic.addExtraParameter("Цвет при значении 11:", ExtraParameterType::Color, ExtraUIElementType::ColorDialog,
                                variant);

        // Тип логического элемента
        logic.setElementLogicType(ElementLogicType::DoubleIndicator);
        // Название модуля
        logic.setModuleName("Индикатор, 2 бита");
        // Сохранение полученного модуля
        logic.setPreviewImagePath("images/Lamp2.png");

        QFile file(mBasePath + "/Lamp2.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }



    // ===============================================================
    // ===============================================================
    // Лампочка (2 бита) ==============================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        // Базовый размер
        const int w = 50;
        const int h = 50;

        // Рамка
        pointsArray.clear();
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);
        pointsArray.clear();
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);

        // Параметр 1 (Обычный, 1 бит)
        parameter.setBitsCount(1);
        parameter.setParameterType(ParameterType::Common);
        parameter.setExtraParameter(-1);
        parameters.append(parameter);
        parameter.setBitsCount(1);
        parameter.setParameterType(ParameterType::Common);
        parameter.setExtraParameter(-1);
        parameters.append(parameter);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        // Размер элемента
        logic.setSize(QSize(w, h));
        // Стартовый угол
        logic.setStartAngle(0);
        // Базовое значение ExtraParameter
        QVariant variant = Qt::transparent;
        // Дополнительные параметры
        logic.addExtraParameter("Название индикатора:", ExtraParameterType::String, ExtraUIElementType::LineEdit);
        logic.addExtraParameter("Цвет при значении 00:", ExtraParameterType::Color, ExtraUIElementType::ColorDialog,
                                variant);
        logic.addExtraParameter("Цвет при значении 01:", ExtraParameterType::Color, ExtraUIElementType::ColorDialog,
                                variant);
        logic.addExtraParameter("Цвет при значении 10:", ExtraParameterType::Color, ExtraUIElementType::ColorDialog,
                                variant);
        logic.addExtraParameter("Цвет при значении 11:", ExtraParameterType::Color, ExtraUIElementType::ColorDialog,
                                variant);

        // Тип логического элемента
        logic.setElementLogicType(ElementLogicType::DoubleIndicatorDark);
        // Название модуля
        logic.setModuleName("Индикатор, 2 бита");
        // Сохранение полученного модуля
        logic.setPreviewImagePath("images/Lamp2.png");

        QFile file(mBasePath + "/Lamp2_Dark.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }



    // ===============================================================
    // ===============================================================
    // Элемент AT ====================================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int w = 150;
        const int h = 150;
        const qreal v = (w + h) * 0.04;

        // Ромб
        pointsArray.clear();
        pointsArray<<w*0.5<<0<<w<<h*0.5<<w*0.5<<h<<0<<h*0.5;
        figure.setFigureParameters(FigureType::Polygon, pointsArray);
        figures.append(figure);
        // Стрелка левая
        pointsArray.clear();
        pointsArray<<0<<h*0.5<<v*2<<h*0.5-v*0.65<<v*2<<h*0.5+v*0.65;
        figure.setFigureParameters(FigureType::Polygon, pointsArray);
        figures.append(figure);
        // Стрелка правая
        pointsArray.clear();
        pointsArray<<w<<h*0.5<<w-v*2<<h*0.5-v*0.65<<w-v*2<<h*0.5+v*0.65;
        figure.setFigureParameters(FigureType::Polygon, pointsArray);
        figures.append(figure);
        // Линия №1 (горизонтальная)
        pointsArray.clear();
        pointsArray<<0<<h*0.5<<w<<h*0.5;
        figure.setFigureParameters(FigureType::Line, pointsArray);
        figures.append(figure);
        // Линия №2 (вертикальная)
        pointsArray.clear();
        pointsArray<<w*0.5<<0<<w*0.5<<h;
        figure.setFigureParameters(FigureType::Line, pointsArray);
        figures.append(figure);
        // Фон для числа (верхний)
        pointsArray.clear();
        pointsArray<<w*0.5-v*4<<h*0.375-v*1.25<<v*8<<v*2.5;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Фон для числа (нижний)
        pointsArray.clear();
        pointsArray<<w*0.5-v*4<<h*0.625-v*1.25<<v*8<<v*2.5;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Текстовая область №1 (верхняя)
        pointsArray.clear();
        pointsArray<<w*0.5-v*4<<h*0.375-v*1.25<<v*8<<v*2.5;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Текстовая область №2 (нижняя)
        pointsArray.clear();
        pointsArray<<w*0.5-v*4<<h*0.625-v*1.25<<v*8<<v*2.5;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);

        // Параметр №1
        parameter.setBitsCount(1);
        parameter.setParameterType(ParameterType::Common);
        parameter.setExtraParameter(-1);
        parameters.append(parameter);
        // Параметр №2
        parameter.setBitsCount(15);
        parameter.setParameterType(ParameterType::Number);
        parameter.setExtraParameter(0);
        parameters.append(parameter);
        // Параметр №3
        parameter.setBitsCount(1);
        parameter.setParameterType(ParameterType::Common);
        parameter.setExtraParameter(-1);
        parameters.append(parameter);
        // Параметр №4
        parameter.setBitsCount(15);
        parameter.setParameterType(ParameterType::Number);
        parameter.setExtraParameter(1);
        parameters.append(parameter);

        // Текстовый элемент
        text.setObjectType(GDIObjectType::Text);
        text.setFontParameters("", FontType::TextType);
        // Текстовый элемент, по умолчанию "0.00"
        defaultText.setObjectType(GDIObjectType::Text);
        defaultText.setFontParameters("0.00", FontType::TextType);

        // Зеленый текст (верхний)
        gdiObjects.clear();
        gdiObjects<<greenMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(7, gdiObjects);
        records.append(record);
        // Красный текст (верхний)
        gdiObjects.clear();
        gdiObjects<<redMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, false));
        record.setRecord(7, gdiObjects);
        records.append(record);
        // Текст
        gdiObjects.clear();
        gdiObjects<<text;
        record.setParameterID(1);
        record.setState(QVector<bool>());
        record.setRecord(7, gdiObjects);
        records.append(record);
        // Зеленый текст (нижний)
        gdiObjects.clear();
        gdiObjects<<greenMediumPen;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, true));
        record.setRecord(8, gdiObjects);
        records.append(record);
        // Красный текст (нижний)
        gdiObjects.clear();
        gdiObjects<<redMediumPen;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, false));
        record.setRecord(8, gdiObjects);
        records.append(record);
        // Текст
        gdiObjects.clear();
        gdiObjects<<text;
        record.setParameterID(3);
        record.setState(QVector<bool>());
        record.setRecord(8, gdiObjects);
        records.append(record);

        // Текст по умолчанию (верхний)
        gdiObjects.clear();
        gdiObjects<<defaultText<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(7, gdiObjects);
        defaultRecords.append(record);
        // Текст по умолчанию (нижний)
        gdiObjects.clear();
        gdiObjects<<defaultText<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(8, gdiObjects);
        defaultRecords.append(record);
        // Ромб
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<whiteBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);
        // Стрелка левая
        gdiObjects.clear();
        gdiObjects<<blackThinPen<<blackBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);
        // Стрелка правая
        gdiObjects.clear();
        gdiObjects<<blackThinPen<<blackBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(2, gdiObjects);
        defaultRecords.append(record);
        // Линия №1
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(3, gdiObjects);
        defaultRecords.append(record);
        // Линия №2
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(4, gdiObjects);
        defaultRecords.append(record);
        // Фон для текста (верхний)
        gdiObjects.clear();
        gdiObjects<<whiteBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(5, gdiObjects);
        defaultRecords.append(record);
        // Фон для текста (нижний)
        gdiObjects.clear();
        gdiObjects<<whiteBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(6, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        logic.setSize(QSize(w, h));
        logic.setStartAngle(0);

        // Дополнительные параметры
        logic.addExtraParameter("Коэффициент №1:", ExtraParameterType::Real, ExtraUIElementType::LineEdit);
        logic.addExtraParameter("Постфикс №1:", ExtraParameterType::String, ExtraUIElementType::LineEdit);
        logic.addExtraParameter("Точность:", ExtraParameterType::Integer, ExtraUIElementType::SpinBox, QVariant(2));
        logic.addExtraParameter("Учитывать знак:", ExtraParameterType::Bool, ExtraUIElementType::CheckBox, QVariant(true));
        logic.addExtraParameter("Коэффициент №2:", ExtraParameterType::Real, ExtraUIElementType::LineEdit);
        logic.addExtraParameter("Постфикс №2:", ExtraParameterType::String, ExtraUIElementType::LineEdit);
        logic.addExtraParameter("Точность:", ExtraParameterType::Integer, ExtraUIElementType::SpinBox, QVariant(2));
        logic.addExtraParameter("Учитывать знак:", ExtraParameterType::Bool, ExtraUIElementType::CheckBox, QVariant(true));

        // Тип логического элемента
        logic.setElementLogicType(ElementLogicType::Common);
        logic.setModuleName("Теплообменник воздухо-воздушный");
        logic.setPreviewImagePath("images/AT.png");

        // Сохранение полученного модуля
        QFile file(mBasePath + "/AT.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;

        // Восстановление параметров Text
        text.setObjectType(GDIObjectType::Text);
        text.setFontParameters("", FontType::HeaderType);
        defaultText.setObjectType(GDIObjectType::Text);
        defaultText.setFontParameters("0.00", FontType::HeaderType);
    }



    // ===============================================================
    // ===============================================================
    // Датчик давления ===============================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int size = 40;

        // Заливка
        pointsArray.clear();
        pointsArray<<0<<0<<size<<size;
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);
        // Контур
        pointsArray.clear();
        pointsArray<<0<<0<<size<<size;
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);
        // Текст
        pointsArray.clear();
        pointsArray<<0<<0<<size<<size;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);

        // Параметры
        const int parametersCount = 4;
        for (int i = 0; i < parametersCount; i++)
        {
            parameter.setBitsCount(1);
            parameters.append(parameter);
        }

        // Текстовый элемент, по умолчанию "0.00"
        GDIObject textDD;
        textDD.setObjectType(GDIObjectType::Text);
        textDD.setFontParameters("К", FontType::HeaderType);

        // Зеленый контур
        gdiObjects.clear();
        gdiObjects<<greenThickPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(1, gdiObjects);
        records.append(record);
        // Зеленая заливка
        gdiObjects.clear();
        gdiObjects<<greenBrush;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Черный текст
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textDD;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        records.append(record);
        // Черный-красный текст
        gdiObjects.clear();
        gdiObjects<<blackRedPen<<textDD;
        record.setParameterID(3);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        records.append(record);

        // Черный контур
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);
        // Белая заливка
        gdiObjects.clear();
        gdiObjects<<whiteBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        // Размер
        logic.setSize(QSize(size, size));
        const int warningOffset = 25;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        // Стартовый угол
        logic.setStartAngle(0);

        logic.setElementLogicType(ElementLogicType::Common);
        logic.setModuleName("Датчик давления");
        logic.setPreviewImagePath("images/DD.png");

        // Сохранение модуля
        QFile file(mBasePath + "/DD.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }


    // ===============================================================
    // ===============================================================
    // Датчик температуры ============================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int w = 40;
        const int h = 40;

        // Фигура 0 - Левый прямоугольник
        pointsArray.clear();
        pointsArray<<0<<h*0.2<<w*0.4<<h*0.6;
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);
        // Фигура 1 - Главный прямоугольник
        pointsArray.clear();
        pointsArray<<w*0.4<<0<<w<<h;
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);
        // Фигура 2 - Основание стрелки
        pointsArray.clear();
        pointsArray<<w*0.9<<h*0.8<<w*0.79<<h*0.4;
        figure.setFigureParameters(FigureType::Line, pointsArray);
        figures.append(figure);
        // Фигура 3 - Горизонтальная линия в дополнительном прямоугольнике
        pointsArray.clear();
        pointsArray<<w*0.2<<h*0.3<<w*0.2<<h*0.7;
        figure.setFigureParameters(FigureType::Line, pointsArray);
        figures.append(figure);
        // Фигура 4 - Вертикальная линия в дополнительном прямоугольнике
        pointsArray.clear();
        pointsArray<<w*0.1<<h*0.7<<w*0.3<<h*0.7;
        figure.setFigureParameters(FigureType::Line, pointsArray);
        figures.append(figure);
        // Фигура 5 - Стрелка
        pointsArray.clear();
        pointsArray<<w*0.77<<h*0.32<<w*0.73<<h*0.6<<w*0.94<<h*0.54;
        figure.setFigureParameters(FigureType::Polygon, pointsArray);
        figures.append(figure);
        // Фигура 6 - Дуга
        pointsArray.clear();
        pointsArray<<w*0.6<<h*0.2<<w*0.6<<h*0.6<<25<<130;
        figure.setFigureParameters(FigureType::Arc, pointsArray);
        figures.append(figure);

        // Параметры
        const int parametersCount = 1;
        for (int i = 0; i < parametersCount; i++)
        {
            parameter.setBitsCount(1);
            parameters.append(parameter);
        }

        // Зеленый заливка, черный контур (малый объект)
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Красная заливка, черный контур (малый объект)
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, false));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Зеленый заливка, черный контур (большой объект)
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(1, gdiObjects);
        records.append(record);
        // Красная заливка, черный контур (большой объект)
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, false));
        record.setRecord(1, gdiObjects);
        records.append(record);

        // Белая заливка, черный контур (малый объект)
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);
        // Белая заливка, черный контур (большой объект)
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);
        // Основание стрелки
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(2, gdiObjects);
        defaultRecords.append(record);
        // Горизонтальная линия
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(3, gdiObjects);
        defaultRecords.append(record);
        // Вертикальная линия
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(4, gdiObjects);
        defaultRecords.append(record);
        // Стрелка
        gdiObjects.clear();
        gdiObjects<<blackThinPen<<blackBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(5, gdiObjects);
        defaultRecords.append(record);
        // Дуга
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(6, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        logic.setSize(QSize(w*1.4, h));
        logic.setStartAngle(0);

        logic.setElementLogicType(ElementLogicType::Common);
        logic.setPreviewImagePath("images/DT.png");
        logic.setModuleName("Датчик температуры");

        // Сохранение полученного модуля
        QFile file(mBasePath + "/DT.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }



    // ===============================================================
    // ===============================================================
    // Эжектор =======================================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int size = 15;

        // Ф0, Конус, заливка
        pointsArray.clear();
        pointsArray<<size*2<<size*1.5<<size*3<<size*7<<0<<size*7<<size<<size*1.5;
        figure.setFigureParameters(FigureType::Polygon, pointsArray);
        figures.append(figure);
        // Ф1, Конус, контур
        pointsArray.clear();
        pointsArray<<size*2<<size*1.5<<size*3<<size*7<<0<<size*7<<size<<size*1.5;
        figure.setFigureParameters(FigureType::Polygon, pointsArray);
        figures.append(figure);
        // Ф2, Круг, заливка
        pointsArray.clear();
        pointsArray<<size*0.5<<0<<size*2<<size*2;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф3, Круг, контур
        pointsArray.clear();
        pointsArray<<size*0.5<<0<<size*2<<size*2;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);

        // П0 - бит 0
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П1 - бит 1
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П2 - бит 2
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П3 - бит 3
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П4 - бит 4
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П5 - бит 5
        parameter.setBitsCount(1);
        parameters.append(parameter);

        // Зеленая заливка у конуса
        gdiObjects.clear();
        gdiObjects<<greenBrush;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Зеленая заливка у круга
        gdiObjects.clear();
        gdiObjects<<greenBrush;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        records.append(record);
        // Красный контур у конуса
        gdiObjects.clear();
        gdiObjects<<redThickPen;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, true));
        record.setRecord(1, gdiObjects);
        records.append(record);
        // Красный контур у круга
        gdiObjects.clear();
        gdiObjects<<redThickPen;
        record.setParameterID(3);
        record.setState(QVector<bool>(1, true));
        record.setRecord(3, gdiObjects);
        records.append(record);
        // Зеленый контур у конуса
        gdiObjects.clear();
        gdiObjects<<greenThickPen;
        record.setParameterID(4);
        record.setState(QVector<bool>(1, true));
        record.setRecord(1, gdiObjects);
        records.append(record);
        // Зеленый контур у круга
        gdiObjects.clear();
        gdiObjects<<greenThickPen;
        record.setParameterID(5);
        record.setState(QVector<bool>(1, true));
        record.setRecord(3, gdiObjects);
        records.append(record);

        // Зеленая заливка у конуса
        gdiObjects.clear();
        gdiObjects<<whiteBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);
        // Зеленая заливка у круга
        gdiObjects.clear();
        gdiObjects<<whiteBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(2, gdiObjects);
        defaultRecords.append(record);
        // Красный контур у конуса
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);
        // Красный контур у круга
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(3, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int offset = 10;
        logic.setSize(QSize(size*3, size*7));
        logic.setWarningSize(QSize(offset, offset));
        logic.setStartAngle(0);

        logic.setElementLogicType(ElementLogicType::Common);
        logic.setPreviewImagePath("images/EJEK.png");
        logic.setModuleName("Эжектор");

        // Сохранение полученного модуля
        QFile file(mBasePath + "/EJEK.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }



    // ===============================================================
    // ===============================================================
    // Электроклапан 1 ===============================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int w = 34;
        const int h = 73;
        qreal v = (w+h)*0.067;

        // Заливка
        pointsArray.clear();
        pointsArray<<0<<0<<w<<h;
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);
        // Центральная линия
        pointsArray.clear();
        pointsArray<<0<<h*0.5<<w<<h*0.5;
        figure.setFigureParameters(FigureType::Line, pointsArray);
        figures.append(figure);
        // Нижняя левая линия (горизонтальная)
        pointsArray.clear();
        pointsArray<<0<<h*0.75<<w*0.33<<h*0.75;
        figure.setFigureParameters(FigureType::Line, pointsArray);
        figures.append(figure);
        // Стрелка
        pointsArray.clear();
        pointsArray<<0<<h*0.25<<v*2<<h*0.25-v*0.75<<v*2<<h*0.25+v*0.75;
        figure.setFigureParameters(FigureType::Polygon, pointsArray);
        figures.append(figure);
        // Нижняя правая линия (горизонтальная)
        pointsArray.clear();
        pointsArray<<w*0.67<<h*0.75<<w<<h*0.75;
        figure.setFigureParameters(FigureType::Line, pointsArray);
        figures.append(figure);
        // Нижняя левая вертикальная линия
        pointsArray.clear();
        pointsArray<<w*0.33<<h*0.625<<w*0.33<<h*0.875;
        figure.setFigureParameters(FigureType::Line, pointsArray);
        figures.append(figure);
        // Нижняя правая вертикальная линия
        pointsArray.clear();
        pointsArray<<w*0.67<<h*0.625<<w*0.67<<h*0.875;
        figure.setFigureParameters(FigureType::Line, pointsArray);
        figures.append(figure);
        // Верхняя линия
        pointsArray.clear();
        pointsArray<<0<<h*0.25<<w<<h*0.25;
        figure.setFigureParameters(FigureType::Line, pointsArray);
        figures.append(figure);
        // Контур
        pointsArray.clear();
        pointsArray<<0<<0<<w<<h;
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);

        // Параметры
        const int parametersCount = 5;
        for (int i = 0; i < parametersCount; i++)
        {
            parameter.setBitsCount(1);
            parameters.append(parameter);
        }

        // Фигура 0 - Заливка
        // Фигура 1 - Нижняя правая вертикальная линия
        // Фигура 2 - Центральная линия
        // Фигура 3 - Верхняя линия
        // Фигура 4 - Стрелка
        // Фигура 5 - Нижняя левая горизонтальная линия
        // Фигура 6 - Нижняя левая вертикальная линия
        // Фигура 7 - Нижняя правая горизонтальная линия
        // Фигура 8 - Контур

        // Зеленая заливка объекта
        gdiObjects.clear();
        gdiObjects<<greenBrush;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Красная мигающая заливка объекта
        gdiObjects.clear();
        gdiObjects<<redWhiteBrush;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Толстый черный контур объекта
        gdiObjects.clear();
        gdiObjects<<blackThickPen;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, true));
        record.setRecord(8, gdiObjects);
        records.append(record);
        // Толстый красный контур объекта
        gdiObjects.clear();
        gdiObjects<<redThickPen;
        record.setParameterID(3);
        record.setState(QVector<bool>(1, true));
        record.setRecord(8, gdiObjects);
        records.append(record);
        // Толстый синий контур объекта
        gdiObjects.clear();
        gdiObjects<<blueThickPen;
        record.setParameterID(4);
        record.setState(QVector<bool>(1, true));
        record.setRecord(8, gdiObjects);
        records.append(record);

        // Белая заливка
        gdiObjects.clear();
        gdiObjects<<whiteBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);
        // Черный контур
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(8, gdiObjects);
        defaultRecords.append(record);
        // Фигура 1
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);
        // Фигура 2
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        defaultRecords.append(record);
        // Фигура 3
        gdiObjects.clear();
        gdiObjects<<blackThinPen<<blackBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(3, gdiObjects);
        defaultRecords.append(record);
        // Фигура 4
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(4, gdiObjects);
        defaultRecords.append(record);
        // Фигура 5
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(5, gdiObjects);
        defaultRecords.append(record);
        // Фигура 6
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(6, gdiObjects);
        defaultRecords.append(record);
        // Фигура 7
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(7, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningSize = 10;
        logic.setSize(QSize(w, h));
        logic.setWarningSize(QSize(warningSize, warningSize));
        logic.setStartAngle(0);

        logic.setElementLogicType(ElementLogicType::Common);
        logic.setModuleName("Электроклапан");
        logic.setPreviewImagePath("images/EK1.png");

        // Сохранение полученного модуля
        QFile file(mBasePath + "/EK1.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }



    // ===============================================================
    // ===============================================================
    // Электроклапан 2 ===============================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int w = 34;
        const int h = 73;
        qreal v = (w+h)*0.067;

        // Заливка
        pointsArray.clear();
        pointsArray<<0<<0<<w<<h;
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);
        // Центральная линия
        pointsArray.clear();
        pointsArray<<0<<h*0.5<<w<<h*0.5;
        figure.setFigureParameters(FigureType::Line, pointsArray);
        figures.append(figure);
        // Нижняя левая линия (горизонтальная)
        pointsArray.clear();
        pointsArray<<0<<h*0.75<<w*0.33<<h*0.75;
        figure.setFigureParameters(FigureType::Line, pointsArray);
        figures.append(figure);
        // Стрелка
        pointsArray.clear();
        pointsArray<<0<<h*0.25<<v*2<<h*0.25-v*0.75<<v*2<<h*0.25+v*0.75;
        figure.setFigureParameters(FigureType::Polygon, pointsArray);
        figures.append(figure);
        // Нижняя правая линия (горизонтальная)
        pointsArray.clear();
        pointsArray<<w*0.67<<h*0.75<<w<<h*0.75;
        figure.setFigureParameters(FigureType::Line, pointsArray);
        figures.append(figure);
        // Нижняя левая вертикальная линия
        pointsArray.clear();
        pointsArray<<w*0.33<<h*0.625<<w*0.33<<h*0.875;
        figure.setFigureParameters(FigureType::Line, pointsArray);
        figures.append(figure);
        // Нижняя правая вертикальная линия
        pointsArray.clear();
        pointsArray<<w*0.67<<h*0.625<<w*0.67<<h*0.875;
        figure.setFigureParameters(FigureType::Line, pointsArray);
        figures.append(figure);
        // Верхняя линия
        pointsArray.clear();
        pointsArray<<0<<h*0.25<<w<<h*0.25;
        figure.setFigureParameters(FigureType::Line, pointsArray);
        figures.append(figure);
        // Контур
        pointsArray.clear();
        pointsArray<<0<<0<<w<<h;
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);

        // Параметры
        const int parametersCount = 3;
        for (int i = 0; i < parametersCount; i++)
        {
            parameter.setBitsCount(1);
            parameters.append(parameter);
        }

        // Зеленая заливка объекта
        gdiObjects.clear();
        gdiObjects<<greenBrush;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Красная мигающая заливка объекта
        gdiObjects.clear();
        gdiObjects<<redWhiteBrush;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Толстый черный контур объекта
        gdiObjects.clear();
        gdiObjects<<blackThickPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(8, gdiObjects);
        records.append(record);

        // Белая заливка
        gdiObjects.clear();
        gdiObjects<<whiteBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);
        // Черный контур
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(8, gdiObjects);
        defaultRecords.append(record);
        // Фигура 1
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);
        // Фигура 2
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        defaultRecords.append(record);
        // Фигура 3
        gdiObjects.clear();
        gdiObjects<<blackThinPen<<blackBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(3, gdiObjects);
        defaultRecords.append(record);
        // Фигура 4
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(4, gdiObjects);
        defaultRecords.append(record);
        // Фигура 5
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(5, gdiObjects);
        defaultRecords.append(record);
        // Фигура 6
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(6, gdiObjects);
        defaultRecords.append(record);
        // Фигура 7
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(7, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningSize = 10;
        logic.setSize(QSize(w, h));
        logic.setWarningSize(QSize(warningSize, warningSize));
        logic.setStartAngle(0);

        logic.setElementLogicType(ElementLogicType::Common);
        logic.setModuleName("Электроклапан");
        logic.setPreviewImagePath("images/EK2.png");

        // Сохранение полученного модуля
        QFile file(mBasePath + "/EK2.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }



    // ===============================================================
    // ===============================================================
    // Электровентилятор =============================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int size = 50;
        const int startAngle = 90;
        const int degrees = 40;

        // Ф0, Фон
        pointsArray.clear();
        pointsArray<<0<<0<<size<<size;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф2, Пирог1
        pointsArray.clear();
        pointsArray<<size*0.2<<size*0.2<<size*0.6<<size*0.6<<startAngle<<degrees;
        figure.setFigureParameters(FigureType::Pie, pointsArray);
        figures.append(figure);
        // Ф3, Пирог1
        pointsArray.clear();
        pointsArray<<size*0.2<<size*0.2<<size*0.6<<size*0.6<<360-startAngle<<degrees;
        figure.setFigureParameters(FigureType::Pie, pointsArray);
        figures.append(figure);

        // Параметры
        const int parametersCount = 2;
        for (int i = 0; i < parametersCount; i++)
        {
            parameter.setBitsCount(1);
            parameters.append(parameter);
        }

        // Зеленая заливка
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Красная заливка
        gdiObjects.clear();
        gdiObjects<<redWhiteBrush<<blackMediumPen;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        records.append(record);

        // Белая заливка
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);
        // Пирог1
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);
        // Пирог2
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningSize = 10;
        logic.setSize(QSize(size, size));
        logic.setWarningSize(QSize(warningSize, warningSize));
        logic.setStartAngle(0);

        logic.setElementLogicType(ElementLogicType::Common);
        logic.setModuleName("Электровентилятор");
        logic.setPreviewImagePath("images/EV.png");

        // Сохранение полученного модуля
        QFile file(mBasePath + "/EV.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }



    // ===============================================================
    // ===============================================================
    // Пульт контроля (KLAP) =========================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int size = 32;

        // Ф0, Контур1
        pointsArray.clear();
        pointsArray<<0<<0<<size*18<<size*4<<size*4/10<<size*4/10;
        figure.setFigureParameters(FigureType::RoundedRect, pointsArray);
        figures.append(figure);
        // Ф1, Контур1, текст
        pointsArray.clear();
        pointsArray<<0<<0<<size*18<<size*1.25;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф2, Контур2
        pointsArray.clear();
        pointsArray<<0<<size*4.5<<size*18<<size*4<<size*4/10<<size*4/10;
        figure.setFigureParameters(FigureType::RoundedRect, pointsArray);
        figures.append(figure);
        // Ф3, Контур2, текст
        pointsArray.clear();
        pointsArray<<0<<size*4.5<<size*18<<size*1.25;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф4, Датчик1, заливка
        pointsArray.clear();
        pointsArray<<size*0.5<<size*1.25*1<<size<<size;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф5, Датчик1, контур
        pointsArray.clear();
        pointsArray<<size*0.5<<size*1.25*1<<size<<size;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф6, Датчик1, текст
        pointsArray.clear();
        pointsArray<<size*2<<size*1.25*1<<size*15.5<<size;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф7, Датчик2, заливка
        pointsArray.clear();
        pointsArray<<size*0.5<<size*1.25*2<<size<<size;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф8, Датчик2, контур
        pointsArray.clear();
        pointsArray<<size*0.5<<size*1.25*2<<size<<size;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф9, Датчик2, текст
        pointsArray.clear();
        pointsArray<<size*2<<size*1.25*2<<size*15.5<<size;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф10, Датчик3, заливка
        pointsArray.clear();
        pointsArray<<size*2.25 + size*6*0<<size*5.75<<size*1.5<<size*1.5;
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);
        // Ф11, Датчик3, контур
        pointsArray.clear();
        pointsArray<<size*2.25 + size*6*0<<size*5.75<<size*1.5<<size*1.5;
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);
        // Ф12, Датчик3, текст
        pointsArray.clear();
        pointsArray<<size*6*0<<size*7.25<<size*6<<size;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф13, Датчик4, заливка
        pointsArray.clear();
        pointsArray<<size*2.25 + size*6*1<<size*5.75<<size*1.5<<size*1.5;
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);
        // Ф14, Датчик4, контур
        pointsArray.clear();
        pointsArray<<size*2.25 + size*6*1<<size*5.75<<size*1.5<<size*1.5;
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);
        // Ф15, Датчик4, текст
        pointsArray.clear();
        pointsArray<<size*6*1<<size*7.25<<size*6<<size;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф16, Датчик5, заливка
        pointsArray.clear();
        pointsArray<<size*2.25 + size*6*2<<size*5.75<<size*1.5<<size*1.5;
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);
        // Ф17, Датчик5, контур
        pointsArray.clear();
        pointsArray<<size*2.25 + size*6*2<<size*5.75<<size*1.5<<size*1.5;
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);
        // Ф18, Датчик5, текст
        pointsArray.clear();
        pointsArray<<size*6*2<<size*7.25<<size*6<<size;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);

        // Параметры
        const int parametersCount = 14;
        for (int i = 0; i < parametersCount; i++)
        {
            parameter.setBitsCount(1);
            parameters.append(parameter);
        }

        // Заголовок 1
        GDIObject textTitle1;
        textTitle1.setObjectType(GDIObjectType::Text);
        textTitle1.setFontParameters("Пульт контроля", FontType::HeaderType);
        // Заголовок 2
        GDIObject textTitle2;
        textTitle2.setObjectType(GDIObjectType::Text);
        textTitle2.setFontParameters("Температура кондиц. кабины", FontType::HeaderType);
        // Текст Д1
        GDIObject textD1;
        textD1.setObjectType(GDIObjectType::Text);
        textD1.setFontParameters("Проверка регул. давления в кабине",
                                 FontType::TextType, 1,
                                 Qt::AlignLeft | Qt::AlignVCenter);
        // Текст Д2
        GDIObject textD2;
        textD2.setObjectType(GDIObjectType::Text);
        textD2.setFontParameters("Проверка предохр. клапана кабины",
                                 FontType::TextType, 1,
                                 Qt::AlignLeft | Qt::AlignVCenter);
        // Текст Д3
        GDIObject textD3;
        textD3.setObjectType(GDIObjectType::Text);
        textD3.setFontParameters("Холоднее", FontType::TextType);
        // Текст Д4
        GDIObject textD4;
        textD4.setObjectType(GDIObjectType::Text);
        textD4.setFontParameters("Теплее", FontType::TextType);
        // Текст Д5
        GDIObject textD5;
        textD5.setObjectType(GDIObjectType::Text);
        textD5.setFontParameters("Ручной", FontType::TextType);

        // Зеленая заливка Д2
        gdiObjects.clear();
        gdiObjects<<greenBrush;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(7, gdiObjects);
        records.append(record);
        // Зеленая заливка у Д1
        gdiObjects.clear();
        gdiObjects<<greenBrush;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(4, gdiObjects);
        records.append(record);
        // Зеленая заливка у Д3
        gdiObjects.clear();
        gdiObjects<<greenBrush;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, true));
        record.setRecord(10, gdiObjects);
        records.append(record);
        // Зеленая заливка у Д4
        gdiObjects.clear();
        gdiObjects<<greenBrush;
        record.setParameterID(3);
        record.setState(QVector<bool>(1, true));
        record.setRecord(13, gdiObjects);
        records.append(record);
        // Зеленая заливка у Д3
        gdiObjects.clear();
        gdiObjects<<greenBrush;
        record.setParameterID(4);
        record.setState(QVector<bool>(1, true));
        record.setRecord(10, gdiObjects);
        records.append(record);
        // Зеленая заливка у Д4
        gdiObjects.clear();
        gdiObjects<<greenBrush;
        record.setParameterID(5);
        record.setState(QVector<bool>(1, true));
        record.setRecord(13, gdiObjects);
        records.append(record);
        // Красный контур у Д3
        gdiObjects.clear();
        gdiObjects<<redThickPen;
        record.setParameterID(6);
        record.setState(QVector<bool>(1, true));
        record.setRecord(11, gdiObjects);
        records.append(record);
        // Красный контур у Д4
        gdiObjects.clear();
        gdiObjects<<redThickPen;
        record.setParameterID(7);
        record.setState(QVector<bool>(1, true));
        record.setRecord(14, gdiObjects);
        records.append(record);
        // Зеленый контур у Д2
        gdiObjects.clear();
        gdiObjects<<greenThickPen;
        record.setParameterID(8);
        record.setState(QVector<bool>(1, true));
        record.setRecord(8, gdiObjects);
        records.append(record);
        // Зеленый контур у Д1
        gdiObjects.clear();
        gdiObjects<<greenThickPen;
        record.setParameterID(9);
        record.setState(QVector<bool>(1, true));
        record.setRecord(5, gdiObjects);
        records.append(record);
        // Красный контур у Д2
        gdiObjects.clear();
        gdiObjects<<redThickPen;
        record.setParameterID(10);
        record.setState(QVector<bool>(1, true));
        record.setRecord(8, gdiObjects);
        records.append(record);
        // Красный контур у Д1
        gdiObjects.clear();
        gdiObjects<<redThickPen;
        record.setParameterID(11);
        record.setState(QVector<bool>(1, true));
        record.setRecord(5, gdiObjects);
        records.append(record);
        // Зеленый контур у Д5
        gdiObjects.clear();
        gdiObjects<<greenThickPen;
        record.setParameterID(12);
        record.setState(QVector<bool>(1, true));
        record.setRecord(17, gdiObjects);
        records.append(record);
        // Красный контур у Д5
        gdiObjects.clear();
        gdiObjects<<redThickPen;
        record.setParameterID(13);
        record.setState(QVector<bool>(1, true));
        record.setRecord(17, gdiObjects);
        records.append(record);

        // Черный контур1
        gdiObjects.clear();
        gdiObjects<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);
        // Черный контур1, текст
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textTitle1;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);
        // Черный контур2
        gdiObjects.clear();
        gdiObjects<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(2, gdiObjects);
        defaultRecords.append(record);
        // Черный контур2, текст
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textTitle2;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(3, gdiObjects);
        defaultRecords.append(record);
        // Д1, заливка белая
        gdiObjects.clear();
        gdiObjects<<whiteBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(4, gdiObjects);
        defaultRecords.append(record);
        // Д1, контур черный
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(5, gdiObjects);
        defaultRecords.append(record);
        // Д1, текст черный
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textD1;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(6, gdiObjects);
        defaultRecords.append(record);
        // Д2, заливка белая
        gdiObjects.clear();
        gdiObjects<<whiteBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(7, gdiObjects);
        defaultRecords.append(record);
        // Д2, контур черный
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(8, gdiObjects);
        defaultRecords.append(record);
        // Д2, текст черный
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textD2;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(9, gdiObjects);
        defaultRecords.append(record);
        // Д3, заливка белая
        gdiObjects.clear();
        gdiObjects<<whiteBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(10, gdiObjects);
        defaultRecords.append(record);
        // Д3, контур черный
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(11, gdiObjects);
        defaultRecords.append(record);
        // Д3, текст черный
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textD3;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(12, gdiObjects);
        defaultRecords.append(record);
        // Д4, заливка белая
        gdiObjects.clear();
        gdiObjects<<whiteBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(13, gdiObjects);
        defaultRecords.append(record);
        // Д4, контур черный
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(14, gdiObjects);
        defaultRecords.append(record);
        // Д4, текст черный
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textD4;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(15, gdiObjects);
        defaultRecords.append(record);
        // Д5, заливка белая
        gdiObjects.clear();
        gdiObjects<<whiteBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(16, gdiObjects);
        defaultRecords.append(record);
        // Д5, контур черный
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(17, gdiObjects);
        defaultRecords.append(record);
        // Д5, текст черный
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textD5;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(18, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        logic.setSize(QSize(size * 18, size * 8.5));
        logic.setStartAngle(0);

        logic.setElementLogicType(ElementLogicType::Common);
        logic.setModuleName("Пульт контроля");
        logic.setPreviewImagePath("images/KLAP.png");

        // Сохранение полученного модуля
        QFile file(mBasePath + "/KLAP.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }



    // ===============================================================
    // ===============================================================
    // Перегрев (PRGV) ===============================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int size = 5;

        // Ф0, Контур
        pointsArray.clear();
        pointsArray<<0<<0<<size*40<<size*30<<size*30/14<<size*30/14;
        figure.setFigureParameters(FigureType::RoundedRect, pointsArray);
        figures.append(figure);
        // Ф1, Текст
        pointsArray.clear();
        pointsArray<<0<<0<<size*40<<size*8;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф2, Эллипс1
        pointsArray.clear();
        pointsArray<<size*1.5<<size*7*0 + size*9<<size*5<<size*5;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф3, Текст1
        pointsArray.clear();
        pointsArray<<size*8.5<<size*7*0+size*9<<size*19<<size*5;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф4, Эллипс2
        pointsArray.clear();
        pointsArray<<size*1.5<<size*7*1 + size*9<<size*5<<size*5;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф5, Текст2
        pointsArray.clear();
        pointsArray<<size*8.5<<size*7*1+size*9<<size*19<<size*5;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф6, Эллипс3
        pointsArray.clear();
        pointsArray<<size*1.5<<size*7*2 + size*9<<size*5<<size*5;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф7, Текст3
        pointsArray.clear();
        pointsArray<<size*8.5<<size*7*2+size*9<<size*19<<size*5;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);

        // Параметры
        const int parametersCount = 3;
        for (int i = 0; i < parametersCount; i++)
        {
            parameter.setBitsCount(1);
            parameters.append(parameter);
        }

        // Текст "Перегрев"
        GDIObject textTitle;
        textTitle.setObjectType(GDIObjectType::Text);
        textTitle.setFontParameters("Перегрев", FontType::HeaderType);
        // Текст "БКС 1"
        GDIObject textBKS1;
        textBKS1.setObjectType(GDIObjectType::Text);
        textBKS1.setFontParameters("БКС1",
                                   FontType::TextType, 1,
                                   Qt::AlignVCenter | Qt::AlignLeft);
        // Текст "БКС 2"
        GDIObject textBKS2;
        textBKS2.setObjectType(GDIObjectType::Text);
        textBKS2.setFontParameters("БКС2",
                                   FontType::TextType, 1,
                                   Qt::AlignVCenter | Qt::AlignLeft);
        // Текст "БКС 3"
        GDIObject textBKS3;
        textBKS3.setObjectType(GDIObjectType::Text);
        textBKS3.setFontParameters("БКС3",
                                   FontType::TextType, 1,
                                   Qt::AlignVCenter | Qt::AlignLeft);

        // Красная заливка 1 круга
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        records.append(record);
        // Красная заливка 1 круга
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, false));
        record.setRecord(2, gdiObjects);
        records.append(record);
        // Красная заливка 2 круга
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackMediumPen;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(4, gdiObjects);
        records.append(record);
        // Зеленая заливка 2 круга
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, false));
        record.setRecord(4, gdiObjects);
        records.append(record);
        // Красная заливка 3 круга
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackMediumPen;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, true));
        record.setRecord(6, gdiObjects);
        records.append(record);
        // Зеленая заливка 3 круга
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, false));
        record.setRecord(6, gdiObjects);
        records.append(record);

        // Контур
        gdiObjects.clear();
        gdiObjects<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);
        // Заголовок
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textTitle;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);
        // Зеленая заливка 1 круга
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        defaultRecords.append(record);
        // Текст №1
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textBKS1;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(3, gdiObjects);
        defaultRecords.append(record);
        // Зеленая заливка 2 круга
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(4, gdiObjects);
        defaultRecords.append(record);
        // Текст №2
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textBKS2;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(5, gdiObjects);
        defaultRecords.append(record);
        // Зеленая заливка 3 круга
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(6, gdiObjects);
        defaultRecords.append(record);
        // Текст №3
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textBKS3;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(7, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        logic.setSize(QSize(size*40, size*30));
        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        logic.setStartAngle(0);

        logic.setElementLogicType(ElementLogicType::Common);
        logic.setModuleName("Перегрев");
        logic.setPreviewImagePath("images/PRGV.bin");

        // Сохранение полученного модуля
        QFile file(mBasePath + "/PRGV.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }



    // ===============================================================
    // ===============================================================
    // Кран RU =======================================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        // Фон
        pointsArray.clear();
        pointsArray<<25<<25<<120<<120;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Центральная полоска
        pointsArray.clear();
        pointsArray<<24<<71<<122<<28;
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);
        // Контур общий
        pointsArray.clear();
        pointsArray<<25<<25<<120<<120;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Стрелка
        pointsArray.clear();
        pointsArray<<10<<0<<20<<15<<30<<5<<30<<30<<5<<30<<15<<20<<0<<10<<10<<0;
        figure.setFigureParameters(FigureType::Polygon, pointsArray, true);
        figures.append(figure);
        // Болванка
        pointsArray.clear();
        figure.setFigureParameters(FigureType::Undefined, pointsArray);
        figures.append(figure);
        figures.append(figure);

        // Параметры
        const int parametersCount = 10;
        for (int i = 0; i < parametersCount; i++)
        {
            parameter.setBitsCount(1);parameter.setParameterType(ParameterType::Common);
            parameters.append(parameter);
        }
        // Параметр №1
        parameter.setBitsCount(1);parameter.setParameterType(ParameterType::Common);
        parameter.setParameterType(ParameterType::Common);
        parameter.setExtraParameter(-1);
        parameters.append(parameter);
        // Параметр №2
        parameter.setBitsCount(15);
        parameter.setParameterType(ParameterType::Number);
        parameter.setExtraParameter(0);
        parameters.append(parameter);
        // Параметр №3
        parameter.setBitsCount(1);parameter.setParameterType(ParameterType::Common);
        parameter.setParameterType(ParameterType::Common);
        parameter.setExtraParameter(-1);
        parameters.append(parameter);
        // Параметр №4
        parameter.setBitsCount(15);
        parameter.setParameterType(ParameterType::Number);
        parameter.setExtraParameter(1);
        parameters.append(parameter);


        // Зеленый очень толстый карандаш (LineWidth::selectionLine)
        GDIObject greenSuperThickPen;
        greenSuperThickPen.setObjectType(GDIObjectType::Pen);
        greenSuperThickPen.setColorParameters(Colors::green, Colors::green, false);
        greenSuperThickPen.setPenParameters(LineWidth::selectionLine * 3, Qt::SolidLine);
        // Красный очень толстый карандаш (LineWidth::selectionLine)
        GDIObject redSuperThickPen;
        redSuperThickPen.setObjectType(GDIObjectType::Pen);
        redSuperThickPen.setColorParameters(Colors::red, Colors::red, false);
        redSuperThickPen.setPenParameters(LineWidth::selectionLine * 3, Qt::SolidLine);
        // Серый очень толстый карандаш (LineWidth::selectionLine)
        GDIObject greySuperThickPen;
        greySuperThickPen.setObjectType(GDIObjectType::Pen);
        greySuperThickPen.setColorParameters(Colors::gray, Colors::gray, false);
        greySuperThickPen.setPenParameters(LineWidth::selectionLine * 3, Qt::SolidLine);

        // Фон по умолчанию
        gdiObjects.clear();
        gdiObjects<<whiteBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);
        // Полоска по умолчанию
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        record.setAngle(0);
        defaultRecords.append(record);
        // Контур по умолчанию
        gdiObjects.clear();
        gdiObjects<<greySuperThickPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(2, gdiObjects);
        defaultRecords.append(record);
        // Фон, синий мигающий, параметр 0
        gdiObjects.clear();
        gdiObjects<<blueWhiteBrush;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Полоска, синия, угол 90 градусов, параметр 1
        gdiObjects.clear();
        gdiObjects<<blueBrush<<blackMediumPen;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(1, gdiObjects);
        record.setAngle(90);
        records.append(record);
        // Фон, зеленый мигающий, параметр 2
        gdiObjects.clear();
        gdiObjects<<greenWhiteBrush;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Полоска, зеленая, угол 0 градусов, параметр 3
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(3);
        record.setState(QVector<bool>(1, true));
        record.setRecord(1, gdiObjects);
        record.setAngle(0);
        records.append(record);
        // Контур зеленый, параметр 4
        gdiObjects.clear();
        gdiObjects<<greenSuperThickPen;
        record.setParameterID(4);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        records.append(record);
        // Контур красный, параметр 4
        gdiObjects.clear();
        gdiObjects<<redSuperThickPen;
        record.setParameterID(4);
        record.setState(QVector<bool>(1, false));
        record.setRecord(2, gdiObjects);
        records.append(record);
        // Стрелка, параметр 5
        gdiObjects.clear();
        gdiObjects<<blackThinPen<<blackBrush;
        record.setParameterID(5);
        record.setState(QVector<bool>(1, true));
        record.setRecord(3, gdiObjects);
        records.append(record);
        // Полоска, параметр 6
        gdiObjects.clear();
        gdiObjects<<blueBrush<<blackMediumPen;
        record.setParameterID(6);
        record.setState(QVector<bool>(1, true));
        record.setRecord(1, gdiObjects);
        record.setAngle(45);
        records.append(record);
        // Полоска, параметр 7
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(7);
        record.setState(QVector<bool>(1, true));
        record.setRecord(1, gdiObjects);
        record.setAngle(45);
        records.append(record);
        // Полоска, красная, угол 90 градусов, параметр 8
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackMediumPen;
        record.setParameterID(8);
        record.setState(QVector<bool>(1, true));
        record.setRecord(1, gdiObjects);
        record.setAngle(90);
        records.append(record);
        // Полоска, красная, угол 90 градусов, параметр 8
        gdiObjects.clear();
        gdiObjects<<redWhiteBrush<<blackMediumPen;
        record.setParameterID(9);
        record.setState(QVector<bool>(1, true));
        record.setRecord(1, gdiObjects);
        record.setAngle(45);
        records.append(record);

        // Текст 1
        gdiObjects.clear();
        gdiObjects<<text;
        record.setParameterID(11);
        record.setState(QVector<bool>());
        record.setRecord(4, gdiObjects);
        records.append(record);
        // Текст 2
        gdiObjects.clear();
        gdiObjects<<text;
        record.setParameterID(13);
        record.setState(QVector<bool>());
        record.setRecord(5, gdiObjects);
        records.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        // Дополнительные параметры
        logic.addExtraParameter("Коэффициент №1:", ExtraParameterType::Real, ExtraUIElementType::LineEdit);
        logic.addExtraParameter("Постфикс №1:", ExtraParameterType::String, ExtraUIElementType::LineEdit);
        logic.addExtraParameter("Точность:", ExtraParameterType::Integer, ExtraUIElementType::SpinBox, QVariant(2));
        logic.addExtraParameter("Учитывать знак:", ExtraParameterType::Bool, ExtraUIElementType::CheckBox, QVariant(true));
        logic.addExtraParameter("Коэффициент №2:", ExtraParameterType::Real, ExtraUIElementType::LineEdit);
        logic.addExtraParameter("Постфикс №2:", ExtraParameterType::String, ExtraUIElementType::LineEdit);
        logic.addExtraParameter("Точность:", ExtraParameterType::Integer, ExtraUIElementType::SpinBox, QVariant(2));
        logic.addExtraParameter("Учитывать знак:", ExtraParameterType::Bool, ExtraUIElementType::CheckBox, QVariant(true));

        // Установка размера
        logic.setStartAngle(0);
        logic.setSize(QSize(170, 170));
        // Установка стартового угла
        logic.setStartAngle(0);

        logic.setElementLogicType(ElementLogicType::Common);
        logic.setModuleName("Заслонка регулирующая");
        logic.setPreviewImagePath("images/RU.png");

        // Сохранение
        QFile file(mBasePath + "/RU.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }



    // ===============================================================
    // ===============================================================
    // Топливный кран ==========================================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        // Фон
        pointsArray.clear();
        pointsArray<<15<<15<<120<<120;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Центральная полоса
        pointsArray.clear();
        pointsArray<<14<<61<<122<<28;
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);
        // Контур
        pointsArray.clear();
        pointsArray<<15<<15<<120<<120;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);

        // Параметры
        const int parametersCount = 2;
        for (int i = 0; i < parametersCount; i++)
        {
            parameter.setBitsCount(1);
            parameter.setParameterType(ParameterType::Common);
            parameters.append(parameter);
        }

        // Зеленый очень толстый карандаш (LineWidth::selectionLine)
        GDIObject greenSuperThickPen;
        greenSuperThickPen.setObjectType(GDIObjectType::Pen);
        greenSuperThickPen.setColorParameters(Colors::lightGreen, Colors::lightGreen, false);
        greenSuperThickPen.setPenParameters(LineWidth::selectionLine * 3, Qt::SolidLine);
        // Синий очень толстый карандаш (LineWidth::selectionLine)
        GDIObject blueSuperThickPen;
        blueSuperThickPen.setObjectType(GDIObjectType::Pen);
        blueSuperThickPen.setColorParameters(Colors::lightBlue, Colors::lightBlue, false);
        blueSuperThickPen.setPenParameters(LineWidth::selectionLine * 3, Qt::SolidLine);
        // Красный очень толстый карандаш (LineWidth::selectionLine)
        GDIObject redSuperThickPen;
        redSuperThickPen.setObjectType(GDIObjectType::Pen);
        redSuperThickPen.setColorParameters(Colors::red, Colors::red, false);
        redSuperThickPen.setPenParameters(LineWidth::selectionLine * 3, Qt::SolidLine);
        // Серый очень толстый карандаш (LineWidth::selectionLine)
        GDIObject greySuperThickPen;
        greySuperThickPen.setObjectType(GDIObjectType::Pen);
        greySuperThickPen.setColorParameters(Colors::gray, Colors::gray, false);
        greySuperThickPen.setPenParameters(LineWidth::selectionLine * 3, Qt::SolidLine);
        // Желтый очень толстый карандаш (LineWidth::selectionLine)
        GDIObject yellowSuperThickPen;
        yellowSuperThickPen.setObjectType(GDIObjectType::Pen);
        yellowSuperThickPen.setColorParameters(Colors::yellow, Colors::yellow, false);
        yellowSuperThickPen.setPenParameters(LineWidth::selectionLine * 3, Qt::SolidLine);

        // Фон по умолчанию
        gdiObjects.clear();
        gdiObjects<<blackBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);
        // Полоска по умолчанию
        gdiObjects.clear();
        gdiObjects<<greyBrush;
        record.setAngle(90);
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);
        // Контур по умолчанию
        gdiObjects.clear();
        gdiObjects<<greySuperThickPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(2, gdiObjects);
        defaultRecords.append(record);
        // Полоска желтая, вертикальная, параметр 0
        gdiObjects.clear();
        gdiObjects<<yellowBrush;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setAngle(90);
        record.setRecord(1, gdiObjects);
        records.append(record);
        // Контур желтый, параметр 0
        gdiObjects.clear();
        gdiObjects<<yellowSuperThickPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        records.append(record);
        // Полоска синяя, вертикальная, параметр 1
        gdiObjects.clear();
        gdiObjects<<greenBrush;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setAngle(0);
        record.setRecord(1, gdiObjects);
        records.append(record);
        // Контур желтый, параметр 1
        gdiObjects.clear();
        gdiObjects<<greenSuperThickPen;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        records.append(record);


        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        // Установка размера
        logic.setSize(QSize(150, 150));
        // Установка стартового угла
        logic.setStartAngle(0);
        // Установка области предупреждения
        logic.setWarningSize(QSize(6, 6));
        // Установка типа элемента
        logic.setElementLogicType(ElementLogicType::Common);
        // Установка названия
        logic.setModuleName("Топливный кран");
        // Указание относительного пути к поясняющему изображению
        logic.setPreviewImagePath("images/Tap.png");

        // Сохранение
        QFile file(mBasePath + "/Tap.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }




    // ===============================================================
    // ===============================================================
    // Элемент SAP ===================================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int size = 35;

        // Ф0, Прямоугольник 1, контур
        pointsArray.clear();
        pointsArray<<0<<0<<size*7.5<<size*3.5<<size*0.35<<size*0.35;
        figure.setFigureParameters(FigureType::RoundedRect, pointsArray);
        figures.append(figure);
        // Ф1, Прямоугольник 2, заголовок
        pointsArray.clear();
        pointsArray<<0<<0<<size*7.5<<size*1.25;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф2, Эллипс первый слева
        pointsArray.clear();
        pointsArray<<size*1.5+size*3.5*0<<size*1.25<<size<<size;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф3, Текст для первого датчика
        pointsArray.clear();
        pointsArray<<size*0.25+size*3.5*0<<size*2.25<<size*3.5<<size;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф4, Эллипс второй слева
        pointsArray.clear();
        pointsArray<<size*1.5+size*3.5*1<<size*1.25<<size<<size;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф5, Текст для второго датчика
        pointsArray.clear();
        pointsArray<<size*0.25+size*3.5*1<<size*2.25<<size*3.5<<size;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);

        // Параметры
        const int parametersCount = 2;
        for (int i = 0; i < parametersCount; i++)
        {
            parameter.setBitsCount(1);
            parameters.append(parameter);
        }

        // Текст "Отказ"
        GDIObject textOtk;
        textOtk.setObjectType(GDIObjectType::Text);
        textOtk.setFontParameters("Отказ", FontType::TextType);
        // Текст "Предел"
        GDIObject textPredel;
        textPredel.setObjectType(GDIObjectType::Text);
        textPredel.setFontParameters("Предел", FontType::TextType);
        // Заголовок
        GDIObject textTitle;
        textTitle.setObjectType(GDIObjectType::Text);
        textTitle.setFontParameters("САП", FontType::HeaderType);

        // Зеленая заливка и черный контур у второго датчика
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(4, gdiObjects);
        records.append(record);
        // Зеленая заливка и черный контур у первого датчика
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        records.append(record);

        // Черный контур для прямоугольника
        gdiObjects.clear();
        gdiObjects<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);
        // Черный текст заголовка
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textTitle;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);
        // Датчик 1, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(2, gdiObjects);
        defaultRecords.append(record);
        // Датчик 1, текст
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textOtk;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(3, gdiObjects);
        defaultRecords.append(record);
        // Датчик 2, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(4, gdiObjects);
        defaultRecords.append(record);
        // Датчик 2, текст
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textPredel;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(5, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        logic.setSize(QSize(size*7.5, size*3.5));
        logic.setStartAngle(0);

        logic.setElementLogicType(ElementLogicType::Common);
        logic.setPreviewImagePath("images/SAP.png");

        // Сохранение полученного модуля
        QFile file(mBasePath + "/SAP.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }



    // ===============================================================
    // ===============================================================
    // Элемент Sensor ================================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int size = 35;

        // Ф0, Контур
        pointsArray.clear();
        pointsArray<<0<<0<<size<<size;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);

        // Параметры
        const int parametersCount = 1;
        for (int i = 0; i < parametersCount; i++)
        {
            parameter.setBitsCount(1);
            parameters.append(parameter);
        }

        // Зеленая заливка
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Красная заливка
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, false));
        record.setRecord(0, gdiObjects);
        records.append(record);

        // Белая заливка
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        logic.setSize(QSize(size, size));
        logic.setStartAngle(0);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));

        logic.setElementLogicType(ElementLogicType::Common);
        logic.setPreviewImagePath("images/Sensor.png");
        logic.setModuleName("Датчик");
        // Сохранение полученного модуля
        QFile file(mBasePath + "/Sensor.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }



    // ===============================================================
    // ===============================================================
    // Элемент SH ====================================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int size = 35;

        // Ф0, Заливка круга
        pointsArray.clear();
        pointsArray<<0<<0<<size<<size;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф1, Контур круга
        pointsArray.clear();
        pointsArray<<0<<0<<size<<size;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);

        // Параметры
        const int parametersCount = 2;
        for (int i = 0; i < parametersCount; i++)
        {
            parameter.setBitsCount(1);
            parameters.append(parameter);
        }

        // Зеленая заливка круга
        gdiObjects.clear();
        gdiObjects<<greenBrush;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Зеленый контур круга
        gdiObjects.clear();
        gdiObjects<<greenThickPen;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(1, gdiObjects);
        records.append(record);

        // Белая заливка
        gdiObjects.clear();
        gdiObjects<<whiteBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);
        // Черный контур
        gdiObjects.clear();
        gdiObjects<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        logic.setSize(QSize(size, size));
        logic.setStartAngle(0);

        logic.setModuleName("Шасси");

        logic.setPreviewImagePath("images/SH.png");

        // Сохранение полученного модуля
        QFile file(mBasePath + "/SH.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }



    // ===============================================================
    // ===============================================================
    // Элемент SKV ===================================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int partsCount = 5;
        const int size = 35;

        // Ф0, Прямоугольник 1, контур
        pointsArray.clear();
        pointsArray<<0<<0<<partsCount*size*3 + size<<size*3.5<<size*0.35<<size*0.35;
        figure.setFigureParameters(FigureType::RoundedRect, pointsArray);
        figures.append(figure);
        // Ф1, Прямоугольник 2, заголовок
        pointsArray.clear();
        pointsArray<<0<<0<<partsCount*size*3 + size<<size*1.25;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф2, Эллипс первый слева
        pointsArray.clear();
        pointsArray<<size*1.5+size*3*0<<size*1.25<<size<<size;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф3, Текст для первого датчика
        pointsArray.clear();
        pointsArray<<size*0.5+size*3*0<<size*2.25<<size*3<<size;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф4, Эллипс второй слева
        pointsArray.clear();
        pointsArray<<size*1.5+size*3*1<<size*1.25<<size<<size;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф5, Текст для второго датчика
        pointsArray.clear();
        pointsArray<<size*0.5+size*3*1<<size*2.25<<size*3<<size;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф6, Эллипс третий слева
        pointsArray.clear();
        pointsArray<<size*1.5+size*3*2<<size*1.25<<size<<size;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф7, Текст для третьего датчика
        pointsArray.clear();
        pointsArray<<size*0.5+size*3*2<<size*2.25<<size*3<<size;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф8, Эллипс четвертый слева
        pointsArray.clear();
        pointsArray<<size*1.5+size*3*3<<size*1.25<<size<<size;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф9, Текст для четвертого датчика
        pointsArray.clear();
        pointsArray<<size*0.5+size*3*3<<size*2.25<<size*3<<size;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф10, Эллипс пятый слева
        pointsArray.clear();
        pointsArray<<size*1.5+size*3*4<<size*1.25<<size<<size;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф11, Текст для пятого датчика
        pointsArray.clear();
        pointsArray<<size*0.5+size*3*4<<size*2.25<<size*3<<size;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);

        // Параметры
        const int parametersCount = 7;
        for (int i = 0; i < parametersCount; i++)
        {
            parameter.setBitsCount(1);
            parameters.append(parameter);
        }

        // Текст "Откл"
        GDIObject textOtkl;
        textOtkl.setObjectType(GDIObjectType::Text);
        textOtkl.setFontParameters("Откл.", FontType::TextType);
        // Текст "БКС 1"
        GDIObject textBKS1;
        textBKS1.setObjectType(GDIObjectType::Text);
        textBKS1.setFontParameters("БКС 1", FontType::TextType);
        // Текст "БКС 2"
        GDIObject textBKS2;
        textBKS2.setObjectType(GDIObjectType::Text);
        textBKS2.setFontParameters("БКС 2", FontType::TextType);
        // Текст "На АУ"
        GDIObject textToAU;
        textToAU.setObjectType(GDIObjectType::Text);
        textToAU.setFontParameters("На АУ", FontType::TextType);
        // Текст "АУ"
        GDIObject textAU;
        textAU.setObjectType(GDIObjectType::Text);
        textAU.setFontParameters("АУ", FontType::TextType);
        // Заголовок
        GDIObject textTitle;
        textTitle.setObjectType(GDIObjectType::Text);
        textTitle.setFontParameters("Система кондиц. воздуха", FontType::HeaderType);

        // Зеленая заливка и черный контур у первого датчика
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        records.append(record);
        // Зеленая заливка и черный контур у второго датчика
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(4, gdiObjects);
        records.append(record);
        // Зеленая заливка и черный контур у второго датчика
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackMediumPen;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, true));
        record.setRecord(4, gdiObjects);
        records.append(record);
        // Зеленая заливка и черный контур у третьего датчика
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(3);
        record.setState(QVector<bool>(1, true));
        record.setRecord(6, gdiObjects);
        records.append(record);
        // Зеленая заливка и черный контур у третьего датчика
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackMediumPen;
        record.setParameterID(4);
        record.setState(QVector<bool>(1, true));
        record.setRecord(6, gdiObjects);
        records.append(record);
        // Зеленая заливка и черный контур у четвертого датчика
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(5);
        record.setState(QVector<bool>(1, true));
        record.setRecord(8, gdiObjects);
        records.append(record);
        // Зеленая заливка и черный контур у пятого датчика
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(6);
        record.setState(QVector<bool>(1, true));
        record.setRecord(10, gdiObjects);
        records.append(record);

        // Черный контур для прямоугольника
        gdiObjects.clear();
        gdiObjects<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);
        // Черный текст заголовка
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textTitle;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);
        // Датчик 1, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(2, gdiObjects);
        defaultRecords.append(record);
        // Датчик 1, текст
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textOtkl;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(3, gdiObjects);
        defaultRecords.append(record);
        // Датчик 2, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(4, gdiObjects);
        defaultRecords.append(record);
        // Датчик 2, текст
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textBKS1;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(5, gdiObjects);
        defaultRecords.append(record);
        // Датчик 3, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(6, gdiObjects);
        defaultRecords.append(record);
        // Датчик 3, текст
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textBKS2;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(7, gdiObjects);
        defaultRecords.append(record);
        // Датчик 4, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(8, gdiObjects);
        defaultRecords.append(record);
        // Датчик 4, текст
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textToAU;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(9, gdiObjects);
        defaultRecords.append(record);
        // Датчик 5, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(10, gdiObjects);
        defaultRecords.append(record);
        // Датчик 5, текст
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textAU;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(11, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        logic.setSize(QSize(partsCount*size*3+size, size*3.5));
        logic.setStartAngle(0);

        logic.setModuleName("Система кондиционирования воздуха");
        logic.setElementLogicType(ElementLogicType::Common);
        logic.setPreviewImagePath("images/SKV.png");

        // Сохранение полученного модуля
        QFile file(mBasePath + "/SKV.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;

    }



    // ===============================================================
    // ===============================================================
    // Элемент SVO ===================================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int size = 35;

        // Ф0, Прямоугольник 1, контур
        pointsArray.clear();
        pointsArray<<0<<0<<size*7.5<<size*3.5<<size*0.35<<size*0.35;
        figure.setFigureParameters(FigureType::RoundedRect, pointsArray);
        figures.append(figure);
        // Ф1, Прямоугольник 2, заголовок
        pointsArray.clear();
        pointsArray<<0<<0<<size*7.5<<size*1.25;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф2, Эллипс первый слева
        pointsArray.clear();
        pointsArray<<size*1.5+size*3.5*0<<size*1.25<<size<<size;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф3, Текст для первого датчика
        pointsArray.clear();
        pointsArray<<size*0.25+size*3.5*0<<size*2.25<<size*3.5<<size;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф4, Эллипс второй слева
        pointsArray.clear();
        pointsArray<<size*1.5+size*3.5*1<<size*1.25<<size<<size;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф5, Текст для второго датчика
        pointsArray.clear();
        pointsArray<<size*0.25+size*3.5*1<<size*2.25<<size*3.5<<size;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);

        // Параметры
        const int parametersCount = 3;
        for (int i = 0; i < parametersCount; i++)
        {
            parameter.setBitsCount(1);
            parameters.append(parameter);
        }

        // Текст "Включить"
        GDIObject text1;
        text1.setObjectType(GDIObjectType::Text);
        text1.setFontParameters("Вкл-ть", FontType::TextType);
        // Текст "Включено"
        GDIObject text2;
        text2.setObjectType(GDIObjectType::Text);
        text2.setFontParameters("Вкл-но", FontType::TextType);
        // Заголовок
        GDIObject textTitle;
        textTitle.setObjectType(GDIObjectType::Text);
        textTitle.setFontParameters("СВО", FontType::HeaderType);

        // Зеленая заливка и черный контур у первого датчика
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        records.append(record);
        // Зеленая заливка и черный контур у первого датчика
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(4, gdiObjects);
        records.append(record);
        // Красная заливка и черный контур у первого датчика
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackMediumPen;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        records.append(record);

        // Черный контур для прямоугольника
        gdiObjects.clear();
        gdiObjects<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);
        // Черный текст заголовка
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textTitle;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);
        // Датчик 1, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(2, gdiObjects);
        defaultRecords.append(record);
        // Датчик 1, текст
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<text1;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(3, gdiObjects);
        defaultRecords.append(record);
        // Датчик 2, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(4, gdiObjects);
        defaultRecords.append(record);
        // Датчик 2, текст
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<text2;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(5, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        logic.setSize(QSize(size*7.5, size*3.5));
        logic.setStartAngle(0);

        logic.setModuleName("Система воздухообмена");
        logic.setElementLogicType(ElementLogicType::Common);
        logic.setPreviewImagePath("images/SVO.png");

        // Сохранение полученного модуля
        QFile file(mBasePath + "/SVO.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }



    // ===============================================================
    // ===============================================================
    // Элемент Text ==================================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int w = 200;
        const int h = 50;

        // Рамка (коэфф. скругления углов)
        pointsArray.clear();
        pointsArray<<0<<0<<w<<h<<w/25<<w/25;
        figure.setFigureParameters(FigureType::RoundedRect, pointsArray);
        figures.append(figure);
        // Текстовая область
        pointsArray.clear();
        pointsArray<<0<<0<<w<<h;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);

        // Параметр 1
        parameter.setBitsCount(1);
        parameter.setParameterType(ParameterType::Common);
        parameter.setExtraParameter(-1);
        parameters.append(parameter);
        // Параметр 2
        parameter.setBitsCount(15);
        parameter.setParameterType(ParameterType::Number);
        parameter.setExtraParameter(0);
        parameters.append(parameter);

        // Параметр 1, истина
        // Рамка зеленая
        gdiObjects.clear();
        gdiObjects<<greenContourPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Рамка красная
        gdiObjects.clear();
        gdiObjects<<redContourPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, false));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Текст зеленый
        gdiObjects.clear();
        gdiObjects<<greenMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(1, gdiObjects);
        records.append(record);
        // Текст красный
        gdiObjects.clear();
        gdiObjects<<redMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, false));
        record.setRecord(1, gdiObjects);
        records.append(record);
        // Собственно текст
        gdiObjects.clear();
        gdiObjects<<text;
        record.setParameterID(1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        records.append(record);

        // Рамка по умолчанию
        gdiObjects.clear();
        gdiObjects<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);
        // Текст по умолчанию
        gdiObjects.clear();
        gdiObjects<<defaultText<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        // Размер элемента
        logic.setSize(QSize(w, h));
        // Стартовый угол
        logic.setStartAngle(0);
        // Дополнительные параметры
        logic.addExtraParameter("Коэффициент:", ExtraParameterType::Real, ExtraUIElementType::LineEdit);
        logic.addExtraParameter("Постфикс:", ExtraParameterType::String, ExtraUIElementType::LineEdit);
        logic.addExtraParameter("Точность:", ExtraParameterType::Integer, ExtraUIElementType::SpinBox, QVariant(2));
        logic.addExtraParameter("Учитывать знак:", ExtraParameterType::Bool, ExtraUIElementType::CheckBox, QVariant(true));

        logic.setModuleName("Цифровой датчик");
        logic.setElementLogicType(ElementLogicType::Common);

        logic.setPreviewImagePath("images/Text.png");

        QFile file(mBasePath + "/Text.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }



    // ===============================================================
    // ===============================================================
    // Элемент Text (темный) =========================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int w = 200;
        const int h = 50;

        // Рамка (коэфф. скругления углов)
        pointsArray.clear();
        pointsArray<<0<<0<<w<<h<<w/25<<w/25;
        figure.setFigureParameters(FigureType::RoundedRect, pointsArray);
        figures.append(figure);
        // Текстовая область
        pointsArray.clear();
        pointsArray<<0<<0<<w<<h;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);

        // Параметр 1
        parameter.setBitsCount(1);
        parameter.setParameterType(ParameterType::Common);
        parameter.setExtraParameter(-1);
        parameters.append(parameter);
        // Параметр 2
        parameter.setBitsCount(15);
        parameter.setParameterType(ParameterType::Number);
        parameter.setExtraParameter(0);
        parameters.append(parameter);

        // Параметр 1, истина
        // Рамка зеленая
        gdiObjects.clear();
        gdiObjects<<greenContourPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Рамка красная
        gdiObjects.clear();
        gdiObjects<<redContourPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, false));
        record.setRecord(0, gdiObjects);
        records.append(record);
        // Текст зеленый
        gdiObjects.clear();
        gdiObjects<<greenMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(1, gdiObjects);
        records.append(record);
        // Текст красный
        gdiObjects.clear();
        gdiObjects<<redMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, false));
        record.setRecord(1, gdiObjects);
        records.append(record);
        // Собственно текст
        gdiObjects.clear();
        gdiObjects<<text;
        record.setParameterID(1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        records.append(record);

        // Рамка по умолчанию
        gdiObjects.clear();
        gdiObjects<<whiteContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);
        // Текст по умолчанию
        gdiObjects.clear();
        gdiObjects<<defaultText<<whiteMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        // Размер элемента
        logic.setSize(QSize(w, h));
        // Стартовый угол
        logic.setStartAngle(0);
        // Дополнительные параметры
        logic.addExtraParameter("Коэффициент:", ExtraParameterType::Real, ExtraUIElementType::LineEdit);
        logic.addExtraParameter("Постфикс:", ExtraParameterType::String, ExtraUIElementType::LineEdit);
        logic.addExtraParameter("Точность:", ExtraParameterType::Integer, ExtraUIElementType::SpinBox, QVariant(2));
        logic.addExtraParameter("Учитывать знак:", ExtraParameterType::Bool, ExtraUIElementType::CheckBox, QVariant(true));

        logic.setModuleName("Цифровой датчик");
        logic.setElementLogicType(ElementLogicType::Common);

        logic.setPreviewImagePath("images/Text.png");

        QFile file(mBasePath + "/Text_Dark.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }


    // ===============================================================
    // ===============================================================
    // Элемент VST ===================================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int partsCount = 3;
        const int size = 35;

        // Ф0, Прямоугольник 1, контур
        pointsArray.clear();
        pointsArray<<0<<0<<partsCount*size*5 + size<<size*3.5<<size*0.35<<size*0.35;
        figure.setFigureParameters(FigureType::RoundedRect, pointsArray);
        figures.append(figure);
        // Ф1, Прямоугольник 2, заголовок
        pointsArray.clear();
        pointsArray<<0<<0<<partsCount*size*5 + size<<size*1.25;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф2, Эллипс первый слева
        pointsArray.clear();
        pointsArray<<size*2.5+size*5*0<<size*1.25<<size<<size;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф3, Текст для первого датчика
        pointsArray.clear();
        pointsArray<<size*0.5+size*5*0<<size*2.25<<size*5<<size;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф4, Эллипс второй слева
        pointsArray.clear();
        pointsArray<<size*2.5+size*5*1<<size*1.25<<size<<size;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф5, Текст для второго датчика
        pointsArray.clear();
        pointsArray<<size*0.5+size*5*1<<size*2.25<<size*5<<size;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф6, Эллипс третий слева
        pointsArray.clear();
        pointsArray<<size*2.5+size*5*2<<size*1.25<<size<<size;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф7, Текст для третьего датчика
        pointsArray.clear();
        pointsArray<<size*0.5+size*5*2<<size*2.25<<size*5<<size;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);

        // П0 - бит 0
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П1 - бит 1
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П2 - бит 3
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П3 - бит 4
        parameter.setBitsCount(1);
        parameters.append(parameter);

        // Текст ">8км (1)"
        GDIObject textGt8Km1;
        textGt8Km1.setObjectType(GDIObjectType::Text);
        textGt8Km1.setFontParameters(">8км (1)", FontType::TextType);
        // Текст ">9.3км (1)"
        GDIObject textGt93Km1;
        textGt93Km1.setObjectType(GDIObjectType::Text);
        textGt93Km1.setFontParameters(">9.3км (1)", FontType::TextType);
        // Текст ">8км (2)"
        GDIObject textGt8Km2;
        textGt8Km2.setObjectType(GDIObjectType::Text);
        textGt8Km2.setFontParameters(">8км (2)", FontType::TextType);
        // Заголовок
        GDIObject textTitle;
        textTitle.setObjectType(GDIObjectType::Text);
        textTitle.setFontParameters("Высота", FontType::HeaderType);

        // Зеленая заливка и черный контур у первого датчика
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        records.append(record);
        // Зеленая заливка и черный контур у третьего датчика
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(6, gdiObjects);
        records.append(record);
        // Красная заливка и черный контур у третьего датчика
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackMediumPen;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, true));
        record.setRecord(6, gdiObjects);
        records.append(record);
        // Зеленая заливка и черный контур у второго датчика
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackMediumPen;
        record.setParameterID(3);
        record.setState(QVector<bool>(1, true));
        record.setRecord(4, gdiObjects);
        records.append(record);

        // Черный контур для прямоугольника
        gdiObjects.clear();
        gdiObjects<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);
        // Черный текст заголовка
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textTitle;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);
        // Датчик 1, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(2, gdiObjects);
        defaultRecords.append(record);
        // Датчик 1, текст
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textGt8Km1;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(3, gdiObjects);
        defaultRecords.append(record);
        // Датчик 2, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(4, gdiObjects);
        defaultRecords.append(record);
        // Датчик 2, текст
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textGt93Km1;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(5, gdiObjects);
        defaultRecords.append(record);
        // Датчик 3, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackMediumPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(6, gdiObjects);
        defaultRecords.append(record);
        // Датчик 3, текст
        gdiObjects.clear();
        gdiObjects<<blackMediumPen<<textGt8Km2;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(7, gdiObjects);
        defaultRecords.append(record);

        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        const int warningOffset = 0;
        logic.setWarningSize(QSize(warningOffset, warningOffset));
        logic.setSize(QSize(partsCount*size*5+size, size*3.5));
        logic.setStartAngle(0);
        logic.setPreviewImagePath("images/VST.png");
        logic.setElementLogicType(ElementLogicType::Common);

        // Сохранение полученного модуля
        QFile file(mBasePath + "/VST.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;
    }



    // ===============================================================
    // ===============================================================
    // Элемент URU ===================================================
    // ===============================================================
    // ===============================================================
    if (_COMPILE_MODULES_)
    {
        ElementLogic logic;
        QVector<Figure> figures;
        QVector<Parameter> parameters;
        Figure figure;
        Parameter parameter;
        Record record;

        QVector<qint32> pointsArray;
        QVector<GDIObject> gdiObjects;
        QVector<Record> records;
        QVector<Record> defaultRecords;

        const int size = 222;

        // Ф0, Прямоугольник 1, заливка
        pointsArray.clear();
        pointsArray<<size/3<<size/3<<size/3<<size/3*4;
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);
        // Ф1, Прямоугольник 1, контур
        pointsArray.clear();
        pointsArray<<size/3<<size/3<<size/3<<size/3*4;
        figure.setFigureParameters(FigureType::Rect, pointsArray);
        figures.append(figure);
        // Ф2, Прямоугольник 1, текст
        pointsArray.clear();
        pointsArray<<size/3<<size/3<<size/3<<size/3*4;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф3, Датчик 1 (левый верхний), заливка + контур
        pointsArray.clear();
        pointsArray<<size/30<<size/3*1<<size/3.75<<size/3.75;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф4, Датчик 1 (левый верхний), текст
        pointsArray.clear();
        pointsArray<<size/30<<size/3*1<<size/3.75<<size/3.75;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф5, Датчик 2 (левый, второй сверху), заливка + контур
        pointsArray.clear();
        pointsArray<<size/30<<size/3*2<<size/3.75<<size/3.75;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф6, Датчик 2 (левый, второй сверху), текст
        pointsArray.clear();
        pointsArray<<size/30<<size/3*2<<size/3.75<<size/3.75;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф7, Датчик 3 (левый, третий сверху), заливка + контур
        pointsArray.clear();
        pointsArray<<size/30<<size/3*3<<size/3.75<<size/3.75;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф8, Датчик 3 (левый, третий сверху), текст
        pointsArray.clear();
        pointsArray<<size/30<<size/3*3<<size/3.75<<size/3.75;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф9, Датчик 4 (левый, четвертый сверху), заливка + контур
        pointsArray.clear();
        pointsArray<<size/30<<size/3*4<<size/3.75<<size/3.75;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф10, Датчик 4 (левый, четвертый сверху), текст
        pointsArray.clear();
        pointsArray<<size/30<<size/3*4<<size/3.75<<size/3.75;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф11, Датчик 5 (правый, первый сверху), заливка + контур
        pointsArray.clear();
        pointsArray<<size/3*2.1<<size/3*1<<size/3.75<<size/3.75;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф12, Датчик 5 (правый, первый сверху), текст
        pointsArray.clear();
        pointsArray<<size/3*2.1<<size/3*1<<size/3.75<<size/3.75;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф13, Датчик 6 (правый, второй сверху), заливка + контур
        pointsArray.clear();
        pointsArray<<size/3*2.1<<size/3*2<<size/3.75<<size/3.75;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф14, Датчик 6 (правый, второй сверху), текст
        pointsArray.clear();
        pointsArray<<size/3*2.1<<size/3*2<<size/3.75<<size/3.75;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф15, Датчик 7 (правый, третий сверху), заливка + контур
        pointsArray.clear();
        pointsArray<<size/3*2.1<<size/3*3<<size/3.75<<size/3.75;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф16, Датчик 7 (правый, третий сверху), текст
        pointsArray.clear();
        pointsArray<<size/3*2.1<<size/3*3<<size/3.75<<size/3.75;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф17, Датчик 8 (правый, четвертый сверху), заливка + контур
        pointsArray.clear();
        pointsArray<<size/3*2.1<<size/3*4<<size/3.75<<size/3.75;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф18, Датчик 8 (правый, четвертый сверху), текст
        pointsArray.clear();
        pointsArray<<size/3*2.1<<size/3*4<<size/3.75<<size/3.75;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);
        // Ф19, Датчик 9 (верхний), заливка + контур
        pointsArray.clear();
        pointsArray<<size/3*1.1<<size/30<<size/3.75<<size/3.75;
        figure.setFigureParameters(FigureType::Ellipse, pointsArray);
        figures.append(figure);
        // Ф20, Датчик 9 (верхний), текст
        pointsArray.clear();
        pointsArray<<size/3*1.1<<size/30<<size/3.75<<size/3.75;
        figure.setFigureParameters(FigureType::Text, pointsArray);
        figures.append(figure);

        // П0 - бит 0
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П1 - бит 1
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П2 - бит 2
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П3 - бит 3
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П4 - бит 4
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П5 - бит 5
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П6 - бит 6
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П7 - бит 7
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П8 - бит 8
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П9 - бит 9
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П10 - бит 10
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П11 - бит 11
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П12 - бит 12
        parameter.setBitsCount(1);
        parameters.append(parameter);
        // П13 - бит 13
        parameter.setBitsCount(1);
        parameters.append(parameter);

        // Текст ДТ1
        GDIObject textDT1;
        textDT1.setObjectType(GDIObjectType::Text);
        textDT1.setFontParameters("ДТ1", FontType::TextType, 0.8);
        // Текст ДТ2
        GDIObject textDT2;
        textDT2.setObjectType(GDIObjectType::Text);
        textDT2.setFontParameters("ДТ2", FontType::TextType, 0.8);
        // Текст ДТ4
        GDIObject textDT4;
        textDT4.setObjectType(GDIObjectType::Text);
        textDT4.setFontParameters("ДТ4", FontType::TextType, 0.8);
        // Текст ДТ8
        GDIObject textDT8;
        textDT8.setObjectType(GDIObjectType::Text);
        textDT8.setFontParameters("ДТ8", FontType::TextType, 0.8);
        // Текст БК2
        GDIObject textBK2;
        textBK2.setObjectType(GDIObjectType::Text);
        textBK2.setFontParameters("БК2", FontType::TextType, 0.8);
        // Текст БК3
        GDIObject textBK3;
        textBK3.setObjectType(GDIObjectType::Text);
        textBK3.setFontParameters("БК3", FontType::TextType, 0.8);
        // Текст БК4
        GDIObject textBK4;
        textBK4.setObjectType(GDIObjectType::Text);
        textBK4.setFontParameters("БК4", FontType::TextType, 0.8);
        // Текст БК6
        GDIObject textBK6;
        textBK6.setObjectType(GDIObjectType::Text);
        textBK6.setFontParameters("БК6", FontType::TextType, 0.8);
        // Текст БКС
        GDIObject textBKS;
        textBKS.setObjectType(GDIObjectType::Text);
        textBKS.setFontParameters("БКС", FontType::TextType, 0.8);
        // Текст K
        GDIObject textK;
        textK.setObjectType(GDIObjectType::Text);
        textK.setFontParameters("К", FontType::HeaderType, 2);

        // Зеленая заливка и черный контур у ДТ1
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackContourPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(3, gdiObjects);
        records.append(record);
        // Текст черный у ДТ1
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textDT1;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(4, gdiObjects);
        records.append(record);
        // Зеленая заливка и черный контур у ДТ2
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackContourPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(5, gdiObjects);
        records.append(record);
        // Текст черный у ДТ2
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textDT2;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(6, gdiObjects);
        records.append(record);
        // Зеленая заливка и черный контур у БК2
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackContourPen;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(11, gdiObjects);
        records.append(record);
        // Текст черный у БК2
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textBK2;
        record.setParameterID(0);
        record.setState(QVector<bool>(1, true));
        record.setRecord(12, gdiObjects);
        records.append(record);

        // Зеленая заливка и черный контур у ДТ1
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackContourPen;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(3, gdiObjects);
        records.append(record);
        // Текст черный у ДТ1
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textDT1;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(4, gdiObjects);
        records.append(record);
        // Зеленая заливка и черный контур у ДТ2
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackContourPen;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(5, gdiObjects);
        records.append(record);
        // Текст черный у ДТ2
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textDT2;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(6, gdiObjects);
        records.append(record);
        // Зеленая заливка и черный контур у БК3
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackContourPen;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(13, gdiObjects);
        records.append(record);
        // Текст черный у БК3
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textBK3;
        record.setParameterID(1);
        record.setState(QVector<bool>(1, true));
        record.setRecord(14, gdiObjects);
        records.append(record);

        // Красная заливка и черный контур у ДТ1
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackContourPen;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, true));
        record.setRecord(3, gdiObjects);
        records.append(record);
        // Текст белый у ДТ1
        gdiObjects.clear();
        gdiObjects<<whiteMediumPen<<textDT1;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, true));
        record.setRecord(4, gdiObjects);
        records.append(record);
        // Красная заливка и черный контур у ДТ2
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackContourPen;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, true));
        record.setRecord(5, gdiObjects);
        records.append(record);
        // Текст белый у ДТ2
        gdiObjects.clear();
        gdiObjects<<whiteMediumPen<<textDT2;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, true));
        record.setRecord(6, gdiObjects);
        records.append(record);
        // Красная заливка и черный контур у БК2
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackContourPen;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, true));
        record.setRecord(11, gdiObjects);
        records.append(record);
        // Текст белый у БК2
        gdiObjects.clear();
        gdiObjects<<whiteMediumPen<<textBK2;
        record.setParameterID(2);
        record.setState(QVector<bool>(1, true));
        record.setRecord(12, gdiObjects);
        records.append(record);

        // Красная заливка и черный контур у ДТ1
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackContourPen;
        record.setParameterID(3);
        record.setState(QVector<bool>(1, true));
        record.setRecord(3, gdiObjects);
        records.append(record);
        // Текст белый у ДТ1
        gdiObjects.clear();
        gdiObjects<<whiteMediumPen<<textDT1;
        record.setParameterID(3);
        record.setState(QVector<bool>(1, true));
        record.setRecord(4, gdiObjects);
        records.append(record);
        // Красная заливка и черный контур у ДТ2
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackContourPen;
        record.setParameterID(3);
        record.setState(QVector<bool>(1, true));
        record.setRecord(5, gdiObjects);
        records.append(record);
        // Текст белый у ДТ2
        gdiObjects.clear();
        gdiObjects<<whiteMediumPen<<textDT2;
        record.setParameterID(3);
        record.setState(QVector<bool>(1, true));
        record.setRecord(6, gdiObjects);
        records.append(record);
        // Красная заливка и черный контур у БК3
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackContourPen;
        record.setParameterID(3);
        record.setState(QVector<bool>(1, true));
        record.setRecord(13, gdiObjects);
        records.append(record);
        // Текст белый у БК3
        gdiObjects.clear();
        gdiObjects<<whiteMediumPen<<textBK3;
        record.setParameterID(3);
        record.setState(QVector<bool>(1, true));
        record.setRecord(14, gdiObjects);
        records.append(record);

        // Зеленая заливка и черный контур у ДТ1
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackContourPen;
        record.setParameterID(4);
        record.setState(QVector<bool>(1, true));
        record.setRecord(3, gdiObjects);
        records.append(record);
        // Текст черный у ДТ1
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textDT1;
        record.setParameterID(4);
        record.setState(QVector<bool>(1, true));
        record.setRecord(4, gdiObjects);
        records.append(record);
        // Зеленая заливка и черный контур у ДТ4
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackContourPen;
        record.setParameterID(4);
        record.setState(QVector<bool>(1, true));
        record.setRecord(7, gdiObjects);
        records.append(record);
        // Текст черный у ДТ4
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textDT4;
        record.setParameterID(4);
        record.setState(QVector<bool>(1, true));
        record.setRecord(8, gdiObjects);
        records.append(record);
        // Зеленая заливка и черный контур у БК4
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackContourPen;
        record.setParameterID(4);
        record.setState(QVector<bool>(1, true));
        record.setRecord(15, gdiObjects);
        records.append(record);
        // Текст черный у БК4
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textBK4;
        record.setParameterID(4);
        record.setState(QVector<bool>(1, true));
        record.setRecord(16, gdiObjects);
        records.append(record);

        // Зеленая заливка и черный контур у ДТ1
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackContourPen;
        record.setParameterID(5);
        record.setState(QVector<bool>(1, true));
        record.setRecord(3, gdiObjects);
        records.append(record);
        // Текст черный у ДТ1
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textDT1;
        record.setParameterID(5);
        record.setState(QVector<bool>(1, true));
        record.setRecord(4, gdiObjects);
        records.append(record);
        // Зеленая заливка и черный контур у ДТ8
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackContourPen;
        record.setParameterID(5);
        record.setState(QVector<bool>(1, true));
        record.setRecord(9, gdiObjects);
        records.append(record);
        // Текст черный у ДТ8
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textDT8;
        record.setParameterID(5);
        record.setState(QVector<bool>(1, true));
        record.setRecord(10, gdiObjects);
        records.append(record);
        // Зеленая заливка и черный контур у БК6
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackContourPen;
        record.setParameterID(5);
        record.setState(QVector<bool>(1, true));
        record.setRecord(17, gdiObjects);
        records.append(record);
        // Текст черный у БК6
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textBK6;
        record.setParameterID(5);
        record.setState(QVector<bool>(1, true));
        record.setRecord(18, gdiObjects);
        records.append(record);

        // Красная заливка и черный контур у ДТ1
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackContourPen;
        record.setParameterID(6);
        record.setState(QVector<bool>(1, true));
        record.setRecord(3, gdiObjects);
        records.append(record);
        // Текст белый у ДТ1
        gdiObjects.clear();
        gdiObjects<<whiteMediumPen<<textDT1;
        record.setParameterID(6);
        record.setState(QVector<bool>(1, true));
        record.setRecord(4, gdiObjects);
        records.append(record);
        // Красная заливка и черный контур у ДТ4
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackContourPen;
        record.setParameterID(6);
        record.setState(QVector<bool>(1, true));
        record.setRecord(7, gdiObjects);
        records.append(record);
        // Текст белый у ДТ4
        gdiObjects.clear();
        gdiObjects<<whiteMediumPen<<textDT4;
        record.setParameterID(6);
        record.setState(QVector<bool>(1, true));
        record.setRecord(8, gdiObjects);
        records.append(record);
        // Красная заливка и черный контур у БК4
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackContourPen;
        record.setParameterID(6);
        record.setState(QVector<bool>(1, true));
        record.setRecord(15, gdiObjects);
        records.append(record);
        // Текст белый у БК4
        gdiObjects.clear();
        gdiObjects<<whiteMediumPen<<textBK4;
        record.setParameterID(6);
        record.setState(QVector<bool>(1, true));
        record.setRecord(16, gdiObjects);
        records.append(record);

        // Красная заливка и черный контур у ДТ1
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackContourPen;
        record.setParameterID(7);
        record.setState(QVector<bool>(1, true));
        record.setRecord(3, gdiObjects);
        records.append(record);
        // Текст белый у ДТ1
        gdiObjects.clear();
        gdiObjects<<whiteMediumPen<<textDT1;
        record.setParameterID(7);
        record.setState(QVector<bool>(1, true));
        record.setRecord(4, gdiObjects);
        records.append(record);
        // Красная заливка и черный контур у ДТ8
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackContourPen;
        record.setParameterID(7);
        record.setState(QVector<bool>(1, true));
        record.setRecord(9, gdiObjects);
        records.append(record);
        // Текст белый у ДТ8
        gdiObjects.clear();
        gdiObjects<<whiteMediumPen<<textDT8;
        record.setParameterID(7);
        record.setState(QVector<bool>(1, true));
        record.setRecord(10, gdiObjects);
        records.append(record);
        // Красная заливка и черный контур у БК6
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackContourPen;
        record.setParameterID(7);
        record.setState(QVector<bool>(1, true));
        record.setRecord(17, gdiObjects);
        records.append(record);
        // Текст белый у БК6
        gdiObjects.clear();
        gdiObjects<<whiteMediumPen<<textBK6;
        record.setParameterID(7);
        record.setState(QVector<bool>(1, true));
        record.setRecord(18, gdiObjects);
        records.append(record);

        // Зеленая заливка и черный контур у БКС
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackContourPen;
        record.setParameterID(8);
        record.setState(QVector<bool>(1, true));
        record.setRecord(19, gdiObjects);
        records.append(record);
        // Текст черный у БКС
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textBKS;
        record.setParameterID(8);
        record.setState(QVector<bool>(1, true));
        record.setRecord(20, gdiObjects);
        records.append(record);
        // Зеленая заливка и черный контур у БК2
        gdiObjects.clear();
        gdiObjects<<greenBrush<<blackContourPen;
        record.setParameterID(8);
        record.setState(QVector<bool>(1, true));
        record.setRecord(11, gdiObjects);
        records.append(record);
        // Текст черный у БК2
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textBK2;
        record.setParameterID(8);
        record.setState(QVector<bool>(1, true));
        record.setRecord(12, gdiObjects);
        records.append(record);

        // Красная заливка и черный контур у БК4
        gdiObjects.clear();
        gdiObjects<<redBrush<<blackContourPen;
        record.setParameterID(9);
        record.setState(QVector<bool>(1, true));
        record.setRecord(15, gdiObjects);
        records.append(record);
        // Текст белый у БК4
        gdiObjects.clear();
        gdiObjects<<whiteMediumPen<<textBK4;
        record.setParameterID(9);
        record.setState(QVector<bool>(1, true));
        record.setRecord(16, gdiObjects);
        records.append(record);

        // Зеленая заливка у прямоугольника
        gdiObjects.clear();
        gdiObjects<<greenBrush;
        record.setParameterID(10);
        record.setState(QVector<bool>(1, true));
        record.setRecord(0, gdiObjects);
        records.append(record);

        // Красная заливка у прямоугольника
        gdiObjects.clear();
        gdiObjects<<redBrush;
        record.setParameterID(10);
        record.setState(QVector<bool>(1, false));
        record.setRecord(0, gdiObjects);
        records.append(record);

        // Черная буква К
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textK;
        record.setParameterID(11);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        records.append(record);

        // Красная буква К
        gdiObjects.clear();
        gdiObjects<<redMediumPen<<textK;
        record.setParameterID(12);
        record.setState(QVector<bool>(1, true));
        record.setRecord(2, gdiObjects);
        records.append(record);

        // Синий контур
        gdiObjects.clear();
        gdiObjects<<blueThickPen;
        record.setParameterID(13);
        record.setState(QVector<bool>(1, true));
        record.setRecord(1, gdiObjects);
        records.append(record);

        // Зеленый контур
        gdiObjects.clear();
        gdiObjects<<greenThickPen;
        record.setParameterID(13);
        record.setState(QVector<bool>(1, false));
        record.setRecord(1, gdiObjects);
        records.append(record);

        // Белая заливка для прямоугольника
        gdiObjects.clear();
        gdiObjects<<whiteBrush;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(0, gdiObjects);
        defaultRecords.append(record);

        // Черный контур для прямоугольника
        gdiObjects.clear();
        gdiObjects<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(1, gdiObjects);
        defaultRecords.append(record);

        // ДТ1, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(3, gdiObjects);
        defaultRecords.append(record);
        // ДТ1, черный текст
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textDT1;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(4, gdiObjects);
        defaultRecords.append(record);
        // ДТ2, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(5, gdiObjects);
        defaultRecords.append(record);
        // ДТ2, черный текст
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textDT2;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(6, gdiObjects);
        defaultRecords.append(record);
        // ДТ4, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(7, gdiObjects);
        defaultRecords.append(record);
        // ДТ4, черный текст
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textDT4;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(8, gdiObjects);
        defaultRecords.append(record);
        // ДТ8, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(9, gdiObjects);
        defaultRecords.append(record);
        // ДТ8, черный текст
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textDT8;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(10, gdiObjects);
        defaultRecords.append(record);
        // БК2, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(11, gdiObjects);
        defaultRecords.append(record);
        // БК2, черный текст
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textBK2;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(12, gdiObjects);
        defaultRecords.append(record);
        // БК3, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(13, gdiObjects);
        defaultRecords.append(record);
        // БК3, черный текст
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textBK3;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(14, gdiObjects);
        defaultRecords.append(record);
        // БК4, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(15, gdiObjects);
        defaultRecords.append(record);
        // БК4, черный текст
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textBK4;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(16, gdiObjects);
        defaultRecords.append(record);
        // БК6, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(17, gdiObjects);
        defaultRecords.append(record);
        // БК6, черный текст
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textBK6;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(18, gdiObjects);
        defaultRecords.append(record);
        // БКС, белая заливка, черный контур
        gdiObjects.clear();
        gdiObjects<<whiteBrush<<blackContourPen;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(19, gdiObjects);
        defaultRecords.append(record);
        // БКС, черный текст
        gdiObjects.clear();
        gdiObjects<<blackContourPen<<textBKS;
        record.setParameterID(-1);
        record.setState(QVector<bool>());
        record.setRecord(20, gdiObjects);
        defaultRecords.append(record);


        // Установка логики
        for (int i = 0; i < parameters.size(); i++)
            logic.addParameter(parameters[i]);
        for (int i = 0; i < figures.size(); i++)
            logic.addFigure(figures[i]);
        for (int i = 0; i < records.size(); i++)
            logic.addRecord(records[i]);
        for (int i = 0; i < defaultRecords.size(); i++)
            logic.addDefaultRecord(defaultRecords[i]);

        logic.setSize(QSize(size, size * 5 / 3));
        logic.setStartAngle(0);
        const int warningOffset = 15;
        logic.setWarningSize(QSize(warningOffset, warningOffset));

        logic.setModuleName("Устройство резервного управления");
        logic.setPreviewImagePath("images/URU.png");
        logic.setElementLogicType(ElementLogicType::Common);

        // Сохранение полученного модуля
        QFile file(mBasePath + "/URU.bin");
        file.open(QIODevice::WriteOnly);
        QDataStream ds(&file);
        ds<<logic;

    }


}



// Загрузка модуля по имени (результат загрузки - loadResult)
QSharedPointer<ElementLogic> LogicLoader::loadModule(QString moduleName, bool& loadResult)
{
    QSharedPointer<ElementLogic> elementLogic(new ElementLogic());

    QFile moduleFile;
    if (QFile::exists(mBasePath + mSlash + moduleName))
        moduleFile.setFileName(mBasePath + mSlash + moduleName);
    else if (QFile::exists(mProjectPath + mSlash + moduleName))
        moduleFile.setFileName(mProjectPath + mSlash + moduleName);

    bool result = moduleFile.open(QIODevice::ReadOnly);
    if (result)
    {
        // Содержимое модуля
        QDataStream moduleStream(&moduleFile);
        moduleStream>>*(elementLogic.data());
        // Хеш
        moduleFile.seek(0);
        QByteArray fileData = moduleFile.readAll();
        QByteArray hash = QCryptographicHash::hash(fileData, QCryptographicHash::Md5);
        elementLogic.data()->setModuleHash(hash.toBase64());

        moduleFile.close();
    }

    loadResult = result;

    return elementLogic;
}



// Обновление списка модулей в директории + расчет хешей
int LogicLoader::refreshModulesList()
{
    // Выбор всех *.bin-файлов в директории
    mModules = mBaseDir.entryList(QStringList() << "*.bin", QDir::Files);

    // Дополнение списка модулей
    QStringList modules = mProjectDir.entryList(QStringList() << "*.bin", QDir::Files);
    for (int i = 0; i < modules.size(); i++)
        if (mModules.indexOf(modules[i]) < 0)
            mModules.append(modules[i]);

    // Сортировка по алфавиту списка модулей
    mModules.sort();

    return 0;
}



int LogicLoader::setProjectDirectory(const QString projectDirectory)
{
    mOldProjectPath = mProjectPath;
    mProjectPath = projectDirectory + mModulesFolder;

    if (projectDirectory.isEmpty())
        return -1;

    // Проверка наличия папок, необходимых для работы программы
    mProjectDir = QDir(mProjectPath);
    if (!mProjectDir.exists())
        QDir().mkdir(mProjectPath);

    if (!mProjectPath.isEmpty())
    {
        // Трекер изменений директорий
        mDirectoryWatcher->removePath(mProjectPath);
        mDirectoryWatcher->addPath(mProjectPath);

        // Трекер изменений файлов
        mFileWatcher->removePath(mProjectPath);
        mFileWatcher->addPath(mProjectPath);
    }

    return 0;
}



bool LogicLoader::copyModuleToProjectDirectory(QString moduleName)
{
    if (QFile::exists(mBasePath + mSlash + moduleName))
        return QFile::copy(mBasePath + mSlash + moduleName, mProjectPath + mSlash + moduleName);
    else if (QFile::exists(mOldProjectPath + mSlash + moduleName))
        return QFile::copy(mOldProjectPath + mSlash + moduleName, mProjectPath + mSlash + moduleName);
    else
        return false;
}



// Получение списка доступных модулей
QStringList LogicLoader::modulesList()
{
    return mModules;
}



// Слот - Обработка изменений в директории с модулями
int LogicLoader::slotDirectoryChanged(QString path)
{
    Q_UNUSED(path);

    this->refreshModulesList();

    emit signalModulesListChanged(mModules);

    return 0;
}



int LogicLoader::slotFilesChanged(QString)
{
    this->refreshModulesList();

    emit signalModulesListChanged(mModules);

    return 0;
}
