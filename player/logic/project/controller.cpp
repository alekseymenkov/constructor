#include "controller.h"



Controller::Controller(int index, QObject *parent) :
    QObject(parent),
    mID(index),
    mIsAutoDBRequests(true),
    mIsForceDBRequests(false),
    mConnectionResult(false)
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



int Controller::setProjectDirectory(const QString projectDirectory)
{
    // Установка целевой папки
    mInterfaceLoader->setProjectDirectory(projectDirectory);
    mLogicLoader->setProjectDirectory(projectDirectory);

    return 0;
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
    }

    return result;
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

        // Регистрация элементов
        for (int i = 0; i < mElementsInterfaces.size(); i++)
        {
            if (mElementsInterfaces[i].data()->moduleState() == ModuleState::Critical)
                continue;

            QVector<quint16> startBytes;
            startBytes.append(mElementsInterfaces[i].data()->startByte());
            QVector<quint16> bytesCount;
            bytesCount.append(mElementsInterfaces[i].data()->bytesCount());

            emit signalRegisterElement(i, startBytes, bytesCount);
        }

        // FIX: Сделать отправку сигнала IsReady только при получении ответа на все запросы
        emit signalIsReady();
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



// Обработка события "Получение данных от сервера"
int Controller::slotReceiveData(quint16 elementID, QByteArray data)
{
    // Преобразование QByteArray в QVector<bool>
    QVector<bool> state;
    for (int i = 0; i < data.size(); i++)
    {
        const int bitsInByte = 8;
        for (int j = 0; j < bitsInByte; j++)
        {
            int value = 1 << j;
            if ((data[i] & value) > 0)
                state.append(true);
            else
                state.append(false);
        }
    }

    // Сознательное дублирование данных
    // Для исключения генерации всплывающей подсказки в GraphicsItem
    mGraphicsItems[elementID].data()->setState(state, true);

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
