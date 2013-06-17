#include "controller.h"



Controller::Controller(int index, QObject *parent) :
    QObject(parent),
    mID(index),
    mIsAutoDBRequests(true),
    mIsForceDBRequests(false),
    mConnectionResult(false),
    mIsControllerChanged(false)
{
    mInterfaceLoader = new InterfaceLoader(index, this);
    mLogicLoader = new LogicLoader(this);

    mMMFName.clear();
    mLoadedElements.clear();
    mElementsLogics.clear();
    mElementsInterfaces.clear();
    mGraphicsItems.clear();

    mRequestID.clear();
    mElementID.clear();
    mParameterID.clear();

    // Сигнал, уведомляющий об изменении файлов в каталоге с модулями
    connect(mLogicLoader,
            SIGNAL(signalModulesListChanged(QStringList)),
            this,
            SLOT(slotModulesListChanged(QStringList)));
}



Controller::~Controller()
{
    mInterfaceLoader->deleteLater();
    mLogicLoader->deleteLater();
}



// Запуск контроллера, координирующего логику и интерфейс
// Возвращаемые значения:
// true - загрузка осуществлена без ошибок
// false - в процессе загрузки возникли ошибки
bool Controller::load()
{
    bool result = false;
    // Загрузка интерфейса из XML-файла
    result = mInterfaceLoader->load();
    // Если интерфейс успешно загружен - получаем данные от загрузчика
    if (result)
    {
        mElementsInterfaces = mInterfaceLoader->elements();
        mGraphicsItems = QVector< QSharedPointer<GraphicsObject> >(mElementsInterfaces.size());
    }

    // Перебираем загруженные элементы интерфейса (ElementInterface) и пытаемся
    // подгрузить для них логику работы (ElementLogic)
    for (int i = 0; i < mElementsInterfaces.count(); i++)
    {
        // Имя подгружаемого модуля
        QString moduleName = mElementsInterfaces[i].data()->moduleName();
        // Указатель на подгружаемый модуль (индекс)
        int index = checkAndLoad(moduleName);
        // Создание элемента для сцены
        mGraphicsItems[i] = QSharedPointer<GraphicsObject>(new GraphicsObject());

        // Если указатель на требуемый ElementLogic корректен, то формируем
        // умный указатель (QSharedPointer) и устанавливаем его базой для
        // ElementInterface и GraphicsItem
        if (index >= 0)
        {
            mElementsInterfaces[i].data()->setElementLogic(mElementsLogics[index]);
            //            mElementsInterfaces[i].data()->setModuleHash(mElementsLogics[index].data()->moduleHash()); // Строка для игнорирования хешей
            mElementsInterfaces[i].data()->checkRange();
            mElementsInterfaces[i].data()->checkCorrect();
            mElementsInterfaces[i].data()->resetState();
            mGraphicsItems[i].data()->setElementLogic(mElementsLogics[index]);
            mGraphicsItems[i].data()->setElementInterface(mElementsInterfaces[i]);
            mGraphicsItems[i].data()->resetState();
        }

        connect(mGraphicsItems[i].data(),
                SIGNAL(signalSendDataToToolTip(QVector<QString>,bool)),
                mElementsInterfaces[i].data(),
                SLOT(slotReceiveDataToToolTip(QVector<QString>,bool)));
        connect(mGraphicsItems[i].data(),
                SIGNAL(signalGraphicsObjectSelect()),
                this,
                SLOT(GraphicsObjectSelected()));
        connect(mGraphicsItems[i].data(),
                SIGNAL(signalGraphicsObjectMoved()),
                this,
                SLOT(slotGraphicsObjectPositionChanged()));
    }

    return result;
}



// Сохранение обновленных в контроллере данных
bool Controller::save()
{
    // Синхронизация положения элементов на экране и в памяти
    for (int i = 0; i < mElementsInterfaces.size(); i++)
    {
        mElementsInterfaces[i].data()->setX(mGraphicsItems[i].data()->x());
        mElementsInterfaces[i].data()->setY(mGraphicsItems[i].data()->y());
    }

    return mInterfaceLoader->save(mElementsInterfaces);
}



bool Controller::remove()
{
    return mInterfaceLoader->remove();
}



// Получение интерфейса элемента с заданным номером
// Ограничения:
// Максимальное количество элементов - 65535
QSharedPointer<ElementInterface> Controller::elementInterface(qint32 elementID)
{
    if (elementID < mElementsInterfaces.size())
        return mElementsInterfaces[elementID];
    else
        return QSharedPointer<ElementInterface>();
}



// Получение интерфейса элемента по названию модуля. Если логика для элемента
// не загружена - осуществляется подгрузка элемента (ElementLogic). На основе
// логики формируется интерфейс, и возвращается из функции. Если запрошенного
// модуля не существует - возвращается пустой объект ElementInterface.
QSharedPointer<ElementInterface> Controller::elementInterface(QString moduleName)
{
    // Указатель на подгружаемый модуль (индекс)
    qint32 index = checkAndLoad(moduleName);

    // Если указатель на требуемый ElementLogic корректен, то формируем
    // умный указатель (QSharedPointer) и создаем на его основе объект
    // ElementInterface. В противном случае - возвращаем пустой объект
    // ElementInterface
    if (index >= 0)
    {
        // Установка указателя в ElementInterface на ElementLogic
        ElementInterface* elementInterface =
                new ElementInterface(mElementsLogics[index]);
        // Установка имени логики для созданного модуля интерфейса
        elementInterface->setModuleName(mLoadedElements[index]);
        return QSharedPointer<ElementInterface>(elementInterface);
    }
    else
    {
        return QSharedPointer<ElementInterface>(new ElementInterface());
    }
}



// Получение элемента графического интерфейса. Создаются при добавлении
// интерфейсного элемента
QSharedPointer<GraphicsObject> Controller::graphicsItem(int itemID)
{
    const bool exp1 = itemID >= 0;
    const bool exp2 = itemID < mGraphicsItems.size();
    if (exp1 && exp2)
        return mGraphicsItems[itemID];
    else
        return QSharedPointer<GraphicsObject>();
}



// Добавление нового элемента интерфейса
int Controller::addElementInterface(QSharedPointer<ElementInterface> elementInterface)
{
    // Дополнение массива интерфейсных модулей
    mElementsInterfaces.append(elementInterface);
    // Дополнение массива GUI модулей
    mGraphicsItems.append(QSharedPointer<GraphicsObject>(new GraphicsObject()));

    // Установка логики для GUI-модуля
    int index = checkAndLoad(elementInterface.data()->moduleName());
    mGraphicsItems[mGraphicsItems.size() - 1].data()->
            setElementLogic(mElementsLogics[index]);
    mGraphicsItems[mGraphicsItems.size() - 1].data()->
            setElementInterface(elementInterface);
    mGraphicsItems[mGraphicsItems.size() - 1].data()->
            setFlag(QGraphicsItem::ItemIsMovable);
    mGraphicsItems[mGraphicsItems.size() - 1].data()->
            resetState();

    connect(mGraphicsItems[mGraphicsItems.size() - 1].data(),
            SIGNAL(signalSendDataToToolTip(QVector<QString>,bool)),
            mElementsInterfaces[mGraphicsItems.size() - 1].data(),
            SLOT(slotReceiveDataToToolTip(QVector<QString>,bool)));
    connect(mGraphicsItems[mGraphicsItems.size() - 1].data(),
            SIGNAL(signalGraphicsObjectSelect()),
            this,
            SLOT(GraphicsObjectSelected()));
    connect(mGraphicsItems[mGraphicsItems.size() - 1].data(),
            SIGNAL(signalGraphicsObject()),
            this,
            SLOT(slotGraphicsObjectPositionChanged()));

    return 0;
}



// Замена элемента интерфейса
int Controller::changeElementInterface(qint32 elementID,
                                       QSharedPointer<ElementInterface> elementInterface)
{
    // Дополнение массива интерфейсных модулей
    mElementsInterfaces[elementID] = elementInterface;

    // Установка логики для GUI-модуля
    int index = checkAndLoad(elementInterface.data()->moduleName());
    mGraphicsItems[elementID].data()->setElementLogic(mElementsLogics[index]);
    mGraphicsItems[elementID].data()->setElementInterface(elementInterface);
    mGraphicsItems[elementID].data()->setFlag(QGraphicsItem::ItemIsMovable);
    mGraphicsItems[elementID].data()->resetState();

    connect(mGraphicsItems[elementID].data(),
            SIGNAL(signalSendDataToToolTip(QVector<QString>,bool)),
            mElementsInterfaces[elementID].data(),
            SLOT(slotReceiveDataToToolTip(QVector<QString>,bool)));
    connect(mGraphicsItems[elementID].data(),
            SIGNAL(signalGraphicsObjectSelect()),
            this,
            SLOT(GraphicsObjectSelected()));
    connect(mGraphicsItems[elementID].data(),
            SIGNAL(signalGraphicsObject()),
            this,
            SLOT(slotGraphicsObjectPositionChanged()));

    return 0;
}



// Удаление элемента интерфейса с заданным номером
int Controller::removeElementInterface(int elementNum)
{
    const bool exp1 = elementNum >= 0;
    const bool exp2 = elementNum < mElementsInterfaces.size();
    if (exp1 && exp2)
    {
        disconnect(mGraphicsItems[elementNum].data(),
                   SIGNAL(signalSendDataToToolTip(QVector<QString>,bool)),
                   mElementsInterfaces[elementNum].data(),
                   SLOT(slotReceiveDataToToolTip(QVector<QString>,bool)));
        disconnect(mGraphicsItems[elementNum].data(),
                   SIGNAL(signalGraphicsObjectSelect()),
                   this,
                   SLOT(GraphicsObjectSelected()));
        disconnect(mGraphicsItems[elementNum].data(),
                   SIGNAL(signalGraphicsObject()),
                   this,
                   SLOT(slotGraphicsObjectPositionChanged()));

        mElementsInterfaces.remove(elementNum);
        mGraphicsItems.remove(elementNum);
    }

    return 0;
}



// Получение количества загруженных элементов (ElementInterface)
// Ограничения:
// Максимальное количество элементов - 65535
int Controller::elementsCount()
{
    return mElementsInterfaces.size();
}



// Установка имени MMF в контроллере. Сохранение обновленных данных будет
// осуществлено при вызове метода save()
int Controller::setMMFName(QString mmfName)
{
    mMMFName = mmfName;
    return 0;
}



// Получение списка доступных для подключения модулей
QStringList Controller::modulesList()
{
    return mLogicLoader->modulesList();
}



// Установка настроек обновления параметров
int Controller::setUpdateParameters(bool isAutoDBRequest, bool isForceDBRequest)
{
    mIsAutoDBRequests = isAutoDBRequest;
    mIsForceDBRequests = isForceDBRequest;
    return 0;
}



int Controller::changeIndex(int index)
{
    mInterfaceLoader->changeIndex(index);
    mInterfaceLoader->save(mElementsInterfaces);
    return 0;
}



int Controller::setProjectDirectory(const QString projectDirectory)
{
    // Установка целевой папки
    mInterfaceLoader->setProjectDirectory(projectDirectory);
    mLogicLoader->setProjectDirectory(projectDirectory);

    return 0;
}



int Controller::copyRequestModules()
{
    bool result = true;
    for (int i = 0; i < mLoadedElements.size(); i++)
        result = mLogicLoader->copyModuleToProjectDirectory(mLoadedElements[i]) && result;

    return result;
}



bool Controller::isControllerChanged()
{
    return mIsControllerChanged;
}



// Генерация и отправка запросов к БД
int Controller::createDatabaseRequests()
{
    mRequestID.clear();
    mElementID.clear();
    mParameterID.clear();

    // Опция получения недостающих параметров отключена
    if (!mIsAutoDBRequests)
        return 0;

    int totalRequests = 0;
    for (int i = 0; i < mElementsInterfaces.size(); i++)
    {
        if (mElementsInterfaces[i].data()->moduleState() == ModuleState::Critical)
            continue;

        qint32 parametersCount = mElementsInterfaces[i].data()->parametersCount();
        for (int j = 0; j < parametersCount; j++)
        {
            if (!mElementsInterfaces[i].data()->isParameterUsed(j))
                continue;

            // Заправшиваем описание параметров в том случае, если его нет...
            bool exp1 = mElementsInterfaces[i].data()->description(j).isEmpty();
            // ...или установлен флаг принудительного обновления
            bool exp2 = mIsForceDBRequests;
            if (exp1 || exp2)
            {
                // Сохранение информации о запросе
                mRequestID.append(totalRequests);
                mElementID.append(i);
                mParameterID.append(j);

                // Если параметр - цифровой, то для запроса информации с сервера используется
                // заранее определенный формат. Бит №0 - достоверность. Бит №1-15 - число.
                // Само число может иметь длину менее 15 разрядов, поэтому для корректной работы,
                // в окне редактирования элемента, необходимо указывать реальное размещение числа в памяти сервера
                quint16 startBit = 0;
                const int defaultStartBitForNumbers = 1;
                if (mElementsInterfaces[i].data()->parameterType(j) == ParameterType::Number)
                    startBit = defaultStartBitForNumbers;
                else
                    startBit = mElementsInterfaces[i].data()->startBit(j);

                emit signalDatabaseRequest(mRequestID[totalRequests],
                                           mElementsInterfaces[i].data()->startByte(j),
                                           startBit);
                totalRequests++;
            }
        }
    }

    return totalRequests;
}



// Проверка существования модуля. В случае его отсутствия - попытка загрузки
int Controller::checkAndLoad(QString moduleName)
{
    // Указатель на подгружаемый модуль (индекс)
    qint32 index = mLoadedElements.indexOf(moduleName);

    // Если модуль не был загружен ранее (index < 0) - загружаем его
    if (index < 0)
    {
        bool result = false;
        QSharedPointer<ElementLogic> elementLogic = mLogicLoader->loadModule(moduleName,
                                                                             result);
        // Если загрузка модуля прошла успешно, сохраняем его в памяти,
        // корректируем значение указателя (index)
        if (result)
        {
            mLoadedElements.append(moduleName);
            mElementsLogics.append(elementLogic);
            index = mLoadedElements.size() - 1;
        }
    }

    return index;
}



// Обработка результата подключения к серверу
int Controller::slotConnectionResult(bool result)
{
    mConnectionResult = result;

    // Подключение установлено
    if (result)
    {
        // Регистрация MMF
        emit signalRegisterMMF(mMMFName);

        // Запрос к БД
        int totalRequests = this->createDatabaseRequests();
        // Если к БД не отправлен ни один запрос - асинхронно разрываем соединение с сервером
        const int requestTimeout = 1000;
        if (totalRequests == 0)
            QTimer::singleShot(requestTimeout, this, SIGNAL(signalDisconnectClient()));
    }
    // Подключение не установлено
    else
    {
        // Сброс состояния всех элементов
        int size = mElementsInterfaces.size();
        for (int i = 0; i < size; i++)
        {
            mElementsInterfaces[i].data()->resetState();
            mGraphicsItems[i].data()->resetState();
        }
    }

    return 0;
}



// Обработка события "Получение описания к элементам"
int Controller::slotReceiveDescription(quint16 requestID, QString description)
{
    // Номер информационной записи о запросе
    qint32 index = mRequestID.indexOf(requestID);
    // Идентификатор элемента
    qint32 elementID = mElementID[index];
    // Идентификатор параметра
    qint32 parameterID = mParameterID[index];
    // Дополнительные данные
    quint16 startByte = mElementsInterfaces[elementID].data()->startByte(parameterID);
    quint16 startBit = mElementsInterfaces[elementID].data()->startBit(parameterID);
    quint16 bitsCount = mElementsInterfaces[elementID].data()->bitsCount(parameterID);
    bool isUsed = mElementsInterfaces[elementID].data()->isParameterUsed(parameterID);
    // Изменение параметра в интерфейсном элементе (ElementInterface)
    mElementsInterfaces[elementID].data()->changeParameter(parameterID, startByte,
                                                           startBit, bitsCount,
                                                           description, isUsed);
    // Удаление записи о выполненном запросе
    mRequestID.remove(index);
    mElementID.remove(index);
    mParameterID.remove(index);

    // Если все запросы для элемента elementID выполнены - обновление иконки
    // в таблице с элементами,
    if (mElementID.indexOf(elementID) < 0)
        emit signalElementInterfaceChanged(mID, elementID);

    // Сохранение данных после обработки всех запросов
    if (mRequestID.size() == 0)
    {
        this->save();
        emit signalDisconnectClient();
    }

    return 0;
}



// Обработка события "Изменение списка модулей в папке"
int Controller::slotModulesListChanged(QStringList modulesList)
{
    emit signalModulesListChanged(mID, modulesList);

    // Корректировка интерфейсных модулей
    QString moduleName;
    for (int i = 0; i < mElementsInterfaces.size(); i++)
    {
        moduleName = mElementsInterfaces[i].data()->moduleName();
        if (modulesList.indexOf(moduleName) < 0)
        {
            mElementsInterfaces[i].data()->setElementLogic(QSharedPointer<ElementLogic>());
            emit signalElementInterfaceChanged(mID, i);
        }
    }

    // Выборка некорректных модулей к удалению
    QVector<int> elementsToDelete;
    for (int i = 0; i < mLoadedElements.size(); i++)
    {
        moduleName = mLoadedElements[i];
        if (modulesList.indexOf(moduleName) < 0)
            elementsToDelete.append(i);
    }

    // Выгрузка модулей (удаление с конца)
    for (int i = 0; i < elementsToDelete.size(); i++)
    {
        mLoadedElements.remove(elementsToDelete[i] - i);
        mElementsLogics.remove(elementsToDelete[i] - i);
    }

    return 0;
}



int Controller::GraphicsObjectSelected()
{
    int elementNum = -1;
    for (int i = 0; i < mGraphicsItems.size(); i++)
    {
        if (mGraphicsItems[i].data() == dynamic_cast<GraphicsObject*>(this->sender()))
        {
            elementNum = i;
            break;
        }
    }

    if (elementNum >= 0)
        emit signalGraphicsObjectSelect(elementNum);

    return 0;
}



int Controller::slotGraphicsObjectPositionChanged()
{
    mIsControllerChanged = true;
    return 0;
}
