#ifndef INTERFACELOADER_H
#define INTERFACELOADER_H



#include <QObject>
#include <QtXml>
#include "../module/elementinterface.h"



class InterfaceLoader : public QObject
{
    Q_OBJECT

public:
    explicit InterfaceLoader(int index, QObject *parent = 0);
    // Загрузка настроек из файла
    bool load();
    // Сохранение настроек в файл
    bool save(QVector<QSharedPointer <ElementInterface> > elementsIterfaces);
    // Удаление файла настроек
    bool remove();
    // Получение вектора интерфейсных модулей
    QVector<QSharedPointer <ElementInterface> > elements();
    // Смена индекса
    int changeIndex(int index);
    // Смена директории с проектом
    int setProjectDirectory(const QString projectDirectory);


private:
    // Строковые константы
    const QString mSlash;
    // Массив модулей интерфейса
    QVector<QSharedPointer <ElementInterface> > mElementsIterfaces;
    // Параметры файловой системы
    QString mSettingsFolder;
    QString mFilename;
};



#endif // INTERFACELOADER_H
