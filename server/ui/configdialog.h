#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H



#include <QtGui>
#include "lineediteventfilter.h"



class ConfigDialog : public QDialog
{
    Q_OBJECT


private slots:
    // Слот - изменение настроек (тип 1)
    qint8 slotNetConfigChanged(int);
    // Слот - изменение настроек (тип 2)
    qint8 slotNetConfigChanged(QString);
    // Слот - изменение настроек (тип 3)
    qint8 slotNetConfigChanged(bool);
    // Слот - щелчок по кнопке "Применить"
    qint8 slotBtnApplyClick();
    // Слот - щелчок по кнопке "Отмена"
    qint8 slotBtnCancelClick();
    // Слот - щелчок по кнопке "Ок"
    qint8 slotBtnOkClick();
    // Слот - щелчок по кнопке "Обзор" (база данных)
    qint8 slotBtnBrowseDBClick();
    // Слот - изменение расположения БД
    qint8 slotDBPathChanged(QString);


public:
    explicit ConfigDialog(bool isServerRunning, QWidget *parent = 0);
    ~ConfigDialog();
    // Установка данных
    qint8 setNetInterface(QStringList interfaces, QString interface);
    qint8 setPort(quint16 port);
    qint8 setServerAutorun(bool autorun);
    qint8 setDBPath(QString dbPath);

    // Получение данных
    QString getNetInterface();
    quint16 getPort();
    bool getServerAutorun();
    QString getDBPath();


private:
    // Данные
    QString mNetInterface;
    quint16 mPort;
    bool mIsServerAutorun;
    QString mDBPath;
    bool mIsServerRunning;
    bool mIsDataSave;

    // Лейауты
    QVBoxLayout* ltMain;
    QHBoxLayout* ltButtons;
    QGridLayout* ltNetConfig;
    QGridLayout* ltDatabase;
    // Элементы UI
    // GroupBox - настройка сети
    QGroupBox* gbNetConfig;
    // GroupBox - настройка базы
    QGroupBox* gbDatabase;
    // Сетевой интерфейс
    QLabel* lblNetInterface;
    QComboBox* cbNetInterface;
    // Порт
    QLabel* lblPort;
    QLineEdit* lePort;
    // Флажок
    QCheckBox* chbStartServer;
    // Сеть, уведомления
    QLabel* lblNetNotification;
    QLabel* lblDBNotification;
    // Настройки базы
    QLabel* lblDatabase;
    QLineEdit* leDatabase;
    QPushButton* btnBrowseDB;
    // Управляющие кнопки
    QPushButton* btnApply;
    QPushButton* btnOk;
    QPushButton* btnCancel;
};



#endif // CONFIGDIALOG_H
