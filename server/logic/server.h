#ifndef SERVER_H
#define SERVER_H



#include <QtNetwork>
#include "mmf.h"



// Проблемный класс
// Детали: http://www.prog.org.ru/topic_23229_30.html



class Server : public QObject
{
    Q_OBJECT


public:
    Server();
    ~Server();

    // Запуск сервера
    int start();
    // Остановка сервера
    int stop();
    // Установка параметров
    int setParameters(QString interface, quint16 port, quint16 maxClients);
    // Получение списка сетевых интерфейсов в системе
    QStringList getNetInterfaces();
    // Информация о состоянии сервера
    bool isRunning();


private:
    // Структура: элементы, закрепленные за клиентским сокетом.
    // 1. count - количество элементов, зарегистрированных за клиентом;
    // 2. id - идентификаторы элементов. Определяются на стороне клиента и отправляются для регистрации на сервере;
    // 3. parts - количество зарегистрированных за одним элементом частей;
    // 4. startByte - массив стартовых байтов, начиная с которых осуществляется чтение информации в MMF (допускается несвязные области);
    // 5. bytesCount - массив, содержащий количество байт, необходимых каждому элементу (допускаются несвязные области);
    // 6. clientData - набор байтов, считанных при предыдущем вызове функции (опция для экономии трафика);
    // 7. isReady - флаг готовности клиента к получению данных.
    // 8. isForWrite - флаг для проверки типа взаимодействия с клиентом: true - передача данных от сервера к клиенту, false - в обратном направлении
    struct ClientData
    {
        quint16 count;
        QVector<quint16> id;
        QVector<quint8> parts;
        QVector< QVector<quint16> > startByte;
        QVector< QVector<quint16> > bytesCount;
        QVector<QByteArray> clientData;
        QVector<bool> isForWrite;
        bool isReady;
    };

    // Перечисление: типы используемых пакетов.
    // 1. Command - управляющий пакет, содержив в себе какую-либо команду;
    // Структура пакета: [размер пакета (2 байта)] + [тип пакета (1 байт)] + [код команда (1 байт)];
    // 2. RegInfo - пакет, содержащий регистрационные данные от клиента;
    // Структура пакета: [размер пакета (2 байта)] + [тип пакета (1 байт)] + [идентификатор элемента (2 байта)] +
    // {[стартовый байт (2 байта)] + [количество байт (2 байта)]} x N;
    // 3. MMFName - пакет, содержащий имя MMF, с которым планирует работать клиент;
    // Структура пакета: [размер пакета (2 байта)] + [тип пакета (1 байт)] + [Текст UTF-8 с именем MMF (N байт)];
    // 4. Data - пакет, содержащий данные из MMF. При наличии нескольких несвязных областей, из которых запрашиваются данные, сервер
    // формирует единый массив байтов в порядке, указанном при регистрации;
    // Структура пакета: [размер пакета (2 байта)] + [тип пакета (1 байт)] + [Идентификатор элемента (2 байта)] + [Массив байтов (N байт)];
    // 5. DescriptionRequest - запрос данных из базы данных;
    // Структура пакета: [размер пакета (2 байта)] + [тип пакета (1 байт)] + [Идентификатор запроса (2 байта)] +
    // [Номер байта (2 байта)] + [Номер бита (2 байт)];
    // 6. DescriptionResponse - ответ из БД;
    // Структура пакета: [размер пакета (2 байта)] + [тип пакета (1 байт)] +
    // [Идентификатор запроса (2 байта)] + [строка в формате UTF-8 (n байт)];
    // 7. CoefficientRequest - запрос коэффициента масштаба из БД
    // Структура пакета: [размер пакета (2 байта)] + [тип пакета (1 байт)] + [Идентификатор запроса (2 байта)] +
    // [Номер байта (2 байта)] + [Номер бита (2 байт)];
    // 7. CoefficientResponse - ответ из БД (коэффициент масштаба)
    // Структура пакета: [размер пакета (2 байта)] + [тип пакета (1 байт)] + [Идентификатор запроса (2 байта)] +
    // [Коэффициент (4 байта, float)];
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
            CoefficientResponse = 7,
            SingleDataRequest = 8,
            SingleDataResponse = 9
        };
    };

    // Перечисление: типы команд.
    // 1. IsReady - команда, отправляемая клиентом при готовности им получать данные с сервера;
    // 2. IsAlive - команда, высылаемая клиентом по таймеру, для проверки связи с сервером. Сервер должен ответить этой же командой;
    // 3. ConnectionLimit - команда, высылаемая от сервера к клиенту в случае перегрузки сервера.
    struct CommandType
    {
        enum Type
        {
            IsReady = 0,
            IsAlive = 1,
            ConnectionLimit = 2
        };
    };

    // Разбор пакета, содержащего имя MMF
    bool parseMMFPackage(quint16 clientNumber, QByteArray data);
    // Разбор пакета, содержащего информацию о регистрации клиентского элемента
    // Данные передаются от сервера к клиенту
    bool parseRegInfoPackage(quint16 clientNumber, QByteArray data);
    // Разбор пакета, содержащего управляющую команду
    bool parseCommandPackage(quint16 clientNumber, QByteArray data);
    // Разбор пакета, содержащего запрос описания из БД
    bool parseDBDescriptionRequestPackage(quint16 clientNumber, QByteArray data);
    // Разбор пакета, содержащего запрос коэффициента из БД
    bool parseDBCoefficientRequestPackage(quint16 clientNumber, QByteArray data);
    // Разбор пакета, содержащего единичный запрос данных из MMF
    bool parseDataRequestPackage(quint16 clientNumber, QByteArray data);
    // Разбор пакета, содержащего данные для записи на сервер
    bool parseDataPackage(quint16 clientNumber, QByteArray package);
    // Генерация пакета "Команда"
    QByteArray createCommandPackage(CommandType::Type commandType);
    // Генерация пакета "Данные от сервера (чтение)"
    QByteArray createDataPackage(quint16 clientNumber);
    // Генерация пакета "Запрос описания из БД"
    QByteArray createDBDescriptionResponsePackage(quint16 requestID, QString description);
    // Генерация пакета "Запрос коэффициента масштаба из БД"
    QByteArray createDBCoefficientResponsePackage(quint16 requestID, float coefficient);
    // Генерация пакета "Единичный запрос данных из MMF"
    QByteArray createDataResponsePackage(quint16 requestID, QByteArray data);
    // Отключение и удаление клиента
    int disconnectClient(quint16 clientNumber);

    // Интервал проверки активности клиентов
    static const quint16 mActivityInterval = 30000;
    // Таймер для проверки активности клиентов
    QTimer* mActivityTimer;
    // Интервал рассылки данных
    static const quint8 mSendDataInterval = 50;
    // Таймер для рассылки данных клиентам
    QTimer* mSendDataTimer;
    // Индикатор работы сервера
    bool mIsServerRunning;
    // Номер порта
    quint16 mPort;
    // Сетевой интерфейс
    QHostAddress mNetInterface;
    // Максимальное количество одновременных подключений
    quint16 mMaxClients;
    // Текущее количество подключений
    quint16 mActiveClients;
    // Серверный сокет
    QPointer<QTcpServer> mServer;
    // Массив клиентских сокетов
    QVector<QPointer<QTcpSocket> > mClients;
    // Массив MMF, закрепленных за клиентами
    MMF** mClientMMF;
    // Массив структур с данными о MMF, закрепленных за клиентами
    ClientData* mClientData;
    // Данные, полученные с сервера
    QVector<QByteArray> mReceivedData;


signals:
    // Запись данных в лог
    int signalSendTextToLog(const QString text);
    int signalSendTextToLogPrivate(const QString text);
    // Отправка запроса к БД на получение описания к элементу
    int signalDBDescriptionRequest(quint16 clientNumber, quint16 requestID,
                                   quint16 startByte, quint16 startBit);
    // Отправка запроса к БД на получение коэффициента масштаба
    int signalDBCoefficientRequest(quint16 clientNumber, quint16 requestID,
                                   quint16 startByte, quint16 startBit);
    // Отправка запроса на полученые данных из MMF
    int signalDataRequest(quint16 clientNumber, quint16 requestID,
                          quint16 startByte, quint16 bytesCount);


private slots:
    // Подключение клиента к серверу
    int slotAcceptClient();
    // Отключение клиента от сервера
    int slotRejectClient();
    // Получение данных от клиента
    int slotReceiveData();
    // Ошибка клиентского сокета
    int slotClientSocketError(QAbstractSocket::SocketError);
    // Отправка данных клиенту
    int slotSendData();
    // Обработчик сигнала "Превышено время ожидание ответа от сервера"
    int slotCheckActivity();
    // Получение описания из базы
    int slotDBDescriptionResponse(quint16 clientNumber, quint16 requestID,
                                  QString description);
    // Получение коэффициента масштаба из базы
    int slotDBCoefficientResponse(quint16 clientNumber, quint16 requestID,
                                  float coefficient);
    // Единичный запрос данных из MMF
    int slotDataResponse(quint16 clientNumber, quint16 requestID,
                         quint16 startByte, quint16 bytesCount);
};



#endif // SERVER_H
