#include "server.h"



Server::Server() :
    mIsServerRunning(false),
    mPort(0),
    mMaxClients(0),
    mActiveClients(0),
    mServer(0),
    mClientMMF(0),
    mClientData(0)
{
    mClients.clear();
    mReceivedData.clear();

    mSendDataTimer = new QTimer(this);
    mSendDataTimer->setInterval(mSendDataInterval);

    // Слот - "Функция отправки данных"
    connect(mSendDataTimer, SIGNAL(timeout()),
            this, SLOT(slotSendData()));

    mActivityTimer = new QTimer(this);
    mActivityTimer->setInterval(mActivityInterval);

    // Стол - "Функция проверки активности клиентов"
    connect(mActivityTimer, SIGNAL(timeout()),
            this, SLOT(slotCheckActivity()));

    // Обходим ошибки многопоточности
    connect(this, SIGNAL(signalSendTextToLogPrivate(QString)),
            this, SIGNAL(signalSendTextToLog(QString)),
            Qt::QueuedConnection);
}



// Деструктор класса Server.
Server::~Server()
{
    this->stop();
}



// Запуск сервера.
int Server::start()
{
    mServer = QPointer<QTcpServer>(new QTcpServer(this));

    // Установка максимального числа подключений
    // maxClients + 1 - слот для уведомления о перегрузке сервера
    mServer.data()->setMaxPendingConnections(mMaxClients + 1);

    bool result = mServer.data()->listen(mNetInterface, mPort);
    if (!result)
    {
        mServer.data()->close();
        mServer.data()->deleteLater();
        mServer = 0;

        emit signalSendTextToLogPrivate(QString("Ошибка запуска сервера!"));

        return -1;
    }

    // Выделение памяти и инициализация необходимых данных
    mClients = QVector<QPointer<QTcpSocket> >(mMaxClients);
    mClientMMF = new MMF* [mMaxClients];
    mClientData = new ClientData [mMaxClients];
    mReceivedData = QVector<QByteArray>(mMaxClients);
    for (int i = 0; i < mMaxClients; i++)
    {
        mClientMMF[i] = 0;
        mClientData[i].count = 0;
        mClientData[i].id.clear();
        mClientData[i].parts.clear();
        mClientData[i].startByte.clear();
        mClientData[i].bytesCount.clear();
        mClientData[i].clientData.clear();
        mClientData[i].isReady = false;
        mReceivedData[i].clear();
    }

    mIsServerRunning = true;

    // Слот - "Новое подключение"
    connect(mServer.data(), SIGNAL(newConnection()),
            this, SLOT(slotAcceptClient()));

    emit signalSendTextToLogPrivate(QString("Сервер запущен. Интерфейс: <b>%1</b>, Порт: <b>%2</b>")
                             .arg(mNetInterface.toString())
                             .arg(mPort));

    mSendDataTimer->start();
    mActivityTimer->start();

    return 0;
}



// Остановка сервера.
int Server::stop()
{
    mActivityTimer->stop();
    mSendDataTimer->stop();

    // Клиентские сокеты
    for (int i = 0; i < mMaxClients; i++)
    {
        if (!mClients[i].isNull())
        {
            // Слот - "Отключение клиента"
            disconnect(mClients[i].data(), SIGNAL(disconnected()),
                       this, SLOT(slotRejectClient()));

            // Слот - "Ошибка сокета"
            disconnect(mClients[i].data(), SIGNAL(error(QAbstractSocket::SocketError)),
                       this, SLOT(slotClientSocketError(QAbstractSocket::SocketError)));

            // Слот - "Получение новых данных"
            disconnect(mClients[i].data(), SIGNAL(readyRead()),
                       this, SLOT(slotReceiveData()));

            mClients[i].data()->close();
            mClients[i].data()->deleteLater();
            mClients[i] = 0;
        }
    }

    // Серверный сокет
    if (!mServer.isNull())
    {
        // Слот - "Новое подключение"
        disconnect(mServer.data(), SIGNAL(newConnection()),
                   this, SLOT(slotAcceptClient()));

        mServer.data()->close();
        mServer.data()->deleteLater();
        mServer = 0;
    }

    // Клиентские MMF
    if (mClientMMF != 0)
    {
        for (int i = 0; i < mMaxClients; i++)
        {
            if (mClientMMF[i] != 0)
            {
                if (mClientMMF[i]->isAttached())
                    mClientMMF[i]->detach();
                mClientMMF[i]->deleteLater();
            }
        }
        delete [] mClientMMF;
        mClientMMF = 0;
    }

    // Клиентские данные
    if (mClientData != 0)
    {
        delete [] mClientData;
        mClientData = 0;
    }

    // Необработанные клиентские данные
    for (int i = 0; i < mMaxClients; i++)
        mReceivedData[i].clear();

    mIsServerRunning = false;
    mActiveClients = 0;

    emit signalSendTextToLogPrivate(QString("Сервер остановлен."));

    return 0;
}



// Установка параметров сервера
int Server::setParameters(QString interface, quint16 port, quint16 maxClients)
{
    mNetInterface = QHostAddress(interface);
    if (mNetInterface == QHostAddress())
        mNetInterface = QHostAddress::Any;

    mPort = port;
    mMaxClients = maxClients;

    return 0;
}



// Запрос списка всех сетевых интерфейсов в системе
QStringList Server::getNetInterfaces()
{
    QList<QHostAddress> netInterfacesList = QNetworkInterface::allAddresses();

    QStringList netInterfacesStringList;
    netInterfacesStringList.append("0.0.0.0");
    foreach (QHostAddress address, netInterfacesList)
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol)
            netInterfacesStringList.append(address.toString());
    }

    return netInterfacesStringList;
}



// Получение состояния сервера
bool Server::isRunning()
{
    return mIsServerRunning;
}



// Подключение клиента к серверному сокету
int Server::slotAcceptClient()
{
    // Превышение максимального количества активных клиентов
    if (mActiveClients == mMaxClients)
    {
        QPointer<QTcpSocket> client = QPointer<QTcpSocket>(mServer.data()->nextPendingConnection());
        client->write(createCommandPackage(CommandType::ConnectionLimit));

        emit signalSendTextToLogPrivate(QString("Перегрузка сервера! Подключение клиента с адреса <b>%1</b> отклонено!")
                                 .arg(client->peerAddress().toString()));
        client->close();
        client->deleteLater();

        return 0;
    }

    bool result = false;
    int clientNumber = 0;
    // Поиск свободного места в массиве для сохранения информации о клиенте
    for (int i = 0; i < mMaxClients; i++)
    {
        if (!mClients[i].isNull())
            continue;
        result = true;
        clientNumber = i;
        break;
    }

    if (!result)
        return -1;

    mClients[clientNumber] = QPointer<QTcpSocket>(mServer.data()->nextPendingConnection());

    // Слот - "Отключение клиента"
    connect(mClients[clientNumber].data(), SIGNAL(disconnected()),
            this, SLOT(slotRejectClient()));

    // Слот - "Ошибка сокета"
    connect(mClients[clientNumber].data(), SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(slotClientSocketError(QAbstractSocket::SocketError)));

    // Слот - "Получение новых данных"
    connect(mClients[clientNumber].data(), SIGNAL(readyRead()),
            this, SLOT(slotReceiveData()));

    mActiveClients++;

    emit signalSendTextToLogPrivate(QString("Подключен клиент №%1 (<b>%2</b>)")
                             .arg(clientNumber)
                             .arg(mClients[clientNumber].data()->peerAddress().toString()));

    // Инициализирующие данные
    if (!mClients[clientNumber].isNull())
        mClients[clientNumber].data()->write(createCommandPackage(CommandType::IsReady));

    return 0;
}



// Отключение клиента
int Server::slotRejectClient()
{
    bool result = false;
    int clientNumber = 0;
    // Поиск клиента, которого требуется отключить
    for (int i = 0; i < mMaxClients; i++)
    {
        if (mClients[i].data() != qobject_cast<QTcpSocket*>(sender()))
            continue;
        result = true;
        clientNumber = i;
        break;
    }

    if (!result)
        return -1;

    return this->disconnectClient(clientNumber);
}



// Отключение и удаление клиента
int Server::disconnectClient(quint16 clientNumber)
{
    // Слот - "Получение новых данных"
    disconnect(mClients[clientNumber].data(), SIGNAL(readyRead()),
               this, SLOT(slotReceiveData()));

    // Слот - "Отключение клиента"
    disconnect(mClients[clientNumber].data(), SIGNAL(disconnected()),
               this, SLOT(slotRejectClient()));

    // Слот - "Ошибка сокета"
    disconnect(mClients[clientNumber].data(), SIGNAL(error(QAbstractSocket::SocketError)),
               this, SLOT(slotClientSocketError(QAbstractSocket::SocketError)));

    if (!mClients[clientNumber].isNull())
    {
        // Сокет
        mClients[clientNumber].data()->close();
        mClients[clientNumber].data()->deleteLater();
        mClients[clientNumber] = 0;
    }

    // MMF
    if (mClientMMF[clientNumber] != 0)
    {
        if (mClientMMF[clientNumber]->isAttached())
            mClientMMF[clientNumber]->detach();
        mClientMMF[clientNumber]->deleteLater();
        mClientMMF[clientNumber] = 0;
    }

    // Принятые от клиента данные
    mReceivedData[clientNumber].clear();

    // Клиентские данные
    mClientData[clientNumber].count = 0;
    mClientData[clientNumber].id.clear();
    mClientData[clientNumber].parts.clear();
    mClientData[clientNumber].startByte.clear();
    mClientData[clientNumber].bytesCount.clear();
    mClientData[clientNumber].clientData.clear();
    mClientData[clientNumber].isReady = false;
    mActiveClients--;

    emit signalSendTextToLogPrivate(QString("Клиент №%1 отключен!").arg(clientNumber));

    return 0;
}



// Чтение данных из MMF и отправка их клиентам
int Server::slotSendData()
{
    // Данные для отправки
    QByteArray data;

    // Перебор подключенных клиентов
    for (int i = 0; i < mMaxClients; i++)
    {
        // Проверка на готовность клиента к приему данных
        if (!mClientData[i].isReady)
            continue;

        // Формирование данных
        data = createDataFromServerPackage(i);

        // Отправка данных
        if (!mClients[i].isNull())
            mClients[i].data()->write(data);
    }

    return 0;
}



// Проверка активности клиентов
int Server::slotCheckActivity()
{
    for (int i = 0; i < mMaxClients; i++)
    {
        // Проверка активности клиентов
        if (!mClients[i].isNull() && mClients[i].data()->state() == QAbstractSocket::ConnectedState)
            mClients[i].data()->write(createCommandPackage(CommandType::IsAlive));
    }

    return 0;
}



// Ответ из БД (описание к параметру элемента)
int Server::slotDBDescriptionResponse(quint16 clientNumber, quint16 requestID,
                                      QString description)
{
    // Формирование данных для отправки клиенту на основе запроса из БД
    QByteArray clientData = createDBDescriptionResponsePackage(requestID,
                                                               description);
    if (!mClients[clientNumber].isNull())
        mClients[clientNumber].data()->write(clientData);

    return 0;
}



// Ответ из БД (масштабный коэффициент)
int Server::slotDBCoefficientResponse(quint16 clientNumber, quint16 requestID,
                                      float coefficient)
{
    // Формирование данных для отправки клиенту на основе запроса из БД
    QByteArray clientData = createDBCoefficientResponsePackage(requestID,
                                                               coefficient);
    if (!mClients[clientNumber].isNull())
        mClients[clientNumber].data()->write(clientData);

    return 0;
}



// Получение данных от клиента, обработка
int Server::slotReceiveData()
{
    bool result = false;
    int clientNumber = 0;
    // Поиск клиента, который выслал данных о MMF
    for (int i = 0; i < mMaxClients; i++)
    {
        if (mClients[i].data() != qobject_cast<QTcpSocket*>(sender()))
            continue;
        result = true;
        clientNumber = i;
        break;
    }

    if (!result)
        return -1;

    quint16 sizeOfPackage = 0;
    quint8 packageType = 0;
    QByteArray package;

    // Парсинг всех пришедших данных
    while (true)
    {
        // Считывание пришедших данных
        if (!mClients[clientNumber].isNull())
            mReceivedData[clientNumber].append(mClients[clientNumber].data()->readAll());

        // Проверка на условие: принято недостаточно данных
        if (mReceivedData[clientNumber].size() < (quint16)sizeof(quint16))
            break;

        // Получение количества отправленных байт (0 - 1 байты)
        memcpy(&sizeOfPackage,
               mReceivedData[clientNumber].data(),
               sizeof(quint16));

        // Проверка на условие: принят не весь пакет целиком
        if (mReceivedData[clientNumber].size() < sizeOfPackage || sizeOfPackage == 0)
            break;

        // Получение типа сообщения (2ой байт)
        memcpy(&packageType,
               mReceivedData[clientNumber].data() + sizeof(quint16),
               sizeof(quint8));

        // Получение основной части пакета
        const int specialData = sizeof(quint16) + sizeof(quint8);
        package = mReceivedData[clientNumber].mid(specialData,
                                                  sizeOfPackage - specialData);

        // Удаление распарсенных данных
        mReceivedData[clientNumber] = mReceivedData[clientNumber].mid(sizeOfPackage);

        // Обработка пакета
        switch (packageType)
        {
        case PackageType::Command:

            result = this->parseCommandPackage(clientNumber, package);
            break;
        case PackageType::RegInfo:
            result = this->parseRegInfoFromServerPackage(clientNumber, package);
            break;
        case PackageType::MMFName:
            result = this->parseMMFPackage(clientNumber, package);
            break;
        case PackageType::DescriptionRequest:
            result = this->parseDBDescriptionRequestPackage(clientNumber, package);
            break;
        case PackageType::CoefficientRequest:
            result = this->parseDBCoefficientRequestPackage(clientNumber, package);
            break;
        }

        if (!result)
            break;
    }

    if (!result)
//        mClients[clientNumber].data()->disconnectFromHost();
        disconnectClient(clientNumber);

    return 0;
}



// Обработка ошибок серверного сокета
int Server::slotClientSocketError(QAbstractSocket::SocketError error)
{
    bool result = false;

    // Поиск клиента, который вызвал ошибку
    for (int i = 0; i < mMaxClients; i++)
    {
        if (mClients[i].data() != qobject_cast<QTcpSocket*>(sender()))
            continue;
        result = true;
        break;
    }

    if (!result)
        return -1;

    emit signalSendTextToLogPrivate(QString("Ошибка клиентского сокета %1 (Удаленный компьютер разорвал соединение)!")
                             .arg(error));

    return 0;
}



// Разбор пакета типа MMFName (MMF, закрепленный за клиентом)
bool Server::parseMMFPackage(quint16 clientNumber, QByteArray data)
{
    bool isExist = mClientMMF[clientNumber] != 0;
    bool isAttached = mClientMMF[clientNumber] != 0 &&
            mClientMMF[clientNumber]->isAttached();
    if (isExist && isAttached)
    {
        emit signalSendTextToLogPrivate(QString("Ошибка! Клиент №%1 - файл, отображаемый в память уже открыт!")
                                 .arg(clientNumber));
        return false;
    }

    mClientMMF[clientNumber] = new MMF(QString(data));
    bool isAttachSuccess = mClientMMF[clientNumber]->attach();

    if (!isAttachSuccess)
    {
        emit signalSendTextToLogPrivate(QString("Ошибка! Клиент №%1 - ошибка при открытии файла, отображаемого в память!")
                                 .arg(clientNumber));
        return false;
    }

    emit signalSendTextToLogPrivate(QString("Клиент №%1 - отображаемый в память файл <b>%2</b> успешно открыт!")
                             .arg(clientNumber).arg(QString(data)));

    return true;
}



// Разбор пакета типа RegInfo (регистрация клиентских элементов на сервере)
bool Server::parseRegInfoFromServerPackage(quint16 clientNumber, QByteArray data)
{
    quint16 id = 0;
    quint16 startByte = 0;
    quint16 bytesCount = 0;
    quint8 partsCount = 0;
    QVector<quint16> startByteList;
    QVector<quint16> bytesCountList;

    // Проверка на некорректный порядок подключения (RegInfo -> MMF) вместо
    // (MMF -> RegInfo)
    if (mClientMMF[clientNumber] == 0)
    {
        emit signalSendTextToLogPrivate(QString("Клиент №%1 - некорректный сетевой протокол!")
                                 .arg(clientNumber));
        return false;
    }

    // Получение идентификатора элемента (2 байта)
    memcpy(&id, data.data(), sizeof(quint16));
    data = data.mid(sizeof(quint16));

    while (data.size() != 0)
    {
        // Получение стартового байта для работы с MMF (2 байта)
        memcpy(&startByte, data.data(), sizeof(quint16));
        // Получение количества байт для работы с MMF (2 байта)
        memcpy(&bytesCount, data.data() + sizeof(quint16), sizeof(quint16));
        // Удаление распарсенных данных
        data = data.mid(2 * sizeof(quint16));

        startByteList.append(startByte);
        bytesCountList.append(bytesCount);
        partsCount++;
    }

    // Создание записи о регистрируемом элементе
    mClientData[clientNumber].id.append(id);
    mClientData[clientNumber].parts.append(partsCount);
    mClientData[clientNumber].startByte.append(startByteList);
    mClientData[clientNumber].bytesCount.append(bytesCountList);
    mClientData[clientNumber].clientData.clear();
    mClientData[clientNumber].count++;

    bool isError = false;

    // Вывод информации о зарегистрированном элементе
    QString infoString = QString("Клиент №%1. Регистрация элемента №%2. Запрашиваемые данные (в байтах): ")
            .arg(clientNumber)
            .arg(id);

    for (int i = 0; i < partsCount; i++)
    {
        if (i != 0)
            infoString += "; ";

        // Определение границ запрашиваемой области данных
        int startByte = startByteList[i];
        int endByte = startByteList[i] + bytesCountList[i] - 1;
        // Проверка, что не запрошена нулевая область данных
        if (endByte < startByte)
            isError = true;

        // Проверка, что запрошенная область не выходит за границы MMF
        if (endByte >= mClientMMF[clientNumber]->size())
            isError = true;

        // Два формата вывода информации о запрашиваемых данных
        if (startByte == endByte)
            infoString += QString("%1").arg(startByteList[i]);
        else
            infoString += QString("%1 - %2").arg(startByte).arg(endByte);
    }

    emit signalSendTextToLogPrivate(infoString);

    // Проверка на отсутствие ошибок при регистрации данных
    if (isError)
    {
        emit signalSendTextToLogPrivate(QString("Клиент №%1, элемент №%2 - ошибка в регистрационных данных!")
                                 .arg(clientNumber)
                                 .arg(id));
        return false;
    }
    else
    {
        return true;
    }
}



// Разбор пакета типа Command (управляющая комманда)
bool Server::parseCommandPackage(quint16 clientNumber, QByteArray data)
{
    quint8 commandCode;

    memcpy(&commandCode, data.data(), sizeof(quint8));

    // Выполнение команды
    switch(commandCode)
    {
    case CommandType::IsReady:
        mClientData[clientNumber].isReady = true;
        mClientData[clientNumber].clientData = QVector<QByteArray>(mClientData[clientNumber].count);
        emit signalSendTextToLogPrivate(QString("Клиент №%1 готов к обмену данными!").arg(clientNumber));
        break;
    case CommandType::IsAlive:
        break;
    }

    return true;
}



// Разбор пакета типа DescriptionRequest
bool Server::parseDBDescriptionRequestPackage(quint16 clientNumber, QByteArray data)
{
    quint16 requestID = 0;
    quint16 startByte = 0;
    quint16 startBit = 0;

    // Байты 0 - 1: идентификатор запроса
    memcpy(&requestID, data.data(), sizeof(quint16));
    // Байты 2 - 3: стартовый байт
    memcpy(&startByte, data.data() + sizeof(quint16), sizeof(quint16));
    // Байты 4 - 5: стартовый бит
    memcpy(&startBit, data.data() + sizeof(quint16) * 2, sizeof(quint16));

    emit signalSendTextToLogPrivate(QString("Клиент №%1, запрос описания параметра: стартовый байт - %2, стартовый бит - %3!")
                             .arg(clientNumber)
                             .arg(startByte)
                             .arg(startBit));

    emit signalDBDescriptionRequest(clientNumber, requestID,
                                    startByte, startBit);

    return true;
}



// Разбор пакета типа CoefficientRequest
bool Server::parseDBCoefficientRequestPackage(quint16 clientNumber, QByteArray data)
{
    quint16 requestID = 0;
    quint16 startByte = 0;
    quint16 startBit = 0;

    // Байты 0 - 1: идентификатор запроса
    memcpy(&requestID, data.data(), sizeof(quint16));
    // Байты 2 - 3: стартовый байт
    memcpy(&startByte, data.data() + sizeof(quint16), sizeof(quint16));
    // Байты 4 - 5: стартовый бит
    memcpy(&startBit, data.data() + sizeof(quint16) * 2, sizeof(quint16));

    emit signalSendTextToLogPrivate(QString("Клиент №%1, запрос масштабного коэффициента: стартовый байт - %2, стартовый бит - %3!")
                             .arg(clientNumber)
                             .arg(startByte)
                             .arg(startBit));

    emit signalDBCoefficientRequest(clientNumber, requestID,
                                    startByte, startBit);

    return true;
}



// Генерация пакета Command
QByteArray Server::createCommandPackage(CommandType::Type commandType)
{
    QByteArray package;
    quint16 sizeOfPackage = sizeof(quint16) + 2 * sizeof(quint8);
    quint8 packageType = PackageType::Command;

    // Байты 0 - 1: размер пакета
    package.append(reinterpret_cast<char*>(&sizeOfPackage),
                   sizeof(quint16));
    // Байт 2: тип пакета
    package.append(reinterpret_cast<char*>(&packageType),
                   sizeof(quint8));
    // Байт 3: код команды
    package.append(reinterpret_cast<char*>(&commandType),
                   sizeof(quint8));

    return package;
}



// Генерация пакета Data
QByteArray Server::createDataFromServerPackage(quint16 clientNumber)
{
    QByteArray data;

    // Перебор всех открытых клиентских элементов
    for (int i = 0; i < mClientData[clientNumber].count; i++)
    {
        QByteArray clientData;

        // Данные для отправки в сыром виде
        for (int j = 0; j < mClientData[clientNumber].parts[i]; j++)
        {
            QByteArray rawData = mClientMMF[clientNumber]->read(mClientData[clientNumber].startByte[i][j],
                                                                mClientData[clientNumber].bytesCount[i][j]);
            clientData.append(rawData);
        }

        // Проверка на изменение данных
        if (clientData == mClientData[clientNumber].clientData[i])
            continue;

        // Размер отправляемых данных
        quint16 sizeOfPackage = 2 * sizeof(quint16) +
                sizeof(quint8) +
                clientData.size();

        // Байты 0 - 1: количество передаваемых байт
        data.append(reinterpret_cast<char*>(&sizeOfPackage),
                    sizeof(quint16));
        // Байт 2: тип передаваемого сообщения
        quint8 packageType = PackageType::Data;
        data.append(reinterpret_cast<char*>(&packageType),
                    sizeof(quint8));
        // Байты 3 - 4: идентификатор элемента
        data.append(reinterpret_cast<char*>(&mClientData[clientNumber].id[i]),
                    sizeof(quint16));
        // Данные из MMF
        data.append(clientData);

        // Кеширование отправленных данных
        mClientData[clientNumber].clientData[i] = clientData;

    }

    return data;
}



// Генерация пакета DescriptionResponse
QByteArray Server::createDBDescriptionResponsePackage(quint16 requestID,
                                                      QString description)
{
    QByteArray package;
    quint16 sizeOfPackage = 2 * sizeof(quint16) + sizeof(quint8) + description.toUtf8().size();
    quint8 packageType = PackageType::DescriptionResponse;

    // Байты 0 - 1: размер пакета
    package.append(reinterpret_cast<char*>(&sizeOfPackage),
                   sizeof(quint16));
    // Байт 2: тип пакета
    package.append(reinterpret_cast<char*>(&packageType),
                   sizeof(quint8));
    // Байт 3 - 4: идентификатор запроса
    package.append(reinterpret_cast<char*>(&requestID),
                   sizeof(quint16));
    // Байт 5 - n: строка в формате UTF-8
    package.append(description.toUtf8());

    return package;
}



// Генерация пакета CoefficientResponse
QByteArray Server::createDBCoefficientResponsePackage(quint16 requestID,
                                                      float coefficient)
{
    QByteArray package;
    quint16 sizeOfPackage = 2 * sizeof(quint16) + sizeof(quint8) + sizeof(float);
    quint8 packageType = PackageType::CoefficientResponse;

    // Байты 0 - 1: размер пакета
    package.append(reinterpret_cast<char*>(&sizeOfPackage),
                   sizeof(quint16));
    // Байт 2: тип пакета
    package.append(reinterpret_cast<char*>(&packageType),
                   sizeof(quint8));
    // Байт 3 - 4: идентификатор запроса
    package.append(reinterpret_cast<char*>(&requestID),
                   sizeof(quint16));
    // Байт 5 - 8: масштабный коэффициент
    package.append(reinterpret_cast<char*>(&coefficient),
                   sizeof(float));

    return package;
}
