#ifndef LINEEDITDELEGATE_H
#define LINEEDITDELEGATE_H



#include <QItemDelegate>
#include <QLineEdit>
#include <QtCore>
#include <QtGui>
#include <QApplication>



// Делегат-валидатор
class LineEditDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    struct DelegateType
    {
        enum Type
        {
            NoEdit = 0,
            NoValidator = 1,
            Validator8Bit = 2,
            Validator16Bit = 3
        };
    };

    explicit LineEditDelegate(DelegateType::Type delegatType, QObject *parent = 0);
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;


protected:
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;


private:
    DelegateType::Type mDelegatType;
};



#endif // LINEEDITDELEGATE_H
