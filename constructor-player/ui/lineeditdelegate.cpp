#include "lineeditdelegate.h"



LineEditDelegate::LineEditDelegate(DelegateType::Type delegatType, QObject *parent) :
    QStyledItemDelegate(parent),
    mDelegatType(delegatType)
{
}




QWidget* LineEditDelegate::createEditor(QWidget *parent,
                                        const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    if (mDelegatType == DelegateType::NoEdit)
    {
        return 0;
    }
    else
    {
        QLineEdit* lineEdit = new QLineEdit(parent);

        quint16 topLimit = 0;
        switch (mDelegatType)
        {
        case DelegateType::Validator8Bit:
            topLimit = 255;
            lineEdit->setValidator(new QIntValidator(0, topLimit, lineEdit));
            break;
        case DelegateType::Validator16Bit:
            topLimit = 65535;
            lineEdit->setValidator(new QIntValidator(0, topLimit, lineEdit));
            break;
        case DelegateType::NoEdit:
        case DelegateType::NoValidator:
            break;
        }

        return lineEdit;
    }
}



void LineEditDelegate::paint ( QPainter * painter, const QStyleOptionViewItem & option,
                               const QModelIndex & index ) const
{
    QStyleOptionViewItemV4 viewItemOption(option);
    viewItemOption.state &= ~QStyle::State_HasFocus;
    QStyledItemDelegate::paint(painter, viewItemOption, index);
    return;
}
