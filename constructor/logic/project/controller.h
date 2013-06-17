#ifndef CONTROLLER_H
#define CONTROLLER_H



#include <QtCore>
#include "../../../constructor-player/project/interfaceloader.h"
#include "../../../constructor-player/project/logicloader.h"
#include "../../../constructor-player/ui/graphicsobject.h"
#include "../../../constructor-player/module/elementinterface.h"
#include "../../../constructor-player/module/elementlogic.h"



class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(int index, QObject* parent = 0);
    ~Controller();
    // Загрузка элементов логики (ElementLogic) и интерфейса (ElementInterface)
    bool load();
    // Сохранение элементов интерфейса (ElementInterface)
    bool save();
    // Удаление файла с настройками элементов интерфейса
    bool remove();
    // Получение количества загруженных элементов (ElementInterface)
    int elementsCount();
    // Получение элемента интерфейса по номеру
    QSharedPointer<ElementInterface> elementInterface(qint32 elementID);
    // Получение нового элемента интерфейса по названию модуля
    QSharedPointer<ElementInterface> elementInterface(QString moduleName);
    // Получение нового графического элемента по номеру
    QSharedPointer<GraphicsObject> graphicsItem(int itemID);
    // Добавление нового элемента интерфейса и создание графического элемента
    int addElementInterface(QSharedPointer<ElementInterface> elementInterface);
    // Замена элемента интерфейса
    int changeElementInterface(qint32 elementID, QSharedPointer<ElementInterface> elementInterface);
    // Удаление элемента интерфейса
    int removeElementInterface(int elementNum);
    // Установка имени MMF для проекта
    int setMMFName(QString mmfName);
    // Получение списка доступных модулей (ElementLogic)
    QStringList modulesList();
    // Установка настроек обновления параметров
    int setUpdateParameters(bool isAutoDBRequest, bool isForceDBRequest);
    // Смена индекса
    int changeIndex(int index);
    // Смена директории с проектом
    int setProjectDirectory(const QString projectDirectory);
    // Копирование требуемых для проекта модулей
    int copyRequestModules();
    // Изменены ли данные в контроллере?
    bool isControllerChanged();


private:
    // Идентификатор экземпляра контроллера
    int mID;
    // Константы
    static const int maxElementsCount = 65535;
    // Загрузчик элементов интерфейса из XML
    InterfaceLoader* mInterfaceLoader;
    // Загрузчик элементов логики из .bin-файлов
    LogicLoader* mLogicLoader;
    // Имя MMF, полученное из InterfaceLoader
    QString mMMFName;
    // Вектор названий загруженных модулей (ElementLogic)
    QVector<QString> mLoadedElements;
    // Вектор загруженных модулей (ElementLogic) и указателей на них
    QVector< QSharedPointer<ElementLogic> > mElementsLogics;
    // Вектор загруженных элементов интерфейса (ElementInterface)
    QVector< QSharedPointer<ElementInterface> > mElementsInterfaces;
    // Вектор графических элементов (GraphicsItem)
    QVector< QSharedPointer<GraphicsObject> > mGraphicsItems;

    // Данные, необходимые для генерации и последующей идентификации
    // запросов к БД сервера
    // Идентификатор запроса
    QVector<quint16> mRequestID;
    // Идентификатор элемента
    QVector<quint16> mElementID;
    // Идентификатор параметра
    QVector<quint16> mParameterID;

    // Флаги обновления описаний параметров
    // Автоматический запрос недостающих данных с сервера
    bool mIsAutoDBRequests;
    // Принудительный запрос всех данных с сервера
    bool mIsForceDBRequests;

    // Флаг подключения к серверу
    bool mConnectionResult;

    // Флаг - изменены ли данные
    bool mIsControllerChanged;

    // Генерация запросов к серверу
    int createDatabaseRequests();
    // Функция, проверяющая, загружен ли модуль ElementLogic. Если не загружен - загрузить
    // Возвращает индекс модуля в массиве
    int checkAndLoad(QString moduleName);


public slots:
    // Обработчик события "Обработка результат подключения к серверу"
    int slotConnectionResult(bool result);
    // Обработчик события "Получение описания к элементам"
    int slotReceiveDescription(quint16 requestID, QString description);
    // Обработчик события "Изменение списка модулей в папке"
    int slotModulesListChanged(QStringList modulesList);
    // Обработчик события "Графический элемент выбран"
    int GraphicsObjectSelected();
    // Обработчик события - данные в контроллере изменены
    int slotGraphicsObjectPositionChanged();


signals:
    // Сигнал - "Изменение списка модулей в папке"
    int signalModulesListChanged(int id, QStringList modulesList);
    // Сигнал - "Изменение состояния элемента"
    int signalElementInterfaceChanged(int id, int elementID);
    // Сигнал - "Отправка запроса на получение описаний к серверу"
    int signalDatabaseRequest(quint16 requestID,
                              quint16 startByte,
                              quint16 startBit);
    // Сиганл - "Отправка серверу регистрационных данных (данные MMF)"
    int signalRegisterMMF(QString mmfName);
    // Сигнал - "Отключить клиентское приложение от сервера"
    int signalDisconnectClient();
    // Сигнал - "Выбрать элемент в таблице"
    int signalGraphicsObjectSelect(int elementNum);
};



#endif // CONTROLLER_H
