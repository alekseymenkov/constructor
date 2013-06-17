#ifndef ELEMENTLOGIC_H
#define ELEMENTLOGIC_H



#include <QtCore>
#include "constants.h"
#include "parameter.h"
#include "figure.h"
#include "record.h"
#include <QtSvg>



class ElementLogic
{

public:
    ElementLogic();
    // Формирование логики
    int addParameter(Parameter parameter);
    int addFigure(Figure figure);
    int addRecord(Record record);
    int addDefaultRecord(Record defaultRecord);
    int addExtraParameter(QString parameterName,
                          ExtraParameterType::Type parameterType,
                          ExtraUIElementType::Type uiType,
                          QVariant defaultValue = QVariant());
    // Получение необходимого элемента
    Parameter parameter(qint32 parameterID);
    Figure figure(qint32 figureID);
    Record record(qint32 recordID);
    Record defaultRecord(qint32 defaultRecordID);
    QString extraParameterName(qint32 id);
    QVariant extraParameterValue(qint32 id);
    ExtraParameterType::Type extraParameterType(qint32 id);
    ExtraUIElementType::Type extraUIElementType(qint32 id);
    // Количество элементов
    qint32 parametersCount();
    qint32 figuresCount();
    qint32 recordsCount();
    qint32 defaultRecordsCount();
    qint32 extraParametersCount();
    // Тип логического элемента
    int setElementLogicType(ElementLogicType::Type type);
    ElementLogicType::Type elementLogicType();
    // Стартовый угол
    int setStartAngle(quint16 startAngle);
    quint16 startAngle();
    // Размер
    int setSize(QSize size);
    QSize size();
    // Размер области предупреждения
    int setWarningSize(QSize warningSize);
    QSize warningSize();
    // Хеш модуля
    int setModuleHash(QString moduleHash);
    QString moduleHash();
    // Имя модуля
    int setModuleName(QString moduleName);
    QString moduleName();
    // Справочное изображение
    int setPreviewImagePath(QString previewImagePath);
    QString previewImagePath();
    // Функции для сериализации и десериализации объекта
    friend QDataStream& operator>>(QDataStream& dataStream, ElementLogic& element);
    friend QDataStream& operator<<(QDataStream& dataStream, const ElementLogic& element);

    // Очистка всех данных
    int clear();



private:
    // Запрет копирования объекта
    void operator=(const ElementLogic& elementLogic);
    // Логика элемента
    QVector<Parameter> mParameters;
    QVector<Figure> mFigures;
    QVector<Record> mRecords;
    QVector<Record> mDefaultRecords;
    // Хеш модуля в BASE-64
    QString mModuleHash;
    // Имя модуля в UTF-8
    QString mModuleName;
    // Картинка-описание
    QString mPreviewImagePath;
    // Стартовый угол поворота элемента
    quint16 mStartAngle;
    // Размер элемента
    QSize mSize;
    // Размер области предупреждения
    QSize mWarningSize;
    // Названия дополнительных параметров
    QVector<QString> mExtraParametersName;
    // Тип дополнительных параметров
    QVector<ExtraParameterType::Type> mExtraParametersType;
    // Тип элементов с дополнительными параметрами
    QVector<ExtraUIElementType::Type> mExtraUIElementType;
    // Параметры по умолчанию
    QVector<QVariant> mDefaultExtraValues;
    // Тип логического параметра
    ElementLogicType::Type mElementLogicType;
};
Q_DECLARE_METATYPE(ElementLogic)



#endif // ELEMENTLOGIC_H
