#include "playersettingsloader.h"



// Установка рекомендуемых размеров
PlayerSettingsLoader::PlayerSettingsLoader(int programID, QObject *parent) :
    QObject(parent),
    mSettingsFolder(qApp->applicationDirPath()),
    mFilename("config.xml"),
    mLastOpenedProject(""),
    mTabNumber(-1),
    mMainWindowSize(QSize(480, 420)),
    mMainWindowPosition(QPoint(0, 0)),
    mIsMaximized(false),
    mIsPanelShowed(true),
    mProgramID(programID)
{
    // Центрирование виджета
    QDesktopWidget desktop;
    QRect rect = desktop.availableGeometry(desktop.primaryScreen());
    mMainWindowPosition = rect.center();
    mMainWindowPosition.setX(mMainWindowPosition.x() - (mMainWindowSize.width() / 2));
    mMainWindowPosition.setY(mMainWindowPosition.y() - (mMainWindowSize.height() / 2));
}



// Загрузка значений настроек из файла
// Если параметр не удастся загрузить - будет использоваться значение по умолчанию
bool PlayerSettingsLoader::load()
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

    // Настройки программы
    rootElement = mainXML.firstChildElement("settings");
    if (rootElement.isNull())
        return false;

    // Поиск необходимой записи
    readFail = true;
    element = rootElement.firstChildElement("program");
    while (!element.isNull())
    {
        contentInt = element.attribute("number").toInt(&result);
        if (result && contentInt == mProgramID)
            readFail = false;
        else
            element = element.nextSiblingElement("program");

        if (!readFail)
            break;
    }

    // Если не найдено подходящей записи - загружаем настройки по умолчанию
    if (readFail)
        return false;

    rootElement = element;

    // Последний открытый проект
    element = rootElement.firstChildElement("lastOpenedProject");
    mLastOpenedProject = element.attribute("path");
    // Номер открытой вкладки
    contentInt = element.attribute("tabNumber").toInt(&result);
    if (!result)
        readFail = false;
    else
        mTabNumber = contentInt;

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
    // Позиция X
    contentInt = element.attribute("x").toInt(&result);
    if (!result)
        readFail = false;
    else
        mMainWindowPosition.setX(contentInt);
    // Позиция Y
    contentInt = element.attribute("y").toInt(&result);
    if (!result)
        readFail = false;
    else
        mMainWindowPosition.setY(contentInt);

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
        mIsPanelShowed = contentInt;

    return readFail;
}



// Сохранение значений настроек в файл
bool PlayerSettingsLoader::save()
{
    bool result = false;
    bool writeFail = false;
    // Индикатор успешности парсинга
    bool readFail = false;
    // Используемые переменные
    int recordsCount = 0;
    int recordID = 0;

    QDomDocument mainXML;
    QDomElement rootElement;
    QDomElement element;
    QDomElement subElement;
    QDomElement tempElement;

    QFile file(mSettingsFolder + "/" + mFilename);

    if (file.exists() && file.open(QIODevice::ReadWrite))
    {
        // Если файл - не xml
        result = mainXML.setContent(&file);
        if (!result)
            mainXML.clear();
        file.close();
    }

    // Настройки программы
    rootElement = mainXML.firstChildElement("settings");
    if (rootElement.isNull())
        readFail = true;

    // Получение данных о количестве записей в конфигурационном файле
    recordsCount = rootElement.attribute("programsCount").toInt(&result);
    if (!result)
        readFail = true;

    // Файл config.xml не существует
    if (readFail)
    {
        recordsCount = 0;
        // Создаем новую структуру XML
        rootElement = mainXML.createElement("settings");
        rootElement.setAttribute("programsCount", recordsCount + 1);
        rootElement = mainXML.appendChild(rootElement).toElement();
        if (rootElement.isNull())
            return false;
    }
    // Файл config.xml существует
    else
    {
        readFail = true;
        element = rootElement.firstChildElement("program");
        while (!element.isNull())
        {
            recordID = element.attribute("number").toInt(&result);
            if (result && recordID == mProgramID)
                readFail = false;
            else
                element = element.nextSiblingElement("program");

            if (!readFail)
                break;
        }

        // Необходимая запись найдена
        if (!readFail)
            rootElement.removeChild(element);
        else
            rootElement.setAttribute("programsCount", recordsCount + 1);
    }

    // Главный элемент
    element = mainXML.createElement("program");
    element.setAttribute("number", mProgramID);
    // Адрес сервера
    subElement = mainXML.createElement("lastOpenedProject");
    subElement.setAttribute("path", mLastOpenedProject);
    subElement.setAttribute("tabNumber", mTabNumber);
    tempElement = element.appendChild(subElement).toElement();
    if (tempElement.isNull())
        writeFail = true;

    subElement = mainXML.createElement("mainWindow");
    subElement.setAttribute("width", mMainWindowSize.width());
    subElement.setAttribute("height", mMainWindowSize.height());
    subElement.setAttribute("x", mMainWindowPosition.x());
    subElement.setAttribute("y", mMainWindowPosition.y());
    subElement.setAttribute("isMaximized", mIsMaximized);
    subElement.setAttribute("isPanelShow", mIsPanelShowed);
    tempElement = element.appendChild(subElement).toElement();
    if (tempElement.isNull())
        writeFail = true;

    tempElement = rootElement.appendChild(element).toElement();
    if (tempElement.isNull())
        writeFail = true;

    // XML-документ создан без ошибок
    if (!writeFail && file.open(QIODevice::WriteOnly))
    {
        QTextStream xmlTS(&file);
        xmlTS.setCodec(QTextCodec::codecForName("UTF-8"));
        xmlTS<<mainXML.toString();
        file.close();
    }

    return !writeFail;
}



bool PlayerSettingsLoader::clear()
{
    return  QFile(mSettingsFolder + "/" + mFilename).remove();;
}



int PlayerSettingsLoader::programsCount()
{
    // Индикатор успешности парсинга
    bool readFail = false;
    bool result = false;

    QFile file(mSettingsFolder + "/" + mFilename);

    // Если файл не существует
    result = file.exists();
    if (!result)
        readFail = true;

    // Если нет прав на чтение
    result = file.open(QIODevice::ReadOnly);
    if (!result)
        readFail = true;

    QDomDocument mainXML;
    QDomElement rootElement;

    // Если файл - не xml
    result = mainXML.setContent(&file);
    if (!result)
        readFail = true;

    // Используемые переменные
    qint32 programsCount = 0;
    // Настройки программы
    rootElement = mainXML.firstChildElement("settings");
    if (rootElement.isNull())
        readFail = true;
    // Получение данных о количестве записей в конфигурационном файле
    programsCount = rootElement.attribute("programsCount").toInt(&result);
    if (!result)
        readFail = true;

    if (!readFail)
        return programsCount;
    else
        return 0;
}



QString PlayerSettingsLoader::lastOpenedProject()
{
    // Получение абсолютного пути до проекта
    QDir currentDir(qApp->applicationDirPath());
    return currentDir.absoluteFilePath(mLastOpenedProject);
}



int PlayerSettingsLoader::tabNumber()
{
    return mTabNumber;
}



QSize PlayerSettingsLoader::mainWindowSize()
{
    return mMainWindowSize;
}



QPoint PlayerSettingsLoader::mainWindowPosition()
{
    return mMainWindowPosition;
}



bool PlayerSettingsLoader::isMaximized()
{
    return mIsMaximized;
}



bool PlayerSettingsLoader::isPanelShow()
{
    return mIsPanelShowed;
}



int PlayerSettingsLoader::setLastOpenedProject(QString lastOpenedProject)
{
    // Получение относительного пути до проекта
    QDir currentDir(qApp->applicationDirPath());
    mLastOpenedProject = currentDir.relativeFilePath(lastOpenedProject);

    return 0;
}



int PlayerSettingsLoader::setMainWindowParameters(QSize mainWindowSize, QPoint mainWindowPosition,
                                                   bool isMaximized, bool isPanelShowed)
{
    mMainWindowSize = mainWindowSize;
    mMainWindowPosition = mainWindowPosition;
    mIsMaximized = isMaximized;
    mIsPanelShowed = isPanelShowed;

    return 0;
}



int PlayerSettingsLoader::setTabNumber(int tabNumber)
{
    mTabNumber = tabNumber;

    return 0;
}
