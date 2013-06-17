 #include "interfaceloader.h"



InterfaceLoader::InterfaceLoader(int index, QObject *parent) :
    QObject(parent),
    mSlash("/"),
    mSettingsFolder(QString()),
    mFilename(QString("elements%1.xml").arg(index))
{
    mElementsIterfaces.clear();
}



// Загрузка интерфейсных модулей
bool InterfaceLoader::load()
{
    bool result = false;

    // Проверка на существование файла
    QFile file(mSettingsFolder + mSlash + mFilename);
    result = file.exists();
    if (!result)
        return false;

    // Если нет прав на чтение
    result = file.open(QIODevice::ReadOnly);
    if (!result)
        return false;

    QDomDocument mainXML;
    QDomElement rootElement;
    QDomElement element;
    QDomElement subElement;

    // Если файл - не xml
    result = mainXML.setContent(&file);
    if (!result)
        return false;

    // Индикаторы успешности парсинга
    bool readResult = true;
    bool convertResult = true;

    qint32 elementsCount = 0;
    QString contentString;
    qreal contentReal = 0.0;
    qint32 contentInt = 0;

    // Параметры элементов
    rootElement = mainXML.documentElement().firstChildElement("elements");
    if (rootElement.isNull())
        return false;

    // Количество элементов
    elementsCount = rootElement.attribute("count").toInt(&convertResult);

    // Перебор элементов
    element = rootElement.firstChildElement("element");
    while(!element.isNull())
    {
        QSharedPointer<ElementInterface> elementInterface(new ElementInterface());

        // Имя модуля
        contentString = element.attribute("moduleName");
        if (contentString.isEmpty())
            readResult = false;
        else
            elementInterface.data()->setModuleName(contentString);

        // Хеш модуля
        contentString = element.attribute("moduleHash");
        elementInterface.data()->setModuleHash(contentString);

        // Название элемента
        contentString = element.attribute("elementName");
        elementInterface.data()->setElementName(contentString);

        // Размер элемента
        contentReal = element.attribute("sizeModificator").toDouble(&convertResult);
        if (!convertResult)
            readResult = false;
        else
            elementInterface.data()->setSizeModificator(contentReal);

        // Ширина
        contentInt = element.attribute("width").toInt(&convertResult);
        if (!convertResult)
            readResult = false;

        // Высота
        contentReal = element.attribute("height").toInt(&convertResult);
        if (!convertResult)
            readResult = false;

        elementInterface.data()->setSize(QSize(contentInt, contentReal));

        // Угол поворота элемента
        contentInt = element.attribute("angle").toInt(&convertResult);
        if (!convertResult)
            readResult = false;
        else
            elementInterface.data()->setStartAngle(contentInt);

        // X-координата элемента
        contentInt = element.attribute("xPosition").toInt(&convertResult);
        if (!convertResult)
            readResult = false;
        else
            elementInterface.data()->setX(contentInt);

        // Y-координата элемента
        contentInt = element.attribute("yPosition").toInt(&convertResult);
        if (!convertResult)
            readResult = false;
        else
            elementInterface.data()->setY(contentInt);

        qint32 startByte = 0;
        qint32 startBit = 0;
        qint32 bitsCount = 0;
        bool isUsed = true;

        static const int minUInt16 = 0;
        static const int maxUInt16 = 65535;

        // Параметры элементов
        subElement = element.firstChildElement("parameter");
        while(!subElement.isNull())
        {
            // Стартовый байт
            contentInt = subElement.attribute("startByte").toInt(&convertResult);
            if (!convertResult || contentInt < minUInt16 || contentInt > maxUInt16)
                readResult = false;
            else
                startByte = contentInt;

            // Стартовый бит в байте
            contentInt = subElement.attribute("startBit").toInt(&convertResult);
            if (!convertResult || contentInt < minUInt16 || contentInt > maxUInt16)
                readResult = false;
            else
                startBit = contentInt;

            // Количество бит (для параметров, где допустимо изменение)
            contentInt = subElement.attribute("bitsCount").toInt(&convertResult);
            if (!convertResult || contentInt < minUInt16 || contentInt > maxUInt16)
                readResult = false;
            else
                bitsCount = contentInt;

            // Используется ли параметр?
            contentInt = subElement.attribute("isUsed").toInt(&convertResult);
            if (!convertResult)
                readResult = false;
            else
                isUsed = static_cast<bool>(contentInt);

            // Описание параметра (может быть пустым, запрашивается из БД)
            contentString = subElement.attribute("description");

            // Добавление параметра к интерфейсному модулю
            elementInterface.data()->addParameter(startByte,
                                                  startBit,
                                                  bitsCount,
                                                  contentString,
                                                  isUsed);

            subElement = subElement.nextSiblingElement("parameter");
        }

        QString extraParameterValue;
        // Дополнительные параметры элементов
        subElement = element.firstChildElement("extraParameter");
        while(!subElement.isNull())
        {
            // Описание параметра (может быть пустым, запрашивается из БД)
            extraParameterValue = subElement.attribute("value");

            // Добавление дополнительного параметра к интерфейсному модулю
            elementInterface.data()->addExtraParameter(QVariant(extraParameterValue));

            subElement = subElement.nextSiblingElement("extraParameter");
        }

        if (readResult)
            mElementsIterfaces.append(elementInterface);
        else
            break;

        element = element.nextSiblingElement("element");
    }

    // Проверка, равно ли реальное количество элементов заявленному
    if (mElementsIterfaces.size() != elementsCount)
        readResult = false;

    return readResult;
}



// Сохранение массива интерфейсных модулей в файл
bool InterfaceLoader::save(QVector<QSharedPointer<ElementInterface> > elementsIterfaces)
{
    mElementsIterfaces = elementsIterfaces;

    QDomDocument mainXML;
    QDomElement rootElement;
    QDomElement elements;
    QDomElement element;
    QDomElement parameter;
    QDomText elementText;

    bool writeResult = true;

    rootElement = mainXML.createElement("constructor");
    rootElement = mainXML.appendChild(rootElement).toElement();
    if (rootElement.isNull())
        return false;

    elements = mainXML.createElement("elements");
    elements.setAttribute("count", mElementsIterfaces.size());

    for (int i = 0; i < mElementsIterfaces.size(); i++)
    {
        element = mainXML.createElement("element");
        element.setAttribute("elementName",
                             mElementsIterfaces[i].data()->elementName());
        element.setAttribute("moduleName",
                             mElementsIterfaces[i].data()->moduleName());
        element.setAttribute("moduleHash",
                             mElementsIterfaces[i].data()->moduleHash());
        element.setAttribute("sizeModificator",
                             mElementsIterfaces[i].data()->sizeModificator());
        element.setAttribute("width",
                             mElementsIterfaces[i].data()->size().width());
        element.setAttribute("height",
                             mElementsIterfaces[i].data()->size().height());
        element.setAttribute("angle",
                             mElementsIterfaces[i].data()->startAngle());
        element.setAttribute("xPosition",
                             mElementsIterfaces[i].data()->x());
        element.setAttribute("yPosition",
                             mElementsIterfaces[i].data()->y());

        for (int j = 0; j < mElementsIterfaces[i].data()->parametersCount(); j++)
        {
            parameter = mainXML.createElement("parameter");
            parameter.setAttribute("startByte",
                                   mElementsIterfaces[i].data()->startByte(j));
            parameter.setAttribute("startBit",
                                   mElementsIterfaces[i].data()->startBit(j));
            parameter.setAttribute("bitsCount",
                                   mElementsIterfaces[i].data()->bitsCount(j));
            parameter.setAttribute("description",
                                   mElementsIterfaces[i].data()->description(j));
            parameter.setAttribute("isUsed",
                                   mElementsIterfaces[i].data()->isParameterUsed(j));

            parameter = element.appendChild(parameter).toElement();
            if (parameter.isNull())
                writeResult = false;
        }

        for (int j = 0; j < mElementsIterfaces[i].data()->extraParametersCount(); j++)
        {
            parameter = mainXML.createElement("extraParameter");
            parameter.setAttribute("value", mElementsIterfaces[i].data()->extraParameter(j).toString());

            parameter = element.appendChild(parameter).toElement();
            if (parameter.isNull())
                writeResult = false;
        }

        element = elements.appendChild(element).toElement();
        if (element.isNull())
            writeResult = false;
    }

    elements = rootElement.appendChild(elements).toElement();
    if (elements.isNull())
        writeResult = false;

    // Файл для записи
    QFile file(QString(mSettingsFolder + '/' + mFilename));
    file.open(QIODevice::WriteOnly);

    // XML-документ создан без ошибок
    if (writeResult)
    {
        QTextStream xmlTS(&file);
        xmlTS.setCodec(QTextCodec::codecForName("UTF-8"));
        xmlTS<<mainXML.toString();
    }

    return writeResult;
}



bool InterfaceLoader::remove()
{
    return QFile::remove(mSettingsFolder + "/" + mFilename);
}



// Получение массива интерфейсных модулей
QVector<QSharedPointer<ElementInterface> > InterfaceLoader::elements()
{
    return mElementsIterfaces;
}



int InterfaceLoader::changeIndex(int index)
{
    this->remove();
    mFilename = QString("elements%1.xml").arg(index);
    return 0;
}



int InterfaceLoader::setProjectDirectory(const QString projectDirectory)
{
    if (projectDirectory.isEmpty())
        return -1;

    // Установка целевой папки
    const QString settingsFolder("/settings");
    mSettingsFolder = projectDirectory + settingsFolder;

    // Проверка наличия папок, необходимых для работы программы
    if (!QDir(mSettingsFolder).exists())
        QDir().mkdir(mSettingsFolder);

    return 0;
}
