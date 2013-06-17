#ifndef SCENESETTINGSLOADER_H
#define SCENESETTINGSLOADER_H



#include <QObject>
#include <QDir>
#include <QtXml>



class SceneSettingsLoader : public QObject
{
    Q_OBJECT

public:
    explicit SceneSettingsLoader(int index, QObject *parent = 0);
    // Загрузка настроек из файла
    bool load();
    // Сохранение настроек в файл
    bool save();
    // Удаление файла с настройками
    bool remove();
    // Полчение значений настроек
    // Если файл не был загружен из-за ошибки - используются значения по умолчанию
    quint16 port();
    QString serverAddress();
    bool isAutoConnect();
    QString image();
    int scaleFactor();
    QString sceneName();
    QString mmfName();
    bool isAutoDBRequest();
    bool isForceDBRequest();
    // Установка значений настроек
    int setNetParameters(const QString serverAddress, const quint16 port, const bool isAutoConnect);
    int setImageParameters(const QString image, const int scaleFactor);
    int setSceneParameters(const QString sceneName);
    int setMMFName(const QString mmfName);
    int setUpdateParameters(bool isAutoDBRequest, bool isForceDBRequest);
    // Смена индекса
    int changeIndex(int index);
    // Смена директории с проектом
    int setProjectDirectory(const QString projectDirectory);
    // Копирование фонового изображения, закрепленного за сценой
    bool copyBackgroundImage();


private:
    // Папка с изображениями - background/
    const QString mImagesFolder;
    // Папка с настройками - settings/
    const QString mSettingsFolder;
    // Слеш
    const QString mSlash;
    // Данные, сеть
    quint16 mPort;
    QString mServerAddress;
    bool mIsAutoConnect;
    // Данные, фон
    QString mImage;
    int mScaleFactor;
    // Данные, имя сцены
    QString mSceneName;
    // Параметры файловой системы
    QString mFilename;
    QString mProjectDirectory;
    QString mOldProjectDirectory;
    // Имя файла, отображаемого в память
    QString mMMFName;
    // Данные, параметры обновления
    bool mIsAutoDBRequest;
    bool mIsForceDBRequest;
};



#endif // SCENESETTINGSLOADER_H
