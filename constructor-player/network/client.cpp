#include "client.h"



Client::Client(QObject* parent) :
    QObject(parent),
    mCurrentConnectionResult(false),
    mOldConnectionResult(false)
{
    mClientSocket = new QTcpSocket(this);

    // Таймер
    mActivityTimer = new QTimer(this);
    mActivityTimer->setInterval(mActivityInterval);

    // Стол - "Функция проверки активности сервера"
    connect(mActivityTimer,
	    SIGNAL(timeout()),
	    this,
	    SLOT(slotCheckActivity()));

    // Слот - "Ошибка сокета"
    connect(mClientSocket,
	    SIGNAL(error(QAbstractSocket::SocketError)),
	    this,
	    SLOT(slotSocketError(QAbstractSocket::SocketError)));

    // Слот - "Клиент подключен к серверу"
    connect(mClientSocket,
	    SIGNAL(connected()),
	    this,
	    SLOT(slotClientConnected()));

    // Слот - "Новые данные"
    connect(mClientSocket,
	    SIGNAL(readyRead()),
	    this,
	    SLOT(slotReceiveData()));
}



Client::~Client()
{
    // Стол - "Функция проверки активности сервера"
    disconnect(mActivityTimer,
	       SIGNAL(timeout()),
	       this,
	       SLOT(slotCheckActivity()));

    // Слот - "Ошибка сокета"
    disconnect(mClientSocket,
	       SIGNAL(error(QAbstractSocket::SocketError)),
	       this,
	       SLOT(slotSocketError(QAbstractSocket::SocketError)));

    // Слот - "Клиент подключен к серверу"
    disconnect(mClientSocket,
	       SIGNAL(connected()),
	       this,
	       SLOT(slotClientConnected()));

    // Слот - "Новые данные"
    disconnect(mClientSocket,
	       SIGNAL(readyRead()),
	       this,
	       SLOT(slotReceiveData()));

    disconnectFromServer();
}



// Подключение к серверу
int Client::connectToServer()
{
    if (mClientSocket->state() != QAbstractSocket::UnconnectedState)
	return -1;

    mClientSocket->connectToHost(mServerAddress, mPort);

    return 0;
}



// Отправка пакета с данными о MMF
int Client::slotSendMMFPackage(QString MMFName)
{
    if (mClientSocket == 0)
	return -1;

    mClientSocket->write(createMMFPackage(MMFName));
    return 0;
}



// Отправка пакета с данными о регистрируемом элементе
int Client::slotSendRegInfoPackage(quint16 elementID,
				   QVector<quint16> startBytes,
				   QVector<quint16> bytesCount)
{
    if (mClientSocket == 0)
	return -1;

    mClientSocket->write(createRegInfoPackage(elementID, startBytes, bytesCount));
    return 0;
}



// Отправка запроса на получение описания к элементу
int Client::slotSendDBRequestPackage(quint16 requestID,
				     quint16 startByte,
				     quint16 startBit)
{
    if (mClientSocket == 0)
	return -1;

    mClientSocket->write(createDBRequestPackage(requestID, startByte, startBit));
    return 0;
}



// Отправка серверу уведомленя о том, что клиент готов к получению данных
int Client::slotSendClientIsReady()
{
    if (mClientSocket == 0)
	return -1;

    mClientSocket->write(createCommandPackage(CommandType::IsReady));
    return 0;
}



// Отключение от сервера
int Client::disconnectFromServer()
{
    mActivityTimer->stop();
    mClientSocket->close();

    mOldConnectionResult = mCurrentConnectionResult;
    mCurrentConnectionResult = false;

    emit signalConnectionResult(false);

    return 0;
}



// Установка параметров клиентского сокета
int Client::setParameters(QString serverAddress, quint16 port)
{
    mPort = port;
    mServerAddress = serverAddress;

    return 0;
}



// Получение состояния клиента
bool Client::isConnected()
{
    if (mClientSocket->state() == QAbstractSocket::ConnectedState)
	return true;
    else
	return false;
}



// Результат предыдущего подключения
bool Client::oldConnectionResult()
{
    return mOldConnectionResult;
}



// Все параметры для подключения к серверу корректны
bool Client::isParametersCorrect()
{
    bool isServerAddressCorrect = !mServerAddress.isEmpty();
    bool isPortCorrect =  mPort != 0;

    return isServerAddressCorrect && isPortCorrect;
}



// Адрес сервера
QString Client::serverAddress()
{
    return mServerAddress;
}



// Порт
quint16 Client::port()
{
    return mPort;
}



// Генерация пакета "Команда"
QByteArray Client::createCommandPackage(CommandType::Type commandType)
{
    QByteArray package;
    quint16 sizeOfPackage = sizeof(quint16) + 2 * sizeof(quint8);
    quint8 packageType = PackageType::Command;

    package.append(reinterpret_cast<char*>(&sizeOfPackage),
		   sizeof(quint16));

    package.append(reinterpret_cast<char*>(&packageType),
		   sizeof(quint8));

    package.append(reinterpret_cast<char*>(&commandType),
		   sizeof(quint8));

    return package;
}



// Генерация пакета "Имя MMF"
QByteArray Client::createMMFPackage(QString MMFName)
{
    QByteArray package;
    quint16 sizeOfPackage = sizeof(quint16) + sizeof(quint8) + MMFName.size();
    quint8 packageType = PackageType::MMFName;

    package.append(reinterpret_cast<char*>(&sizeOfPackage),
		   sizeof(quint16));

    package.append(reinterpret_cast<char*>(&packageType),
		   sizeof(quint8));

    package.append(MMFName);

    return package;
}



// Генерация пакета "Регистрационная информация"
QByteArray Client::createRegInfoPackage(quint16 elementID,
					QVector<quint16> startBytes,
					QVector<quint16> bytesCount)
{
    QByteArray package;
    quint8 packageType = PackageType::RegInfo;
    quint8 partsCount = startBytes.count();
    quint16 sizeOfPackage = sizeof(quint16) * 2 + sizeof(quint8) + 4 * partsCount;

    package.append(reinterpret_cast<char*>(&sizeOfPackage),
		   sizeof(quint16));

    package.append(reinterpret_cast<char*>(&packageType),
		   sizeof(quint8));

    package.append(reinterpret_cast<char*>(&elementID),
		   sizeof(quint16));

    for (int i = 0; i < partsCount; i++)
    {
	package.append(reinterpret_cast<char*>(&startBytes[i]),
		       sizeof(quint16));

	package.append(reinterpret_cast<char*>(&bytesCount[i]),
		       sizeof(quint16));
    }

    return package;
}

QByteArray Client::createDBCoefficientRequestPackage(quint16 requestID, quint16 startByte, quint16 startBit)
{
    QByteArray package;
    quint16 sizeOfPackage = 4 * sizeof(quint16) + sizeof(quint8);
    quint8 packageType = PackageType::CoefficientRequest;

    // Байты 0 - 1: размер пакета
    package.append(reinterpret_cast<char*>(&sizeOfPackage),
		   sizeof(quint16));
    // Байт 2: тип пакета
    package.append(reinterpret_cast<char*>(&packageType),
		   sizeof(quint8));
    // Байт 3 - 4: идентификатор запроса
    package.append(reinterpret_cast<char*>(&requestID),
		   sizeof(quint16));
    // Байт 5 - 6: стартовый байт
    package.append(reinterpret_cast<char*>(&startByte),
		   sizeof(quint16));
    // Байт 7 - 8: стартовый бит
    package.append(reinterpret_cast<char*>(&startBit),
		   sizeof(quint16));

    return package;
}



// Генерация пакета "Запрос к БД"
QByteArray Client::createDBRequestPackage(quint16 requestID, quint16 startByte,
					  quint16 startBit)
{
    QByteArray package;
    quint16 sizeOfPackage = 4 * sizeof(quint16) + sizeof(quint8);
    quint8 packageType = PackageType::DescriptionRequest;

    // Байты 0 - 1: размер пакета
    package.append(reinterpret_cast<char*>(&sizeOfPackage),
		   sizeof(quint16));
    // Байт 2: тип пакета
    package.append(reinterpret_cast<char*>(&packageType),
		   sizeof(quint8));
    // Байт 3 - 4: идентификатор запроса
    package.append(reinterpret_cast<char*>(&requestID),
		   sizeof(quint16));
    // Байт 5 - 6: стартовый байт
    package.append(reinterpret_cast<char*>(&startByte),
		   sizeof(quint16));
    // Байт 7 - 8: стартовый бит
    package.append(reinterpret_cast<char*>(&startBit),
		   sizeof(quint16));

    return package;
}



// Разбор пакета "Команда"
bool Client::parseCommandPackage(QByteArray data)
{
    quint8 commandCode;
    memcpy(&commandCode, data.data(), sizeof(quint8));

    // Выполнение команды
    switch(commandCode)
    {
    case CommandType::IsAlive:
	break;
    case CommandType::IsReady:
	emit signalConnectionResult(true);
	break;
    case CommandType::ConnectionLimit:
	break;
    }

    return true;
}



// Разбор пакета "Данные"
bool Client::parseDataPackage(QByteArray package)
{
    quint16 elementID = 0;
    // Байты 0 - 1: идентификатор элемента
    memcpy(&elementID, package.data(), sizeof(quint16));
    // Байты 2 - n: данные из MMF
    package = package.mid(sizeof(quint16));

    emit signalSendDataToElement(elementID, package);

    return true;
}



// Разбор пакета "Запрос к БД"
bool Client::parseDBResponsePackage(QByteArray package)
{
    quint16 requestID = 0;
    // Байты 1 - 2: идентификатор запроса
    memcpy(&requestID, package.data(), sizeof(quint16));
    // Байты 3 - n: строка в формате UTF-8
    package = package.mid(sizeof(quint16));

    emit signalSendDescriptionToElement(requestID, QString(package));

    return true;
}



// Получение данных от сервера
int Client::slotReceiveData()
{
    bool result = false;
    QByteArray data;
    quint16 sizeOfPackage = 0;
    quint8 packageType = 0;

    while (true)
    {
	if (mClientSocket != 0)
	    mReceivedData.append(mClientSocket->readAll());

	if (mReceivedData.size() < (quint16)sizeof(quint16))
	    break;

	memcpy(&sizeOfPackage,
	       mReceivedData.data(),
	       sizeof(quint16));

	if (mReceivedData.size() < sizeOfPackage || sizeOfPackage == 0)
	    break;

	memcpy(&packageType,
	       mReceivedData.data() + sizeof(quint16),
	       sizeof (quint8));

	const quint16 specialData = sizeof(quint16) + sizeof(quint8);
	data = mReceivedData.mid(specialData,
				 sizeOfPackage - specialData);

	mReceivedData = mReceivedData.mid(sizeOfPackage);

	// Обработка пакета
	switch (packageType)
	{
	case PackageType::Command:
	    result = this->parseCommandPackage(data);
	    break;
	case PackageType::Data:
	    result = this->parseDataPackage(data);
	    break;
	case PackageType::DescriptionResponse:
	    result = this->parseDBResponsePackage(data);
	    break;
	default:
	    result = true;
	}

	if (!result)
	    break;
    }

    if (!result)
	disconnectFromServer();

    return 0;
}



// Слот - "Превышено время ожидания ответа от сервера"
int Client::slotCheckActivity()
{
    mClientSocket->write(createCommandPackage(CommandType::IsAlive));

    return 0;
}



// Слот - "Ошибка клиентского сокета"
int Client::slotSocketError(QAbstractSocket::SocketError)
{
    disconnectFromServer();

    return 0;
}



// Слот - "Подключение клиента"
int Client::slotClientConnected()
{
    mReceivedData.clear();
    mActivityTimer->start();

    mOldConnectionResult = mCurrentConnectionResult;
    mCurrentConnectionResult = true;

    return 0;
}



// Обработка события "Отключение клиента"
int Client::slotClientDisconnected()
{
    disconnectFromServer();

    return 0;
}



// Обработчик сигнала "Отключиться от сервера"
int Client::slotDisconnectClient()
{
    if (this->isConnected())
	this->disconnectFromServer();

    return 0;
}
