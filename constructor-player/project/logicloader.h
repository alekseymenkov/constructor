#ifndef LOGICLOADER_H
#define LOGICLOADER_H



#include <QtCore>
#include "../module/elementinterface.h"
#include "../module/elementlogic.h"



// Класс, отвечающий за загрузку логики приложения
class LogicLoader : public QObject
{
    Q_OBJECT

public:
    LogicLoader(QObject *parent = 0);
    // Получение списка модулей
    QStringList modulesList();
    // Загрузка модуля
    QSharedPointer<ElementLogic> loadModule(QString moduleName, bool &loadResult);
    // Смена директории с проектом
    int setProjectDirectory(const QString projectDirectory);
    // Копирование необходимых модулей в директорию с проектом
    bool copyModuleToProjectDirectory(QString moduleName);


private:
    // Символические константы
    const QString mSlash;
    const QString mModulesFolder;
    // Список доступных модулей (+ список хешей)
    QStringList mModules;
    // Директория с модулями
    QString mBasePath;
    QString mProjectPath;
    QString mOldProjectPath;
    QDir mBaseDir;
    QDir mProjectDir;
    // Трекер изменений в директории
    QFileSystemWatcher* mFileWatcher;
    // Трекер изменения самой директории
    QFileSystemWatcher* mDirectoryWatcher;
    // Обновление списка доступных модулей + обновление хешей модулей
    int refreshModulesList();

private slots:
    // Обработчик события - "Изменение списка файлов в директории"
    int slotFilesChanged(QString);
    // Обработчик события - "Изменение директории"
    int slotDirectoryChanged(QString);


signals:
    // Сигнал - "Список модулей в папке изменился"
    int signalModulesListChanged(QStringList);
};



#endif // LOGICLOADER_H
