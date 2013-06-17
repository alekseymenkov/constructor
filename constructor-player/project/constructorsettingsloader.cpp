#include "constructorsettingsloader.h"



// Установка рекомендуемых размеров
ContructorSettingsLoader::ContructorSettingsLoader(QObject *parent) :
    QObject(parent),
    mSettingsFolder(qApp->applicationDirPath()),
    mFilename("config.xml"),
    mLastOpenedProject(""),
    mMainWindowSize(QSize(978, 442)),
    mElementEditDialogSize(QSize(750, 400)),
    mConstructorSize(QSize(493, 420)),
    mIsMaximized(false),
    mIsConstructorShow(true),
    mIsPanelShow(true)
{
}



// Загрузка значений настроек из файла
// Если параметр не удастся загрузить - будет использоваться значение по умолчанию
bool ContructorSettingsLoader::load()
{
    bool result = false;

    QFile file(mSettingsFolder + "/" + mFilename);

    // Если файл не существует
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

    // Если файл - не xml
    result = mainXML.setContent(&file);
    if (!result)
        return false;

    // Индикатор успешности парсинга
    bool readFail = false;

    // Используемые переменные
    qint32 contentInt = 0;
    QString contentString;

    // Настройки программы
    rootElement = mainXML.documentElement().firstChildElement("programSettings");
    if (rootElement.isNull())
        return false;

    // Количество закладок
    contentString = rootElement.firstChildElement("lastOpenedProject").text();
    if (!result)
        readFail = true;
    else
        mLastOpenedProject = contentString;

    // Размеры окон
    rootElement = mainXML.documentElement().firstChildElement("sizes");
    if (rootElement.isNull())
        return false;

    // Главное окно
    element = rootElement.firstChildElement("mainWindow");
    // Ширина
    contentInt = element.attribute("width").toInt(&result);
    if (!result)
        readFail = false;
    else
        mMainWindowSize.setWidth(contentInt);
    // Высота
    contentInt = element.attribute("height").toInt(&result);
    if (!result)
        readFail = false;
    else
        mMainWindowSize.setHeight(contentInt);
    // Приложение в полноэкранном режиме
    contentInt = element.attribute("isMaximized").toInt(&result);
    if (!result)
        readFail = true;
    else
        mIsMaximized = contentInt;
    // Панель инструментов отображена?
    contentInt = element.attribute("isPanelShow").toInt(&result);
    if (!result)
        readFail = true;
    else
        mIsPanelShow = contentInt;

    // Окно конструктора
    element = rootElement.firstChildElement("constructorWindow");
    // Ширина
    contentInt = element.attribute("width").toInt(&result);
    if (!result)
        readFail = false;
    else
        mConstructorSize.setWidth(contentInt);

    // Высота
    contentInt = element.attribute("height").toInt(&result);
    if (!result)
        readFail = false;
    else
        mConstructorSize.setHeight(contentInt);

    // Отображена ли панель конструктора?
    contentInt = element.attribute("isConstructorShow").toInt(&result);
    if (!result)
        readFail = true;
    else
        mIsConstructorShow = contentInt;

    // Окно редактирования элемента
    element = rootElement.firstChildElement("elementEditDialog");
    // Ширина
    contentInt = element.attribute("width").toInt(&result);
    if (!result)
        readFail = false;
    else
        mElementEditDialogSize.setWidth(contentInt);

    // Высота
    contentInt = element.attribute("height").toInt(&result);
    if (!result)
        readFail = false;
    else
        mElementEditDialogSize.setHeight(contentInt);

    return readFail;
}



// Сохранение значений настроек в файл
bool ContructorSettingsLoader::save()
{
    QDomDocument mainXML;
    QDomElement rootElement;
    QDomElement element;
    QDomElement subElement;
    QDomElement tempElement;
    QDomText elementText;

    bool writeFail = false;

    rootElement = mainXML.createElement("settings");
    rootElement = mainXML.appendChild(rootElement).toElement();
    if (rootElement.isNull())
        return false;

    // Настройки сети
    element = mainXML.createElement("programSettings");
    rootElement.appendChild(element);

    // Адрес сервера
    subElement = mainXML.createElement("lastOpenedProject");
    elementText = mainXML.createTextNode(mLastOpenedProject);
    subElement.appendChild(elementText);
    tempElement = element.appendChild(subElement).toElement();
    if (tempElement.isNull())
        writeFail = true;

    element = mainXML.createElement("sizes");
    rootElement.appendChild(element);

    subElement = mainXML.createElement("mainWindow");
    tempElement = element.appendChild(subElement).toElement();
    if (tempElement.isNull())
        writeFail = true;
    subElement.setAttribute("width", mMainWindowSize.width());
    subElement.setAttribute("height", mMainWindowSize.height());
    subElement.setAttribute("isMaximized", mIsMaximized);
    subElement.setAttribute("isPanelShow", mIsPanelShow);

    subElement = mainXML.createElement("constructorWindow");
    tempElement = element.appendChild(subElement).toElement();
    if (tempElement.isNull())
        writeFail = true;
    subElement.setAttribute("width", mConstructorSize.width());
    subElement.setAttribute("height", mConstructorSize.height());
    subElement.setAttribute("isConstructorShow", mIsConstructorShow);

    subElement = mainXML.createElement("elementEditDialog");
    tempElement = element.appendChild(subElement).toElement();
    if (tempElement.isNull())
        writeFail = true;
    subElement.setAttribute("width", mElementEditDialogSize.width());
    subElement.setAttribute("height", mElementEditDialogSize.height());

    // Файл для записи
    QFile file(QString(mSettingsFolder + '/' + mFilename));
    file.open(QIODevice::WriteOnly);

    // XML-документ создан без ошибок
    if (!writeFail)
    {
        QTextStream xmlTS(&file);
        xmlTS.setCodec(QTextCodec::codecForName("UTF-8"));
        xmlTS<<mainXML.toString();
    }

    return writeFail;
}



QString ContructorSettingsLoader::lastOpenedProject()
{
    // Получение абсолютного пути до проекта
    QDir currentDir(qApp->applicationDirPath());
    return currentDir.absoluteFilePath(mLastOpenedProject);
}



int ContructorSettingsLoader::setElementEditDialogSize(QSize elementEditDialogSize)
{
    mElementEditDialogSize = elementEditDialogSize;
    return 0;
}



QSize ContructorSettingsLoader::mainWindowSize()
{
    return mMainWindowSize;
}



QSize ContructorSettingsLoader::elementEditDialogSize()
{
    return mElementEditDialogSize;
}



QSize ContructorSettingsLoader::constructorSize()
{
    return mConstructorSize;
}



bool ContructorSettingsLoader::isMaximized()
{
    return mIsMaximized;
}



bool ContructorSettingsLoader::isConstructorShow()
{
    return mIsConstructorShow;
}



bool ContructorSettingsLoader::isPanelShow()
{
    return mIsPanelShow;
}



int ContructorSettingsLoader::setLastOpenedProject(QString lastOpenedProject)
{
    // Получение относительного пути до проекта
    QDir currentDir(qApp->applicationDirPath());
    mLastOpenedProject = currentDir.relativeFilePath(lastOpenedProject);

    return 0;
}



int ContructorSettingsLoader::setMainWindowParameters(QSize mainWindowSize, bool isMaximized)
{
    mMainWindowSize = mainWindowSize;
    mIsMaximized = isMaximized;
    return 0;
}



int ContructorSettingsLoader::setPanelState(bool isPanelShow)
{
    mIsPanelShow = isPanelShow;
    return 0;
}



int ContructorSettingsLoader::setConstructorParameters(QSize constructorSize, bool isConstructorShow)
{
    mConstructorSize = constructorSize;
    mIsConstructorShow = isConstructorShow;
    return 0;
}
