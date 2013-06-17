#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H



#include <QtGui>
#include "lineediteventfilter.h"



class ConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigDialog(bool isClientRunning, int isBlockMode = false, QWidget *parent = 0);
    ~ConfigDialog();
    // Установка данных
    int setProjectName(const QString projectName);
    int setSceneName(const QString sceneName);
    int setMMFName(const QString mmfName);
    int setServerAddress(const QString serverAddress);
    int setPort(quint16 port);
    int setIsAutoConnect(bool isAutoConnect);
    int setIsAutoDBRequest(bool isAutoDBRequest);
    int setIsForceDBRequest(bool isForceDBRequest);
    // Получение данных
    QString projectName();
    QString sceneName();
    QString mmfName();
    QString serverAddress();
    quint16 port();
    bool isAutoConnect();
    bool isAutoDBRequest();
    bool isForceDBRequest();


private slots:
    // Слот - изменение настроек
    int slotNetConfigChanged(QString);
    int slotNetConfigChanged();
    // Слот - изменение название вкладки
    int slotSceneNameChanged(QString);
    // Слот - изменение текста
    int slotTextChanged(QString);
    // Слот - щелчок по кнопке "Применить"
    int slotBtnApplyClick();
    // Слот - щелчок по кнопке "Отмена"
    int slotBtnCancelClick();
    // Слот - щелчок по кнопке "Ок"
    int slotBtnOkClick();
    // Слот - изменение настроек обновления
    int slotUpdateConfigChanged();


private:
    // Данные
    QString mProjectName;
    QString mSceneName;
    QString mMMFName;
    QString mServerAddress;
    quint16 mPort;
    bool mIsClientRunning;
    bool mIsDataSave;
    bool mIsAutoConnect;
    bool mIsAutoDBRequest;
    bool mIsForceDBRequest;
    // Лейауты
    QVBoxLayout* ltMain;
    QHBoxLayout* ltButtons;
    QGridLayout* ltRegConfig;
    QGridLayout* ltAppearance;
    QGridLayout* ltUpdate;
    // Элементы UI
    // GroupBox - настройка сети
    QGroupBox* gbRegConfig;
    // GroupBox - настройка фонового изображения
    QGroupBox* gbAppearance;
    // GroupBox - настройка дополнительный параметров
    QGroupBox* gbUpdate;
    // Название проекта
    QLabel* lblProjectName;
    QLineEdit* leProjectName;
    // Имя MMF
    QLabel* lblMMFName;
    QLineEdit* leMMFName;
    // Адрес сервера
    QLabel* lblServerAddress;
    QLineEdit* leServerAddress;
    // Порт
    QLabel* lblPort;
    QLineEdit* lePort;
    // Уведомления
    QLabel* lblNetNotification;
    QLabel* lblUpdateNotification;
     // Настройка имени вкладки
    QLabel* lblSceneName;
    QLineEdit* leSceneName;
    // Автоматическое подключение к серверу
    QCheckBox* cbAutoConnect;
    // Прочие параметры
    QCheckBox* cbAutoDBRequest;
    QCheckBox* cbForceDBRequest;
    // Управляющие кнопки
    QPushButton* btnApply;
    QPushButton* btnOk;
    QPushButton* btnCancel;
};



#endif // CONFIGDIALOG_H
