#ifndef PROJECTSETTINGSLOADER_H
#define PROJECTSETTINGSLOADER_H



#include <QObject>
#include <QDir>
#include <QtXml>



class ProjectSettingsLoader : public QObject
{
    Q_OBJECT

public:
    explicit ProjectSettingsLoader(QObject *parent = 0);
    // Загрузка настроек из файла
    bool load(const QString settingsFolder);
    // Сохранение настроек в файл
    bool save(const QString settingsFolder);
    // Полчение значений настроек
    // Если файл не был загружен из-за ошибки - используются значения по умолчанию
    int unitsCount();
    QString projectName();
    // Установка значений настроек
    int setUnitsCount(int unitsCount);
    int setProjectName(const QString projectName);
    // Смена директории с проектом
    int setProjectDirectory(const QString projectDirectory);


private:
    // Строковые константы
    const QString mSlash;
    // Параметры файловой системы
    QString mSettingsFolder;
    QString mFileName;
    // Данные
    int mUnitsCount;
    QString mProjectName;
};



#endif // PROJECTSETTINGSLOADER_H
