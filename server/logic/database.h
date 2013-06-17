#ifndef DATABASE_H
#define DATABASE_H



#include <QObject>
#include <QtSql>



class Database : public QObject
{
    Q_OBJECT

public:
    explicit Database(QObject *parent);
    int setDatabaseName(QString databaseName);


public slots:
    // Запрос описания к параметрам элементов
    int slotDBDescriptionRequest(quint16 clientNumber, quint16 requestID,
                                 quint16 startByte, quint16 startBit);
    // Запрос коэффициента масштабирования
    int slotDBCoefficientRequest(quint16 clientNumber, quint16 requestID,
                                 quint16 startByte, quint16 startBit);
    

signals:
    // Результат запроса описания
    int signalDBDescriptionResponse(quint16 clientNumber, quint16 requestID,
                                    QString description);
    // Результат запроса коэффициента масштабирования
    int signalDBCoefficientResponse(quint16 clientNumber, quint16 requestID,
                                    float coefficient);
    // Информационный вывод
    int signalSendTextToLog(QString text);

private:
    QSqlDatabase mDB;
    // Таймер для отложенного закрытия БД
    QTimer* mDatabaseTimer;
    static const int mDatabaseTime = 60000;


private slots:
    int slotCloseDatabase();
};



#endif // DATABASE_H
