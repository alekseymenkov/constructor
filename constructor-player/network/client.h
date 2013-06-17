#ifndef CLIENT_H
#define CLIENT_H



#include <QtNetwork>



class Client : public QObject
{
    Q_OBJECT


public:
    // Конструктор
    Client(QObject* parent = 0);
    // Деструктор
    ~Client();

    // Установка сетевых настроек для клиента
    int setParameters(QString serverAddress, quint16 port);
    // Метод, выполняющий попытку подключения к серверу
    int connectToServer();
    // Принудительное отключение клиента
    int disconnectFromServer();
    // Состояние клиента
    bool isConnected();
    // Результат предыдущего подключения
    bool oldConnectionResult();
    // Корректность параметров подключения (адрес сервера + порт)
    bool isParametersCorrect();
    // Адрес сервера
    QString serverAddress();
    // Порт
    quint16 port();


private:
    // Клиентский сокет
    QTcpSocket* mClientSocket;
    // Данные, полученные с сервера
    QByteArray mReceivedData;
    // Адрес сервера
    QString mServerAddress;
    // Порт сервера
    quint16 mPort;
    // Таймер для отправки сигналов квитирования
    static const quint16 mActivityInterval = 30000;
    QTimer* mActivityTimer;
    // Результат текущего и предыдущего подключения
    bool mCurrentConnectionResult;
    bool mOldConnectionResult;

    struct PackageType
    {
        enum Type
        {
            Command = 0,
            RegInfo = 1,
            MMFName = 2,
            Data = 3,
            DescriptionRequest = 4,
            DescriptionResponse = 5,
            CoefficientRequest = 6,
            CoefficientResponse = 7
        };
    };

    struct CommandType
    {
        enum Type
        {
            IsReady = 0,
            IsAlive = 1,
            ConnectionLimit = 2,
            ReverseMode = 3
        };
    };

    // Обработка пакета "Команда"
    bool parseCommandPackage(QByteArray data);
    // Обработка пакета "Данные"
    bool parseDataPackage(QByteArray package);
    // Обработка пакета "Запрос из БД"
    bool parseDBResponsePackage(QByteArray package);
    // Генерация пакета "Команда"
    QByteArray createCommandPackage(CommandType::Type commandType);
    // Генерация пакета "Имя MMF"
    QByteArray createMMFPackage(QString MMFName);
    // Генерация пакета "Запрос из БД"
    QByteArray createDBRequestPackage(quint16 requestID, quint16 startByte,
                                      quint16 startBit);
    // Генерация пакета типа "Регистрация элементов"
    QByteArray createRegInfoPackage(quint16 elementID,
                                    QVector<quint16> startBytes,
                                    QVector<quint16> bytesCount);

    // Генерация пакета "Запрос из БД"
    QByteArray createDBCoefficientRequestPackage(quint16 requestID, quint16 startByte,
                                      quint16 startBit);

public slots:
    // Обработчик сигнала "Подключение к серверу"
    int slotClientConnected();
    // Обработчик сигнала "Отключение от сервера"
    int slotClientDisconnected();
    // Обработчик сигнала "Отключиться от сервера"
    int slotDisconnectClient();
    // Обработчик сигнала "Ошибка сокета"
    int slotSocketError(QAbstractSocket::SocketError);
    // Обработчик сигнала "Появление новых данных для чтения"
    int slotReceiveData();
    // Обработчик сигнала "Превышено время ожидание ответа от сервера"
    int slotCheckActivity();
    // Подтверждение готовности клиента
    int slotSendClientIsReady();
    // Регистрация MMF, закрепленной за клиентом
    int slotSendMMFPackage(QString MMFName);
    // Добавление отслеживаемого элемента
    int slotSendRegInfoPackage(quint16 elementID,
                               QVector<quint16> startBytes,
                               QVector<quint16> bytesCount);
    // Запрос из БД (создается список с запросами, каждому присваивается ID.
    // При ответе, запросы удаляются из списка)
    int slotSendDBRequestPackage(quint16 requestID,
                                 quint16 startByte,
                                 quint16 startBit);


signals:
    // Сигнал "Результат подключения к серверу"
    // Сигнал высылается при отключении от сервера с флагом false и
    // при получении от сервера пакета Command, IsReady
    int signalConnectionResult(bool success);
    // Сигнал "Отправка данных элементу"
    int signalSendDataToElement(quint16 id, QByteArray data);
    // Сигнал "Отправка описателей к элементам"
    int signalSendDescriptionToElement(quint16 requestID, QString description);
};



#endif // CLIENT_H
