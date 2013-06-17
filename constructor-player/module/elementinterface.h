#ifndef ELEMENTINTERFACE_H
#define ELEMENTINTERFACE_H



#include <QtCore>
#include <QCursor>
#include "constants.h"
#include "elementlogic.h"



class ElementInterface : public QObject
{
    Q_OBJECT

public:
    ElementInterface(QObject* object = 0);
    ElementInterface(QSharedPointer<ElementLogic> elementLogic, QObject* object = 0);
    ElementInterface(const ElementInterface& elementInterface);
    // Добавление параметра
    bool addParameter(quint16 startByte, quint8 startBit,
                      quint16 bitsCount, QString description, bool isUsed);
    // Изменение параметра
    bool changeParameter(qint32 parameterID, quint16 startByte,
                         quint8 startBit, quint16 bitsCount,
                         QString description, bool isUsed);
    // Размер
    QSize size();
    int setSize(QSize size);
    // Модификатор размера
    int setSizeModificator(qreal sizeModificator);
    qreal sizeModificator();
    // Название файла с модулем
    int setModuleName(QString moduleName);
    QString moduleName();
    QString fullModuleName();
    // Хеш модуля
    int setModuleHash(QString ModuleHash);
    QString moduleHash();
    // Угол поворота элемента
    int setStartAngle(qint16 angle);
    qint16 startAngle();
    // Название элемента
    int setElementName(QString elementName);
    QString elementName();
    // Количество параметров
    qint32 parametersCount();
    // Актуальное количество параметров
    qint32 actualParametersCount();
    // Установка состояния элемента
    int setModuleState(ModuleState::State moduleState);
    ModuleState::State moduleState();
    // Диапазон запрашиваемых данных (результат по всем параметрам)
    quint16 startByte();
    quint16 bytesCount();
    // Координаты
    int setX(qint32 x);
    qint32 x();
    int setY(qint32 y);
    qint32 y();
    // Установка состояния элемента
    int setState(QVector<bool> state);
    int resetState();
    // Параметры запрашиваемых байтов и битов
    quint16 startByte(qint32 parameterNum);
    quint8 startBit(qint32 parameterNum);
    quint16 bitsCount(qint32 parameterNum);
    QString description(qint32 parameterNum);
    ParameterType::Type parameterType(qint32 parameterNum);
    bool isParameterCorrect(qint32 parameterNum);
    bool isParameterUsed(qint32 parameterNum);
    // Проверка диапазона необходимых данных
    int checkRange();
    // Очистка всех данных в элементе
    int clear();
    // Проверка корректности элемента
    int checkCorrect();
    // Установка логики для элемента
    int setElementLogic(QSharedPointer<ElementLogic> elementLogic);
    // Функции для работы со всплывающей подсказкой
    int generateToolTip(bool isConflict);
    bool isToolTipShow();
    // Дополнительные параметры
    int addExtraParameter(QVariant parameter);
    int setExtraParameter(qint32 parameterID, QVariant parameter);
    // Значение дополнительного параметра
    QVariant extraParameter(qint32 parameterID);
    QVariant defaultExtraParameter(qint32 parameterID);
    // Количество дополнительных параметров
    qint32 extraParametersCount();
    // Название дополнительного параметра
    QString extraParameterName(qint32 id);
    // Тип дополнительного параметра
    ExtraParameterType::Type extraParameterType(qint32 id);
    // Тип элемента UI, связанного с параметров
    ExtraUIElementType::Type extraUIElementType(qint32 id);
    // Справочное изображения
    QString previewImagePath();
    // Тип логического элемента
    ElementLogicType::Type elementLogicType();

private:
    // Запрет копирования объекта
    void operator=(const ElementInterface& elementInterface);
    // Данные о запрашиваемых байтах и битах
    QVector<quint16> mStartBytes;
    QVector<quint8> mStartBits;
    QVector<quint16> mBitsCount;
    QVector<QString> mDescriptions;
    QVector<bool> mIsParameterCorrect;
    QVector<bool> mIsParameterUsed;
    // Диапазон запрашиваемых данных
    quint16 mMinByte;
    quint16 mMaxByte;
    // Состояние элемента
    QVector<bool> mState;
    // Координаты элемента
    qint32 mX;
    qint32 mY;
    // Размер
    QSize mSize;
    // Указатель на логику работы указателя
    QSharedPointer<ElementLogic> mElementLogic;
    // Количество параметров (фигур)
    qint32 mParametersCount;
    // Реальное количество параметров
    qint32 mActualParametersCount;
    // Модификатор размера
    qreal mSizeModificator;
    // Имя файла с модулем
    QString mModuleName;
    // Хеш модуля
    QString mModuleHash;
    // Имя элемента
    QString mElementName;
    // Угол поворота элемента
    qint16 mAngle;
    // Работоспособность модуля
    ModuleState::State mModuleState;
    // Дополнительные параметры
    QVector<QVariant> mExtraParameters;
    // Координаты всплывающей подсказки
    QPoint mMousePosition;
    // Флаг отображния всплывающей подсказки
    bool mIsToolTipShow;


signals:
    // Первичная отправка информации
    int signalSendToopTipInfo(QString name,
                              int count,
                              const QVector<QString> bits,
                              const QVector<QString> values,
                              const QVector<QString> description,
                              QPoint pos);
    // Повторное получение информации
    int signalSendToolTipInfo(QString name,
                              int count,
                              const QVector<QString> values);


public slots:
    // Слот - получение данных для генерации всплывающей подсказки
    int slotReceiveDataToToolTip(QVector<QString> parametersValues,
                                 bool isConflict);
    // Слот - всплывающая подсказка скрыта
    int slotToolTipHide();
};



#endif // ELEMENTINTERFACE_H
