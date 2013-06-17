#include "database.h"



Database::Database(QObject *parent) :
    QObject(parent)
{
    mDatabaseTimer = new QTimer(this);
    mDatabaseTimer->setInterval(mDatabaseTime);

    connect(mDatabaseTimer, SIGNAL(timeout()),
            this, SLOT(slotCloseDatabase()));

    mDB = QSqlDatabase::addDatabase("QODBC");
}



// Установка БД
int Database::setDatabaseName(QString databaseName)
{
    mDB.setDatabaseName(QString("DRIVER={Microsoft Access Driver (*.mdb)};FIL={MS Access};DBQ=%1").arg(databaseName));

    return 0;
}



// Запрос описания к элементу
int Database::slotDBDescriptionRequest(quint16 clientNumber, quint16 requestID,
                                       quint16 startByte, quint16 startBit)
{
    if (!mDB.isOpen() && !mDB.open())
    {
        emit signalSendTextToLog("Ошибка при открытии базы данных!");
        emit signalDBDescriptionResponse(clientNumber, requestID, QString());
        return -1;
    }

    // Перезапуск таймера
    mDatabaseTimer->start();

    bool queryResult = false;
    QString queryString = QString("SELECT Name_parametr FROM SKV_BKS WHERE N_byte = %1 AND N_bit = %2")
            .arg(startByte)
            .arg(startBit);
    QSqlQuery query(mDB);

    queryResult = query.exec(queryString);

    QString description;
    if (query.next())
        description = query.value(0).toString();

    if (queryResult)
        emit signalDBDescriptionResponse(clientNumber, requestID, description);
    else
        emit signalSendTextToLog("Ошибка при выполнении запроса к базе данных!");

    return 0;
}



// Запрос коэффициента масштабирования
int Database::slotDBCoefficientRequest(quint16 clientNumber, quint16 requestID,
                                       quint16 startByte, quint16 startBit)
{
    if (!mDB.isOpen() && !mDB.open())
    {
        emit signalSendTextToLog("Ошибка при открытии базы данных!");
        emit signalDBCoefficientResponse(clientNumber, requestID, 0);
        return -1;
    }

    // Перезапуск таймера
    mDatabaseTimer->start();

    bool queryResult = false;
    QString queryString = QString("SELECT Zsr FROM SKV_BKS WHERE N_byte = %1 AND N_bit = %2")
            .arg(startByte)
            .arg(startBit);
    QSqlQuery query(mDB);

    queryResult = query.exec(queryString);

    float coefficient = 0;
    bool convertResult = false;
    if (query.next())
        coefficient = query.value(0).toFloat(&convertResult);
    if (!convertResult)
        coefficient = 0;

    if (queryResult)
        emit signalDBCoefficientResponse(clientNumber, requestID, coefficient);
    else
        emit signalSendTextToLog("Ошибка при выполнении запроса к базе данных!");

    return 0;
}



// Закрытие БД
int Database::slotCloseDatabase()
{
    // Остановка таймера
    mDatabaseTimer->stop();

    mDB.close();

    return 0;
}
