#ifndef LINEEDITEVENTFILTER_H
#define LINEEDITEVENTFILTER_H



#include <QObject>
#include <QEvent>
#include <QLineEdit>



// Фильтр событий
class LineEditEventFilter : public QObject
{
    Q_OBJECT

public:
    explicit LineEditEventFilter(quint16 port, QObject *parent = 0);


protected:
    bool eventFilter(QObject* object, QEvent* event);


private:
    quint16 mPort;
};



#endif // LINEEDITEVENTFILTER_H
