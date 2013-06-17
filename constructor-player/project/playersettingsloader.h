#ifndef PLAYERSETTINGSLOADER_H
#define PLAYERSETTINGSLOADER_H



#include <QObject>
#include <QDir>
#include <QtXml>
#include <QDesktopWidget>



class PlayerSettingsLoader : public QObject
{
    Q_OBJECT

public:
    explicit PlayerSettingsLoader(int programID = 0, QObject *parent = 0);
    // Загрузка настроек из файла
    bool load();
    // Сохранение настроек в файл
    bool save();
    // Удаление файла
    bool clear();
    // Количество записей в конфигурационном файле
    int programsCount();
    // Полчение значений настроек
    // Если файл не был загружен из-за ошибки - используются значения по умолчанию
    QString lastOpenedProject();
    int tabNumber();
    QSize mainWindowSize();
    QPoint mainWindowPosition();
    bool isMaximized();
    bool isPanelShow();
    // Установка значений настроек
    int setLastOpenedProject(QString lastOpenedProject);
    int setTabNumber(int tabNumber);
    int setMainWindowParameters(QSize mainWindowSize, QPoint mainWindowPosition,
                                bool isMaximized, bool isPanelShowed);
    int setPanelState();

private:
    // Параметры файловой системы
    QString mSettingsFolder;
    QString mFilename;
    // Последний открытый проект
    QString mLastOpenedProject;
    // Номер открытой вкладки
    int mTabNumber;
    // Данные о размерах окон
    QSize mMainWindowSize;
    QPoint mMainWindowPosition;
    bool mIsMaximized;
    bool mIsPanelShowed;
    // Номер экземпляра запущенной программы
    int mProgramID;
};



#endif // PLAYERSETTINGSLOADER_H
