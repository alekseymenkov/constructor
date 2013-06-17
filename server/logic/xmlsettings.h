#ifndef XMLSETTINGS_H
#define XMLSETTINGS_H



#include <QObject>
#include <QDir>
#include <QtXml>



class XMLSettings : public QObject
{
    Q_OBJECT


public:
    explicit XMLSettings(QObject *parent = 0);
    // Загрузка настроек из файла
    qint8 load();
    // Сохранение настроек в файл
    qint8 save();
    // Полчение значений настроек
    // Если файл не был загружен из-за ошибки - используются значения по умолчанию
    quint16 getPort();
    quint16 getMaxClients();
    QString getNetInterface();
    bool getServerAutorun();
    QString getDBPath();
    // Установка значений настроек
    qint8 setNetParameters(quint16 port, QString netInterface, bool isServerAutorun);
    qint8 setDBParameters(QString DBPath);


private:
    // Данные, сеть
    quint16 mPort;
    quint16 mMaxClients;
    QString mNetInterface;
    bool mIsServerAutorun;
    // Данные, БД
    QString mDBPath;
    // Параметры файловой системы
    QString mSettingsFolder;
    QString mFilename;
};



#endif // XMLSETTINGS_H
