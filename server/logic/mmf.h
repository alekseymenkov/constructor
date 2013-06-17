#ifndef MMF_H
#define MMF_H



#include <QSharedMemory>



class MMF : public QSharedMemory
{
    Q_OBJECT

public:
    MMF(const QString& key, QObject* parent = 0);
    ~MMF();

    // Чтение данных из MMF
    QByteArray read(quint16 startByte, quint16 bytesCount);
    // Запись данных в MMF
    bool write(quint16 startByte, QByteArray& data);
    // Получение доступа к MMF
    qint8 attach();
};



#endif // MMF_H
