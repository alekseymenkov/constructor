#include "graphicsobject.h"



GraphicsObject::GraphicsObject(QGraphicsItem* parent) :
    QGraphicsObject(parent),
    mMaxSize(QSize(0, 0)),
    mOffset(QSize(0, 0)),
    mCurrentTimerValue(0),
    mOldTimerValue(0),
    mIsRepaintNeeded(false),
    mIsDataFromServer(false),
    mIsConflict(false),
    mIsToolTipShow(false),
    mBaseAngle(0),
    mIsConstructorShowed(false)
{
    mState.clear();
    mCurrentColors.clear();

    // Установка опции для работы с событиями мыши
    this->setFlag(QGraphicsObject::ItemIsSelectable, true);

    // Настройка таймера
    mRepaintTimer = new QTimer(this);
    connect(mRepaintTimer, SIGNAL(timeout()), this, SLOT(slotRepaintTimer()));
    mRepaintTimer->setInterval(mRepaintTimerInterval);
    mRepaintTimer->start();

    // Настройка шрифтов
    mMainFont = QFont(Font::family, Font::mainFontSize,
                      Font::mainFontBold, false);
    mMainFont.setLetterSpacing(QFont::PercentageSpacing, Font::letterSpace);

    mHeaderFont = QFont(Font::family, Font::headerFontSize,
                        Font::headerFontBold, false);
    mHeaderFont.setLetterSpacing(QFont::PercentageSpacing, Font::letterSpace);
}



// Основная процедура прорисовки
int GraphicsObject::drawFigure(QPainter* painter, Figure &figure, QPen &pen, QBrush &brush, QFont &font, QString &text)
{
    // Массив точек, параметры фигуры
    QVector<qint32> points = figure.points();

    // Установка объектов рисования
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->setFont(font);

    QVector<QPoint> coordinates;
    qreal xRadius = 0;
    qreal yRadius = 0;
    // Прорисовка фигуры
    switch (figure.figureType())
    {
    // Эллипс (контур + заливка)
    case FigureType::Ellipse:
        painter->drawEllipse(points[0] * mElementInterface.data()->sizeModificator() + mOffset.width(),
                points[1] * mElementInterface.data()->sizeModificator() + mOffset.height(),
                points[2] * mElementInterface.data()->sizeModificator(),
                points[3] * mElementInterface.data()->sizeModificator());
        break;
        // Прямоугольник (контур)
    case FigureType::Rect:
        painter->drawRect(points[0] * mElementInterface.data()->sizeModificator() + mOffset.width(),
                points[1] * mElementInterface.data()->sizeModificator() + mOffset.height(),
                points[2] * mElementInterface.data()->sizeModificator(),
                points[3] * mElementInterface.data()->sizeModificator());
        break;
    case FigureType::RoundedRect:
        xRadius = 15 * points[2] * mElementInterface.data()->sizeModificator() / 560;
        yRadius = 12 * points[3] * mElementInterface.data()->sizeModificator() / 122;
        painter->drawRoundedRect(points[0] * mElementInterface.data()->sizeModificator() + mOffset.width(),
                points[1] * mElementInterface.data()->sizeModificator() + mOffset.height(),
                points[2] * mElementInterface.data()->sizeModificator(),
                points[3] * mElementInterface.data()->sizeModificator(),
                xRadius,
                yRadius,
                Qt::AbsoluteSize);
        break;
    case FigureType::Chord:
        painter->drawChord(points[0] * mElementInterface.data()->sizeModificator() + mOffset.width(),
                points[1] * mElementInterface.data()->sizeModificator() + mOffset.height(),
                points[2] * mElementInterface.data()->sizeModificator(),
                points[3] * mElementInterface.data()->sizeModificator(),
                points[4] * 16,
                points[5] * 16);
        break;
    case FigureType::Line:
        painter->drawLine(points[0] * mElementInterface.data()->sizeModificator() + mOffset.width(),
                points[1] * mElementInterface.data()->sizeModificator() + mOffset.height(),
                points[2] * mElementInterface.data()->sizeModificator() + mOffset.width(),
                points[3] * mElementInterface.data()->sizeModificator() + mOffset.height());
        break;
    case FigureType::Pie:
        painter->drawPie(points[0] * mElementInterface.data()->sizeModificator() + mOffset.width(),
                points[1] * mElementInterface.data()->sizeModificator() + mOffset.height(),
                points[2] * mElementInterface.data()->sizeModificator(),
                points[3] * mElementInterface.data()->sizeModificator(),
                points[4] * 16,
                points[5] * 16);
        break;
    case FigureType::Arc:
        painter->drawArc(points[0] * mElementInterface.data()->sizeModificator() + mOffset.width(),
                points[1] * mElementInterface.data()->sizeModificator() + mOffset.height(),
                points[2] * mElementInterface.data()->sizeModificator(),
                points[3] * mElementInterface.data()->sizeModificator(),
                points[4] * 16,
                points[5] * 16);
        break;
    case FigureType::Polygon:
        for (int i = 0; i < points.size(); i += 2)
            coordinates.append(QPoint(points[i] * mElementInterface.data()->sizeModificator() + mOffset.width(),
                                      points[i + 1] * mElementInterface.data()->sizeModificator() + mOffset.height()));
        painter->drawPolygon(coordinates.data(), coordinates.size());
        break;
    case FigureType::Undefined:
        break;
        // Текст. Просто текст
    case FigureType::Text:
        painter->drawText(points[0] * mElementInterface.data()->sizeModificator()+ mOffset.width(),
                points[1] * mElementInterface.data()->sizeModificator()+ mOffset.height(),
                points[2] * mElementInterface.data()->sizeModificator(),
                points[3] * mElementInterface.data()->sizeModificator(),
                Qt::AlignCenter,
                text);
        break;
    }

    return 0;
}



// Прорисовка стандартного элемента (графические примитивы и/или числовые зн.)
int GraphicsObject::drawStandartElement(QPainter *painter)
{
    // Флаг конфликта устанавливается в истину при каждом вызове функции
    mIsConflict = false;

    // Количество зарегистрированных фигур в элементе
    const int figuresCount = mElementLogic.data()->figuresCount();
    // Количество зарегистрированных параметров в элементе
    const int parametersCount = mElementLogic.data()->parametersCount();
    // Количество записей, зарегистированных для элемента
    const int recordsCount = mElementLogic.data()->recordsCount();
    // Количество записей по умолчанию
    const int defaultRecordsCount = mElementLogic.data()->defaultRecordsCount();

    // Перебор фигур с учетом z-индекса
    for (int i = 0; i < figuresCount; i++)
    {
        // Конкретные объекты для рисования
        // Карандаш
        QPen pen(Qt::transparent);
        // Кисть
        QBrush brush(Qt::transparent);
        // Шрифт
        QFont font;
        // Отображаемый текст
        QString drawingText;

        // Новый угол поворота для фигуры
        int newAngle = 0;
        int correctionAngle = 0;

        // Фигура для перерисовки
        Figure figure = mElementLogic.data()->figure(i);

        // Каждый бит - фигура. Если она прорисовывается дважды - возникает конфликт
        const int gdiObjectsCount = 3;
        QVector<bool> conflictFlags(gdiObjectsCount, false);

        // Работа с параметрами только в том случае,
        // если данные получены с сервера
        if (mIsDataFromServer)
        {
            // Перебор массива бит (и параметров)
            for (int j = 0; j < parametersCount; j++)
            {
                if (!mElementInterface.data()->isParameterUsed(j))
                    continue;

                // Основной параметр
                Parameter parameter = mElementLogic.data()->parameter(j);
                ParameterType::Type parameterType = parameter.parameterType();

                for (int k = 0; k < recordsCount; k++)
                {
                    Record record = mElementLogic.data()->record(k);

                    // Поиск необходимой записи
                    // Запись описывает нужную фигуру...
                    bool exp1 = i == record.figureID();
                    // ... нужный параметр...
                    bool exp2 = j == record.parameterID();
                    // ... и в нужном состоянии
                    bool exp3 = mParametersValues[j] == record.state();
                    // Дополнительное условие - параметр текстовый
                    bool exp4 = parameterType == ParameterType::Number;
                    // Продолжаем, если запись подходит
                    if (!((exp1 && exp2) && (exp3 || exp4)))
                        continue;

                    newAngle = record.angle();

                    // Получаем массив элементов GDI
                    QVector<GDIObject> gdiObjects = record.GDIObjects();
                    // Перебор объектов GDI
                    for (int l = 0; l < gdiObjects.size(); l++)
                    {
                        if (gdiObjects[l].isBlinked())
                            mIsRepaintNeeded = true;

                        // Условие проверки на конфликтные биты
                        if (conflictFlags[static_cast<int>(gdiObjects[l].objectType())])
                            mIsConflict = true;
                        conflictFlags[static_cast<int>(gdiObjects[l].objectType())] = true;

                        // В соответствии с типом объекта, инициализируем
                        // соответствующий объект для рисования
                        switch (gdiObjects[l].objectType())
                        {
                        case GDIObjectType::Pen:
                            // Установка цвета
                            pen.setColor(gdiObjects[l].color(mCurrentColors[i]));
                            // Установка толщины карандаша
                            pen.setWidth(gdiObjects[l].penWidth() * mElementInterface.data()->sizeModificator());
                            // Установка стиля карандаша
                            pen.setStyle(gdiObjects[l].penStyle());
                            break;

                        case GDIObjectType::Brush:
                            // Установка цвета
                            brush.setColor(gdiObjects[l].color(mCurrentColors[i]));
                            // Установка стиля кисти
                            brush.setStyle(Qt::SolidPattern);
                            break;
                        case GDIObjectType::Text:
                            // Установка отображаемого текста для числового параметра
                            if (parameterType == ParameterType::Number)
                            {
                                const int coeffID = parameter.extraParameter() * 4;
                                const int postfixID = parameter.extraParameter() * 4 + 1;
                                const int precisionID = parameter.extraParameter() * 4 + 2;
                                const int signID = parameter.extraParameter() * 4 + 3;

                                qreal coeff = mElementInterface.data()->extraParameter(coeffID).toFloat();
                                QString postfix = mElementInterface.data()->extraParameter(postfixID).toString();
                                bool isSigned = mElementInterface.data()->extraParameter(signID).toBool();
                                int precision = mElementInterface.data()->extraParameter(precisionID).toInt();

                                const int precisionMin = 0;
                                const int precisionMax = 6;
                                const int precisionDefault = 2;
                                if (precision < precisionMin || precision > precisionMax)
                                    precision = precisionDefault;

                                if (!postfix.isEmpty())
                                    postfix = " " + postfix;
                                qreal number = parseNumber(mParametersValues[j], coeff, isSigned);

                                // Расчет плотности
                                if (mElementInterface.data()->elementLogicType() == ElementLogicType::FuelDensity)
                                {
                                    const int densityID = mElementInterface.data()->extraParametersCount() - 1;
                                    qreal density = mElementInterface.data()->extraParameter(densityID).toFloat();
                                    number = calculateFuelDensity(density, number);
                                }

                                if (mElementInterface.data()->elementLogicType() == ElementLogicType::FuelDensity)
                                    mParametersString[j] = QString::number(number, 'f', precision);
                                else
                                    mParametersString[j] = QString::number(number, 'f', precision);

                                // Сохранение полученного числа
                                drawingText = mParametersString[j] +
                                        QString("%1").arg(postfix);

                                if (!mElementInterface.data()->isParameterUsed(parameter.extraParameter()))
                                {
                                    for (int m = 0; m < mElementLogic.data()->defaultRecordsCount(); m++)
                                    {
                                        Record defaultRecord = mElementLogic.data()->defaultRecord(m);
                                        if (defaultRecord.figureID() != i)
                                            continue;
                                        pen = QPen(defaultRecord.GDIObjects()[1].color(0));
                                    }
                                }
                            }
                            else
                            {
                                drawingText = gdiObjects[l].text();
                            }

                            // Настройка шрифта в соответствии с его типом
                            if (gdiObjects[l].fontType() == FontType::HeaderType)
                                font = mHeaderFont;
                            else
                                font = mMainFont;

                            font.setPointSize(font.pointSize() *
                                              gdiObjects[l].sizeModificator() *
                                              mElementInterface.data()->sizeModificator());
                            break;
                        case GDIObjectType::Undefined:
                            break;
                        }
                    }
                }
            }
        }

        // Проверка, применен ли хоть один параметр GDI к фигуре
        bool defaultPaint = true;
        for (int j = 0; j < gdiObjectsCount; j++)
            if (conflictFlags[j])
                defaultPaint = false;

        if (defaultPaint)
        {
            for (int j = 0; j < defaultRecordsCount; j++)
            {
                Record record = mElementLogic.data()->defaultRecord(j);

                // Поиск необходимой записи
                // Запись описывает нужную фигуру...
                if (i != record.figureID())
                    continue;

                // Получаем массив элементов GDI
                QVector<GDIObject> gdiObjects = record.GDIObjects();
                // Перебор объектов GDI
                for (int k = 0; k < gdiObjects.size(); k++)
                {
                    if (gdiObjects[k].isBlinked())
                        mIsRepaintNeeded = true;

                    // В соответствии с типом объекта, инициализируем
                    // соответствующий объект для рисования
                    switch (gdiObjects[k].objectType())
                    {
                    case GDIObjectType::Pen:
                        // Установка цвета
                        pen.setColor(gdiObjects[k].color(mCurrentColors[i]));
                        // Установка толщины карандаша
                        pen.setWidth(gdiObjects[k].penWidth() * mElementInterface.data()->sizeModificator());
                        // Установка стиля карандаша
                        pen.setStyle(gdiObjects[k].penStyle());
                        break;

                    case GDIObjectType::Brush:
                        // Установка цвета
                        brush.setColor(gdiObjects[k].color(mCurrentColors[i]));
                        // Установка стиля кисти
                        brush.setStyle(Qt::SolidPattern);
                        break;

                    case GDIObjectType::Text:
                        drawingText = gdiObjects[k].text();
                        // Настройка шрифта в соответствии с его типом
                        if (gdiObjects[k].fontType() == FontType::HeaderType)
                            font = mHeaderFont;
                        else
                            font = mMainFont;

                        font.setPointSize(font.pointSize() *
                                          gdiObjects[k].sizeModificator() *
                                          mElementInterface.data()->sizeModificator());
                        break;

                    case GDIObjectType::Undefined:
                        break;
                    }
                }
            }

        }

        const int degreesInCircle = 360;
        if (figure.isStatic())
            correctionAngle = degreesInCircle - mBaseAngle;
        else
            correctionAngle = 0;

        int resultAngle = (newAngle < 0 ? 0 : newAngle) + correctionAngle;
        resultAngle = resultAngle % degreesInCircle;
        if (resultAngle >= 0)
        {
            int boundingWidth = this->boundingRect().width() / 2;
            int boundingHeight = this->boundingRect().height() / 2;
            painter->setTransform(QTransform()
                                  .translate(boundingWidth,
                                             boundingHeight)
                                  .rotate(resultAngle)
                                  .translate(-boundingWidth,
                                             -boundingHeight),
                                  true);
        }

        drawFigure(painter, figure, pen, brush, font, drawingText);

        if (resultAngle >= 0)
        {
            const int boundingWidth = this->boundingRect().width() / 2;
            const int boundingHeight = this->boundingRect().height() / 2;
            painter->setTransform(QTransform()
                                  .translate(boundingWidth,
                                             boundingHeight)
                                  .rotate(-resultAngle)
                                  .translate(-boundingWidth,
                                             -boundingHeight),
                                  true);
        }

    }

    if (mIsConflict)
    {
        // Прямоугольник с размерами области предупреждения
        // X, Y - отступ от нулевой координаты за вычетом размера области предупреждения
        // Width, Height - основной размер * модификатор + двойной отступ при предупреждении
        const QRect roundedRect(mOffset.width() - mElementLogic.data()->warningSize().width(),
                                mOffset.height() - mElementLogic.data()->warningSize().height(),
                                mMaxSize.width() * mElementInterface.data()->sizeModificator() + mElementLogic.data()->warningSize().width() * 2,
                                mMaxSize.height() * mElementInterface.data()->sizeModificator() + mElementLogic.data()->warningSize().height() * 2);
        const qreal xRadius = 5;
        const qreal yRadius = xRadius * roundedRect.width() / roundedRect.height();
        painter->setPen(Qt::NoPen);
        painter->setBrush(gradient);
        painter->drawRoundedRect(roundedRect, xRadius, yRadius, Qt::RelativeSize);
    }

    return 0;
}



// Прорисовка индикатора (1 бит)
int GraphicsObject::drawSingleIndicator(QPainter *painter)
{
    // Фигура для перерисовки
    QVector<qint32> points;
    points<<0<<0<<mElementInterface.data()->size().width()<<mElementInterface.data()->size().height();

    Figure rectFigure = mElementLogic.data()->figure(0);
    rectFigure.setFigureParameters(rectFigure.figureType(), points, rectFigure.isStatic(), rectFigure.zIndex());
    Figure textFigure = mElementLogic.data()->figure(1);
    textFigure.setFigureParameters(textFigure.figureType(), points, textFigure.isStatic(), textFigure.zIndex());
    // Конкретные объекты для рисования
    // Карандаш
    // Кисть
    QPen pen;
    QBrush brush(Qt::transparent);
    QFont font;
    QString string;

    // Работа с параметрами только в том случае,
    // если данные получены с сервера
    if (mIsDataFromServer)
    {
        // mParameterValues[0] == true, используем нулевой параметр в качестве цвета
        if (mParametersValues[0][0])
            brush = QBrush(mElementInterface.data()->extraParameter(2).value<QColor>());
        else
            brush = QBrush(mElementInterface.data()->extraParameter(1).value<QColor>());
    }
    else
    {
        if (mElementLogic.data()->elementLogicType() == ElementLogicType::SingleIndicator)
            brush = QBrush(Qt::white);
        else if (mElementLogic.data()->elementLogicType() == ElementLogicType::SingleIndicatorDark)
            brush = QBrush(Qt::black);
    }

    if (mElementLogic.data()->elementLogicType() == ElementLogicType::SingleIndicator)
        pen = QPen(Qt::black, LineWidth::outerLine, Qt::SolidLine);
    else if (mElementLogic.data()->elementLogicType() == ElementLogicType::SingleIndicatorDark)
        pen = QPen(Qt::white, LineWidth::outerLine, Qt::SolidLine);

    drawFigure(painter, rectFigure, pen, brush, font, string);

    if ((brush.color().red() + brush.color().green() + brush.color().blue()) < 96)
        pen.setColor(Qt::white);
    else
        pen.setColor(Qt::black);

    string = mElementInterface.data()->extraParameter(0).toString();
    font = mHeaderFont;
    drawFigure(painter, textFigure, pen, brush, font, string);

    return 0;
}



int GraphicsObject::drawDoubleIndicator(QPainter *painter)
{
    // Фигура для перерисовки
    QVector<qint32> points;
    points<<0<<0<<mElementInterface.data()->size().width()<<mElementInterface.data()->size().height();

    Figure rectFigure = mElementLogic.data()->figure(0);
    rectFigure.setFigureParameters(rectFigure.figureType(), points, rectFigure.isStatic(), rectFigure.zIndex());
    Figure textFigure = mElementLogic.data()->figure(1);
    textFigure.setFigureParameters(textFigure.figureType(), points, textFigure.isStatic(), textFigure.zIndex());
    // Конкретные объекты для рисования
    // Карандаш
    QPen pen(Qt::white, LineWidth::outerLine, Qt::SolidLine);
    // Кисть
    QBrush brush(Qt::transparent);
    QFont font;
    QString string;

    // Работа с параметрами только в том случае,
    // если данные получены с сервера
    if (mIsDataFromServer)
    {
        // Значение 00, используем нулевой параметр в качестве цвета
        if (!mParametersValues[0][0] && !mParametersValues[1][0])
            brush = QBrush(mElementInterface.data()->extraParameter(1).value<QColor>());
        else if (!mParametersValues[0][0] && mParametersValues[1][0])
            brush = QBrush(mElementInterface.data()->extraParameter(2).value<QColor>());
        else if (mParametersValues[0][0] && !mParametersValues[1][0])
            brush = QBrush(mElementInterface.data()->extraParameter(3).value<QColor>());
        else if (mParametersValues[0][0] && mParametersValues[1][0])
            brush = QBrush(mElementInterface.data()->extraParameter(4).value<QColor>());
    }
    else
    {
        if (mElementLogic.data()->elementLogicType() == ElementLogicType::DoubleIndicator)
            brush = QBrush(Qt::white);
        else if (mElementLogic.data()->elementLogicType() == ElementLogicType::DoubleIndicatorDark)
            brush = QBrush(Qt::black);
    }

    if (mElementLogic.data()->elementLogicType() == ElementLogicType::DoubleIndicator)
        pen = QPen(Qt::black, LineWidth::outerLine, Qt::SolidLine);
    else if (mElementLogic.data()->elementLogicType() == ElementLogicType::DoubleIndicatorDark)
        pen = QPen(Qt::white, LineWidth::outerLine, Qt::SolidLine);

    drawFigure(painter, rectFigure, pen, brush, font, string);

    if ((brush.color().red() + brush.color().green() + brush.color().blue()) < 96)
        pen.setColor(Qt::white);
    else
        pen.setColor(Qt::black);

    string = mElementInterface.data()->extraParameter(0).toString();
    font = mHeaderFont;
    drawFigure(painter, textFigure, pen, brush, font, string);

    return 0;
}



// Прорисовка створки
int GraphicsObject::drawFlap(QPainter *painter)
{
    // Фигура для перерисовки
    QVector<qint32> points;
    points<<0<<0<<mElementInterface.data()->size().width()<<mElementInterface.data()->size().height();

    Figure rectFigure = mElementLogic.data()->figure(0);
    rectFigure.setFigureParameters(rectFigure.figureType(), points, rectFigure.isStatic(), rectFigure.zIndex());

    // Конкретные объекты для рисования
    // Карандаш
    // Кисть
    QPen pen;
    QBrush brush(Qt::transparent);
    QFont font;
    QString string;

    // Работа с параметрами только в том случае,
    // если данные получены с сервера
    if (mIsDataFromServer)
    {
        // Используем пользовательский цвет
        if (mParametersValues[0][0])
            brush = QBrush(mElementInterface.data()->extraParameter(3).value<QColor>());
        else
            brush = QBrush(mElementInterface.data()->extraParameter(2).value<QColor>());
    }
    else
    {
        // Или цвет по умолчанию, зависящий от цветовой схемы
        if (mElementLogic.data()->elementLogicType() == ElementLogicType::Flap)
            brush = QBrush(Qt::white);
        else if (mElementLogic.data()->elementLogicType() == ElementLogicType::FlapDark)
            brush = QBrush(Qt::black);
    }

    // Цвет контура, зависящий от цветовой схемы
    if (mElementLogic.data()->elementLogicType() == ElementLogicType::Flap)
        pen = QPen(Qt::black, LineWidth::outerLine, Qt::SolidLine);
    else if (mElementLogic.data()->elementLogicType() == ElementLogicType::FlapDark)
        pen = QPen(Qt::white, LineWidth::outerLine, Qt::SolidLine);

    // Выбор точки вращения
    int rotatePoint = mElementInterface.data()->extraParameter(0).toInt();
    int angle = mElementInterface.data()->extraParameter(1).toInt() % 360;
    int translateX = 0;
    int translateY = 0;

    if (mParametersValues[0][0])
    {
        // Точка вращения с левой стороны фигуры
        if (rotatePoint == 1)
        {
            translateX = mOffset.width();
            translateY = mOffset.height() + mElementInterface.data()->size().height() / 2;
        }
        // Точка вращения с правой стороны фигуры
        else if (rotatePoint == 2)
        {
            translateX = mOffset.width() + mElementInterface.data()->size().width();
            translateY = mOffset.height() + mElementInterface.data()->size().height() / 2;
        }


        if (angle != 0)
        {
            // Вращение
            this->setTransform(QTransform()
                               .translate(this->boundingRect().width() / 2,
                                          this->boundingRect().height() / 2)
                               .rotate(mBaseAngle)
                               .translate(-this->boundingRect().width() / 2,
                                          -this->boundingRect().height() / 2)
                               .translate(translateX, translateY)
                               .rotate(angle)
                               .translate(-translateX, -translateY));
        }
    }
    else
    {
        // Обратное вращение
        this->setTransform(QTransform()
                           .translate(this->boundingRect().width() / 2,
                                      this->boundingRect().height() / 2)
                           .rotate(mBaseAngle)
                           .translate(-this->boundingRect().width() / 2,
                                      -this->boundingRect().height() / 2));
    }

    drawFigure(painter, rectFigure, pen, brush, font, string);

    if (mParametersValues[0][0] && angle != 0)
    {
        // Вращение
        this->setTransform(QTransform()
                           .translate(this->boundingRect().width() / 2,
                                      this->boundingRect().height() / 2)
                           .rotate(mBaseAngle)
                           .translate(-this->boundingRect().width() / 2,
                                      -this->boundingRect().height() / 2)
                           .translate(translateX, translateY)
                           .rotate(-angle)
                           .translate(-translateX, -translateY));
    }

    return 0;
}



// Прорисовка шины
int GraphicsObject::drawBus(QPainter *painter)
{
    // Коэффициенты размеров стрелок
    static const double coeffWidth = 1;
    static const double coeffHeight = 0.45;

    // Вычисление координат фигур
    int totalWidth = mElementInterface.data()->size().width();
    int totalHeight = mElementInterface.data()->size().height();

    // Основание стрелки
    Figure rectFigure = mElementLogic.data()->figure(0);
    // Стрелки
    Figure arrow1 = mElementLogic.data()->figure(0);
    Figure arrow2 = mElementLogic.data()->figure(0);
    // Линии контура
    Figure line1 = mElementLogic.data()->figure(0);
    Figure line2 = mElementLogic.data()->figure(0);

    QVector<qint32> points;
    // Координаты основания
    points.clear();
    points<<totalHeight * coeffWidth<<totalHeight * coeffHeight<<
            totalWidth<<totalHeight;
    rectFigure.setFigureParameters(rectFigure.figureType(), points, rectFigure.isStatic(), rectFigure.zIndex());

    // Координаты стрелки №1
    points.clear();
    points<<0<<totalHeight / 2 + totalHeight * coeffHeight<<
            totalHeight * coeffWidth * 1.5<<0<<
            totalHeight * coeffWidth * 1.5<<totalHeight + totalHeight * coeffHeight * 2;
    arrow1.setFigureParameters(FigureType::Polygon, points, rectFigure.isStatic(), rectFigure.zIndex());

    // Координаты стрелки №2
    points.clear();
    points<<totalWidth + totalHeight * coeffWidth * 2<<totalHeight / 2 + totalHeight * coeffHeight<<
            totalWidth + totalHeight * coeffWidth * 2 - totalHeight * coeffWidth * 1.5<<0<<
            totalWidth + totalHeight * coeffWidth * 2 - totalHeight * coeffWidth * 1.5<<totalHeight + totalHeight * coeffHeight * 2;
    arrow2.setFigureParameters(FigureType::Polygon, points, rectFigure.isStatic(), rectFigure.zIndex());

    // Координаты линии №1
    points.clear();
    points<<totalHeight * coeffWidth * 1.5<<totalHeight * coeffHeight<<
            totalWidth + totalHeight * coeffWidth / 2<<totalHeight * coeffHeight;
    line1.setFigureParameters(FigureType::Line, points, rectFigure.isStatic(), rectFigure.zIndex());

    // Координаты линии №2
    points.clear();
    points<<totalHeight * coeffWidth * 1.5<<totalHeight + totalHeight * coeffHeight<<
            totalWidth + totalHeight * coeffWidth / 2<<totalHeight + totalHeight * coeffHeight;
    line2.setFigureParameters(FigureType::Line, points, rectFigure.isStatic(), rectFigure.zIndex());

    // Конкретные объекты для рисования
    // Карандаш
    // Кисть
    QPen pen;
    QPen modifiedPen(Qt::NoPen);
    QBrush brush(Qt::transparent);
    QFont font;
    QString string;

    // Работа с параметрами только в том случае,
    // если данные получены с сервера
    if (mIsDataFromServer)
    {
        // Извлечение цвета из логики
        if (mParametersValues[0][0])
            brush = QBrush(mElementLogic.data()->record(1).GDIObjects()[0].color(0));
        else
            brush = QBrush(mElementLogic.data()->record(0).GDIObjects()[0].color(0));
    }
    else
    {
        // Выбор цвета в зависимости от цветовой схемы
        if (mElementLogic.data()->elementLogicType() == ElementLogicType::Bus)
            brush = QBrush(Qt::white);
        else if (mElementLogic.data()->elementLogicType() == ElementLogicType::BusDark)
            brush = QBrush(Qt::black);
    }

    // Установка цвета контура
    GDIObject object = mElementLogic.data()->defaultRecord(0).GDIObjects()[0];
    pen.setColor(object.color(0));
    pen.setWidth(object.penWidth() * mElementInterface.data()->sizeModificator());
    pen.setStyle(object.penStyle());

    // Прорисовка элементов
    drawFigure(painter, arrow1, pen, brush, font, string);
    drawFigure(painter, arrow2, pen, brush, font, string);
    drawFigure(painter, rectFigure, modifiedPen, brush, font, string);
    drawFigure(painter, line1, pen, brush, font, string);
    drawFigure(painter, line2, pen, brush, font, string);

    return 0;
}




// Функция отвечающая за смену цвета элемента
int GraphicsObject::slotRepaintTimer()
{
    const int totalColorsCount = 2;
    for (int i = 0; i < mCurrentColors.size(); i++)
        mCurrentColors[i] = (mCurrentColors[i] + 1) % totalColorsCount;

    if (mIsRepaintNeeded)
        this->update();

    return 0;
}



// Установка логики для объекта
// Рассчет отступа для прорисовки границ
// Установка размера
int GraphicsObject::setElementLogic(QSharedPointer<ElementLogic> elementLogic)
{
    mElementLogic = elementLogic;

    mCurrentColors = QVector<int>(mElementLogic.data()->figuresCount(), 0);

    mParametersValues = QVector< QVector<bool> >(mElementLogic.data()->parametersCount());
    mParametersString = QVector<QString>(mElementLogic.data()->parametersCount());

    // Установка зоны предупреждения по Х (mPaintOffset - минимум)
    if (elementLogic.data()->warningSize().width() < mPaintOffset)
        mOffset.setWidth(mPaintOffset);
    else
        mOffset.setWidth(elementLogic.data()->warningSize().width());

    // Установка зоны предупреждения по Y (mPaintOffset - минимум)
    if (elementLogic.data()->warningSize().height() < mPaintOffset)
        mOffset.setHeight(mPaintOffset);
    else
        mOffset.setHeight(elementLogic.data()->warningSize().height());

    // Установка размера
    mMaxSize = QSize(mElementLogic.data()->size().width(),
                     mElementLogic.data()->size().height());

    return 0;
}



// Установка интерфейса для объекта
// Создание градиента
int GraphicsObject::setElementInterface(QSharedPointer<ElementInterface> elementInterface)
{
    mElementInterface = elementInterface;

    // Градиент для заливки
    gradient.setCenter(this->boundingRect().width() / 2,
                       this->boundingRect().height() / 2);
    gradient.setFocalPoint(this->boundingRect().width() / 2,
                           this->boundingRect().height() / 2);
    gradient.setRadius(this->boundingRect().width() / 2);
    gradient.setColorAt(0, QColor(255, 127, 39, 160));
    gradient.setColorAt(1, QColor(255, 127, 39, 32));

    if (!mElementInterface.isNull())
        mToolTipText = QString("<nobr><b>Название элемента:</b> %1</nobr><br><nobr><b>Модуль:</b> %2</nobr>")
                .arg(mElementInterface.data()->elementName())
                .arg(mElementInterface.data()->moduleName());

    return 0;
}



// Установка состояния элемента (данные для визуализации)
// Функция выполняет дробление всего набора данных на части, относящиеся
// к каждому параметру
int GraphicsObject::setState(QVector<bool> state, bool isDataFromServer)
{
    if (mElementLogic.isNull())
        return -1;

    if (mElementInterface.data()->parametersCount() == 0)
        return -1;

    // Установка нового состояния
    mState = state;
    mIsDataFromServer = isDataFromServer;

    // Количество зарегистрированных параметров в элементе
    int parametersCount = mElementLogic.data()->parametersCount();
    //    int currentByte = mElementInterface.data()->startByte(0);
    //    int bytesCounter = 0;
    for (int i = 0; i < parametersCount; i++)
    {
        if (!mElementInterface.data()->isParameterUsed(i))
            continue;

        //        if (mElementInterface.data()->startByte(i) != currentByte)
        //            bytesCounter++;

        //        const int bitsInByte = 8;
        //        // Определение границ данных, относящихся к текущему параметру
        //        int startBit = bytesCounter * bitsInByte + mElementInterface.data()->startBit(i);
        //        // Состояние основного параметра
        //        // Копируем из массива набор битов, начиная с mainStartBit
        //        // в количестве parameter.bitsCount()
        //        mParametersValues[i] = mState.mid(startBit,
        //                                          mElementInterface.data()->bitsCount(i));
        //        if (mElementLogic.data()->parameter(i).parameterType() == ParameterType::Common)
        //        {
        //            mParametersString[i].clear();
        //            for (int j = 0; j < mParametersValues[i].size(); j++)
        //                mParametersString[i] += QString("%1").arg(mParametersValues[i][j]);
        //        }
        //        else if (mElementLogic.data()->parameter(i).parameterType() == ParameterType::Number)
        //        {
        //            mParametersString[i] = "0.00";
        //        }












        const int bitsInByte = 8;
        // Определение границ данных, относящихся к текущему параметру
        int startBit = (mElementInterface.data()->startByte(i) -
                        mElementInterface.data()->startByte()) * bitsInByte +
                mElementInterface.data()->startBit(i);
        // Состояние основного параметра
        // Копируем из массива набор битов, начиная с mainStartBit
        // в количестве parameter.bitsCount()
        mParametersValues[i] = mState.mid(startBit,
                                          mElementInterface.data()->bitsCount(i));
        if (mElementLogic.data()->parameter(i).parameterType() == ParameterType::Common)
        {
            mParametersString[i].clear();
            for (int j = 0; j < mParametersValues[i].size(); j++)
                mParametersString[i] += QString("%1").arg(mParametersValues[i][j]);
        }
        else if (mElementLogic.data()->parameter(i).parameterType() == ParameterType::Number)
        {
            mParametersString[i] = "0.00";
        }
    }

    // Перерисовка
    this->update();

    return 0;
}



// Сброс состояния элемента (получение нулевых данных)
int GraphicsObject::resetState()
{
    const int bitsInByte = 8;
    if (!mElementInterface.isNull())
        this->setState(QVector<bool>(mElementInterface.data()->bytesCount() * bitsInByte, false), false);

    return 0;
}



// Прорисовка элемента
// Принцип следующий: перебираем фигуры, параметры, записи, объекты рисования.
// Если для фигуры заданы 2 и более одинаковых объекта рисования - конфликт.
// Если какая-либо фигура не была прорисована во время перебора - существует
// понятие "Прорисовка по умолчанию". Название говорит само за себя.
// Плюс несколько флагов для исключения лишней прорисовки.
// Если цвет какого-нибудь объекта помечен флагом IsBlinked, то устанавливаем
// в истину флаг mIsRepaintNeeded и перерисовываем объект по таймеру.
// Что касается модификаторов поворота, тут ужас. Во первых, для всего элемента
// определен общий угол поворота, так называемый mBaseAngle. Его мы используем
// для поворота всего элемента через rotate() для QGraphicsObject.
// Далее, для фигуры может быть задан флаг вращения, тогда при ее прорисовке
// используем этот флаг, зовется он newAngle.
// Еще один момент, бывают даже статически фигуры, которые вращать нам вроде
// как бы и не нужно, несмотря на mBaseAngle. За это отвечает флаг mIsStatic.
// В случае, если для фигуры установлен mIsStatic, то перед ее прорисовкой,
// поворачиваем фигуру на угол -mBaseAngle.
// В самом конце рисуем конфликтную область, если требуется. Тут ничего сложного.
// Еще один нюанс с прорисовкой фигур. Если встречается RoundedRect, то с ее
// прориской нужно рассчитывать коэффициент закругленных углов, что до сих пор
// корректно не работает - не всегда получается красиво.
// Для параметров может быть установлен тип, причем если тип = IsNumber,
// то вступает в силу хитрая процедура, вместо обычной прорисовки текста
// вызывается функция parseNumber, которая парсит массив битов по непонятному
// алгоритму, и возвращает результат цифрой, которую и нужно нарисовать вместо
// простого текста.
void GraphicsObject::paint(QPainter *painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    // Неиспользуемые параметры
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (mElementLogic.isNull())
        return;

    bool exp1 = mElementInterface.data()->startAngle() >= 0;
    bool exp2 = mElementInterface.data()->startAngle() != mBaseAngle;
    if (exp1 && exp2)
    {
        mBaseAngle = mElementInterface.data()->startAngle();
        this->setTransform(QTransform()
                           .translate(this->boundingRect().width() / 2,
                                      this->boundingRect().height() / 2)
                           .rotate(mBaseAngle)
                           .translate(-this->boundingRect().width() / 2,
                                      -this->boundingRect().height() / 2));
    }

    if (mElementLogic.data()->elementLogicType() == ElementLogicType::Common ||
            mElementLogic.data()->elementLogicType() == ElementLogicType::FuelDensity)
        drawStandartElement(painter);
    else if (mElementLogic.data()->elementLogicType() == ElementLogicType::SingleIndicator ||
             mElementLogic.data()->elementLogicType() == ElementLogicType::SingleIndicatorDark)
        drawSingleIndicator(painter);
    else if (mElementLogic.data()->elementLogicType() == ElementLogicType::DoubleIndicator ||
             mElementLogic.data()->elementLogicType() == ElementLogicType::DoubleIndicatorDark)
        drawDoubleIndicator(painter);
    else if (mElementLogic.data()->elementLogicType() == ElementLogicType::Flap ||
             mElementLogic.data()->elementLogicType() == ElementLogicType::FlapDark)
        drawFlap(painter);
    else if (mElementLogic.data()->elementLogicType() == ElementLogicType::Bus ||
             mElementLogic.data()->elementLogicType() == ElementLogicType::BusDark)
        drawBus(painter);

    if (mIsToolTipShow)
        emit signalSendDataToToolTip(mParametersString, mIsConflict);

    return;
}



// Размеры элемента
QRectF GraphicsObject::boundingRect() const
{
    bool isCommon = mElementInterface.data()->elementLogicType() == ElementLogicType::Common;
    bool isFuelDensity = mElementInterface.data()->elementLogicType() == ElementLogicType::FuelDensity;
    bool isSingleIndicator = mElementInterface.data()->elementLogicType() == ElementLogicType::SingleIndicator;
    bool isDoubleIndicator = mElementInterface.data()->elementLogicType() == ElementLogicType::DoubleIndicator;
    bool isSingleIndicatorDark = mElementInterface.data()->elementLogicType() == ElementLogicType::SingleIndicatorDark;
    bool isDoubleIndicatorDark = mElementInterface.data()->elementLogicType() == ElementLogicType::DoubleIndicatorDark;
    bool isFlap = mElementInterface.data()->elementLogicType() == ElementLogicType::Flap;
    bool isFlapDark = mElementInterface.data()->elementLogicType() == ElementLogicType::FlapDark;
    bool isBus = mElementInterface.data()->elementLogicType() == ElementLogicType::Bus;
    bool isBusDark = mElementInterface.data()->elementLogicType() == ElementLogicType::BusDark;

    if (isCommon || isFuelDensity)
    {
        return QRectF(0,
                      0,
                      (mMaxSize.width()) * mElementInterface.data()->sizeModificator() + mOffset.width() * 2,
                      (mMaxSize.height()) * mElementInterface.data()->sizeModificator() + mOffset.height() * 2);
    }
    else if (isSingleIndicator || isDoubleIndicator ||
             isSingleIndicatorDark || isDoubleIndicatorDark ||
             isFlap || isFlapDark)
    {
        return QRectF(0,
                      0,
                      mElementInterface.data()->size().width() + mOffset.width() * 2,
                      mElementInterface.data()->size().height() + mOffset.width() * 2);
    }
    else if (isBus || isBusDark)
    {
        // Коэффициенты размеров стрелок (не входят в BoundingRect)
        static const double coeffWidth = 1;
        static const double coeffHeight = 0.45;
        return QRectF(0,
                      0,
                      mElementInterface.data()->size().width() + mElementInterface.data()->size().height() * (1 + coeffWidth * 2) + mOffset.width() * 2,
                      mElementInterface.data()->size().height() * (1 + coeffHeight * 2) + mOffset.width() * 2);
    }
    else
    {
        return QRectF();
    }
}



// Установка флага
void GraphicsObject::setFlag(QGraphicsItem::GraphicsItemFlag flag, bool enabled)
{
    QGraphicsObject::setFlag(flag, enabled);

    if ((this->flags() & QGraphicsObject::ItemIsMovable) > 0)
        mIsConstructorShowed = true;
    else
        mIsConstructorShowed = false;

    if (mIsConstructorShowed)
        this->setToolTip(mToolTipText);
    else
        this->setToolTip(QString());

    return;
}



// Установка флагов
void GraphicsObject::setFlags(QGraphicsItem::GraphicsItemFlags flags)
{
    QGraphicsObject::setFlags(flags);

    if ((this->flags() & QGraphicsObject::ItemIsMovable) > 0)
        mIsConstructorShowed = true;
    else
        mIsConstructorShowed = false;

    if (mIsConstructorShowed)
        this->setToolTip(mToolTipText);
    else
        this->setToolTip(QString());

    return;
}



// Обработка событий мыши, вызов всплывающей подсказки
void GraphicsObject::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    // Проверка, что событие вызвано левой кнопкой мыши
    if (event->button() != Qt::LeftButton)
        return QGraphicsObject::mouseReleaseEvent(event);

    // Проверка корректности положения курсора
    bool exp1 = event->pos().x() < 0 || event->pos().x() > this->boundingRect().width();
    bool exp2 = event->pos().y() < 0 || event->pos().y() > this->boundingRect().height();
    if (exp1 || exp2)
        return QGraphicsObject::mouseReleaseEvent(event);

    // Проверка, что не включен режим конструктора
    if ((this->flags() & QGraphicsObject::ItemIsMovable) > 0)
    {
        emit signalGraphicsObjectMoved();
        emit signalGraphicsObjectSelect();
        return QGraphicsObject::mouseReleaseEvent(event);
    }

    emit signalSendDataToToolTip(mParametersString, mIsConflict);

    mIsToolTipShow = true;

    return;
}



// Парсинг двухбайтового числа, записанного в доп. коде.
// 1 байт. 1 бит - знак, 7 бит - значение числа
// 2 байт. 7 бит - значение числа, 1 бит не используется
// 1 0000000 0000000 {1}
qreal GraphicsObject::parseNumber(QVector<bool> numberBits, qreal coeff, bool isSigned)
{
    qint32 number = 0;
    qint32 maxNumber = 0;

    // Защита от дурака
    if (numberBits.size() == 0)
        return number;

    if (isSigned)
    {
        // Преобразование (1 крайний бит - знаковый)
        bool signBit = numberBits[numberBits.size() - 1];
        for (int i = 0; i < numberBits.size() - 1; i++)
        {
            // Считаем единички, если число положительное
            if (numberBits[i] && !signBit)
                number += (1 << i);
            // Считаем нолики, если число отрицательное (после инверсии - единички)
            else if (!numberBits[i] && signBit)
                number += (1 << i);
        }

        // Обработка знакового бита
        if (signBit)
        {
            number++;
            number = - number;
        }

        maxNumber = 1 << (numberBits.size() - 2);
    }
    else
    {
        // Преобразование
        for (int i = 0; i < numberBits.size(); i++)
        {
            // Считаем единички
            if (numberBits[i])
                number += (1 << i);
        }

        maxNumber = 1;
    }

    return coeff / maxNumber * number;
}



// Функция расчета плотности топлива [ГОСТ 3900-85 (СЭВ 6754-89)]
qreal GraphicsObject::calculateFuelDensity(qreal baseDensity, qreal temperature)
{
    // Данные таблицы
    const qreal arrayKey[] = {0.7, 0.71, 0.72, 0.73, 0.74, 0.75, 0.76, 0.77,
                              0.78, 0.79, 0.80, 0.81, 0.82, 0.83, 0.84, 0.85};
    const qreal arrayValue[] = {0.000844, 0.000829, 0.000813, 0.000798, 0.000783,
                                0.000769, 0.000756, 0.000743, 0.000731, 0.000718,
                                0.000704, 0.000693, 0.000681, 0.000672, 0.000664,
                                0.000658};
    // Размерность
    const int arraySize = 16;

    // Выполнялся ли расчет temperatureFix
    static bool isCalculate = false;
    // Старое значение плотности (на случай изменения параметров элемента)
    static qreal oldBaseDensity = 0;
    // Температурная поправка
    static qreal temperatureFix = 0;

    // Выполнялся ли расчет температурной поправки со значением плотности baseDensity
    if (qAbs(oldBaseDensity - baseDensity) > 1e-6)
        isCalculate = false;

    if (!isCalculate)
    {
        isCalculate = true;
        oldBaseDensity = baseDensity;

        int key1 = -1;
        int key2 = -1;
        qreal roundedBaseDensity = qRound(baseDensity * 100.0) / 100.0;

        // Поиск границ, в которых лежит базовая плотность (первая граница)
        for (int i = 0; i < arraySize; i++)
        {
            if (qAbs(arrayKey[i] - roundedBaseDensity) < 1e-6)
            {
                key1 = i;
                break;
            }
        }

        // Вторая граница
        if (roundedBaseDensity < baseDensity)
            key2 = key1 + 1;
        else
            key2 = key1 - 1;

        // Если осуществлен выход за пределы заданных данных - возвращаем нуль
        if (key1 < 0 || key1 >= arraySize)
            return 0;
        if (key2 < 0 || key2 >= arraySize)
            return 0;

        // Расчет шага для температурной поправки
        qreal step = arrayValue[key1] - arrayValue[key2];
        // Множитель 100 - магическое число
        temperatureFix = arrayValue[key1] + step * (roundedBaseDensity - baseDensity) * 100;
    }

    const int baseTemperature = 20;
    qreal realDensity = baseDensity + temperatureFix * (baseTemperature - temperature);

    return realDensity;
}



// Уведомления об отображении/скрытии всплывающей подсказки
int GraphicsObject::slotToolTipHide()
{
    mIsToolTipShow = false;
    return 0;
}
