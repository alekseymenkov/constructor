#include "projectsettingsloader.h"



// Установка рекомендуемых размеров
ProjectSettingsLoader::ProjectSettingsLoader(QObject *parent) :
    QObject(parent),
    mSlash(QString("/")),
    mSettingsFolder(QString()),
    mFileName(QString("project.xml"))
{
}



// Загрузка значений настроек из файла
// Если параметр не удастся загрузить - будет использоваться значение по умолчанию
bool ProjectSettingsLoader::load(const QString settingsFolder)
{
    bool result = false;

    mSettingsFolder = settingsFolder;
    QFile projectFile(mSettingsFolder + mSlash + mFileName);

    // Если файл не существует
    result = projectFile.exists();
    if (!result)
        return false;

    // Если нет прав на чтение
    result = projectFile.open(QIODevice::ReadOnly);
    if (!result)
        return false;

    QDomDocument mainXML;
    QDomElement rootElement;

    // Если файл - не xml
    result = mainXML.setContent(&projectFile);
    if (!result)
        return false;

    // Индикатор успешности парсинга
    bool readFail = false;

    // Используемые переменные
    qint32 contentInt = 0;
    QString contentString;

    // Настройки проекта
    rootElement = mainXML.documentElement().firstChildElement("projectSettings");
    if (rootElement.isNull())
        return false;

    // Количество закладок
    contentInt = rootElement.firstChildElement("unitsCount").text().toInt();
    if (!result)
        readFail = true;
    else
        mUnitsCount = contentInt;

    if (mUnitsCount < 1)
        readFail = true;

    // Название проекта
    contentString = rootElement.firstChildElement("projectName").text();
    if (!result)
        readFail = true;
    else
        mProjectName = contentString;

    return !readFail;
}



// Сохранение значений настроек в файл
bool ProjectSettingsLoader::save(const QString settingsFolder)
{
    QDomDocument mainXML;
    QDomElement rootElement;
    QDomElement element;
    QDomElement subElement;

    bool writeFail = false;

    rootElement = mainXML.createElement("settings");
    rootElement = mainXML.appendChild(rootElement).toElement();
    if (rootElement.isNull())
        return false;

    // Настройки проекта
    element = mainXML.createElement("projectSettings");
    rootElement.appendChild(element);

    // Количество закладок
    subElement = mainXML.createElement("unitsCount");
    subElement.appendChild(mainXML.createTextNode(QString::number(mUnitsCount)));
    subElement = element.appendChild(subElement).toElement();
    if (subElement.isNull())
        writeFail = true;

    // Название проекта
    subElement = mainXML.createElement("projectName");
    subElement.appendChild(mainXML.createTextNode(mProjectName));
    subElement = element.appendChild(subElement).toElement();
    if (subElement.isNull())
        writeFail = true;

    // Файл для записи
    mSettingsFolder = settingsFolder;
    QFile file(mSettingsFolder + mSlash + mFileName);
    file.open(QIODevice::WriteOnly);

    // XML-документ создан без ошибок
    if (!writeFail)
    {
        QTextStream xmlTS(&file);
        xmlTS.setCodec(QTextCodec::codecForName("UTF-8"));
        xmlTS<<mainXML.toString();
    }

    return !writeFail;
}



int ProjectSettingsLoader::unitsCount()
{
    return mUnitsCount;
}



QString ProjectSettingsLoader::projectName()
{
    return mProjectName;
}



int ProjectSettingsLoader::setUnitsCount(int unitsCount)
{
    mUnitsCount = unitsCount;

    return 0;
}



int ProjectSettingsLoader::setProjectName(const QString projectName)
{
    mProjectName = projectName;

    return 0;
}



int ProjectSettingsLoader::setProjectDirectory(const QString projectDirectory)
{
    // Установка целевой папки
    mSettingsFolder = projectDirectory;

    return 0;
}
