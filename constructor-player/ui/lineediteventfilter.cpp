#include "lineediteventfilter.h"
#include "QDebug"


LineEditEventFilter::LineEditEventFilter(quint16 port, QObject *parent) :
    QObject(parent)
{
    mPort = port;
}



bool LineEditEventFilter::eventFilter(QObject* object, QEvent* event)
{
    // Обрабатываем только FocusOut-событий
    if (event->type() == QEvent::FocusOut)
    {
        // Если в LineEdit нет текста, то восстанавливаем значение по умолчанию
        QLineEdit* lePort = qobject_cast<QLineEdit*>(object);
        if (lePort->text().isEmpty())
            lePort->setText(QString::number(mPort));
    }

    return QObject::eventFilter(object, event);;
}

