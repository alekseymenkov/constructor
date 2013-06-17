#include "mmf.h"


MMF::MMF(const QString &key, QObject *parent) :
    QSharedMemory(parent)
{
    setNativeKey(key);
}



MMF::~MMF()
{
    if (isAttached())
        detach();
}



// Чтение данных из MMF.
// Аргументы: стартовый байт (qint16), количество байт (qint16).
// Возвращаемое значение: массив данных (QByteArray).
QByteArray MMF::read(quint16 startByte, quint16 bytesCount)
{
    if (isAttached())
        return QByteArray((char*)this->constData() + startByte, bytesCount);
    else
        return QByteArray();
}



// Запись данных в MMF
bool MMF::write(quint16 startByte, QByteArray &data)
{
    if (isAttached())
    {
        this->lock();
        memcpy(((char*)this->data() + startByte), data.constData(), data.size());
        this->unlock();
        return true;
    }

    return false;
}



// Получение доступа к MMF.
qint8 MMF::attach()
{
    return QSharedMemory::attach(QSharedMemory::ReadWrite);
}
