#include "tooltip.h"



ToolTip::ToolTip(QWidget *parent, Qt::WindowFlags f) :
    QWidget(parent, f),
    mIsShow(false)
{ 
    // Виджет под таблицу с серым фоном и скруглением углов в 2px
    wgtTable = new QWidget(this);
    wgtTable->setStyleSheet("QWidget {background: #808080; border-radius: 2px;}");

    const int spacing = 2;
    const int margin = 2;

    ltMain = new QVBoxLayout(this);

    ltTable = new QGridLayout(wgtTable);
    ltTable->setVerticalSpacing(spacing);
    ltTable->setHorizontalSpacing(spacing);
    ltTable->setMargin(margin);
    // Устанавливаем растяжки
    // Колонка 1 - 20%
    ltTable->setColumnStretch(0, 1);
    // Колонка 2 - 20%
    ltTable->setColumnStretch(1, 1);
    // Колонка 3 - 60%
    ltTable->setColumnStretch(2, 3);

    // Имя элемента
    lblName = QPointer<QLabel>(new QLabel(this));
    lblName.data()->setStyleSheet("QLabel {color: #525252}");
    // Заголовки таблицы
    lblCaption.clear();
    lblCaption.append(QPointer<QLabel>(new QLabel(QString("<b>Бит</b>"), wgtTable)));
    lblCaption.append(QPointer<QLabel>(new QLabel(QString("<b>Значение</b>"), wgtTable)));
    lblCaption.append(QPointer<QLabel>(new QLabel(QString("<b>Описание</b>"), wgtTable)));

    const int labelsMargin = 3;
    for (int i = 0; i < lblCaption.size(); i++)
    {
        lblCaption[i].data()->setStyleSheet("QLabel { background: #F5F6FA; color: #525252}");
        lblCaption[i].data()->setMargin(labelsMargin);
        lblCaption[i].data()->setAlignment(Qt::AlignCenter);
    }

    ltMain->addWidget(lblName, 0, Qt::AlignLeft | Qt::AlignVCenter);
    ltMain->addWidget(wgtTable, 1, Qt::AlignCenter);

    this->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    this->setWindowFlags(Qt::Popup);
}



void ToolTip::mousePressEvent(QMouseEvent *event)
{
    // Скрытие с экрана всплывающей подсказки
    emit signalToolTipHide();
    this->hide();
    this->resize(0, 0);

    return QWidget::mousePressEvent(event);
}



// Прорисовка обводки виджета
void ToolTip::paintEvent(QPaintEvent *event)
{
    int roundness(3);
    QPainter painter(this);
    painter.save();

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::red);

    QPainterPath roundedRect;
    roundedRect.addRoundRect(1, 1, this->rect().width() - 2, this->rect().height() - 2, roundness, roundness);
    painter.setClipPath(roundedRect);

    QRegion maskregion = painter.clipRegion();
    setMask(maskregion);
    painter.setBrush(Qt::NoBrush);
    painter.setPen(QPen(QColor(128, 128, 128), 3, Qt::SolidLine));
    painter.drawRoundedRect(1, 1, this->rect().width() - 2, this->rect().height() - 2, roundness + 2, roundness + 2);
    // restore painter
    painter.restore();

    QWidget::paintEvent(event);
}



int ToolTip::slotReceiveToopTipInfo(QString name,
                                    int count,
                                    const QVector<QString> bits,
                                    const QVector<QString> values,
                                    const QVector<QString> description,
                                    QPoint pos)
{
    lblName.data()->setText(name);

    // Очистка памяти
    if (lbls.size() > 0)
    {
        for (int i = 0; i < lbls.size(); i++)
        {
            ltTable->removeWidget(lbls[i].data());
            lbls[i].data()->deleteLater();
        }
        lbls.clear();
    }

    // Выделение памяти
    lbls = QVector< QPointer<QLabel> >(count * 3);

    // Заголовок
    ltTable->addWidget(lblCaption[0].data(), 0, 0, 1, 1);
    ltTable->addWidget(lblCaption[1].data(), 0, 1, 1, 1);
    ltTable->addWidget(lblCaption[2].data(), 0, 2, 1, 1);

    const int labelsMargin = 3;
    for (int i = 0; i < count; i++)
    {
        lbls[i * 3] = QPointer<QLabel>(new QLabel(bits[i], wgtTable));
        lbls[i * 3].data()->setAlignment(Qt::AlignCenter);
        lbls[i * 3].data()->setStyleSheet("QLabel { background: #F5F6FA; color: #525252}");
        lbls[i * 3].data()->setMargin(labelsMargin);
        ltTable->addWidget(lbls[i * 3].data(), i + 1, 0, 1, 1);
        lbls[i * 3 + 1] = QPointer<QLabel>(new QLabel(values[i], wgtTable));
        lbls[i * 3 + 1].data()->setAlignment(Qt::AlignCenter);
        lbls[i * 3 + 1].data()->setStyleSheet("QLabel { background: #F5F6FA; color: #525252}");
        lbls[i * 3 + 1].data()->setMargin(labelsMargin);
        ltTable->addWidget(lbls[i * 3 + 1].data(), i + 1, 1, 1, 1);
        lbls[i * 3 + 2] = QPointer<QLabel>(new QLabel(description[i], wgtTable));
        lbls[i * 3 + 2].data()->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        lbls[i * 3 + 2].data()->setStyleSheet("QLabel { background: #F5F6FA; color: #525252}");
        lbls[i * 3 + 2].data()->setMargin(labelsMargin);
        ltTable->addWidget(lbls[i * 3 + 2].data(), i + 1, 2, 1, 1);
    }

    // Получение указателя на главное окно системы
    QWidget* screen = QApplication::desktop()->screen(0);
    int screenWidth = screen->width();
    int screenHeight = screen->height();

    // Корректировка координат всплывающей подсказки
    if (pos.x() + this->width() > screenWidth)
        pos.setX(pos.x() - this->width());

    if (pos.y() + this->height() > screenHeight)
        pos.setY(pos.y() - this->height());
    else
        pos.setY(pos.y() + 20);

    this->move(pos);


    // Отображение виджета
    mIsShow = true;

    // Масштабирование виджета до нужных размеров
    wgtTable->adjustSize();
    this->adjustSize();

    this->show();

    return 0;
}



int ToolTip::slotReceiveToolTipInfo(QString name,
                                    int count,
                                    const QVector<QString> values)
{
    lblName.data()->setText(name);
    for (int i = 0; i < count; i++)
        lbls[i * 3 + 1].data()->setText(values[i]);
    return 0;
}
