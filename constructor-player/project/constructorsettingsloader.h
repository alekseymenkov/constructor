#ifndef CONSTRUCTORSETTINGSLOADER_H
#define CONSTRUCTORSETTINGSLOADER_H



#include <QObject>
#include <QDir>
#include <QtXml>



class ContructorSettingsLoader : public QObject
{
    Q_OBJECT

public:
    explicit ContructorSettingsLoader(QObject *parent = 0);
    // Загрузка настроек из файла
    bool load();
    // Сохранение настроек в файл
    bool save();
    // Полчение значений настроек
    // Если файл не был загружен из-за ошибки - используются значения по умолчанию
    QString lastOpenedProject();
    QSize mainWindowSize();
    QSize elementEditDialogSize();
    QSize constructorSize();
    bool isMaximized();
    bool isConstructorShow();
    bool isPanelShow();
    // Установка значений настроек
    int setLastOpenedProject(QString lastOpenedProject);
    int setMainWindowParameters(QSize mainWindowSize, bool isMaximized);
    int setPanelState(bool isPanelShow);
    int setConstructorParameters(QSize constructorSize, bool isConstructorShow);
    int setElementEditDialogSize(QSize elementEditDialogSize);


private:
    // Параметры файловой системы
    QString mSettingsFolder;
    QString mFilename;
    // Последний открытый проект
    QString mLastOpenedProject;
    // Данные о размерах окон
    QSize mMainWindowSize;
    QSize mElementEditDialogSize;
    QSize mConstructorSize;
    bool mIsMaximized;
    bool mIsConstructorShow;
    bool mIsPanelShow;
    // Номер экземпляра запущенной программы
    bool mProgramID;
};



#endif // CONSTRUCTORSETTINGSLOADER_H
