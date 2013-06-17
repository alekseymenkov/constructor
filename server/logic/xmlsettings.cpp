#include "xmlsettings.h"



XMLSettings::XMLSettings(QObject *parent) :
    QObject(parent),
    mPort(5525),
    mMaxClients(32),
    mNetInterface("0.0.0.0"),
    mIsServerAutorun(false),
    mDBPath(""),
    mSettingsFolder(qApp->applicationDirPath() + "/settings"),
    mFilename("config.xml")
{
    // Проверка наличия папок, необходимых для работы программы
    if (!QDir(mSettingsFolder).exists())
            QDir().mkdir(mSettingsFolder);
}



// Загрузка значений настроек из файла
// Если параметр не удастся загрузить - будет использоваться значение по умолчанию
qint8 XMLSettings::load()
{
    bool result = false;

    QFile file(mSettingsFolder + "/" + mFilename);

    // Если файл не существует
    result = file.exists();
    if (!result)
        return -1;

    // Если нет прав на чтение
    result = file.open(QIODevice::ReadOnly);
    if (!result)
        return -1;

    QDomDocument mainXML;
    QDomElement rootElement;

    // Если файл - не xml
    result = mainXML.setContent(&file);
    if (!result)
        return -1;

    // Индикаторы успешности парсинга
    bool readFail = false;

    // Используемые переменные
    quint16 port = 0;
    quint16 maxClients = 0;
    QString netInterface;
    bool isServerAutorun = false;

    // Настройки сети
    rootElement = mainXML.documentElement().firstChildElement("networkSettings");
    if (rootElement.isNull())
        return -1;

    // Сетевой интерфейс
    netInterface = rootElement.firstChildElement("netInterface").text();
    if (netInterface.isEmpty())
        readFail = true;
    else
        mNetInterface = netInterface;

    // Порт
    port = rootElement.firstChildElement("port").text().toInt(&result);
    if (!result)
        readFail = true;
    else
        mPort = port;

    // Максимальное количество подключений
    maxClients = rootElement.firstChildElement("maxClients").text().toInt(&result);
    if (!result)
        readFail = true;
    else
        mMaxClients = maxClients;

    // Флаг автозапуска сервера
    isServerAutorun = rootElement.firstChildElement("serverAutorun").text().toInt(&result);
    if (!result)
        readFail = true;
    else
        mIsServerAutorun = isServerAutorun;

    // Настройки базы данных
    rootElement = mainXML.documentElement().firstChildElement("DBSettings");
    if (rootElement.isNull())
        return -1;

    // База данных
    mDBPath = rootElement.firstChildElement("path").text();

    if (readFail)
        return -1;
    else
        return 0;
}



// Сохранение значений настроек в файл
qint8 XMLSettings::save()
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
        return -1;

    // Настройки сети
    element = mainXML.createElement("networkSettings");
    rootElement.appendChild(element);

    // Сетевой интерфейс
    subElement = mainXML.createElement("netInterface");
    elementText = mainXML.createTextNode(mNetInterface);
    subElement.appendChild(elementText);
    tempElement = element.appendChild(subElement).toElement();
    if (tempElement.isNull())
        writeFail = true;

    // Порт
    subElement = mainXML.createElement("port");
    elementText = mainXML.createTextNode(QString::number(mPort));
    subElement.appendChild(elementText);
    tempElement = element.appendChild(subElement).toElement();
    if (tempElement.isNull())
        writeFail = true;

    // Максимальное количество клиентов
    subElement = mainXML.createElement("maxClients");
    elementText = mainXML.createTextNode(QString::number(mMaxClients));
    subElement.appendChild(elementText);
    tempElement = element.appendChild(subElement).toElement();
    if (tempElement.isNull())
        writeFail = true;

    // Автозапуск сервера
    subElement = mainXML.createElement("serverAutorun");
    elementText = mainXML.createTextNode(QString::number(mIsServerAutorun));
    subElement.appendChild(elementText);
    tempElement = element.appendChild(subElement).toElement();
    if (tempElement.isNull())
        writeFail = true;

    // Настройки БД
    element = mainXML.createElement("DBSettings");
    rootElement.appendChild(element);

    // Сетевой интерфейс
    subElement = mainXML.createElement("path");
    elementText = mainXML.createTextNode(mDBPath);
    subElement.appendChild(elementText);
    tempElement = element.appendChild(subElement).toElement();
    if (tempElement.isNull())
        writeFail = true;

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

    return 0;
}



// Установка параметров БД
qint8 XMLSettings::setDBParameters(QString DBPath)
{
    mDBPath = DBPath;

    return 0;
}



// Установка параметров сети
qint8 XMLSettings::setNetParameters(quint16 port, QString netInterface, bool isServerAutorun)
{
    mPort = port;
    mNetInterface = netInterface;
    mIsServerAutorun = isServerAutorun;

    return 0;
}



// Полчение значения порта
quint16 XMLSettings::getPort()
{
    return mPort;
}



// Получение используемого сетевого интерфейса
QString XMLSettings::getNetInterface()
{
    return mNetInterface;
}



// Получение значения лимита подключений к серверу
quint16 XMLSettings::getMaxClients()
{
    return mMaxClients;
}



// Параметры автозапуска сервера
bool XMLSettings::getServerAutorun()
{
    return mIsServerAutorun;
}



// Полчение значения порта
QString XMLSettings::getDBPath()
{
    return mDBPath;
}
