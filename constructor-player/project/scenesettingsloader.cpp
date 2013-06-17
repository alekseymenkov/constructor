#include "scenesettingsloader.h"



SceneSettingsLoader::SceneSettingsLoader(int index, QObject *parent) :
    QObject(parent),
    mImagesFolder(QString("background/")),
    mSettingsFolder(QString("settings/")),
    mSlash(QString("/")),
    mPort(5525),
    mServerAddress(QString("127.0.0.1")),
    mIsAutoConnect("false"),
    mImage(QString()),
    mSceneName(QString()),
    mFilename(QString("scene%1.xml").arg(index)),
    mProjectDirectory(QString()),
    mOldProjectDirectory(QString()),
    mIsAutoDBRequest(true),
    mIsForceDBRequest(false)
{
}



// Загрузка значений настроек из файла
// Если параметр не удастся загрузить - будет использоваться значение по умолчанию
bool SceneSettingsLoader::load()
{
    bool result = false;

    QFile file(mProjectDirectory + mSlash + mSettingsFolder + mFilename);

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

    // Если файл - не xml
    result = mainXML.setContent(&file);
    if (!result)
        return false;

    // Индикатор успешности парсинга
    bool readFail = false;

    // Используемые переменные
    QString contentString;
    qint32 contentInt = 0;

    // Настройки сети
    rootElement = mainXML.documentElement().firstChildElement("networkSettings");
    if (rootElement.isNull())
        return false;

    // Адрес сервера
    contentString = rootElement.firstChildElement("serverAddress").text();
    if (contentString.isEmpty())
        readFail = true;
    else
        mServerAddress = contentString;

    // Порт
    contentInt = rootElement.firstChildElement("port").text().toInt(&result);
    if (!result)
        readFail = true;
    else
        mPort = contentInt;

    // Флаг автоматического подключения
    contentInt = rootElement.firstChildElement("isAutoConnect").text().toInt(&result);
    if (!result)
        readFail = true;
    else
        mIsAutoConnect = contentInt;

    // Фоновое изображение
    rootElement = mainXML.documentElement().firstChildElement("sceneSettings");
    if (rootElement.isNull())
        return false;

    // Адрес фонового изображения
    mSceneName = rootElement.firstChildElement("sceneName").text();

    // Имя отображаемого файла
    mMMFName = rootElement.firstChildElement("mmfName").text();

    // Адрес фонового изображения
    mImage = rootElement.firstChildElement("backgroundImage").text();

    // Коэффициент масштаба
    const int scaleFactorConst = 100;
    contentInt = rootElement.firstChildElement("scaleFactor").text().toInt(&result);
    if (!result)
    {
        mScaleFactor = scaleFactorConst;
        readFail = true;
    }
    else
    {
        mScaleFactor = contentInt;
    }

    // Параметры обновления описаний к элементам
    rootElement = mainXML.documentElement().firstChildElement("updateSettings");
    if (rootElement.isNull())
        return false;

    // Автоматическое обновление описаний к параметрам
    contentInt = rootElement.firstChildElement("isAutoDBRequest").text().toInt(&result);
    if (!result)
        readFail = true;
    else
        mIsAutoDBRequest = contentInt;

    // Принудительное обновление описаний к параметрам
    contentInt = rootElement.firstChildElement("isForceDBRequest").text().toInt(&result);
    if (!result)
        readFail = true;
    else
        mIsForceDBRequest = contentInt;

    return !readFail;
}



// Сохранение значений настроек в файл
bool SceneSettingsLoader::save()
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
    element = mainXML.createElement("networkSettings");
    rootElement.appendChild(element);

    // Адрес сервера
    subElement = mainXML.createElement("serverAddress");
    elementText = mainXML.createTextNode(mServerAddress);
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

    // Флаг автоматического подключения
    subElement = mainXML.createElement("isAutoConnect");
    elementText = mainXML.createTextNode(QString::number(mIsAutoConnect));
    subElement.appendChild(elementText);
    tempElement = element.appendChild(subElement).toElement();
    if (tempElement.isNull())
        writeFail = true;

    // Настройки сцены
    element = mainXML.createElement("sceneSettings");
    rootElement.appendChild(element);

    // Фоновое изображение
    subElement = mainXML.createElement("backgroundImage");
    elementText = mainXML.createTextNode(mImage);
    subElement.appendChild(elementText);
    tempElement = element.appendChild(subElement).toElement();
    if (tempElement.isNull())
        writeFail = true;

    // Порт
    subElement = mainXML.createElement("scaleFactor");
    elementText = mainXML.createTextNode(QString::number(mScaleFactor));
    subElement.appendChild(elementText);
    tempElement = element.appendChild(subElement).toElement();
    if (tempElement.isNull())
        writeFail = true;

    // Название сцены
    subElement = mainXML.createElement("sceneName");
    elementText = mainXML.createTextNode(mSceneName);
    subElement.appendChild(elementText);
    tempElement = element.appendChild(subElement).toElement();
    if (tempElement.isNull())
        writeFail = true;

    // Имя отображаемого в память файла
    subElement = mainXML.createElement("mmfName");
    elementText = mainXML.createTextNode(mMMFName);
    subElement.appendChild(elementText);
    tempElement = element.appendChild(subElement).toElement();
    if (tempElement.isNull())
        writeFail = true;

    // Параметры обновления описаний к элементам
    element = mainXML.createElement("updateSettings");
    rootElement.appendChild(element);

    // Автоматическое обновление описаний к параметрам
    subElement = mainXML.createElement("isAutoDBRequest");
    elementText = mainXML.createTextNode(QString::number(mIsAutoDBRequest));
    subElement.appendChild(elementText);
    tempElement = element.appendChild(subElement).toElement();
    if (tempElement.isNull())
        writeFail = true;

    // Принудительное обновление описаний к параметрам
    subElement = mainXML.createElement("isForceDBRequest");
    elementText = mainXML.createTextNode(QString::number(mIsForceDBRequest));
    subElement.appendChild(elementText);
    tempElement = element.appendChild(subElement).toElement();
    if (tempElement.isNull())
        writeFail = true;

    // Файл для записи
    QFile file(QString(mProjectDirectory + mSlash + mSettingsFolder + mFilename));
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



bool SceneSettingsLoader::remove()
{
    return QFile::remove(mProjectDirectory + mSlash + mSettingsFolder + mFilename);
}



int SceneSettingsLoader::setNetParameters(const QString serverAddress, const quint16 port,
                                          const bool isAutoConnect)
{
    mServerAddress = serverAddress;
    mPort = port;
    mIsAutoConnect = isAutoConnect;

    return 0;
}



int SceneSettingsLoader::setImageParameters(const QString image, const int scaleFactor)
{
    mImage = image;
    mScaleFactor = scaleFactor;

    return 0;
}



int SceneSettingsLoader::setSceneParameters(const QString sceneName)
{
    mSceneName = sceneName;

    return 0;
}



int SceneSettingsLoader::setMMFName(const QString mmfName)
{
    mMMFName = mmfName;

    return 0;
}



int SceneSettingsLoader::setUpdateParameters(const bool isAutoDBRequest, const bool isForceDBRequest)
{
    mIsAutoDBRequest = isAutoDBRequest;
    mIsForceDBRequest = isForceDBRequest;

    return 0;
}



int SceneSettingsLoader::changeIndex(int index)
{
    this->remove();
    mFilename = QString("scene%1.xml").arg(index);
    this->save();

    return 0;
}



// Установка директории проекта
int SceneSettingsLoader::setProjectDirectory(const QString projectDirectory)
{
    // Установка целевой папки
    mOldProjectDirectory = mProjectDirectory;
    mProjectDirectory = projectDirectory;

    if (projectDirectory.isEmpty())
        return -1;

    // Проверка наличия папки настроек
    if (!QDir(mProjectDirectory + mSlash + mSettingsFolder).exists())
        QDir().mkdir(mProjectDirectory + mSlash + mSettingsFolder);

    // Проверка наличия папки с фоновыми изображениями
    if (!QDir(mProjectDirectory + mSlash + mImagesFolder).exists())
        QDir().mkdir(mProjectDirectory + mSlash + mImagesFolder);

    return 0;
}



// Копирование фонового изображения, закрепленного за сценой
bool SceneSettingsLoader::copyBackgroundImage()
{
    if (mImage.isEmpty())
        return false;

    // Определяем старое местоположение изображения и новое
    QString oldImageLocation;
    QString newImageLocation = mProjectDirectory + mSlash
            + mImagesFolder + QFileInfo(mImage).fileName();

    // Старое местоположение может быть как относительным, так и абсолютным
    // Прежде чем выполнять копирование, необходимо привести путь к абсолютному виду
    if (!QDir::isRelativePath(mImage))
        oldImageLocation = mImage;
    else
        oldImageLocation = mOldProjectDirectory + mSlash + mImage;

    // Копирование изображения
    bool result = QFile::copy(oldImageLocation, newImageLocation);
    // При успешном копировании - сохраняем информацию о новом местоположении изображения
    if (result)
        mImage = mImagesFolder + QFileInfo(mImage).fileName();

    return result;
}



quint16 SceneSettingsLoader::port()
{
    return mPort;
}



QString SceneSettingsLoader::serverAddress()
{
    return mServerAddress;
}



bool SceneSettingsLoader::isAutoConnect()
{
    return mIsAutoConnect;
}



QString SceneSettingsLoader::image()
{
    if (mImage.isEmpty() || !QDir::isRelativePath(mImage))
        return mImage;
    else
        return mProjectDirectory + mSlash + mImage;
}



int SceneSettingsLoader::scaleFactor()
{
    return mScaleFactor;
}



QString SceneSettingsLoader::sceneName()
{
    return mSceneName;
}



QString SceneSettingsLoader::mmfName()
{
    return mMMFName;
}



bool SceneSettingsLoader::isAutoDBRequest()
{
    return mIsAutoDBRequest;
}



bool SceneSettingsLoader::isForceDBRequest()
{
    return mIsForceDBRequest;
}
